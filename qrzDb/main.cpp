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
#include "qrz.h"
int main(int argc, char *argv[])
{
  QTranslator translator;
  if(translator.load("../qrzDb/qtlogQRZ_en")) qDebug()<<"Got the translation for qslCardDiag.\n";
  QApplication a(argc, argv);
  a.installTranslator(&translator);
  Qrz qr;
  qr.show();
  return a.exec();
}
