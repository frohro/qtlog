/* **************************************************************
   dl1hbd 20.02.2009
*************************************************************** */
#include <QCoreApplication>
#include <QStringList>
#include <QTimer>
#include <stdio.h>
#include "downloadMgr.h"
int main(int argc, char **argv)
{
    QTranslator translator;
    if(translator.load("../downLoadMgr/qtlogDownload_en")) qDebug()<<"Got the translation.\n";
    QApplication app(argc, argv);
    app.installTranslator(&translator);
    DownloadMgr manager;
    QTimer::singleShot(0, &manager, SLOT(execute()));
    app.exec();
}
