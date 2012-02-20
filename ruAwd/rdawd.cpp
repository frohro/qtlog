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
#include "rdawd.h"

Rda::Rda(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
  setupUi(this);
  int n = settings.value("FontSize").toString().toInt();
  QFont font;
  font.setPointSize(n); 			
  setFont(font);
   connect(actionENDE, SIGNAL(triggered(bool)), this, SLOT(goExit()));
   connect(actionRdAwdListeladen, SIGNAL(triggered(bool)), this, SLOT(rdAwdTextFileAc()));
   int z,r,i;
   readSettings();

   //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
   db = QSqlDatabase::addDatabase("QMYSQL");
   db.setHostName(settings.value("host").toString()); 
   db.setDatabaseName(settings.value("dbname").toString());
   db.setUserName(settings.value("dbuser").toString());
   db.setPassword(settings.value("dbpasswd").toString());
   if(!db.open()) {
       qDebug() << db.lastError();                      
   }
   ruAwdTable->setColumnWidth(0,60);
   ruAwdTable->setColumnWidth(1,50);
   ruAwdTable->setColumnWidth(2,250);
   ruAwdTable->setColumnWidth(3,50);
   qy = "SELECT * FROM rutable";
   QSqlQuery query(qy);
   row = query.size();                        
   ruAwdTable->setRowCount(row);              
   row = 0;
   while(query.next()) {
    z = 0;
    r = 0;
    col = 0;
    i = 0;
    QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(col)));  
    newItem->setText(query.value(i++).toString());
    ruAwdTable->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                  
    newItem->setText(query.value(i++).toString());
    newItem->setTextAlignment(Qt::AlignCenter);
    ruAwdTable->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                  
    newItem->setText(query.value(i++).toString());
    ruAwdTable->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                  
    newItem->setText(query.value(i++).toString());
    newItem->setTextAlignment(Qt::AlignCenter);
    ruAwdTable->setItem(row,col++,newItem);
    row++;
   }
}

Rda::~Rda()
{
   writeSettings();
}

void Rda::goExit()
{
  db.close();
  qApp->quit();
}

void Rda::writeSettings()
{
  settings.setValue("ruAwd/Size",size());
  settings.setValue("ruAwd/Properties",saveState());
}

void Rda::readSettings()
{
  resize(settings.value("ruAwd/Size",sizeHint()).toSize());
  restoreState(settings.value("ruAwd/Properties").toByteArray());
}


void Rda::rdAwdTextFileAc()
{
  int i, n, p, l;
  QString zeile, z, a;
  char buf[50];
   sprintf(buf,"%s/log/table_rda.awd",(char*)getenv("HOME"));
   QFile datei(buf);
   if(! datei.open(QFile::ReadOnly)) {
     QMessageBox::information( this,
       tr("table_rda.awd"),
       tr("Datei 'table_rda.awd' kann nicht geoeffnet werden !"));
     return;
   }
   QSqlQuery query;
   qy = "DELETE FROM rutable";                  
   query.exec(qy);
   QTextStream inStream(&datei);
   l = 0;
   QString qy;
   while(1) {                                  
     while( !inStream.atEnd()) {               
        zeile = inStream.readLine(0);
        if(zeile[0] != QChar('#'))              
          break;
     }
     while( 1 ) {
       i = 0, n = 0, p = 0;
       if(zeile[0] == QChar('#'))               
          break;;
       qy = "INSERT INTO rutable VALUES (";
       while( n != 4) {                         
          while(zeile[i] != QChar('|')) i++;    
          z = "";
          a = "";
          while(i != p) a += zeile[p++];        
          z += a.simplified();                   
          qy += "'"+z;
          if( n == 3) {
              break;
          }
          qy += "',";
          i++;                                   
          p++;                                   
          n++;
      }
      qy += "')";                                
      query.exec(qy);                            
      break;
     }
     
     
     if(zeile[0] == QChar('#'))                 
      break;
   }
   datei.close();
}
