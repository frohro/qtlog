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
#include "awdst.h"
#include "dxlist.h"
#include "../qtlogDiag/version.h"
#include "../qtlogDiag/dirmngr.h"

awdst::awdst(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n); 			
    setFont(font);
    connect(actionExit, SIGNAL(triggered(bool)), this, SLOT(goExit()));
    connect(actionDxStatus, SIGNAL(triggered(bool)), this, SLOT(getDxStatus()));
    connect(actionJahresMode, SIGNAL(triggered(bool)), this, SLOT(getModeAktivitaet()));
    connect(actionJahresBand, SIGNAL(triggered(bool)), this, SLOT(getBandAktivitaet()));
    connect(actionMakeDxStatusList, SIGNAL(triggered(bool)), this, SLOT(makedxccStatusListAc()));
    connect(actionAwardDLD, SIGNAL(triggered(bool)), this, SLOT(awStatusDldAc()));
    connect(actionAwardIOTA, SIGNAL(triggered(bool)), this, SLOT(awStatusIotaAc()));
    connect(actionHilfe, SIGNAL(triggered(bool)), this, SLOT(getHilfeAc()));
    connect(checkBoxBand, SIGNAL(clicked()), this, SLOT(checkBoxBandCb()));
    connect(ButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
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
    DxLabelCall->setText(settings.value("Call").toString());
    labelLogBook->setText(settings.value("logfile").toString());
    logFile = settings.value("Logfile").toString();

    QSqlQuery query;
    qy = "SELECT mode FROM wmode WHERE work != 0 ORDER by work";
    query.exec(qy);
    n = 0;
    while(query.next()) {                                
      i = 0;
      BoxMode->insertItem(n, query.value(i).toString()); 
    }

    
    n = 4;
    qy = "SELECT band,mband FROM wband WHERE work != 0 ORDER BY work";  
    query.exec(qy);
    hcount = query.size();
    while(query.next()) {
      i = 0;
      s = query.value(i++).toString();           
     ms = query.value(i++).toString();           
      toMband.insert(s,ms);                      
      toSband.insert(ms,s);                      
      StatusList->headerItem()->setText(n,ms);
      StatusList->setColumnWidth(n++,45);
      bandlist.append(s);                        
    }
    qy = "SELECT atype FROM wawdlist WHERE aset !='0' ORDER BY aset";
    query.exec(qy);
    n = 0;
    while(query.next())                          
      BoxAwd->insertItem(n++, query.value(0).toString());  
    BoxAwd->setCurrentIndex(settings.value("BoxAwd").toString().toInt()); 
    s = settings.value("Val").toString();
    if(s.compare("0") == 0 ) {
      n = BoxMode->findText("SSB");
      BoxMode->setCurrentIndex (n);
      getDxStatus();                             
    }
    else
    if(s.compare("1") == 0 ) {
       n = BoxMode->findText("CW");
       BoxMode->setCurrentIndex (n);
       getDxStatus();                             
    }
    else
    if(s.compare("2") == 0 )                     
       getModeAktivitaet(); 
    else
    if(s.compare("3") == 0 )                     
       getBandAktivitaet();
    else
    if(s.compare("4") == 0 ) {                   
      n = BoxAwd->findText("DLD");               
      BoxAwd->setCurrentIndex (n);
      awStatusDLD("DLD");
    }
    else
    if(s.compare("5") == 0 ) {                   
      n = BoxAwd->findText("IOTA");              
      BoxAwd->setCurrentIndex (n);
      awStatusIOTA("IOTA");
    }
    connect(BoxMode, SIGNAL(currentIndexChanged(QString)), this, SLOT(setDxModeCb(QString)));
    connect(BoxAwd, SIGNAL(currentIndexChanged(QString)), this, SLOT(BoxAwdCb(QString)));
}

awdst::~awdst()
{
   db.close();
   writeSettings();
}


void awdst::writeSettings()
{
    settings.setValue("awdst/Size",size());
    settings.setValue("awdst/Properties",saveState());
}


void awdst::readSettings()
{
    resize(settings.value("awdst/Size",sizeHint()).toSize());
    restoreState(settings.value("awdst/Properties").toByteArray());
}


void awdst::goExit()                             
{
    db.close();                                  
    qApp->quit();
}

