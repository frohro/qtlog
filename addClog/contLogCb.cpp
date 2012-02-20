/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n : 30.07.09
********************************************************************** */

#include "contLog.h"
#include "defs.h"

extern cBase * myContest;
extern int Qpts;               
extern int Mul1p;                
extern int Mul2p;                
extern int qsoPoint;             //
extern QString Multi1;           //
extern QString Multi2;           


void contLog::clearAll()
{
    lineCallsign->setText("");
    lineCallsign->setFocus();
    lineExchange->setText("");
    wMultNew->setText("");
    wMult2->setText("");
    lineEditPoints->setText("");
    labelInfo->setText("");
    labelWarnung2->setText("");
    
    laPref = 0;
    cPref = 0;
    scPref = 0;
    nlen = 0;
    olen = 0;
    qNrlen = 0;
    idx = 0;
    clFlg = 0;
    numflg = 1;
    
    cpref = "";
    dxcc = "";
    wae = ""; 
    cnt = "";
    Multi1 = "";
    Multi2 = "";
    
    QPalette palette6;
    QBrush brush10(QColor(255,255,255));         
    palette6.setBrush(QPalette::Active, QPalette::Base,brush10);
    wMultNew->setPalette(palette6);
    wMult2->setPalette(palette6);
    
    //setColorMult(Multi1);
    setColorQsoPoints(0);
}


void contLog::setColorMult(int mp)
{
      QPalette palette7;
       if(mp) {                                             
       QBrush brush8(QColor(255, 247, 0, 255));             
       palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
       wMultNew->setPalette(palette7);
       labelWarnung2->setText("");
     }
     else
      if( ! mp ) {
        QBrush brush8(QColor(255, 255, 255));               
        palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
        wMultNew->setPalette(palette7);
      }
}

void contLog::setColorQsoPoints(int qpts)
{
     QPalette palette7;   
     if( qpts ) {                                            
       QBrush brush8(QColor(255, 247, 0, 255));              
       brush8.setStyle(Qt::SolidPattern);
       palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
       lineEditPoints->setPalette(palette7);
       labelWarnung2->setText("");
    }
    if( !qpts ) {                                             
       QBrush brush8(QColor(255, 255, 255));                  
       palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
       lineEditPoints->setPalette(palette7);
    }
}

void contLog::setColorMult2(int mu2)
{
    QPalette palette7;   
     if( mu2 ) {                                             
       QBrush brush8(QColor(255, 247, 0, 255));              
       brush8.setStyle(Qt::SolidPattern);
       palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
       wMult2->setPalette(palette7);
       labelWarnung2->setText("");
    }
    if( !mu2 ) {                                             
       QBrush brush8(QColor(255, 255, 255));                 
       palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
       wMult2->setPalette(palette7);
    }
}

void contLog::setColorBandMin(QString col)
{
    QPalette palette7;   
    if(col.indexOf("WEISS") == -1) { 
       QBrush brush8(QColor(255, 233, 175, 255));             
       brush8.setStyle(Qt::SolidPattern);
       palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
       lineBandMin->setPalette(palette7);
    }
    else {
       QBrush brush8(QColor(255, 255, 255));                  
       palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
       lineBandMin->setPalette(palette7);
    }
}

void contLog::setColorButtonUhr(QString col)
{ 
    if(col.indexOf("WEISS") == -1)  
       ButtonUhr->setPalette( QPalette(QColor(250, 117, 105)));  
    else 
       ButtonUhr->setPalette( QPalette(QColor(238, 238, 238)));  
}

void contLog::setColorUhr(QString col)
{
    QPalette palette7;   
    if(col.indexOf("WEISS") == -1) { 
       QBrush brush8(QColor(255, 245, 192, 255));             
       brush8.setStyle(Qt::SolidPattern);
       palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
       cTime->setPalette(palette7);
    }
    else {
       QBrush brush8(QColor(241, 241, 241));                 
       palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
       cTime->setPalette(palette7);
    }
}



