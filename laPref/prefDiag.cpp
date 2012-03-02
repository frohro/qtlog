/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QtGui>
#include <QtSql>
#include "prefDiag.h"

// ------------------------------------------------------------------
prefDiag::prefDiag(QWidget * parent) : QWidget(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);

   int n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n); 			
   setFont(font);
   //-- File ---
   connect(ButtonExit, SIGNAL(pressed()), this, SLOT(goExit()));
   connect(ButtonHilfe, SIGNAL(pressed()), this, SLOT(getHilfeDxPrefCb()));
   connect(lineEditSpref, SIGNAL(textEdited(QString)), this, SLOT(getSubPrefs(QString)));
   connect(lineEditPref, SIGNAL(textEdited(QString)), this, SLOT(getPrefs(QString)));

   //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
   db = QSqlDatabase::addDatabase("QMYSQL");
   db.setHostName(settings.value("host").toString());  
   db.setDatabaseName(settings.value("dbname").toString());
   db.setUserName(settings.value("dbuser").toString());
   db.setPassword(settings.value("dbpasswd").toString());
  
   if(!db.open()) {
      qDebug() << db.lastError();                      
   }
  
   PrefTable->setColumnWidth(0,85);                     // spref
   PrefTable->setColumnWidth(1,80);                     // pref
   PrefTable->setColumnWidth(2,210);                    // lname
  
   qy = "SELECT spref,pref,lname FROM (tpref,tla) WHERE pref=la ORDER BY spref";
   showPrefs(qy);
   lineEditSpref->setFocus();
}

prefDiag::~prefDiag()
{
}

void prefDiag::goExit()
{
  db.close();
  qApp->quit();
}
//-----------------------------------------------
void prefDiag::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
   }
}

// hilfe
// ---------------------------------------------
void prefDiag::getHilfeDxPrefCb()
{
   settings.setValue("Val","DX-LaenderListe");
   settings.sync();
   system("hilfedb &");
}


// Prefix text in LineEditPtref editiert
// ---------------------------------------
void prefDiag::getPrefs(QString str)
{
   a = str.toUpper();
   lineEditPref->setText(a);
   qy = "SELECT spref,pref,lname FROM (tpref,tla) WHERE pref=la AND pref LIKE '"+str+"%'";  // matchen
   showPrefs(qy);
   lineEditPref->setFocus();
}


// SUB.prefiex text Edited in lineEditSpref
// --------------------------------------------------
void prefDiag::getSubPrefs(QString str)
{
    qy = "SELECT spref,pref,lname FROM (tpref,tla) WHERE pref=la AND spref LIKE '"+str+"%'";
    showPrefs(qy);
    lineEditSpref->setFocus();
}

// -------------------------------------------------
void prefDiag::showPrefs(QString q)
{
  QSqlQuery query;
  query.exec(q);
  row = query.size();                            //  rows ?
   // col = query.record().count();              //  Spalten sind 3
  PrefTable->setRowCount(row);                   //  TabellenGroeße setzen - col ist schon gesetzt
  row = 0;
  while(query.next()) {
    col = 0;
    i = 0;
    QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(col)));  // sub_pref
    newItem->setText(query.value(i++).toString());
    PrefTable->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                  // pref
    newItem->setText(query.value(i++).toString());
    PrefTable->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                  // landes_name
    newItem->setText(query.value(i++).toString());
    PrefTable->setItem(row,col++,newItem);
    row++;
  }
  labelcount->setText(a.setNum(row));
}
