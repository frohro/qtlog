/* ********************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QtGui>
#include "start.h"
#include "../qtlogDiag/version.h"

start::start(QWidget * parent) : QDialog(parent)
{
  setupUi(this);
  connect(ButtonOK, SIGNAL(clicked()), this, SLOT(goExit()));
  QString s =tr("QtLog Version  - ");
  s += VERSION;
  textEdit->append(s);
}

start::~start()                      
{
}

void start::goExit()                 
{
   qApp->quit();                     
}
