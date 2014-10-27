#include <QCoreApplication>
#include <QDebug>
#include <QString>

#include "../cbaidutranslater.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CBaiduTranslater m_baiduTranslater;
    m_baiduTranslater.setAPI_Key("YaGqITH4r1i95Xp8izAhrxwT");
    QString input = argv[1];
    m_baiduTranslater.translateCli(input);

    //QString data = m_baiduTranslater.returnResult();
    qDebug(input.toLatin1().data());


    return a.exec();
}
