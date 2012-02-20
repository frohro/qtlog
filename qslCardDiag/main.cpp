/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   QtLog V 1.5.n : 
********************************************************************** */

#include <QApplication>
#include "qsldb.h"
#include "../qtlogDiag/dirmngr.h"
DirMngr dirMngr;

int main(int argc, char *argv[])
{
  dirMngr.SetProgramDir(argv[0]);
  QApplication a(argc, argv);
  qsldb qsldbDiag;
  qsldbDiag.show();
  return a.exec();
}
