/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QSettings>
#include <QString>
#include <QTabWidget>
#include <QtGui>
#include <QtSql>
#include "qrz.h"
#include "../qtlogDiag/dirmngr.h"

Qrz::Qrz(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n); 			
   setFont(font);
   connect(actionExit, SIGNAL(triggered(bool)), this, SLOT(goExit()));
   connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(ButtonCdExit, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(actionQrzTabelle, SIGNAL(triggered(bool)), this, SLOT(rdCall()));
   connect(actionQrzRead, SIGNAL(triggered(bool)), this, SLOT(cdRead()));
   connect(actionImageView, SIGNAL(triggered(bool)), this, SLOT(imageView()));
   connect(actionHilfeQrz, SIGNAL(triggered(bool)), this, SLOT(getHilfeQrzCb()));
   connect(PathButton, SIGNAL(clicked()), this, SLOT(getQrzFilePathCb()));
   connect(ButtonLoadDBdata, SIGNAL(clicked()), this, SLOT(loadDataCb()));          
   connect(checkBoxCall, SIGNAL(clicked()), this, SLOT(checkCallChangedCb()));      
   connect(checkBoxImage, SIGNAL(clicked()), this, SLOT(checkImageChangedCb()));    
   connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxChangedCb(int))); 
   connect(lookupW, SIGNAL(returnPressed()), this, SLOT(getCallData()));
   connect(qrzListView, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(qrzListClickedCb(QTreeWidgetItem*,int)));
   connect(ButtonBack, SIGNAL(clicked()), this, SLOT(backToPage0Cb()));
   //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
   db = QSqlDatabase::addDatabase("QMYSQL");
   db.setHostName(settings.value("host").toString()); 
   db.setDatabaseName(settings.value("dbname").toString());
   db.setUserName(settings.value("dbuser").toString());
   db.setPassword(settings.value("dbpasswd").toString());
   if(!db.open()) {
       qDebug("err");                                 
   }
   tname = "icall";
   qrzListView->setColumnWidth(2,60);             
   QSqlQuery query;
   qy = "SELECT COUNT(*) FROM qrzbkc";
   query.exec(qy);                                 
   query.next();
   s = query.value(0).toString();
   s.insert(1,QString("."));
   s.insert(5,QString("."));
   s += tr("  - Rufzeichen");
   dbInfo->setText(s);                             
   lookupW->setFocus();
}
Qrz::~Qrz()
{
}

void Qrz::goExit()
{
  db.close();
  qApp->quit();
}


void Qrz::getHilfeQrzCb()
{
   settings.setValue("Val","QRZ Callbook");
   settings.sync();
   StartProcess("hilfedb &");
}

void Qrz::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
   }
}

void Qrz::cdRead()
{
   stackedWidget->setCurrentIndex(1);
}

void Qrz::rdCall()
{
   stackedWidget->setCurrentIndex(0);
}

void Qrz::imageView()
{
   stackedWidget->setCurrentIndex(2);
}

void Qrz::checkCallChangedCb()
{
   if(checkBoxCall->isChecked() == true)
      checkBoxImage->setChecked(false);
}

void Qrz::checkImageChangedCb()
{
   if(checkBoxImage->isChecked() == true)
      checkBoxCall->setChecked(false);
}

void Qrz::comboBoxChangedCb(int index)
{
   switch( index ) {
      case 0 : tname = "icall";
               break;
      case 1 : tname = "mcity";
               break;
      case 2 : tname = "fname";
               break;
   }
}

void Qrz::backToPage0Cb()
{
    stackedWidget->setCurrentIndex(0);            
}



void Qrz::qrzListClickedCb(QTreeWidgetItem * item, int in)
{
    QSqlQuery query;
    qy = "SELECT icall,fname,lname,mcity,mstreet FROM qrzbkc ";
    qy += "WHERE icall='";
    qy += item->text(0)+"'";
    query.exec(qy);
    query.next();                                        
    n = 0;
    i = 0;
    labelCall->setText(query.value(n++).toString());     
    labelVname->setText(query.value(n++).toString());    
    labelNname->setText(query.value(n++).toString());    
    labelQth->setText(query.value(n++).toString());      
    labelStreet->setText(query.value(n++).toString());   
    if( in == 2 ) {                               
       if(item->text(2).compare("*") == 0) {      
          QImage bild;                            
          QDir dir;
          s = dir.homePath();
          s += "/log/images/";                    
          if( dir.exists(s)) {
             s += item->text(0)+".jpg";           
             s = s.toLower();                     
             BildLabel->setPixmap(QPixmap(s));    
             BildLabel->adjustSize();             
             labelName->setText(item->text(1));   
             labelCall2->setText(item->text(0));  
             stackedWidget->setCurrentIndex(2);   
          }
       }
       else {
          qDebug() << "Ordner nicht vorhanden";   
       }
    }
    lookupW->setFocus();
}





