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
    QCoreApplication app(argc, argv);
    DownloadMgr manager;
    QTimer::singleShot(0, &manager, SLOT(execute()));
    app.exec();
}
