#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile styleFile(":/res/css/qtCSS.qss"); // Replace with the path to your stylesheet
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        a.setStyleSheet(style); // Apply the stylesheet globally
        styleFile.close();
    } else {
        qDebug() << "Failed to load stylesheet!";
    }

    MainWindow w;
    w.show();
    return a.exec();
}
