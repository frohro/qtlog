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
#include "dok.h"
#include "../qtlogDiag/dirmngr.h"
extern DirMngr dirMngr;

dok::dok(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n); 			
    setFont(font);
   
    connect(ButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
    connect(dokListe, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(dokListeClickedCb(QTreeWidgetItem *,int)));
    connect(sdokListe, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(sdokListeClickedCb(QTreeWidgetItem *,int)));

    connect(ButtonUpdate, SIGNAL(clicked()), this, SLOT(dokUpdateCb()));
    connect(ButtonNEU, SIGNAL(clicked()), this, SLOT(dokNeuCb()));
    connect(ButtonDEL, SIGNAL(clicked()), this, SLOT(dokDeleteCb()));
    connect(wlineEditDok, SIGNAL(textEdited(QString)), this, SLOT(getDokCb(QString)));
    connect(BoxDOK, SIGNAL(currentIndexChanged(int)), this, SLOT(boxDokChangedCb(int)));
    connect(wEditDok, SIGNAL(textEdited(QString)), this, SLOT(wEditDokCb(QString)));
    connect(wEditDxcc, SIGNAL(textEdited(QString)), this, SLOT(wEditDxccCb(QString)));

    connect(ButtonSdokUpdate, SIGNAL(clicked()), this, SLOT(sdokUpdateCb()));
    connect(ButtonSdokNeu, SIGNAL(clicked()), this, SLOT(sdokNeuCb()));
    connect(ButtonSdokDel, SIGNAL(clicked()), this, SLOT(sdokDeleteCb()));
    connect(wlineEditSdok, SIGNAL(textEdited(QString)), this, SLOT(getSdokCb(QString)));
    connect(BoxSDOK, SIGNAL(currentIndexChanged(int)), this, SLOT(boxSdokChangedCb(int)));
    connect(wEditSdok, SIGNAL(textEdited(QString)), this, SLOT(wEditSdokCb(QString)));
    connect(wEditCall, SIGNAL(textEdited(QString)), this, SLOT(wEditCallCb(QString)));
    connect(wEditHdok, SIGNAL(textEdited(QString)), this, SLOT(wEditHdokCb(QString)));
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabwidgetCurrentChangedCb(int)));
    connect(ButtonHilfe, SIGNAL(clicked()), this, SLOT(getHilfeCb()));
    //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("host").toString());      
    db.setDatabaseName(settings.value("dbname").toString());
    db.setUserName(settings.value("dbuser").toString());
    db.setPassword(settings.value("dbpasswd").toString());
    if(!db.open()) {
       qDebug() << "usrDB_FEHLER - " << db.lastError();     
    }
    readSettings();                              
    sdokListe->clear();
    sdokListe->setColumnWidth(0,80);             
    sdokListe->setColumnWidth(1,90);             
    sdokListe->setColumnWidth(2,150);            
    sdokListe->setColumnWidth(3,80);             
    sdokListe->setColumnWidth(4,100);            
    sdokListe->setColumnWidth(5,100);            
    dokListe->clear();
    dokListe->setColumnWidth(0,60);              
    dokListe->setColumnWidth(1,160);             
    dokListe->setColumnWidth(2,50);              
    tabWidget->setCurrentIndex(0);
    tabIdx = 0;

    QDate heute = QDate::currentDate();         
    dateEditVon->setDate(heute);                
    heute = heute.addMonths(12);                
    dateEditBis->setDate(heute);
    qy = "SELECT * FROM tdok WHERE kdxcc = 'DL' ORDER BY dok";
    showDok(qy);
}

dok::~dok()
{
   writeSettings();
}


void dok::goExit()                               
{
    db.close();                                  
    qApp->quit();
}

void dok::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
   }
}


void dok::readSettings()
{
    resize(settings.value("dok/Size",sizeHint()).toSize());
    restoreState(settings.value("dok/Properties").toByteArray());
}


void dok::writeSettings()
{
    settings.setValue("dok/Size",size());
    settings.setValue("dok/Properties",saveState());
}

void dok::tabwidgetCurrentChangedCb(int i)
{
   switch(i) {
      case 0: break;               
      case 1: break;               
      case 2 : if( tabIdx != 3 )
                 getSdokList();    
               tabIdx = 2;
              break;
      case 3 :                     
              break;
   }
}
void dok::getHilfeCb()
{
   settings.setValue("Val","DOK Verwaltung");
   StartProcess("hilfedb &");
}

void dok::wEditDokCb(QString st)
{
    QString f;
    f = st.toUpper();
    wEditDok->setText(f);
}

void dok::wEditDxccCb(QString st)
{
    QString f;
    f = st.toUpper();
    wEditDxcc->setText(f);
}



void dok::showDok(QString q)
{
    dokListe->clear();
    QSqlQuery query;
    query.exec(q);
    n = query.size();
    while(query.next()) {
      i = 0;
      r = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(dokListe);
      item->setText(r++,query.value(i).toString());        
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(r++,query.value(i++).toString());      
      item->setText(r++,query.value(i).toString());        
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(r++,query.value(i++).toString());      
      item->setText(r++,query.value(i++).toString());      
    }
    t = BoxDOK->currentText();
    t += " : "+s.setNum(n);
    t += tr("   Eintraege");
    DokCntLabel->setText(t);
    wlineEditDok->setFocus();                   
}



