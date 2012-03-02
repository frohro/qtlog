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
#include "prefDiag.h"


int main(int argc, char *argv[])
{
  int Lang;
  QString home = QDir::homePath(); 
  
  QTranslator translator;
  //if(translator.load("../laPref/qtlogPrefix_en")) qDebug()<<"Got the translation.\n";
 
  QApplication a(argc, argv);
  QLocale locale;
  Lang = locale.country();
  Lang = 100;                 // TEST
  
  if(Lang != 82) {
       home+="/log/laPref/qtlogLaPrefix_en";
       translator.load(home);  
       a.installTranslator(&translator);
  }
  
  a.installTranslator(&translator);
  prefDiag pDiag;
  pDiag.show();
  return a.exec();
}