void Qrz::getCallData()
{
  int x;
   QSqlQuery query;
   qrzListView->clear();                                 
   qy = "SELECT icall,fname,jpg,mcity FROM qrzbkc ";
   qy += "WHERE "+tname+" LIKE '"+lookupW->text()+"%' LIMIT 200";
   query.exec(qy);
   x = query.size();
   while(query.next()) {                                 
      n = 0;
      i = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(qrzListView);
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i,query.value(n++).toString());      
      item->setTextAlignment (i++, Qt::AlignHCenter);
      item->setText(i++,query.value(n++).toString());    
   }
   labelCall->setText("");                                
   labelVname->setText("");                               
   labelNname->setText("");                               
   labelQth->setText("");
   labelStreet->setText("");
}


QString Qrz::DirSelector()
{
   if( !dirsel ) {
     QString directory = QFileDialog::getExistingDirectory(this,
             tr("Waehle einen Ordner"), QDir::homePath());
     return directory;
   }
   else {
     QString fileName = QFileDialog::getOpenFileName(this,
             tr("Open File"),"/", tr("( *.dat)"));
     return fileName;
   }
}



void Qrz::getQrzFilePathCb()
{
   dirsel = 0;
   lineEditPath->setText(DirSelector());   
}

void Qrz::loadDataCb()
{
     if(checkBoxCall->isChecked() == true)
         loadCallBkcdatDb();
     else
      if(checkBoxImage->isChecked() == true)
         loadImageDB();
     else {
         QMessageBox::about(this,"Info",tr("\nWaehle erst eine Datenbank "));
         return;
     }
}




void Qrz::loadCallBkcdatDb()
{
 QString q, zeile, t, k;
 int numFiles, n, nn, tok, index, linecnt;
     QString s = lineEditPath->text()+"callbkc.dat";
     QFile file(s);
     if (!file.open(QIODevice::ReadOnly)) {
         QMessageBox::about(this,"Info",tr("\nKann QRZ-CD nicht oeffnen"));
         return;
     }
     t.sprintf("%s/",(char*)getenv("HOME"));
     t += "qrzCallbk.db";
     QTextStream inStream(&file);
     QFile data(t);
     data.open(QFile::WriteOnly | QFile::Truncate);
     QTextStream out(&data);
     QSqlField field("feld",QVariant::String);
     numFiles = 1400400;                                
     QProgressDialog progress(tr("\nLade  ~1.200.000 QRZ Rufzeichen-Datensaetze"), tr("Abbruch ..."), 0, numFiles, this);
     progress.setWindowModality(Qt::WindowModal);
     linecnt = 0;
     QSqlQuery query;
     qy = "DELETE FROM qrzbkc";                         
     query.exec(qy);
     dbInfo->setText("");                               
     while( !inStream.atEnd()) {
        progress.setValue(linecnt);
        qApp->processEvents();
        if (progress.wasCanceled())
           break;
        zeile = inStream.readLine(0);                   
        if(zeile.count() > 16) {                        
          index = 0;
          tok = 0;
          nn = 0;
          q = "";
          while( nn != 14) {
             tok = zeile.indexOf(',',index);             
             k = zeile.mid(index, tok -index);           
             k =  k.simplified();
             if( nn == 0 ) {                             
                n = k.indexOf(' ',0);                    
                if(n != -1 )
                   k.remove(n, 1);                       
             }
             field.setValue(k);
             q += db.driver()->formatValue(field,false); 
             q += ",";
             if( nn == 13 ) break;
             index = tok;
             index++;
             nn++;
          }
          index = tok;
          index++;
          tok = zeile.indexOf('\n',index);          
          k = zeile.mid(index, tok -index);         
          q +=  k.simplified();
          q += "'"+k+"'\n";
          out << q;
          linecnt++;
         }
    }
    file.close();
    data.close();
    insertData();
    qy = "SELECT COUNT(*) FROM qrzbkc";
    query.exec(qy);                                 
    query.next();
    s = query.value(0).toString();
    s.insert(1,QString("."));
    s.insert(5,QString("."));
    s += tr("  - Rufzeichen");
    dbInfo->setText(s);                            
}


