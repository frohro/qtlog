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
#include "mkLquery.h"

mkLquery::mkLquery(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(ButtonGetRecord, SIGNAL(clicked()), this, SLOT(mkLqueryRec()));
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                       
   setFont(font);
   s = settings.value("LotLastUp").toString();
   QDate d = QDate::fromString (s,"yyyy-MM-dd");
   dateEdit->setDate(d);



   QString mb, ab;
   QSqlQuery query;
   qy = "SELECT mband,aband FROM wband WHERE work != 0";
   query.exec(qy);
   while(query.next()) {                       //
       n = 0;
       mb = query.value(n++).toString();         
       ab = query.value(n++).toString();         
       toAdifBand.insert(mb,ab);                 
   }


    QString rn, an;
    qy = "SELECT refnam,adifnam FROM refnamen WHERE type='6'";
    query.exec(qy);
    while(query.next()) {                       //
       n = 0;
       rn = query.value(n++).toString();         
       an = query.value(n++).toString();         
       toAdifMode.insert(rn,an);                 
    }
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

mkLquery::~mkLquery()
{
}

void mkLquery::goExit()
{
   getRec = 0;
   accept();  
}



void mkLquery::mkLqueryRec()
{
    QString user, passwd;
    user = settings.value("LotUser").toString();
    passwd = settings.value("LotPasswd").toString();
    if((user.count() == 0) || (passwd.count() == 0)) {
      QMessageBox::information( this,tr("Fehler !"),tr("Username oder Passwort nicht vorhanden"),
      QMessageBox::Ok);
      return;
    }
    //qyrec = "https://www.arrl.org/lotwuser/lotwreport.adi?login="+user+" password="+passwd;
    qyrec = "https://p1k.arrl.org/lotwuser/lotwreport.adi?login="+user+" password="+passwd; 
    qyrec += " qso_query=1";
    s = BoxLast->text().left(3).simplified();               
    n = s.toInt();
    QDate d = dateEdit->date(); 
    s = d.toString("yyyy-MM-dd");                           
    int err;
    err = 0;
    switch( n ) {
     case 1 : 
             qyrec += " qso_qslsince="+s;
             break;
     case 2 :
             qyrec += " qsorxsince="+s;
             break;
     case 3 :                     
             if(lineEdit_4->text().count() != 0)
               qyrec += " qso_callsign="+lineEdit_4->text(); 
             else
              err = 3;
             break;
     case 4 :                     
             s = getAdifMode(lineEdit_4->text());
             if(s.count())
                qyrec += " qso_mode="+s; 
             else
              err = 4;
             break;
     case 5 :                    
             s = getAdifBand(lineEdit_4->text());
             if(s.count())
               qyrec += " qso_band="+getAdifBand(lineEdit_4->text()); 
             else
               err = 5;
             break;
     case 6 :                    
             if( n == 6) {
                QSqlQuery query;
                qy = "SELECT adif FROM tla,tpref WHERE pref = la AND spref='"+lineEdit_4->text()+"'";
                query.exec(qy);
                if(query.size()) {
                  query.next();
                  s = query.value(0).toString();
                  qyrec += " qso_dxcc="+s;           
                }
                else 
                 err = 6;
             }
             break;
     case 7 :
             break;
    }
    if(err) {
      s = "Query Nr "+f.setNum(n)+" : Text < ";
      s += lineEdit_4->text()+" > ist nicht korrekt";
      QMessageBox::information( this,tr("Fehler im Query"),( s ),
      QMessageBox::Ok);
      return;
    }
    getRec = 1;
    
  accept();                                      
}

void mkLquery::checkQslQueryType_1()
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
void mkLquery::checkQslQueryType_2()
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
void mkLquery::checkQslQueryType_3()
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
void mkLquery::checkQslQueryType_4()
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
void mkLquery::checkQslQueryType_5()
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
void mkLquery::checkQslQueryType_6()
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
void mkLquery::checkQslQueryType_7()
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

