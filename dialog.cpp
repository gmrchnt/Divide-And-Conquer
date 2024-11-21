#include "dialog.h"
#include "ui_dialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QRandomGenerator64>
#include <QInputDialog>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <QRadioButton>
#include <QMainWindow>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
    , currentFilePath("")
    , changesMade(false)
{
    ui->setupUi(this);

    // Create shortcuts and connect them to their respective slots
    createAction("Ctrl+R", &Dialog::on_randomizeButton_clicked);
    createAction("Ctrl+E", &Dialog::on_cancelButton_clicked);
    createAction("Ctrl+S", &Dialog::on_saveButton_clicked);
}

void Dialog::createAction(const QString &shortcut, void (Dialog::*slotFunc)())
{
    QAction *action = new QAction(this);
    action->setShortcut(QKeySequence(shortcut));
    action->setShortcutContext(Qt::WindowShortcut); // Ensure it works when the dialog is active

    // Connect the action to the specified slot
    connect(action, &QAction::triggered, this, slotFunc);

    // Add the action to the dialog's action list
    addAction(action);
}


Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setFile(const QString &filePath)
{
    currentFilePath = filePath;

    QFile file(currentFilePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(this, "File Error", "Could not open the file for reading.");
        return;
    }

    // Read the file contents and set them to fileContent
    QTextStream in(&file);
    ui->fileContent->setPlainText(in.readAll());
    file.close();

    // Extract the file name without path and extension
    QFileInfo fileInfo(currentFilePath);
    ui->label->setText(fileInfo.baseName());
}

void Dialog::on_saveButton_clicked() // Save button
{
    // If there is no current file path (e.g., a new file is being created)
    if (currentFilePath.isEmpty())
    {
        // Define the directory for saving new files
        QDir currentDir(QDir::currentPath());
        QString folderName = "inputFiles";

        // Check if the "inputFiles" directory exists, if not, create it
        if (!currentDir.exists(folderName))
        {
            if (!currentDir.mkdir(folderName))
            {
                QMessageBox::critical(this, "Directory Error", "Could not create the 'inputFiles' directory.");
                return;
            }
        }

        // Prompt the user to enter a name for the new file
        QString newFileName = QFileDialog::getSaveFileName(this, "Save New File", currentDir.filePath(folderName), "Text Files (*.txt);;All Files (*.*)");
        if (newFileName.isEmpty())
        {
            QMessageBox::information(this, "Save Canceled", "File save operation was canceled.");
            return;
        }

        currentFilePath = newFileName; // Update the current file path to the new file
    }

    // Open the file for writing
    QFile file(currentFilePath);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::critical(this, "File Error", "Could not open the file for writing.");
        return;
    }

    // Write the changes to the file
    QTextStream out(&file);
    QString text = ui->fileContent->toPlainText();
    out << text;
    file.close();

    changesMade = false;
    QMessageBox::information(this, "File Saved", "Changes have been saved to the file.");
}


void Dialog::on_cancelButton_clicked() // Cancel button
{
    if (changesMade)
    {
        int response = QMessageBox::warning(this, "Unsaved Changes",
                                            "You have unsaved changes. Are you sure you want to cancel?",
                                            QMessageBox::Yes | QMessageBox::No);
        if (response == QMessageBox::No)
        {
            return;
        }
    }

    close();
}

void Dialog::on_fileContent_textChanged()
{
    changesMade = true; // Mark that changes have been made
}


void Dialog::on_randomizeButton_clicked()
{
    bool ok;
    //QRadioButton *codeRadioButton = mainWindow->findChild<QRadioButton *>("codeRadioButtonName");
    int numberOfLines = QInputDialog::getInt(this, "Number of Lines", "Enter the number of lines to generate:", 10, 1, 10000, 1, &ok);

    if (!ok) {
        // User cancelled the input dialog
        return;
    }

    QRandomGenerator *rd = QRandomGenerator::global();

    QString content;

    // Generate random real numbers in the range of -50.0 to +50.0
    for (int i = 0; i < numberOfLines; ++i) {
        double num1 = rd->generateDouble() * 200.0 - 100.0;
        double num2 = rd->generateDouble() * 200.0 - 100.0;

        QString line = QString::number(num1, 'f', 1) + " " + QString::number(num2, 'f', 1);
        content += line + "\n";
        ui->fileContent->appendPlainText(line);
    }
}