void awdst::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
    default : break;
   }
}


void awdst::getHilfeAc()
{
   settings.setValue("Val","Awd-Status");
   StartProcess("hilfedb &");
}


QString awdst::getSysband(QString myband)
{
   QHash<QString, QString>::const_iterator i = toSband.find(myband);
   if(i == toMband.end())
      return "?";
   return i.value();
}


QString awdst::getMyband(QString sband)
{
   QHash<QString, QString>::const_iterator i = toMband.find(sband);
   if(i == toSband.end())
      return "?";
   return i.value();
}


void awdst::setDxModeCb(QString text)
{
    s = text;
    getDxStatus();
}


void awdst::awStatusDldAc()
{
    n = BoxAwd->findText("DLD");
    BoxAwd->setCurrentIndex (n);
    if(n == 0)
      awStatusDLD("DLD");
}


void awdst::awStatusIotaAc()
{
    n = BoxAwd->findText("IOTA");
    BoxAwd->setCurrentIndex (n);
}



void awdst::checkBoxBandCb()
{
   if(checkBoxBand->isChecked() == true)
      getAWDband(BoxAwd->currentText());      
   else
    BoxAwdCb(BoxAwd->currentText());          
}



void awdst::getDxStatus()
{
 int ht, ri, r,gCount, bCount, lcnt, x;
 QString m;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    
    StatusList->clear();                         
    StatusList->setColumnWidth(0,60);            
    StatusList->setColumnWidth(1,65);            
    StatusList->setColumnWidth(2,90);            
    StatusList->setColumnWidth(3,80);            
    
    QSqlQuery query;
    gCount = 0;
    bCount = 0;
    lcnt = 0;
    ht = 4;
    qy = "SELECT dxpref,mode,cntx,cntb";                  
    while(ht != hcount +4) {
      qy += ",b"+getSysband(StatusList->headerItem()->text(ht++));
    }
    qy += " FROM "+labelLogBook->text();
    qy += "dx WHERE mode='"+BoxMode->currentText();
    qy += "' ORDER BY dxpref";
    query.exec(qy);
    r = query.size();
    m = m.setNum(r)+QString::fromUtf8("  Länder");
    labelData->setText(m);
    while(query.next()) {
      r = 0;
      ri = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(StatusList);
      item->setText(r++,query.value(ri).toString());       
      item->setTextAlignment(ri++,Qt::AlignCenter);
      item->setText(r++,query.value(ri).toString());       
      item->setTextAlignment(ri++,Qt::AlignCenter);
      item->setText(r++,query.value(ri).toString());       
      x = query.value(ri).toInt();
      item->setTextAlignment(ri++,Qt::AlignCenter);
      gCount = gCount + x;
      item->setText(r++,query.value(ri).toString());       
      x = query.value(ri).toInt();
      item->setTextAlignment(ri++,Qt::AlignCenter);
      bCount = bCount + x;
      while( ri != hcount +4) {
	if(ri == 4) {
	  StatusList->setColumnWidth(ri,55);               
	  item->setText(r++,query.value(ri).toString());   //
          item->setTextAlignment(ri++,Qt::AlignCenter);
	}
	else {
         item->setText(r++,query.value(ri).toString());    //
         item->setTextAlignment(ri++,Qt::AlignCenter);
	}
      }
      lcnt++;                                              
    }

    
    i = 0;
    QTreeWidgetItem *item = new QTreeWidgetItem(StatusList);     
    while(i != hcount +4) {
       item->setText(i,"-------");
       item->setTextAlignment(i++,Qt::AlignCenter);
    }
    QTreeWidgetItem *bitem = new QTreeWidgetItem(StatusList);
    bitem->setText(0,s.setNum(lcnt));                            
    bitem->setTextAlignment(0,Qt::AlignCenter);
    bitem->setText(2,s.setNum(gCount));                          
    bitem->setTextAlignment(2,Qt::AlignCenter);
    bitem->setText(3,s.setNum(bCount));                          
    bitem->setTextAlignment(3,Qt::AlignCenter);
   for(int i = 0; i < bandlist.size(); ++i) {                    
      qy = "SELECT COUNT(*) FROM ";
      qy += settings.value("Logfile").toString()+"dx";
      qy += " WHERE mode='"+BoxMode->currentText()+"' AND b";
      qy += bandlist.at(i)+"='e'";                               
      query.exec(qy);
      query.next();
      bitem->setText(i+4, query.value(0).toString());            
      bitem->setTextAlignment(i +4,Qt::AlignCenter);
   }
   stackedWidget->setCurrentIndex(0);
   QApplication::restoreOverrideCursor();
}



