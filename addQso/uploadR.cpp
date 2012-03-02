/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 16-03-2010
********************************************************************** */
#include <QtSql>
#include <qmessagebox.h>
#include "uploadR.h"

// -------------------------------------------
uploadR::uploadR(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    connect(ButtonESC, SIGNAL(pressed()), this, SLOT(goExit()));
    connect(checkBoxRealtime, SIGNAL(clicked()), this, SLOT(qslviaChanged()));
    
    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    
    lineEditPasswd->setText(settings.value("EqslPasswd").toString());
    
    if(settings.value("EqslRealtime").toString().toInt()) {
      checkBoxRealtime->setChecked(TRUE);
      checkBoxRealtime->setText(" Upload freigegeben");
      upLd = 1;
    }
    else {
      checkBoxRealtime->setChecked(FALSE);
      checkBoxRealtime->setText(" Upload gesterrt");
      upLd = 0;
    } 
}// --------------------
uploadR::~uploadR()
{
}

// --------------------
void uploadR::qslviaChanged()
{
    if(checkBoxRealtime->isChecked() == TRUE) {
      settings.setValue("EqslRealtime",1);
      checkBoxRealtime->setText(" Upload freigegeben");
      upLd = 1;
    }
    else {
      settings.setValue("EqslRealtime",0);
      checkBoxRealtime->setText(" Upload gesterrt");
      upLd = 0;
    }
}// -------------------
void uploadR::goExit()
{
   accept();
}
