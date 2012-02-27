/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QApplication>
#include <QSettings>
#include "Qtlog.h"
#include "version.h"
#include "dirmngr.h"
#include <QTranslator>
#include <QDebug>
#include <QLocale>
 
DirMngr dirMngr;

// -------------------------------------
int main(int argc, char *argv[])
{
  int Lang = 1;                              // language
  QString home = QDir::homePath(); 
  
    dirMngr.SetProgramDir(argv[0]);
    if( argc >= 2 ) {
       QString b = argv[1];
       if(b.compare("-q") == 0) {
         StartProcess("addqsodb &");
         exit(0);
       }
       if(b.compare("-c") == 0) {
	 StartProcess("addCqsodb &");
         exit(0);
       }
       else
        StartProcess("startoptdb &");
       exit(0);
    }
<<<<<<< HEAD
    
    QTranslator translator;
    QLocale locale;
    Lang = locale.country();                // 82 for Germany
    Lang = 100;                             // only for TEST dl1hbd - country outside Germany

    QApplication a(argc, argv);
    if(Lang != 82) {                        // every country except Germany
       home+="/log.git/qtlogDiag/qtlog_en";
       translator.load(home);  
       a.installTranslator(&translator);
    }
    
    QtLog qtlogDialog((QWidget *)0, Lang);
    qtlogDialog.show();
 
    return a.exec();
=======
  QTranslator translator;
  if(translator.load("../qtlogDiag/qtlog_en")) qDebug()<<"Got the translation.\n";
  QApplication a(argc, argv);
  a.installTranslator(&translator);
  QtLog qtlogDialog;
  qtlogDialog.show();
  return a.exec();
>>>>>>> 798097ced011160e4523e0ff40c826fd2aae7fa7
}
