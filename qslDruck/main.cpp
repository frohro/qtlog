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
#include "qslPr.h"
#include "../qtlogDiag/dirmngr.h"
DirMngr dirMngr;
int main(int argc, char *argv[])
{
   dirMngr.SetProgramDir(argv[0]);
   QTranslator translator;
   if(translator.load("../qslDruck/qtlogQSLlpr_en")) qDebug()<<"Got the translation for qsllpr.\n";
   QApplication a(argc, argv);
   a.installTranslator(&translator);
   qslPr qPr;
   qPr.show();
  return a.exec();
}