void awdst::getModeAktivitaet()
{
 int r, ri, ht,hd;
 QString m;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    ModeLabelCall->setText( settings.value("Call").toString());

    ModeList->clear();
    QSqlQuery query;
    qy = "SELECT mode FROM wmode WHERE work != 0 ORDER by work";
    query.exec(qy);
    hd = query.size();                                    
    ModeList->setColumnCount(hd +2);
    i = 0;
    ModeList->setColumnWidth(i++,60);
    ModeList->setColumnWidth(i++,80);
    while(query.next()) {
      n = 0;
      ModeList->headerItem()->setText(i,query.value(n++).toString());
      ModeList->setColumnWidth(i++,60);
    }

    ModeList->clear();
    qy = "SELECT COUNT(*) FROM "+settings.value("Logfile").toString();
    query.exec(qy);
    query.next();
    m = query.value(0).toString() +" QSOs";
    labelQsoCnt->setText(m);                                 

    
    QDate d = QDate::currentDate();                          
    i = d.year();                                            
    qy = "SELECT day FROM "+settings.value("Logfile").toString()+" WHERE  day > 0 LIMIT 1";
    query.exec(qy);
    h = query.size();  
    if( h ) {
      query.next();
      s = query.value(0).toString();
      s = s.left(4);
      n = s.toInt();                                           
      while( i +1 != n ) {
        r = 0;
        ri = 0;
        ht = 2;
        qy = "SELECT COUNT(*) FROM "+settings.value("Logfile").toString()+" WHERE day BETWEEN '"+s.setNum(i)+"-01-01' AND '"+s.setNum(i)+"-12-31'";
        query.exec(qy);
        query.next();
        QTreeWidgetItem *item = new QTreeWidgetItem(ModeList);
        item->setText(ri,s.setNum(i));                         
        item->setTextAlignment(ri++,Qt::AlignCenter);
        item->setText(ri,query.value(r).toString());           
        item->setTextAlignment(ri++,Qt::AlignCenter);
        while( ht != hd +2) {
          qy = "SELECT COUNT(*) FROM "+settings.value("Logfile").toString()+" WHERE mode='";
          qy += ModeList->headerItem()->text(ht++)+"' AND ";
          qy += "day BETWEEN '"+s.setNum(i)+"-01-01' AND '"+s.setNum(i)+"-12-31'";
          query.exec(qy);
          query.next();
          item->setText(ri,query.value(r).toString());       
          item->setTextAlignment(ri++,Qt::AlignCenter);
        }
        i--;
        ri--;
        item->setTextAlignment(ri,Qt::AlignLeft);
      }
   }
   stackedWidget->setCurrentIndex(1);
   QApplication::restoreOverrideCursor();
}



