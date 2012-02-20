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
#include "laDiag.h"
#include "../qtlogDiag/dirmngr.h"

laDiag::laDiag(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
  setupUi(this);
  setupAction();
}
void laDiag::setupAction()
{
   int n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n); 			
   setFont(font);
  //-- File ---
   connect(actionENDE, SIGNAL(triggered(bool)), this, SLOT(goExit()));
   
   //connect(action_dxcc_Liste_laden, SIGNAL(triggered(bool)), this, SLOT(readlaTextFile()));
   
   connect(actionNeue_cty_dat, SIGNAL(triggered(bool)), this, SLOT(readFileCtyWtDat()));
   /*
   connect(actionSave_adifData, SIGNAL(triggered(bool)), this, SLOT(saveAdifDataAc()));
   connect(actionSet_adifData, SIGNAL(triggered(bool)), this, SLOT(setAdifDataAc()));
   
   connect(actionSave_geoData, SIGNAL(triggered(bool)), this, SLOT(saveGeoDataAc()));
   connect(actionSet_geoData, SIGNAL(triggered(bool)), this, SLOT(setGeoDataAc()));
   
   connect(actionSave_waeData, SIGNAL(triggered(bool)), this, SLOT(saveWaeDataAc()));
   connect(actionSet_waeData, SIGNAL(triggered(bool)), this, SLOT(setWaeDataAc()));
   */
   
   connect(actionSubPrefixdb, SIGNAL(triggered(bool)), this, SLOT(subPrefixdb()));
   connect(actionHilfeLaenderListe, SIGNAL(triggered(bool)), this, SLOT(getHilfeDxListCb()));
   connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(combiBoxChangedCb(int)));
   
  
   connect(ButtonENDE, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(WlineEditPref, SIGNAL(textEdited(QString)), this, SLOT(getPraefixList(QString)));
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
   qy = "SELECT * FROM tla";
   getLaTable(qy);                                        
   
   connect (laTabelle, SIGNAL(itemChanged(QTableWidgetItem *)), this,SLOT(updateLaItem(QTableWidgetItem *)));
   dbflg = 0;
}

laDiag::~laDiag()
{
  writeSettings();
}
void laDiag::goExit()
{
  db.close();
  qApp->quit();
}

void laDiag::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
     case Qt::Key_Escape :
           WlineEditPref->setText("");
           WlineEditPref->setFocus();
           break;
   }
}

void laDiag::writeSettings()
{
  settings.setValue("laDiag/Size",size());
  settings.setValue("laDiag/Properties",saveState());
}

void laDiag::readSettings()
{
  resize(settings.value("laDiag/Size",sizeHint()).toSize());
  restoreState(settings.value("laDiag/Properties").toByteArray());
}

void laDiag::subPrefixdb()
{
   StartProcess("prefdb &");
}


void laDiag::getHilfeDxListCb()
{
   settings.setValue("Val","DX-LaenderListe");
   StartProcess("hilfedb &");
}

void laDiag::combiBoxChangedCb(int id)
{
    int i;
    i = id;
    WlineEditPref->setFocus();
}



void laDiag::getPraefixList(QString str)
{
     dbflg = 1;
     y = str.toUpper();
     WlineEditPref->setText(y);
 
     if(comboBox->currentIndex() == 0) {
       qy = "SELECT * FROM tla WHERE la LIKE '" +y+"%'";      
     }
     else
     if(comboBox->currentIndex() == 1) {
        qy = "SELECT * FROM tla WHERE lname LIKE '" +y+"%'";  
     }
     else
     if(comboBox->currentIndex() == 2) {
        qy = "SELECT * FROM tla WHERE adif LIKE '" +y+"%'";   
     }
     else
     if(comboBox->currentIndex() == 3) {
        qy = "SELECT * FROM tla WHERE lwae LIKE '" +y+"%'";   
     }
     else
     if(comboBox->currentIndex() == 4) {
        //qy = "SELECT * FROM tla WHERE lwae != '' And LIKE '" +y+"%'";   
        qy = "SELECT * FROM tla WHERE lwae != ''";   
     }
     else
     if(comboBox->currentIndex() == 5) {
        qy = "SELECT * FROM tla WHERE lcn LIKE '" +y+"%'";    
     }
     
     getLaTable(qy);
     dbflg = 0;
}