void dok::getDokCb(QString str)
{
    QString f;
    f = str.toUpper();
    wlineEditDok->setText(f);
    qy = "SELECT * FROM tdok WHERE kdxcc ='"+BoxDOK->currentText()+"' AND dok LIKE '"+f+"%' ORDER BY dok";
    showDok(qy);
}


void dok::boxDokChangedCb(int id)
{
  id = 0;
  wlineEditDok->setFocus();
}



void dok::dokListeClickedCb(QTreeWidgetItem * item, int i)
{
     int d = i;
     d = 0;
     wEditDok->setText(item->text(0));
     wEditOVname->setText(item->text(1));
     wEditDxcc->setText(item->text(2));
     wEditDistrikt->setText(item->text(3));
     wEditBemerkung->setText(item->text(4));
     tabWidget->setCurrentIndex(1);
     uItem = item;
}


//----------------------------------------------------------
void dok::dokUpdateCb()
{
   if(wEditDok->text().count() == 0)
     return;
   QSqlQuery query;
   qy = "UPDATE tdok SET ovnam='"+wEditOVname->text();
   qy += "',kdxcc='"+wEditDxcc->text();
   qy += "',disnam='"+wEditDistrikt->text();
   qy += "',bemerk='"+wEditBemerkung->text();
   qy += "' WHERE dok = '"+wEditDok->text()+"'";
   query.exec(qy);
   uItem->setText(1,wEditOVname->text());
   uItem->setText(2,wEditDxcc->text());
   uItem->setText(3,wEditDistrikt->text());
   uItem->setText(4,wEditBemerkung->text());
   tabWidget->setCurrentIndex(0);
}



void dok::dokNeuCb()
{
   if(wEditDok->text().count() == 0)
     return;
   QSqlQuery query;
   qy = "SELECT dok FROM tdok WHERE dok='"+wEditDok->text()+"'";
   query.exec(qy);
   if(query.size() != 0) {
     
     return;
   }
   qy = "INSERT INTO tdok VALUES ('";
   qy += wEditDok->text();
   qy += "','"+wEditOVname->text();
   qy += "','"+wEditDxcc->text();
   qy += "','"+wEditDistrikt->text();
   qy += "','"+wEditBemerkung->text();
   qy += "')";
   query.exec(qy);
   r = 0;
   QTreeWidgetItem *item = new QTreeWidgetItem(dokListe);
   item->setText(r,wEditDok->text());              
   item->setTextAlignment(r++,Qt::AlignCenter);
   item->setText(r++,wEditOVname->text());         
   item->setText(r,wEditDxcc->text());             
   item->setTextAlignment(r++,Qt::AlignCenter);
   item->setText(r++,wEditDistrikt->text());       
   item->setText(r++,wEditBemerkung->text());      
   tabWidget->setCurrentIndex(0);
}



void dok::dokDeleteCb()
{
    QSqlQuery query;
    qy = " DELETE FROM tdok WHERE dok='"+wEditDok->text()+"'";
    query.exec(qy);
    n = dokListe->indexOfTopLevelItem(uItem);
    dokListe->takeTopLevelItem( n );
    tabWidget->setCurrentIndex(0);
}



void dok::wEditSdokCb(QString st)
{
    QString f;
    f = st.toUpper();
    wEditSdok->setText(f);
}

void dok::wEditCallCb(QString st)
{
    QString f;
    f = st.toUpper();
    wEditCall->setText(f);
}
//-------------------------------
void dok::wEditHdokCb(QString st)
{
    QString f;
    f = st.toUpper();
    wEditHdok->setText(f);
}



void dok::getSdokList()
{
    qy = "SELECT * FROM tsdok";
    getSdokData(qy);
}



void dok::getSdokData(QString q)
{
    sdokListe->clear();
    QSqlQuery query;
    query.exec(q);
    n = query.size();
    while(query.next()) {
      i = 0;
      r = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(sdokListe);
      item->setText(r++,query.value(i++).toString());        
      item->setText(r++,query.value(i++).toString());        
      item->setText(r++,query.value(i++).toString());        
      item->setText(r++,query.value(i).toString());          
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(r++,query.value(i++).toString());        
      item->setText(r++,query.value(i++).toString());        
      item->setText(r++,query.value(i++).toString());        
    }
    t = tr("-  S-DOK  ");
    t += s.setNum(n);
    SdokCntLabel->setText(t);
    wlineEditSdok->setFocus();                   
}


void dok::boxSdokChangedCb(int id)
{
  id = 0;
  wlineEditSdok->setFocus();
}



void dok::getSdokCb(QString str)
{
    QString f;
    f = str.toUpper();
    wlineEditDok->setText(f);
    if(BoxSDOK->currentText().compare("DOK") == 0)
       s = "H"+BoxSDOK->currentText();
    else
      s = BoxSDOK->currentText();
    qy = "SELECT * FROM tsdok WHERE ";
    qy += s+" LIKE '"+f+"%' ORDER BY sdok";
    getSdokData(qy);
}