void Qrz::insertData()
{
  QString t, q;
     QSqlQuery query;
     t.sprintf("%s/",(char*)getenv("HOME"));
     t += "qrzCallbk.db";
     qy = "LOAD DATA INFILE '"+t+"' REPLACE INTO table qrzbkc ";
     qy += " fields terminated by ',' enclosed by ''''";
     query.exec(qy);
     q = "rm "+t+" &";
     system ( q.toAscii() );
}



void Qrz::loadImageDB()
{
 QString tmpDir, dir, q, fdir, c;
 int linecnt, count, nt;
     dir = lineEditPath->text();
     QDir dr( dir );
     if( !dr.exists() ) {
       QMessageBox::about(this,tr("Info !!"),tr("Kann image-ordner auf der CD nicht finden"));
       return;
     }
     
     tmpDir.sprintf("%s/",(char *)getenv("HOME"));      
     tmpDir += "log/images/";
     
     
     count = 11500;
     linecnt = 0;
     QProgressDialog progress(tr("\nKopiere  ~11.000 QRZ - Bilder nach (HOME)/gifs"), tr("Abbruch ..."), 0, count, this);
     progress.setWindowModality(Qt::WindowModal);
     progress.setValue(linecnt);
     qApp->processEvents();
     dr.setFilter( QDir::Dirs );                    
     dr.setSorting( QDir::Name );                   
     QFileInfoList list = dr.entryInfoList();       
     for (int i = 0; i < list.size(); ++i) {        
         QFileInfo fileInfo = list.at(i);
         if(fileInfo.fileName() == "." || fileInfo.fileName() == ".." )     
           ;
         else {
            
            fdir = dir+fileInfo.fileName();
            fdir += "/";
            QDir d( fdir );
            d.setFilter( QDir::Files );
            QFileInfoList f = d.entryInfoList();
            for(int n = 0; n < f.size(); ++n) {     
               QFileInfo fInfo = f.at(n);
               s = fInfo.fileName();
               c = "cp "+fdir+s;
               if(s.indexOf(".jpg") != -1) {         
                  c += " "+tmpDir;
                  linecnt++;
                  
                  system(c.toAscii());               
                  progress.setValue(linecnt);
                  qApp->processEvents();
                  if (progress.wasCanceled())
                     break;
               }
            }
         }
     }
     
     
     count = 11500;
     linecnt = 0;
     QProgressDialog progres(tr("\nMarkiere QRZ -Rufzeichenbank mit Image-Flg '*'"), tr("Abbruch ..."), 0, count, this);
     progres.setWindowModality(Qt::WindowModal);
     progres.setValue(linecnt);
     qApp->processEvents();
     dr.cd(tmpDir);
     if(!dr.exists()) {
       qDebug() << "FEHLER";
       return;
     }
     QSqlQuery query;
     qy = "UPDATE qrzbkc SET jpg=''";               
     query.exec(qy);

     dr.setFilter( QDir::Files );                   
     QFileInfoList f = dr.entryInfoList();          
     for(int n = 0; n < f.size(); ++n) {            
        QFileInfo fInfo = f.at(n);
        s = fInfo.fileName();                       
        if(fInfo.fileName() == "." || fInfo.fileName() == ".." ) 
           ;
         else {
             QFileInfo fInfo = f.at(n);             
             s = fInfo.fileName();                  
             nt = s.indexOf(".",0);                 
             c = s.left(nt);                        
             qy = "UPDATE qrzbkc SET jpg = '*' WHERE icall='"+c+"'"; 
             query.exec(qy);                                         
             linecnt++;
             
             progres.setValue(linecnt);
             qApp->processEvents();
             if (progres.wasCanceled())
               break;
         }
     }
}
