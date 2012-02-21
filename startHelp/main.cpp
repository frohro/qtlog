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
#include <QTranslator>
#include <QDebug>
#include "start.h"

int main(int argc, char *argv[])
{
  QTranslator translator;
  if(translator.load("../startHelp/qtlogStart_en")) qDebug()<<"Got the translation for startHelp.\n";
  QApplication a(argc, argv);
  a.installTranslator(&translator);

  start startD;
  startD.show();
  return a.exec();
}
