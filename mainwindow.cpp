#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QCheckBox>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QProcess>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , fileSet()
    , codeSet()
{
    ui->setupUi(this);

    // Load font
    QFontDatabase::addApplicationFont(":/res/Fonts/TT Rounds Neue Trial Regular.ttf");

    // Add first file to list and grid layout
    QString firstFilePath = "D:/k224550DAA_Project/visualizer/Project1/ClosestPoint.cpp";
    ui->codeFileList->addItem(firstFilePath);

    QRadioButton *codeRadioButton = new QRadioButton("ClosestPoint", this);
    codeRadioButton->setProperty("filePath", firstFilePath);
    QGridLayout *codeGridLayout = qobject_cast<QGridLayout*>(ui->timeCodeFileCheck->layout());
    if (codeGridLayout) {
        codeGridLayout->addWidget(codeRadioButton, 0, 0);
    }
    codeSet.insert("main");
    QString secondFilePath = "D:/k224550DAA_Project/visualizer/Project2/IntegerMultiplication.cpp";
    ui->codeFileList->addItem(secondFilePath);

    QRadioButton *codeRadioButton2 = new QRadioButton("IntegerMultiplication", this);
    codeRadioButton2->setProperty("filePath", secondFilePath);
    QGridLayout *codeGridLayout2 = qobject_cast<QGridLayout*>(ui->timeCodeFileCheck->layout());
    if (codeGridLayout2) {
        codeGridLayout->addWidget(codeRadioButton2, 0, 1);
    }
    codeSet.insert("main2");
    // Initialize QProcess
    process = new QProcess(this);

    // Connect signals for process completion
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [](int exitCode, QProcess::ExitStatus exitStatus) {
                qDebug() << "Process finished with exit code:" << exitCode
                         << "Exit status:" << exitStatus;
            });
}

MainWindow::~MainWindow()
{
    if (process->state() == QProcess::Running) {
        process->terminate();
    }
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionOpen_File_triggered()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Open Data Files", QDir::currentPath(), tr("Text (*.txt)"));

    if (filePaths.isEmpty())
    {
        ui->statusbar->showMessage("No files were selected. Please try again.", 2000);
        return;
    }

    QGridLayout *openGridLayout = qobject_cast<QGridLayout *>(ui->openDataFileRadio->layout());
    QGridLayout *checkGridLayout = qobject_cast<QGridLayout *>(ui->checkDataFileRadio->layout());
    if (!openGridLayout || !checkGridLayout)
    {
        QMessageBox::critical(this, "Layout Error", "openDataFileRadio or checkDataFileRadio does not have a QGridLayout.");
        return;
    }

    // Calculate starting position for openDataFileRadio
    int openNumItems = openGridLayout->count();
    int openRow = openNumItems / 3; // 3 columns per row
    int openCol = openNumItems % 3;

    // Calculate starting position for checkDataFileRadio
    int checkNumItems = checkGridLayout->count();
    int checkRow = checkNumItems / 3; // 3 columns per row
    int checkCol = checkNumItems % 3;

    for (const QString &filePath : filePaths)
    {
        QFileInfo fileInfo(filePath);
        QString baseName = fileInfo.baseName();

        // Check for duplicates
        if (fileSet.contains(filePath))
        {
            ui->statusbar->showMessage("File " + baseName + " is already added.", 2000);
            continue;
        }

        // Add the full file path to the dataFileList
        ui->dataFileList->addItem(filePath);

        // Create radio buttons for each layout
        QRadioButton *oRadioButton = new QRadioButton(baseName, this);
        oRadioButton->setProperty("filePath", filePath);
        QRadioButton *cRadioButton = new QRadioButton(baseName, this);
        cRadioButton->setProperty("filePath", filePath);

        // Add the radio button to the openDataFileRadio grid layout
        openGridLayout->addWidget(oRadioButton, openRow, openCol);
        openCol++;
        if (openCol == 3) // Move to the next row after 3 columns
        {
            openCol = 0;
            openRow++;
        }

        // Add the radio button to the checkDataFileRadio grid layout
        checkGridLayout->addWidget(cRadioButton, checkRow, checkCol);
        checkCol++;
        if (checkCol == 3) // Move to the next row after 3 columns
        {
            checkCol = 0;
            checkRow++;
        }

        // Add the file path to the set for uniqueness
        fileSet.insert(filePath);
    }
}


