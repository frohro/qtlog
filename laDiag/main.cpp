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
#include <QTranslator>

#include "laDiag.h"


// -----------------------------
int main(int argc, char *argv[])
{
  int Lang;
  QString home = QDir::homePath(); 
  
  QApplication a(argc, argv);
  
  QTranslator translator;
  QLocale locale;
  //if(translator.load("../laDiag/qtlogLaDialog_en")) qDebug()<<"Got the translation.\n";
  Lang = locale.country();
  Lang = 100;                 // TEST
  
  if(Lang != 82) {
       home+="/log/laDiag/qtlogLaDialog_en";
       translator.load(home);  
       a.installTranslator(&translator);
  }
  
  //a.installTranslator(&translator);
  laDiag lDiag;
  lDiag.show();
  return a.exec();
}
