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
#include "dxspot.h"


int main(int argc, char *argv[])
{
  int Lang;
  QString home = QDir::homePath(); 
  
   QTranslator translator;
   QLocale locale;
   
   QApplication dxsp(argc, argv);
   //if(translator.load("../dxSpot/qtlogDXSpot_en")) qDebug()<<"Got the translation.\n";
   Lang = locale.country();
   Lang = 100;                 // TEST
   
   if(Lang != 82) {
       home+="/log/dxSpot/qtlogDXSpot_en";
       translator.load(home);  
       dxsp.installTranslator(&translator);
   }
  
   dxspot dxspotDiag;
   dxspotDiag.show();
   //QTimer::singleShot(0, &manager, SLOT(execute()));
   return dxsp.exec();
}
