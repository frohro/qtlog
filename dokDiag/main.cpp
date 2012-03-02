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
#include "dok.h"

int main(int argc, char *argv[])
{
  int Lang;
  QString home = QDir::homePath(); 
  
  QTranslator translator;
  //if(translator.load("../dokDiag/qtlogDok_en")) qDebug()<<"Got the translation.\n";
  QLocale locale;
  Lang = locale.country();
  Lang = 100;                 // TEST
 
  QApplication a(argc, argv);
  
   if(Lang != 82) {
       home+="/log/dokDiag/qtlogDok_en";
       qDebug() << home;
       translator.load(home);  
       a.installTranslator(&translator);
  }

  dok dokD;
  dokD.show();
  return a.exec();
}