void dok::sdokListeClickedCb(QTreeWidgetItem * item, int i)
{
     int d = i;
     d = 0;
     wEditSdok->setText(item->text(0));                 
     wEditCall->setText(item->text(1));                 
     wEditAnlass->setText(item->text(2));               
     wEditHdok->setText(item->text(3));                 
     s = item->text(4);                                 
     QDate da = QDate::fromString (s,"dd-MM-yyyy");
     dateEditVon->setDate(da);
     s = item->text(5);                                 
     da = QDate::fromString (s,"dd-MM-yyyy");
     dateEditBis->setDate(da);
     wEditId->setText(item->text(6));                   
     tabWidget->setCurrentIndex(3);
     uItem = item;
}


//--------------------------------------------------------
void dok::sdokUpdateCb()
{
   if(wEditSdok->text().count() == 0)
     return;
   QSqlQuery query;
   qy = "UPDATE tsdok SET sdok='"+wEditSdok->text();
   qy += "',acall='"+wEditCall->text();
   qy += "',anlass='"+wEditAnlass->text();
   qy += "',hdok='"+wEditHdok->text();
   qy += "',von='";                           
   QDate d = dateEditVon->date();             
   s = "";
   t.setNum(d.day());                         
   if(t.count() == 1)
      s = "0";
   qy += s+t+"-";
   t.setNum(d.month());                       
   s = "";
   if(t.count() == 1)
      s = "0";
   qy += s+t;
   qy += "-"+t.setNum(d.year());              
   qy += "',bis='";                           
   d = dateEditBis->date();                   
   s = "";
   t.setNum(d.day());
   if(t.count() == 1)
      s = "0";
   qy += s+t+"-";                             
   t.setNum(d.month());
   if(t.count() == 1)
        s = "0";
   qy += s+t;                                 
   qy += "-"+t.setNum(d.year());              
   qy += "' WHERE id="+wEditId->text();
   query.exec(qy);
   uItem->setText(0,wEditSdok->text());
   uItem->setText(1,wEditCall->text());
   uItem->setText(2,wEditAnlass->text());
   uItem->setText(3,wEditHdok->text());

   d = dateEditVon->date();                   
   s = "";
   t.setNum(d.day());                         
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
//--
   t.setNum(d.month());                       
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   s += t.setNum(d.year());                   
   uItem->setText(4,s);                       

   d = dateEditBis->date();                   
   s = "";
   t.setNum(d.day());                         
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
//--
   t.setNum(d.month());                       
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   s += t.setNum(d.year());                   
   uItem->setText(5,s);                       
   uItem->setText(6,s.setNum(n));             
   tabIdx = 3;
   tabWidget->setCurrentIndex(2);
}



void dok::sdokNeuCb()
{
   if(wEditSdok->text().count() == 0)
     return;
   QSqlQuery query;
   qy = "INSERT INTO tsdok VALUES ('";
   qy += wEditSdok->text();                   
   qy += "','"+wEditCall->text();             
   qy += "','"+wEditAnlass->text();           
   qy += "','"+wEditHdok->text();             
   qy += "','";
   QDate d = dateEditVon->date();             
   s = "";
   t.setNum(d.day());                         
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   t.setNum(d.month());                       
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
   qy += s+t.setNum(d.year());                
   qy += "','";

   d = dateEditBis->date();                   
   s = "";
   t.setNum(d.day());                         
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   t.setNum(d.month());                       
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";
   qy += s+t.setNum(d.year());                
   qy += "',0)";                              
   query.exec(qy);
   n = query.lastInsertId().toInt();
   r = 0;
   QTreeWidgetItem *item = new QTreeWidgetItem(sdokListe);
   item->setText(r++,wEditSdok->text());      
   item->setText(r++,wEditCall->text());      
   item->setText(r++,wEditAnlass->text());    
   item->setText(r,wEditHdok->text());        
   item->setTextAlignment(r++,Qt::AlignCenter);

   d = dateEditVon->date();                   
   s = "";
   t.setNum(d.day());                         
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   t.setNum(d.month());                       
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   s += t.setNum(d.year());                   
   item->setText(r++,s);                      

   d = dateEditBis->date();                   
   s = "";
   t.setNum(d.day());                         
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   t.setNum(d.month());                       
   if(t.count() == 1)
      s += "0";
   s += t;
   s += "-";

   s += t.setNum(d.year());                   
   item->setText(r++,s);                      
   item->setText(r++,s.setNum(n));            
   tabIdx = 3;
   tabWidget->setCurrentIndex(2);
   
}



void dok::sdokDeleteCb()
{
    QSqlQuery query;
    qy = " DELETE FROM tsdok WHERE id="+wEditId->text();
    query.exec(qy);
    n = sdokListe->indexOfTopLevelItem(uItem);
    sdokListe->takeTopLevelItem( n );
    tabIdx = 3;
    tabWidget->setCurrentIndex(2);
    
}
