#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "dialog.h"
#include "dialog2.h"
#include "dialog3.h"

#include <QSet>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionOpen_File_triggered();

    void on_actionNew_File_triggered();

    void on_openButton_clicked();

    void on_actionUse_triggered();

    void on_actionGroup_Info_triggered();

    void on_checkButton_clicked();

    void on_actionNewCodeFile_triggered();

    void on_timeButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::MainWindow *ui;
    QProcess *process;
    Dialog *dialogObj;
    Dialog2 *dialog2Obj;
    Dialog3 *dialog3Obj;
    QSet<QString> fileSet;
    QSet<QString> codeSet;
};
#endif // MAINWINDOW_H
