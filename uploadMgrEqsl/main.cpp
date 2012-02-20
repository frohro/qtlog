/* **************************************************************
   dl1hbd 20.02.2009
*************************************************************** */
#include <QCoreApplication>
#include <QStringList>
#include <QTimer>
#include <stdio.h>
#include <QApplication>
#include <QSettings>
#include "uploadMgr.h"
int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QtLog");
    QCoreApplication::setApplicationName("qtlog");
    UploadMgr manager;
    QTimer::singleShot(0, &manager, SLOT(execute()));
    app.exec();
}
