/* **************************************************************
   dl1hbd 20.02.2009
*************************************************************** */

#include <QCoreApplication>
#include <QStringList>
#include <QTimer>
#include <stdio.h>

#include "getmgr.h"

int main(int argc, char **argv)
{
    //QTranslator translator;
    //if(translator.load("../getitMgr/qtlogGetMgr_en")) qDebug()<<"Got the translation.\n";
    QCoreApplication app(argc, argv);
    //app.installTranslator(&translator);
    getMgr manager;
    QTimer::singleShot(0, &manager, SLOT(execute()));
    app.exec();
}
