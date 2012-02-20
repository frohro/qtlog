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
#include "hlib.h"
#include "defs.h"


hlib::hlib(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    connect(buttonExit, SIGNAL(pressed()), this, SLOT(goExit()));
    connect(checkBoxRuning, SIGNAL(pressed()), this, SLOT(checkBoxRChanged()));
    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    
    run = 0;
    QSqlQuery query; 
    qy = "SELECT retry,runing FROM wproz WHERE proz='HamLib'";
    query.exec(qy);
    n = query.size();
    if( !n )
      QTimer::singleShot(10, this, SLOT(hamlibError()));  
    else {
      while(query.next()) {
        wRetry->setText(query.value(0).toString());
        run = query.value(1).toInt();
      }
      
      run = settings.value("Val").toString().toInt();
      if(run) {
        checkBoxRuning->setChecked(TRUE);
        checkBoxRuning->setText("running");
      }
      else {
        checkBoxRuning->setChecked(FALSE);
        checkBoxRuning->setText("stopped");
      }
    }
}

hlib::~hlib()
{
}

void hlib::goExit()
{
   nRetry = wRetry->text();
   accept();
}

void hlib::checkBoxRChanged()
{
    if(checkBoxRuning->isChecked() == FALSE) {
      checkBoxRuning->setChecked(FALSE);
      checkBoxRuning->setText("run");
      run = 1;
    }
    else {
      checkBoxRuning->setChecked(TRUE);
      checkBoxRuning->setText("stop");
      run = 0;
    }
    settings.setValue("Val",run);
}

void hlib::hamlibError()
{
     QMessageBox::information(this,tr("Hamlib Client"),
                   tr("\nThe host was not found.\n"
                   "Config Hamlib and restart contest logging."));
     goExit();                   
}
