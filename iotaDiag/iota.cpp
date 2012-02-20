/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QApplication>
#include <QtGui>
#include <QtSql>
#include "iota.h"
#include "../qtlogDiag/dirmngr.h"

iota::iota(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);                       
    setFont(font);
    
    connect(ButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
    connect(wLineEdit, SIGNAL(textEdited(QString)), this, SLOT(wLineEditCb(QString)));
    connect(iotaList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(iotaListClickedCb(QTreeWidgetItem *,int)));
    connect(ButtonBack, SIGNAL(clicked()), this, SLOT(backCb()));
    connect(ButtonUpdate, SIGNAL(clicked()), this, SLOT(updateCb()));
    connect(ButtonNeu, SIGNAL(clicked()), this, SLOT(neuCb()));
    connect(iotaBox, SIGNAL(currentIndexChanged(int)), this, SLOT(iotaBoxChangedCb(int)));
    readSettings();                                        
    //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("host").toString());     
    db.setDatabaseName(settings.value("dbname").toString());
    db.setUserName(settings.value("dbuser").toString());
    db.setPassword(settings.value("dbpasswd").toString());
    if(!db.open()) {
       qDebug() << "usrDB_FEHLER - " << db.lastError();    
    }
    iotaList->clear();
    iotaList->setColumnWidth(0,80);              
    iotaList->setColumnWidth(1,120);             
    qy = "SELECT * FROM tiota ORDER by iota";
    showIota(qy);
}

iota::~iota()
{
   writeSettings();
}


void iota::goExit()                               
{
    db.close();                                  
    qApp->quit();
}


void iota::readSettings()
{
    resize(settings.value("iota/Size",sizeHint()).toSize());
    restoreState(settings.value("iota/Properties").toByteArray());
}


void iota::writeSettings()
{
    settings.setValue("iota/Size",size());
    settings.setValue("iota/Properties",saveState());
}

void iota::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
   }
}

void iota::iotaBoxChangedCb(int i)
{
   n = i;
   wLineEdit->setText("");
   wLineEdit->setFocus();                        
}



void iota::showIota(QString q)
{
    iotaList->clear();
    QSqlQuery query;
    query.exec(q);
    n = query.size();
    while(query.next()) {
      i = 0;
      r = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(iotaList);
      item->setText(r++,query.value(i).toString());        
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(r++,query.value(i).toString());        
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(r++,query.value(i).toString());        
    }
    t = "Eintraege : "+t.setNum(n);
    labelCount->setText(t);
    wLineEdit->setFocus();                        
}


void iota::wLineEditCb(QString str)
{
    if(iotaBox->currentText().compare("IOTA") == 0)
         qy = "SELECT * FROM tiota WHERE "+iotaBox->currentText()+" LIKE '"+str+"%' ORDER BY iota";
    else
      if(iotaBox->currentText().compare("Dxcc") == 0) {
         t = "idxcc";
         qy = "SELECT * FROM tiota WHERE "+t+" LIKE '"+str+"%' ORDER BY idxcc";
      }
    else
     if(iotaBox->currentText().compare("Name") == 0) {
         t = "iconty";
         qy = "SELECT * FROM tiota WHERE "+t+" LIKE '"+str+"%' ORDER BY iota";
     }
    showIota(qy);
}



void iota::iotaListClickedCb(QTreeWidgetItem * item, int i)
{
     int d = i;
     d = 0;
     wEditIota->setText(item->text(0));
     wEditDxcc->setText(item->text(1));
     wEditName->setText(item->text(2));
     uItem = item;                          
     tabWidget->setCurrentIndex(1);
}

void iota::backCb()
{
    tabWidget->setCurrentIndex(0);
}


void iota::updateCb()
{
   if(wEditIota->text().compare(uItem->text(0)) != 0) {
      QMessageBox::information( this, tr("Info"),
      tr("\n\nKein Update fuer IOTA-Kenner !\n"),
      QMessageBox::Ok);
   }
   else {
     QSqlQuery query;
     tabWidget->setCurrentIndex(0);          
     qy = "UPDATE tiota Set idxcc='"+wEditDxcc->text();
     qy += "',iconty='"+wEditName->text();
     qy += "' WHERE iota='"+uItem->text(0)+"'";
     query.exec(qy);
     uItem->setText(1,wEditDxcc->text());
     uItem->setText(2,wEditName->text());
   }
}


void iota::neuCb()
{
   QSqlQuery query;                     
   qy = "SELECT iota FROM tiota WHERE iota='"+wEditIota->text()+"'";
   query.exec(qy);
   if(query.size() != 0) {
     QMessageBox::information( this, tr("Info"),
     tr("\nIOTA  schon vorhanden !\n"),
     QMessageBox::Ok);
     return;
   }
   qy = "INSERT INTO tiota VALUES ('"+wEditIota->text();
   qy += "','"+wEditDxcc->text();
   qy += "','"+wEditName->text();
   qy += "')";
   query.exec(qy);
   tabWidget->setCurrentIndex(0);             
   QTreeWidgetItem *item = new QTreeWidgetItem(iotaList);
   item->setText(0,wEditIota->text());        
   item->setTextAlignment(0,Qt::AlignCenter);
   item->setText(1,wEditDxcc->text());        
   item->setTextAlignment(1,Qt::AlignCenter);
   item->setText(2,wEditName->text());        
}