void MainWindow::on_actionNew_File_triggered()
{
    dialogObj = new Dialog(this);
    dialogObj->setModal(true);
    dialogObj->show();
}

void MainWindow::on_openButton_clicked()
{
    // Ensure openDataFileRadio has a grid layout
    QGridLayout *openGridLayout = qobject_cast<QGridLayout *>(ui->openDataFileRadio->layout());
    if (!openGridLayout)
    {
        QMessageBox::critical(this, "Layout Error", "opeDataFileRadio does not have a QGridLayout.");
        return;
    }

    // Find the selected radio button
    QRadioButton *selectedRadioButton = nullptr;

    for (int i = 0; i < openGridLayout->count(); ++i)
    {
        QWidget *widget = openGridLayout->itemAt(i)->widget();
        if (widget)
        {
            QRadioButton *radioButton = qobject_cast<QRadioButton *>(widget);
            if (radioButton && radioButton->isChecked())
            {
                selectedRadioButton = radioButton;
                break;
            }
        }
    }

    if (!selectedRadioButton)
    {
        QMessageBox::warning(this, "No Selection", "Please select a file to open.");
        return;
    }

    // Retrieve the file path from the radio button's property
    QString filePath = selectedRadioButton->property("filePath").toString();

    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "File Path Error", "No file path associated with the selected radio button.");
        return;
    }

    // Open the selected file in the dialog
    Dialog *dialogObj = new Dialog(this);
    dialogObj->setFile(filePath); // Pass the file path to the dialog
    dialogObj->show();
}

void MainWindow::on_actionUse_triggered()
{
    hide();
    dialog2Obj = new Dialog2(this);
    dialog2Obj->setModal(true);
    dialog2Obj->show();
    dialog2Obj->exec();
    show();
}

void MainWindow::on_actionGroup_Info_triggered()
{
    hide();
    dialog3Obj = new Dialog3(this);
    dialog3Obj->setModal(true);
    dialog3Obj->show();
    dialog3Obj->exec();
    show();
}

void MainWindow::on_checkButton_clicked()
{
    // Ensure openDataFileRadio has a grid layout
    QGridLayout *openGridLayout = qobject_cast<QGridLayout *>(ui->checkDataFileRadio->layout());
    if (!openGridLayout)
    {
        QMessageBox::critical(this, "Layout Error", "checkDataFileRadio does not have a QGridLayout.");
        return;
    }

    // Find the selected radio button
    QRadioButton *selectedRadioButton = nullptr;

    for (int i = 0; i < openGridLayout->count(); ++i)
    {
        QWidget *widget = openGridLayout->itemAt(i)->widget();
        if (widget)
        {
            QRadioButton *radioButton = qobject_cast<QRadioButton *>(widget);
            if (radioButton && radioButton->isChecked())
            {
                selectedRadioButton = radioButton;
                break;
            }
        }
    }

    if (!selectedRadioButton)
    {
        QMessageBox::warning(this, "No Selection", "Please select a file to check.");
        return;
    }

    // Retrieve the file path from the radio button's property
    QString filePath = selectedRadioButton->property("filePath").toString();

    if (filePath.isEmpty())
    {
        QMessageBox::warning(this, "File Path Error", "No file path associated with the selected radio button.");
        return;
    }

    QFile inputFile(filePath);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Could not open file: " + filePath);
        return;
    }

    QTextStream in(&inputFile);
    QString line;
    int lineNumber = 1;
    bool allLinesValid = true;
    QString errorMessages;

    // Reset the message variables properly before the check
    errorMessages.clear();

    // Start reading and validating the file line by line
    while (!in.atEnd()) {
        line = in.readLine();
        QStringList numbers = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        if (numbers.size() != 2) {
            errorMessages += "Error: Invalid format on line " + QString::number(lineNumber) + "\n";
            allLinesValid = false;
        } else {
            bool ok1, ok2;
            numbers[0].toDouble(&ok1);
            numbers[1].toDouble(&ok2);

            if (!ok1 || !ok2) {
                errorMessages += "Error: Invalid numbers on line " + QString::number(lineNumber) + "\n";
                allLinesValid = false;
            }
        }
        lineNumber++;
    }

    inputFile.close();

    // Display result based on the check
    if (allLinesValid) {
        // QMessageBox::information(this, "Success", "All lines are in the correct format.");
        ui->statusbar->showMessage("All lines are in the correct format.",2000);
    } else {
        QMessageBox::warning(this, "Invalid Format", errorMessages);
        // ui->statusbar->showMessage("Invalid Format",2000);
    }
}