void awdst::getBandAktivitaet()
{
 int r, ri, ht;
 QString m;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    BandList->clear();
    QSqlQuery query;
    BandList->clear();
    qy = "SELECT mband FROM wband WHERE work != 0 ORDER BY work";  
    query.exec(qy);
    int h = 0;
    h = query.size();                                              
    BandList->setColumnCount(h +2);
    i = 0;
    BandList->setColumnWidth(i++,55);                              
    BandList->setColumnWidth(i++,65);                              
    while(query.next()) {                                          
       n = 0;
       if(n == 0 ) {
	 BandList->headerItem()->setText(i,query.value(n++).toString()); 
	 BandList->setColumnWidth(i++,55);
       }
       else {
       BandList->headerItem()->setText(i,query.value(n++).toString());   
       BandList->setColumnWidth(i++,45);
       }
    }

    labelCall->setText( settings.value("Call").toString());
    qy = "SELECT COUNT(*) FROM "+settings.value("Logfile").toString();   //hbd";
    query.exec(qy);
    query.next();
    m = (query.value(0).toString());
    if( m.toInt() != 0) {
      m = m+" QSOs";
      labelBandCnt->setText(m);                             

      QDate d = QDate::currentDate();                       
      i = d.year();                                         
      qy = "SELECT day FROM "+settings.value("Logfile").toString()+" WHERE  day > 0 LIMIT 1";
      query.exec(qy);                                       
      query.next();
      s = query.value(0).toString();
      s = s.left(4);
      n = s.toInt();                                        
      while( i +1 != n ) {
        r = 0;
        ri = 0;
        ht = 2;
        qy = "SELECT COUNT(*) FROM "+settings.value("Logfile").toString()+" WHERE day BETWEEN '"+s.setNum(i)+"-01-01' AND '"+s.setNum(i)+"-12-31'";
        query.exec(qy);
        query.next();
        QTreeWidgetItem *item = new QTreeWidgetItem(BandList);
        item->setText(ri,s.setNum(i));                      
        item->setTextAlignment(ri++,Qt::AlignCenter);
        item->setText(ri,query.value(r).toString());        
        item->setTextAlignment(ri++,Qt::AlignCenter);
        while( ht != hcount +2) {
          qy = "SELECT COUNT(*) FROM "+settings.value("Logfile").toString()+" WHERE band='";
          s = BandList->headerItem()->text(ht++);         
          s = getSysband(s);                              
          qy += s+"' AND ";
          qy += "day BETWEEN '"+s.setNum(i)+"-01-01' AND '"+s.setNum(i)+"-12-31'";
          query.exec(qy);
          query.next();
          item->setText(ri,query.value(r).toString());    
          //item->setTextAlignment(ri++,Qt::AlignRight);
	  item->setTextAlignment(ri++,Qt::AlignCenter);
        }
        i--;
        ri--;
        item->setTextAlignment(ri++,Qt::AlignLeft);
      }
     }
     stackedWidget->setCurrentIndex(2);
     QApplication::restoreOverrideCursor();
}



void awdst::makedxccStatusListAc()
{
 QString mode, band, qslr, dxcc;
 int Id, x;
   stackedWidget->setCurrentIndex(4);
   InfoText->clear();
   InfoText->append(tr("\nNeue DX-Statusliste erzeugt:\n"));
   InfoText->append("---------------------------------------------------\n");
   
   
   x = 0;
   QSqlQuery query;
   qy = "SELECT logname FROM dblogs";
   query.exec(qy);
   while( query.next() ) {
      logbook.append(query.value(0).toString());
   }
   dxlist dxl;
   for(int i = 0; i < logbook.size(); ++i) {  
      s = logbook.at(i)+"dx";                 
      dxl.setLogname(s);                      
      dxl.setDxlistClear();                   
      dxl.clrErrStr();                        
      dxl.clrErrCnt();                        
      h = 0;
      qy = "SELECT id,mode,band,qslr,dxcc FROM "+logbook.at(i)+" WHERE qslr != '-' AND qslr != '' ORDER BY day";
      query.exec(qy);
      while( query.next() ) {
         n = 0;
         Id = query.value(n++).toInt();
         mode = query.value(n++).toString();
         band = query.value(n++).toString();
         qslr = query.value(n++).toString();
         dxcc = query.value(n++).toString();
         dxl.insertDxItem(Id,mode, band, qslr, dxcc);   
      }
      n = dxl.getErrCnt();
      
      if( !n ) {                                       
         ms = tr("Im Logbook  '")+logbook.at(i)+tr("'  wurden keine Abweichungen gefunden.\n");
         InfoText->append(ms);
      }
      else {
       QString a;
       ms = tr("Im Logbook  '")+logbook.at(i)+tr("'  sind folgende Abweichungen aufgetreten:\n");
       ms += "Anzahl: "+a.setNum(n)+"\n";
       InfoText->append(ms);
       InfoText->append(dxl.getErorrList());
       x = 1;
      }
   }
   if( x ) {
     ms = tr("In einigen QSOs ist der 'DX-Prefix' nicht korrekt bzw. fehlt oder veraltet.\n");
     ms += tr("Bitte korr. damit der DX-Status richtig dargestellt wird.\n\n");
     InfoText->append(QString::fromUtf8(ms.toAscii()));
   }
   ms = tr("Die Status-Liste ist über das Auswahlmenü  'DX-Länder Status'  abrufbar.\n\n");
   InfoText->append(QString::fromUtf8(ms.toAscii()));
}
