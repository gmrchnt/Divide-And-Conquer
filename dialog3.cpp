#include "dialog3.h"
#include "ui_dialog3.h"

Dialog3::Dialog3(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog3)
{
    ui->setupUi(this);
    QAction *actionClose = new QAction(this);
    actionClose->setShortcut(QKeySequence("Ctrl+E"));
    actionClose->setShortcutContext(Qt::WindowShortcut); // Ensure it works when the dialog is active

    // Connect the action to the same slot as the close button
    connect(actionClose, &QAction::triggered, this, &Dialog3::on_pushButton_clicked);

    // Add the action to the dialog's action list
    this->addAction(actionClose);
}

Dialog3::~Dialog3()
{
    delete ui;
}

void Dialog3::on_pushButton_clicked()
{
    close();
}


