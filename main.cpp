#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QDir>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString appDir = QApplication::applicationDirPath();

    /* load language file */
    QTranslator t;
    if(t.load(appDir + "/language/zh.qm"))
        a.installTranslator(&t);

    /* load style sheet. */
    QFile file(appDir + "/style/style.css");
    if (file.open(QIODevice::ReadOnly))
    {
        QString style(file.readAll());
        style.replace("$AppDir", appDir);
//        qDebug() << style;
        a.setStyleSheet(style);
        file.close();
    }

    MainWindow w;
    w.show();

    return a.exec();
}
