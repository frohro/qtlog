/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 28.02.09
********************************************************************** */

#include <QSettings>
#include <QString>
#include <QTabWidget>
#include <QtGui>
#include <QtSql>
#include "mkEquery.h"

mkEquery::mkEquery(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(ButtonRecord, SIGNAL(clicked()), this, SLOT(mkEqueryRec()));
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                       
   setFont(font);



   QString mb, ab;
   QSqlQuery query;
   qy = "SELECT mband,aband FROM wband WHERE work != 0";
   query.exec(qy);
   while(query.next()) { 
       n = 0;
       mb = query.value(n++).toString();         
       ab = query.value(n++).toString();         
       toAdifBand.insert(mb,ab);                 
   }


    QString rn, an;
    qy = "SELECT refnam,adifnam FROM refnamen WHERE type='6'";
    query.exec(qy);
    while(query.next()) {                        //
       n = 0;
       rn = query.value(n++).toString();         
       an = query.value(n++).toString();         
       toAdifMode.insert(rn,an);                 
    }
   heute = QDate::currentDate();                 
   int i = heute.day();                          
   i--;
   heute = heute.addDays(-i);
   dateEdit->setDate(heute);                
   connect(checkBox_1, SIGNAL(clicked()), this, SLOT(checkQslQueryType_1()));
   connect(checkBox_2, SIGNAL(clicked()), this, SLOT(checkQslQueryType_2()));
   connect(checkBox_3, SIGNAL(clicked()), this, SLOT(checkQslQueryType_3()));
   connect(checkBox_4, SIGNAL(clicked()), this, SLOT(checkQslQueryType_4()));
   connect(checkBox_5, SIGNAL(clicked()), this, SLOT(checkQslQueryType_5()));
   connect(checkBox_6, SIGNAL(clicked()), this, SLOT(checkQslQueryType_6()));
   connect(checkBox_7, SIGNAL(clicked()), this, SLOT(checkQslQueryType_7()));
   
   BoxLast = checkBox_1;
   getRec = 0;
}

mkEquery::~mkEquery()
{
}

void mkEquery::goExit()
{
   accept();  
}



void mkEquery::mkEqueryRec()
{
    QString user, passwd;
    user = settings.value("EqslUser").toString();
    passwd = settings.value("EqslPasswd").toString();
    if((user.count() == 0) || (passwd.count() == 0)) {
      QMessageBox::information( this,tr("Fehler !"),tr("Username oder Passwort nicht vorhanden"),
      QMessageBox::Ok);
      return;
    }
    QDate d = dateEdit->date();                
    s = BoxLast->text().left(3).simplified();  
    box = s.toInt();
    qyrec = "http://www.eqsl.cc/qslcard/DownloadInBox.cfm?UserName="+user+" Password="+passwd;
    switch(  box ) {
     case 1 : 
             qyrec += " Archive=0";         
             break;
     case 2 :
             qyrec += " ConfirmedOnly=1";   
             break;
     case 3 :
             qyrec += " UnconfirmedOnly=1"; 
             break;
     case 4 : 
             s = d.toString("yyyy-MM-dd");
             s = s.remove(QChar('-'), Qt::CaseInsensitive);
             qyrec += " RcvdSince="+s;      
             break;
     case 5 :
             qyrec += " Archive=1";         
             break;
     case 6 :
             //qyrec = "http://www.eqsl.cc/qslcard/VerifyQSO.cfm?";  
             break;
     case 7 :                               
             break;
    }
  getRec = 1;
  accept();
}

void mkEquery::checkQslQueryType_1()
{
   if( checkBox_1 == BoxLast ) 
     checkBox_1->setChecked(TRUE);
   else {
     if(checkBox_1->isChecked() == TRUE) {
         n = 1;
         BoxLast->setChecked(FALSE);
         BoxLast = checkBox_1;
     }
   }
}

void mkEquery::checkQslQueryType_2()
{
    if( checkBox_2 == BoxLast ) 
      checkBox_2->setChecked(TRUE);
    else {
     if(checkBox_2->isChecked() == TRUE) {
          n = 2;
          BoxLast->setChecked(FALSE);
          BoxLast = checkBox_2;
      }
    }
}

void mkEquery::checkQslQueryType_3()
{
   if( checkBox_3 == BoxLast ) 
      checkBox_3->setChecked(TRUE);
   else {
     if(checkBox_3->isChecked() == TRUE) {
         n = 3;
         BoxLast->setChecked(FALSE);
         BoxLast = checkBox_3;
     }
   }
}

void mkEquery::checkQslQueryType_4()
{
  if( checkBox_4 == BoxLast ) 
      checkBox_4->setChecked(TRUE);
   else {
     if(checkBox_4->isChecked() == TRUE) {
         n = 4;
         BoxLast->setChecked(FALSE);
         BoxLast = checkBox_4;
     }
   }
}

void mkEquery::checkQslQueryType_5()
{
  if( checkBox_5 == BoxLast ) 
      checkBox_5->setChecked(TRUE);
   else {
     if(checkBox_5->isChecked() == TRUE) {
         n = 5;
        BoxLast->setChecked(FALSE);
        BoxLast = checkBox_5;
     }
   }
}

void mkEquery::checkQslQueryType_6()
{
  if( checkBox_6 == BoxLast ) 
      checkBox_6->setChecked(TRUE);
   else {
     if(checkBox_6->isChecked() == TRUE) {
         n = 6;
        BoxLast->setChecked(FALSE);
        BoxLast = checkBox_6;
     }
   }
}

void mkEquery::checkQslQueryType_7()
{
  if( checkBox_7 == BoxLast ) 
      checkBox_7->setChecked(TRUE);
   else {
     if(checkBox_7->isChecked() == TRUE) {
         n = 7;
        BoxLast->setChecked(FALSE);
        BoxLast = checkBox_7;
     }
   }
}
