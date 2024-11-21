#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    void setFile(const QString &filePath);

private slots:
    void on_saveButton_clicked();

    void on_cancelButton_clicked();

    void on_fileContent_textChanged();

    void on_randomizeButton_clicked();

private:
    Ui::Dialog *ui;
    QString currentFilePath;
    bool changesMade;

    void createAction(const QString &shortcut, void (Dialog::*slotFunc)());
};

#endif // DIALOG_H
