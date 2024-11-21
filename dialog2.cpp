#include "dialog2.h"
#include "ui_dialog2.h"

Dialog2::Dialog2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog2)
{
    ui->setupUi(this);
    QAction *actionClose = new QAction(this);
    actionClose->setShortcut(QKeySequence("Ctrl+E"));
    actionClose->setShortcutContext(Qt::WindowShortcut); // Ensure it works when the dialog is active

    // Connect the action to the same slot as the close button
    connect(actionClose, &QAction::triggered, this, &Dialog2::on_pushButton_clicked);

    // Add the action to the dialog's action list
    this->addAction(actionClose);
}

Dialog2::~Dialog2()
{
    delete ui;
}

void Dialog2::on_pushButton_clicked()
{
    close();
}