void contLog::insertQso()
{
   QString str;
   int i;
     i = 0;
     qy = "INSERT INTO wCqso VALUES ("+str.setNum(qsoRow++)+",'";  
     qy += ContestID;                                              
     qy += "','";                                                  
     qy += "','"+cDate->text();
     
     if((eflg == 1) || (timeflg == 0)) {              
       qy += "','"+cTime->text();
       qy += ":00";
     }
     else {
       str = h.time().toString("hh:mm:ss");
       str = str.mid(5);
       qy += "','"+cTime->text()+str;
     }  
     qy += "','"+getBand(MHzFreq);                    
     qy += "','"+str.setNum(freqBox->value());        
     qy += "','"+modeBox->currentText();
     qy += "','"+lineCallsign->text().simplified();   
     qy += "','"+lineEditSrst->text();                
     
     
     str = lineEditSent->text();                      
     
     idx = 0;
     while(1) {
        if(str.at(idx) >= QChar('0') && str.at(idx) <= QChar('9'))    
           idx++;                                     
        else
          break;
     }
     if(idx) {
       if(str.at(idx) == QChar('/')) {                
	  qy += "','"+str.left(idx);                  
          qy += "','";
          qy += str.mid(idx +1);                      
       }
       else {                                         
        qy += "','"+str.left(idx);                    
        qy += "','";
        qy += str.mid(idx);                           
       }
     }
     else {                                           
      //qy += "','"+lineExchange->text();               
      qy += "','"+str;                               
      qy += "','-";                                   
     }
     qy += "','"+lineEditRrst->text();                
                                                      
     str = lineExchange->text();                      
     idx = 0;
     while(1) {                                       
        if(str.at(idx) >= QChar('0') && str.at(idx) <= QChar('9')) 
           idx++;                                     
        else
          break;
     }
     if(idx) {                                        
       if(str.at(idx) == QChar('/')) {                
	  qy += "','"+str.left(idx);                  
          qy += "','"+str.mid(idx +1);                
       }
       else {                                         
        qy += "','"+str.left(idx);                    
        qy += "','"+str.mid(idx);                     
       }
     }
     else {                                           
      qy += "','"+lineExchange->text();               
      qy += "','-";                                   
     }
     
     qy += "','"+wae;                                 
     qy += "','"+dxcc;                                
     qy += "','"+cnt;                                 
     qy += "','"+cpref;                               
     qy += "',"+str.setNum(Qpts);                     
     qy += ",'"+Multi1;                               
     qy += "',"+str.setNum(Mul1p);                    
     qy += ",'"+Multi2;                               
     qy += "',"+str.setNum(Mul2p);                    
     qy += ","+str.setNum(qtcflg);                    
     qy += ",'0')";                                   
     QSqlQuery query(qy);
     qy = "SELECT callsign FROM wCmaster WHERE callsign='"+lineCallsign->text().simplified()+"'";
     query.exec(qy);
     if(!query.size()) {                             
       qy = "INSERT INTO wCmaster VALUES ('"+lineCallsign->text().simplified()+"',0,0,0,0,0,0)";
       query.exec(qy);
     }
     
     qy = "UPDATE wCmaster SET "+band+"=1 WHERE callsign='"+lineCallsign->text().simplified()+"'";
     query.exec(qy);
     

     i = getBindex(band);
     QTreeWidgetItem * qItem;
     qItem = qsoPointsList->topLevelItem(0);            
     qItem->setSelected(true);                          
     bqsos++;                                           
     hqsos.insert(band,bqsos);                          
     qItem->setText(i,str.setNum(bqsos));               
     //-
     tqsos++;                                           
     hqsos.insert("total",tqsos);
     qItem->setText(7,str.setNum(tqsos));               
     qItem->setSelected(false);
    
//- points -----
     qItem = qsoPointsList->topLevelItem(1);            
     bpoints += Qpts;                                   
     hpoints.insert(band,bpoints);
     qItem->setText(i,str.setNum(bpoints));             
     //-
     tpoints += Qpts;                                   
     hpoints.insert("total",tpoints); 
     qItem->setText(7,str.setNum(tpoints));             

     qItem = qsoPointsList->topLevelItem(2);            
     bmulti1p += Mul1p;
     hmult1.insert(band,bmulti1p);  
     qItem->setText(i,str.setNum(bmulti1p));            
     tmulti1p += Mul1p;
    
     if(Mul1p) {                                        
       if(offband) 
        myContest->insertMulti1(Multi1,Multi1);         
       else
	myContest->insertMulti1(Multi1+band,Multi1);    
     }
     hmult1.insert("total",tmulti1p);
     qItem->setText(7,str.setNum(tmulti1p));            

     qItem = qsoPointsList->topLevelItem(3);            
     bmulti2p += Mul2p;                                 
     hmult2.insert(band,bmulti2p); 
     qItem->setText(i,str.setNum(bmulti2p));            
     tmulti2p += Mul2p;                                 
     
     if(Mul2p) {                                        
       if(offband)                                      
         myContest->insertMulti2(Multi2,Multi2);        
       else
	myContest->insertMulti2(Multi2+band,Multi2);    
     }
     hmult2.insert("total",tmulti2p);
     qItem->setText(7,str.setNum(tmulti2p));            
/*  

//- QSOs/h
   //qItem = qsoPointsList->topLevelItem(4);            
//- Score
    qItem = qsoPointsList->topLevelItem(5);            
    hscore.insert(t,0);
    qItem->setText(getBindex(band),s.setNum(???));
   
    Band Verweildauer messen
    if(! bQso ) {                                      
      bflg = 1;                                        
      bQso++;                                          
      setColorBandMin("BLAU");                         
    }
*/
     Mul1p = 0;
     Mul2p = 0;
}