void MainWindow::on_actionNewCodeFile_triggered()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this, "Open Code Files", QDir::currentPath(), tr("Cpp (*.cpp)"));
    if (filePaths.isEmpty())
    {
        ui->statusbar->showMessage("No files were selected. Please try again.", 2000);
        return;
    }

    QGridLayout *codeGridLayout = qobject_cast<QGridLayout *>(ui->timeCodeFileCheck->layout());
    if (!codeGridLayout) {
        QMessageBox::critical(this, "Layout Error", "timeCodeFileCheck does not have a QGridLayout.");
        return;
    }

    // Calculate the starting position (assume 3 columns per row)
    int numItems = codeGridLayout->count();
    int codeRow = numItems / 3; // 3 columns per row
    int codeCol = numItems % 3;

    for (const QString &filePath : filePaths)
    {
        QFileInfo fileInfo(filePath);
        QString baseName = fileInfo.baseName();

        // Check for duplicates
        if (fileSet.contains(filePath)) {
            ui->statusbar->showMessage("File " + baseName + " is already added.", 2000);
            continue;
        }

        // Add the full file path to the list widget
        ui->codeFileList->addItem(filePath);

        // Create a new radio button for the file
        QRadioButton *codeRadioButton = new QRadioButton(baseName, this);
        codeRadioButton->setProperty("filePath", filePath);

        // Add the radio button to the grid layout
        codeGridLayout->addWidget(codeRadioButton, codeRow, codeCol);

        // Update row and column for the next item
        if (codeCol == 2) { // Adjust for 3 columns
            codeRow++;
            codeCol = 0;
        } else {
            codeCol++;
        }

        // Update the fileSet and codeSet to track added files
        fileSet.insert(filePath);
        codeSet.insert(baseName);
    }
}

