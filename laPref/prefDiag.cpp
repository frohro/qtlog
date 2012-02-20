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
#include "../qtlogDiag/dirmngr.h"

prefDiag::prefDiag(QWidget * parent) : QWidget(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
  setupUi(this);
  setupAction();
}
void prefDiag::setupAction()
{
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
  PrefTable->setColumnWidth(0,85);                     
  PrefTable->setColumnWidth(1,80);                     
  PrefTable->setColumnWidth(2,210);                    
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


void prefDiag::getHilfeDxPrefCb()
{
   settings.setValue("Val","DX-LaenderListe");
   settings.sync();
   StartProcess("hilfedb &");
}


void prefDiag::addPref()
{
 QString a;
    qy = "INSERT INTO tpref VALUE ('";
    qy += lineEditSpref->text();             
    qy += "','"+lineEditPref->text()+"')";  
    
    QSqlQuery query(qy);
    
    
    
    
    PrefTable->setRowCount(row+1);                  
    col = 0;
    QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(col)));  
    newItem->setText(lineEditSpref->text());
    PrefTable->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                  
    newItem->setText(lineEditPref->text());
    PrefTable->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                  
    newItem->setText(lineEditPref->text());
    PrefTable->setItem(row,col++,newItem);
 
    row = PrefTable->rowCount();
    a.setNum(row);
}


void prefDiag::delPref()
{
 int r;
   r = PrefTable->currentRow();
   QTableWidgetItem * rItem = PrefTable->item(r,0);
   qy = "DELETE FROM tpref WHERE spref='"+rItem->text()+"'";
   QSqlQuery query(qy);
   PrefTable->removeRow(r);
}


void prefDiag::getPrefs(QString str)
{
   a = str.toUpper();
   lineEditPref->setText(a);
   qy = "SELECT spref,pref,lname FROM (tpref,tla) WHERE pref=la AND pref LIKE '"+str+"%'"; 
   showPrefs(qy);
   lineEditPref->setFocus();
}


void prefDiag::getSubPrefs(QString str)
{
    qy = "SELECT spref,pref,lname FROM (tpref,tla) WHERE pref=la AND spref LIKE '"+str+"%'";
    showPrefs(qy);
    lineEditSpref->setFocus();
}

void prefDiag::showPrefs(QString q)
{
  QSqlQuery query;
  query.exec(q);
  row = query.size();                           
  
  PrefTable->setRowCount(row);                  
  row = 0;
  while(query.next()) {
    col = 0;
    i = 0;
    QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(col)));  
    newItem->setText(query.value(i++).toString());
    PrefTable->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                  
    newItem->setText(query.value(i++).toString());
    PrefTable->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                  
    newItem->setText(query.value(i++).toString());
    PrefTable->setItem(row,col++,newItem);
    row++;
  }
  labelcount->setText(a.setNum(row));
}





void prefDiag::updatePrefItem( QTableWidgetItem *item )
{
    if (item != PrefTable->currentItem())             
        return;
    if (item) {                                        
       //QSqlQuery query;
       row = PrefTable->row( item );                   
       col = PrefTable->column ( item );               
       
       QSqlField field("feld",QVariant::String);
       qy = "UPDATE tpref SET wae=";
       field.setValue(item->data(Qt::EditRole).toString());   
       QTableWidgetItem * eItem = PrefTable->item( row, 0);   
       qy += db.driver()->formatValue(field,false)+" WHERE spref="+eItem->text();
       //qDebug() << qy;
    }
}
