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

#include "addQso.h"


int main(int argc, char *argv[])
{
  int Lang = 1;                           // language
  QString home = QDir::homePath(); 
  
  QTranslator translator;
  QLocale locale;
  Lang = locale.country();                // 82 for Germany
  Lang = 100;                             // only for TEST dl1hbd - country outside Germany
  
  QApplication a(argc, argv);
  
  if(Lang != 82) {                        // every country except Germany
      home+="/log/qtlogDiag/qtlogaddQso_en";
      translator.load(home);  
      a.installTranslator(&translator);
  }
  
  addQso addDiag;
  addDiag.show();
  
  return a.exec();
}