void MainWindow::on_timeButton_clicked()
{
    ui->outputTextEdit->clear();
    ui->timeButton->setEnabled(false); // Disable the button to prevent multiple clicks
    QDir currentDir(QDir::currentPath());
    QString folderName = "exeFiles";

    // Ensure the "exeFiles" directory exists
    if (!currentDir.exists(folderName) && !currentDir.mkdir(folderName)) {
        ui->outputTextEdit->addItem("Error: Could not create the 'exeFiles' directory.");
        ui->timeButton->setEnabled(true); // Re-enable button
        return;
    }

    // Retrieve the selected source file
    QGridLayout *codeGridLayout = qobject_cast<QGridLayout *>(ui->timeCodeFileCheck->layout());
    if (!codeGridLayout) {
        ui->outputTextEdit->addItem("Error: timeCodeFileCheck does not have a grid layout.");
        ui->timeButton->setEnabled(true); // Re-enable button
        return;
    }

    QRadioButton *selectedRadioButton = nullptr;
    for (int i = 0; i < codeGridLayout->count(); ++i) {
        QWidget *widget = codeGridLayout->itemAt(i)->widget();
        if (widget) {
            QRadioButton *radioButton = qobject_cast<QRadioButton *>(widget);
            if (radioButton && radioButton->isChecked()) {
                selectedRadioButton = radioButton;
                break;
            }
        }
    }
    if (!selectedRadioButton)
    {
        QMessageBox::warning(this,"No Selection","Please select a file to compile and run.");
        ui->timeButton->setEnabled(true); // Re-enable button
        return;
    }
    QString sourceFile = selectedRadioButton->property("filePath").toString();
    bool containsSFML = false;
    bool containsArgs = false;
    QFile sfile(sourceFile);

    if (!sfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->outputTextEdit->addItem("Error: Unable to open the source file for checking.");
        return;
    }

    QTextStream in(&sfile);
    QString fileContent = in.readAll();
    sfile.close();

    // Check if the file content contains the specific main function signature
    if (fileContent.contains("int main(int argc, char* argv[])")) {
        containsArgs = true;
    } else if (fileContent.contains("int main(int argc, char** argv)")) {
        containsArgs = true;
    }

    if (sourceFile.isEmpty() || !QFile::exists(sourceFile)) {
        ui->outputTextEdit->addItem("Error: Invalid or missing source file.");
        ui->timeButton->setEnabled(true); // Re-enable button
        return;
    }

    QFileInfo file(sourceFile);
    QString outputExecutable = file.baseName();
    QString outputExecutablePath = currentDir.filePath(folderName + "/" + outputExecutable + ".exe");

    // Compile the source file
    QString program = "g++";
    QStringList compileArgs;
    compileArgs << "-c" << sourceFile
                << "-o" << file.path() + "/" + outputExecutable + ".o";

    QProcess compileProcess;
    ui->outputTextEdit->addItem("Starting compilation...");
    compileProcess.start(program, compileArgs);

    if (!compileProcess.waitForFinished()) {
        ui->outputTextEdit->addItem("Error: Compilation process failed to start.");
        ui->timeButton->setEnabled(true); // Re-enable button
        return;
    }

    QString compileError = compileProcess.readAllStandardError();

    if(!compileError.isEmpty() && compileError.contains("SFML"))
    {
        containsSFML = true;
        compileProcess.terminate();
    }
    if(containsSFML)
    {
        compileError.clear();
        compileArgs.clear();
        compileArgs << "-ID:/k224550DAA_Project/visualizer/External/include"
                    << "-c" << sourceFile
                    << "-o" << file.path() + "/" + outputExecutable + ".o";
        compileProcess.start(program, compileArgs);
        if (!compileProcess.waitForFinished()) {
            ui->outputTextEdit->addItem("Error: Compilation process failed to start.");
            ui->timeButton->setEnabled(true); // Re-enable button
            return;
        }
    }

    if (!compileError.isEmpty()) {
        ui->outputTextEdit->addItem("Compilation Errors:\n" + compileError);
        ui->timeButton->setEnabled(true); // Re-enable button
        return;
    }
    ui->outputTextEdit->addItem("Compilation Successful.");

    // Link the object file to create the executable

    QStringList linkArgs;
    if(containsSFML)
    {
        linkArgs << file.path() + "/" + outputExecutable + ".o"
                 << "-o" << outputExecutablePath
                 << "-LD:/k224550DAA_Project/visualizer/External/lib"
                 << "-lsfml-graphics"
                 << "-lsfml-window"
                 << "-lsfml-system";
    }
    else
    {
        linkArgs << file.path() + "/" + outputExecutable + ".o"
                 << "-o" << outputExecutablePath;
    }
    compileProcess.start(program, linkArgs);
    if (!compileProcess.waitForFinished()) {
        ui->outputTextEdit->addItem("Error: Linking process failed to start.");
        ui->timeButton->setEnabled(true); // Re-enable button
        return;
    }

    QString linkError = compileProcess.readAllStandardError();
    if (!linkError.isEmpty()) {
        ui->outputTextEdit->addItem("Linking Errors:\n" + linkError);
        ui->timeButton->setEnabled(true); // Re-enable button
        return;
    }
    ui->outputTextEdit->addItem("Linking Successful. Executable created.");

    // Retrieve the selected data file from checkDataFileRadio
    QGridLayout *dataGridLayout = qobject_cast<QGridLayout *>(ui->checkDataFileRadio->layout());
    if (!dataGridLayout) {
        ui->outputTextEdit->addItem("Error: checkDataFileRadio does not have a grid layout.");
        ui->timeButton->setEnabled(true); // Re-enable button
        return;
    }

    QRadioButton *selectedDataRadioButton = nullptr;
    for (int i = 0; i < dataGridLayout->count(); ++i) {
        QWidget *widget = dataGridLayout->itemAt(i)->widget();
        if (widget) {
            QRadioButton *radioButton = qobject_cast<QRadioButton *>(widget);
            if (radioButton && radioButton->isChecked()) {
                selectedDataRadioButton = radioButton;
                break;
            }
        }
    }
    QString dataFile;
    QStringList runArgs;
    if(containsArgs)
    {
        if (!selectedDataRadioButton) {
            ui->outputTextEdit->addItem("Error: Please select a data file to pass as an argument.");
            ui->timeButton->setEnabled(true); // Re-enable button
            return;
        }

        dataFile = selectedDataRadioButton->property("filePath").toString();
        if (dataFile.isEmpty() || !QFile::exists(dataFile)) {
            ui->outputTextEdit->addItem("Error: Invalid or missing data file.");
            ui->timeButton->setEnabled(true); // Re-enable button
            return;
        }
    }

    // Run the executable with the data file as an argument

    if(containsArgs)
    {
        process->terminate();
        runArgs << dataFile;  // Add the selected data file as a command-line argument
        process->setProgram(outputExecutablePath);
        process->setArguments(runArgs);
    }
    else
    {
        process->terminate();
        process->setProgram(outputExecutablePath);
    }

    long long duration = 0;  // Declare duration outside the if-else block
    if (containsSFML && containsArgs)
    {
        process->terminate();
        process->start();
        process->setParent(nullptr);
        if (!process->waitForFinished()) {
            ui->outputTextEdit->addItem("Error: Execution process failed to start.");
            ui->timeButton->setEnabled(true); // Re-enable button
            return;
        }
    }
    else
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        ui->outputTextEdit->addItem("Running the program...");
        process->terminate();
        process->start();
        process->setParent(nullptr);
        if (!process->waitForFinished()) {
            ui->outputTextEdit->addItem("Error: Execution process failed to start.");
            ui->timeButton->setEnabled(true); // Re-enable button
            return;
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    }

    QString runOutput = process->readAllStandardOutput();
    QString runError = process->readAllStandardError();

    if (!runError.isEmpty()) {
        ui->outputTextEdit->addItem("Runtime Errors:\n" + runError);
        ui->timeButton->setEnabled(true); // Re-enable button
        return;
    }

    ui->outputTextEdit->addItem("Program Output:\n" + runOutput);
    if (!containsSFML || !containsArgs)
    {
        ui->outputTextEdit->addItem("Program Runtime: " + QString::number(duration) + " microseconds");
    }
    ui->outputTextEdit->addItem("Program finished successfully.");

    // Re-enable the button after the process completes
    ui->timeButton->setEnabled(true);
}

void MainWindow::on_saveButton_clicked()
{
    QDir currentDir(QDir::currentPath());
    QString folderName = "outputFiles";

    // Ensure the "outputFiles" directory exists
    if (!currentDir.exists(folderName) && !currentDir.mkdir(folderName)) {
        ui->outputTextEdit->addItem("Error: Could not create the 'outputFiles' directory.");
        return;
    }

    // Prompt the user to select a file name for saving
    QString newFileName = QFileDialog::getSaveFileName(this, "Save New File", currentDir.filePath(folderName), "Text Files (*.txt);;All Files (*.*)");
    if (newFileName.isEmpty())
    {
        QMessageBox::information(this, "Save Canceled", "File save operation was canceled.");
        return;
    }

    QFile file(newFileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::critical(this, "File Error", "Could not open the file for writing.");
        return;
    }

    // Write the contents of QListWidget (outputTextEdit) to the file
    QTextStream out(&file);
    for (int i = 0; i < ui->outputTextEdit->count(); ++i) {
        QListWidgetItem *item = ui->outputTextEdit->item(i);
        out << item->text() << "\n";
    }
    file.close();

    QMessageBox::information(this, "File Saved", "Content has been saved to the file.");
}