void laDiag::updateLaItem( QTableWidgetItem *item )
{
  int row, col;
  QString s;
  
    if(item != laTabelle->currentItem())                
        return;
    
    QSqlField field("feld",QVariant::String);
    if(item) {                                          
      col = laTabelle->column ( item );                 
      row = laTabelle->row( item );                     
      QTableWidgetItem * v = laTabelle->verticalHeaderItem ( row ); 
   
      if((col == 8) || (col == 10) || (col == 11)) {
	if(col == 8)
	  s = "lwae";
	if(col == 10)
	  s = "adif";                                   
	if(col == 11) 
	  s = "notiz";                                  
	
	qy = "UPDATE tla SET "+s+"=";
	field.setValue(item->data(Qt::EditRole).toString());
	qy += db.driver()->formatValue(field,false);
        qy += " WHERE la='"+v->text()+"'";
	QSqlQuery query(qy);
      }
    }
    WlineEditPref->setFocus();
}
/*




void laDiag::saveAdifDataAc()
{
  QString path, p, line;
  int n;
  
      path =  QDir::homePath();                     
      path += "/log/iniFile/";                              
      p = path+"adifCountryCodes.csv";                      

      QFile file(p);
      if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
          qDebug() << "Dein Linux ist kaputt";
         return;
      }
    
      QTextStream out(&file);
      line = "# Primary Prefix -> Adif_Nummern\n";
      out << line;
    
      QSqlQuery query; 
      qy = "SELECT la,lname,ldxcc,adif,notiz FROM tla";
      query.exec(qy); 
      n = query.size();                         
      col = query.record().count();             
      line = "";
      
      while(query.next() ) {                    
        n = 0;
	p = query.value(n++).toString();        
	p = p.leftJustified(10,' ');
	line += p;
	
	p = query.value(n++).toString();        
	p = p.leftJustified(44,' ');
	p += ":";
	line += p;
 
        p = query.value(n++).toString();        
	p = p.leftJustified(8,' ');
	p += ":";
	line += p;
	
        p = query.value(n++).toString();        
	p = p.leftJustified(8,' ');
	p += ":";
	line += p;
	
	p = query.value(n++).toString();        
	p += ";";
	line += p;
	
	out << line+"\n";
	line = "";
      }
      file.close();
}
*/
/*


void laDiag::saveWaeDataAc()
{
  int n;
  QString path, p, line;
    
      path =  QDir::homePath();                     
      path += "/log/iniFile/";                      
      p = path+"waedata.csv";                       

      QFile file(p);
      if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
          qDebug() << "Dein Linux ist kaputt";
         return;
      }
    
      QTextStream out(&file);
      line = "# Primary Prefix -> WAE\n";
      out << line;
      
      dbflg = 1;
      QSqlQuery query;
      qy = "SELECT ldxcc,lwae FROM tla WHERE lwae != ''";
      query.exec(qy); 
      line = "";
      while(query.next() ) {                    
        n = 0;
	p = query.value(n++).toString();        
	p = p.leftJustified(10,' ');
	p += ":";
	line += p;
      
      	p = query.value(n++).toString();        
	line += p;
	
	out << line+"\n";
	line = "";
      }
      file.close();
}
*/
/*




void laDiag::saveGeoDataAc()
{
  QString path, p, line;
  int n;
  
      path =  QDir::homePath();                     
      path += "/log/iniFile/";                     
      p = path+"geodata.csv";                      

      QFile file(p);
      if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
          qDebug() << "Dein Linux ist kaputt";
         return;
      }
    
      QTextStream out(&file);
      line = "# geographische Koordinaten\n";
      out << line;
    
      QSqlQuery query; 
      qy = "SELECT la,lname,lcn,ituz,cqz,timez,br,lg,notiz FROM tla";
      query.exec(qy); 
      n = query.size();                         
      col = query.record().count();             
      line = "";
      
      while(query.next() ) {                    
        n = 0;
	p = query.value(n++).toString();        
	p = p.leftJustified(10,' ');
	p += ":";
	line += p;
	
        p = query.value(n++).toString();        
	p = p.leftJustified(44,' ');
	line += p;
	
	p = query.value(n++).toString();        
	p = p.leftJustified(8,' ');
	line += p;
	
	p = query.value(n++).toString();        
	p = p.leftJustified(8,' ');
	line += p;
	
	p = query.value(n++).toString();        
	p = p.leftJustified(8,' ');
	line += p;
	
	p = query.value(n++).toString();        
	p = p.leftJustified(8,' ');
	p += ":";
	line += p;
	
	p = query.value(n++).toString();        
	p = p.leftJustified(8,' ');
	p += ":";
	line += p;
	
	p = query.value(n++).toString();        
	p = p.leftJustified(8,' ');
	p += ":";
	line += p;
	
	p = query.value(n++).toString();        
	p += ";";
	line += p;
	
	out << line+"\n";
	line = "";
      }
      file.close();
}
*/
/*




void laDiag::setAdifDataAc()
{
  QString path, p, dxcc, adif, zeile;
  int i;
  
      path =  QDir::homePath();                     
      path += "/log/iniFile/";                              
      p = path+"adifCountryCodes.csv";                      
      

      QFile datei(p);
      if (!datei.open(QIODevice::ReadOnly)) {
          qDebug() << "Dein Linux ist kaputt";
         return;
      }
      
      QSqlQuery query;
      i = 0;
      QTextStream inStream(&datei);           
      while(1) {
         zeile = inStream.readLine(0);
         if(zeile[0] == QChar('#'))           
          break;
      }
      
      do {
	  p = "";
	  zeile = inStream.readLine(0);
	  i = zeile.indexOf(":");
	  i++;
	  while(zeile[i] != QChar(':')) {
	    p += zeile[i++];
	  }
	  dxcc = p.simplified();
	  i++;
	  p = "";
	  while(zeile[i] != QChar(':')) {
	    p += zeile[i++];
	  }
	  adif = p.simplified();
	  qy = "UPDATE tla SET adif="+adif+" WHERE ldxcc='"+dxcc+"'";
	  query.exec(qy); 
      } while(inStream.atEnd() != true);
      datei.close();
      qy = "SELECT * FROM tla";
      getLaTable(qy);                               
}
*/
/*

void laDiag::setWaeDataAc()
{
  QString path, p, dxcc, wae, zeile;
  int i;
  
      path =  QDir::homePath();                     
      path += "/log/iniFile/";                              
      p = path+"waedata.csv";                               

      QFile datei(p);
      if (!datei.open(QIODevice::ReadOnly)) {
          qDebug() << "Dein Linux ist kaputt";
         return;
      }
      
      QSqlQuery query;
      i = 0;
      QTextStream inStream(&datei);           
      while(1) {
         zeile = inStream.readLine(0);
         if(zeile[0] == QChar('#'))          
           break;
      }
      
      do {
	  p = "";
	  zeile = inStream.readLine(0);
	  i = 0;
	  while(zeile[i] != QChar(':')) {
	    p += zeile[i++];
	  }
	  dxcc = p.simplified();
	  
	  i++;
	  zeile = zeile.remove(0,i);
	  wae = zeile.simplified();
	  qy = "UPDATE tla SET lwae='"+wae+"' WHERE ldxcc='"+dxcc+"'";
	  query.exec(qy); 
      } while(inStream.atEnd() != true);
      datei.close();
      
      qy = "SELECT * FROM tla";
      getLaTable(qy);                               
}
*/
/*



void laDiag::setGeoDataAc()
{
  QString path, p, la, br, lg, zeile;
  int i;
  
      path =  QDir::homePath();                     
      path += "/log/iniFile/";                     
      p = path+"geodata.csv";                      

      QFile datei(p);
      if (!datei.open(QIODevice::ReadOnly)) {
          qDebug() << "Dein Linux ist kaputt";
         return;
      }
      
      QSqlQuery query;
      i = 0;
      QTextStream inStream(&datei);           
      while(1) {
         zeile = inStream.readLine(0);
         if(zeile[0] == QChar('#'))           
           break;
      }
      
      do {
	  i = 0;
	  p = "";
	  zeile = inStream.readLine(0);
	  while(zeile[i] != QChar(':')) {        
	    p += zeile[i++];
	  }
	  la = p.simplified();
	  i++;
	  zeile = zeile.remove(0,i);
	  
	  i = zeile.indexOf(":");
	  i++;
	  p = "";
	  while(zeile[i] != QChar(':')) {        
	    p += zeile[i++];
	  }
	  br = p.simplified();
	  i++;
	  zeile = zeile.remove(0,i);
	  
	  i = 0;
	  p = "";
	  while(zeile[i] != QChar(':')) {       
	    p += zeile[i++];
	  }
	  lg = p.simplified();
	  
	  qy = "UPDATE tla SET br='"+br+"',lg='"+lg+"' WHERE la='"+la+"'";
	  query.exec(qy); 
      } while(inStream.atEnd() != true);
      datei.close();
      qy = "SELECT * FROM tla";
      getLaTable(qy);                               
}
*/