void contLog::showPoints(QString mband)
{
   int i;
//-Qsos
   i = getBindex(mband);
   QTreeWidgetItem * qItem;
   qItem = qsoPointsList->topLevelItem(0);            
   //qItem->setSelected(true);                        
   hqsos.insert(mband,bqsos);                          
   qItem->setText(i,s.setNum(bqsos));                 
   hqsos.insert("total",tqsos);
   qItem->setText(7,s.setNum(tqsos));                 
   //qItem->setSelected(false);
    
//-Points
   qItem = qsoPointsList->topLevelItem(1);            
   hpoints.insert(mband,bpoints);
   qItem->setText(i,s.setNum(bpoints));               
   hpoints.insert("total",tpoints); 
   qItem->setText(7,s.setNum(tpoints));               
//-Multi1
   qItem = qsoPointsList->topLevelItem(2);            
   hmult1.insert(mband,bmulti1p);                     
   qItem->setText(i,s.setNum(bmulti1p));              
   hmult1.insert("total",tmulti1p);                   
   qItem->setText(7,s.setNum(tmulti1p));              
//-Multi2
   qItem = qsoPointsList->topLevelItem(3);            
   hmult2.insert(mband,bmulti2p); 
   qItem->setText(i,s.setNum(bmulti2p));              
   hmult2.insert("total",tmulti2p);
   qItem->setText(7,s.setNum(tmulti2p));              
}
/*
     
      QHash<QString,QString>::const_iterator n = hashMulti1.constBegin();
      while (n != hashMulti1.constEnd()) {
        qDebug() << "key_hashMulti_1" << n.key() << ": " << n.value();
        ++n;
      }
     
      QHash<QString,QString>::const_iterator i = hashMulti2.constBegin();
      while (i != hashMulti2.constEnd()) {
        qDebug() << "key_hashMulti_2:" << i.key() << ": " << i.value();
        ++i;
      }
*/
  
  





