/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 16.11.2008
********************************************************************** */

#include <QSettings>
#include <QString>
#include <QTabWidget>
#include <QtGui>
#include <QtSql>
#include "qslConf.h"

qslConf::qslConf(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(checkBoxLotwShort, SIGNAL(pressed()), this, SLOT(checkBoxLotwShortChanged()));
   connect(checkBoxLotwLong, SIGNAL(pressed()), this, SLOT(checkBoxLotwLongChanged()));
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                       
   setFont(font);
   if((settings.value("Lotdataset").toInt()) == 0) 
     checkBoxLotwShort->setChecked(TRUE);     
   else
    checkBoxLotwLong->setChecked(TRUE);       
}

qslConf::~qslConf()
{
}

void qslConf::goExit()
{
   accept();  
}


void qslConf::checkBoxLotwShortChanged()
{
  if(checkBoxLotwShort->isChecked() == FALSE) {
     settings.setValue("LotdataSet","0");
     checkBoxLotwLong->setChecked(FALSE);
  }
}


void qslConf::checkBoxLotwLongChanged()
{
  if(checkBoxLotwLong->isChecked() == FALSE) { 
     checkBoxLotwShort->setChecked(FALSE); 
     settings.setValue("LotdataSet","1");
  }
}