void contLog::removeQso(QString Id) 
{ 
     QString x = Id;
     QSqlQuery query;                                 
     qy = "DELETE FROM wCqso WHERE contid='"+ContestID+"' AND cqsoid="+d_Id;
     query.exec(qy);
    
     QString xqsoid, str;
     int row;
     
     
     
     
     
     row = 0; 
     if(d_Mul2p != 0) {                               
        qy = "SELECT cqsoid FROM wCqso ";             
        qy += "WHERE contid='"+ContestID+"' AND band='"+d_Band+"' AND rcvd='"+d_Rcvd+"' AND mul2=0";
        query.exec(qy);
        row = query.size();                           
        if(row) {                                     
           query.next();                              
           xqsoid = query.value(0).toString(); 
           qy = "UPDATE wCqso SET multi2='"+d_Multi2+"',mul2="+str.setNum(d_Mul2p);
           qy += " WHERE contid='"+ContestID+"' AND band='"+d_Band+"' AND cqsoid="+xqsoid;
           query.exec(qy);
        }
        else {                                         
          bmulti2p -= d_Mul2p;                         
          tmulti2p -= d_Mul2p;
          myContest->hashMulti2.remove(d_Multi2+band); 
        }
      }
      
      
      
      if(d_Mul1p != 0) {                       
         qy = "SELECT cqsoid FROM wCqso ";     
         qy += "WHERE contid='"+ContestID+"' AND band='"+d_Band+"' AND cpref='"+d_Pref+"' AND mul1=0";
         query.exec(qy);
         row = query.size();                   
         if(row) {                             
            query.next();                      
            xqsoid = query.value(0).toString();
            
            qy = "UPDATE wCqso SET multi1='"+d_Multi1+"',mul1="+str.setNum(d_Mul1p);
            qy += " WHERE contid='"+ContestID+"' AND band='"+d_Band+"' AND cqsoid="+xqsoid;
            query.exec(qy);
         }
         else {                              
          bmulti1p -= d_Mul1p;               
          tmulti1p -= d_Mul1p;               
          myContest->hashMulti1.remove(d_Multi1+band);  
        }
     }
     bpoints -= d_Pts;                       
     tpoints -= d_Pts;                       
     bqsos--;                                
     tqsos--;                                
     showPoints(band);
}
  



void contLog::readMasterAc()
{
    int Status = QMessageBox::question(this,
           tr("Tabelle ueberschreiben?"),
           tr("Die Tabelle Master.Callsign existiert bereits.\n"
              "Wollen Sie diese Tabelle ueberschreiben?\n"
              "Alle neu eingetragenen Rufzeichen gehen verloren !"),
           QMessageBox::Yes,QMessageBox::No);
    if(Status != QMessageBox::Yes)
      return;
    setNewMasterTable();
}

void contLog::setNewMasterTable()
{
  QString zeile;
  char buf[50];
  
    sprintf(buf,"%s/log/iniFiles/MASTER.asc",(char*)getenv("HOME"));
    QFile datei(buf);
    if(! datei.open(QFile::ReadOnly)) {
      QMessageBox::information( this,
        tr("MASTER.asc"),
        tr("Datei 'MASTER.asc' kann nicht geoeffnet werden !"));
      return;
    }
    
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QSqlQuery query;
    qy = "DELETE FROM wCmaster";                 
    query.exec(qy);
    QTextStream inStream(&datei);                
    QString qy;
    while(1) {                                   
      while( !inStream.atEnd()) {                
         zeile = inStream.readLine(0);
         if(zeile[0] != QChar('#'))              
            break;                               
      }
      
      qy = "INSERT INTO wCmaster VALUES ('"+zeile+"',0,0,0,0,0,0)";
      query.exec(qy);
      while( !inStream.atEnd()) {
        zeile = inStream.readLine(0);
	if(zeile.count() == 0) break;
        qy = "INSERT INTO wCmaster VALUES ('"+zeile+"',0,0,0,0,0,0)";
	query.exec(qy);
      }
      break;
    }
   datei.close();
   QApplication::restoreOverrideCursor();
}
 
 
