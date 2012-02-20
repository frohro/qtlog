/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n : 20.08.09
********************************************************************** */

#include <QTimer>
#include "contLog.h"
#include "contRules.h"
#include "../qtlogDiag/version.h"
#include "../qtlogDiag/dirmngr.h"
#include "editLog.h"
#include "hlib.h"
#include "defs.h"
#include "logexp.h"

cBase * myContest;
extern int Qpts;                 
extern int Mul1p;                
extern int Mul2p;                
extern QString Multi1;           //
extern QString Multi2;           



void contLog::newContestLogAc()
{
    if(CNr) {                                            
       saveAll();                                        
       CNr = 0;                                          
       labelContestName->setText("No Name");
       labelContestId->setText("No Name");
       stackedWidget->setCurrentIndex(2);
    }
    contRules *rules = new contRules();
    rules->exec();                                       

    if(rules->stat) {                                    
      labelContestName->setText(rules->confcontestName); 
      labelContestId->setText(rules->confcontestId);
      ContestID = labelContestId->text();                
      NamId = rules->namId;                              
      s = rules->cType;
      CNr = s.toInt();                                   
      viewtiming = rules->view.toInt();                  
      s = rules->confBand.toInt();                  
      i = modeBox->findText(rules->confMode);
      modeBox->setCurrentIndex(i);                       
      incNr = rules->Inc;                                
      
      if(incNr)                                          
	stxNr = 1;

      Contestday = rules->StartDay;                      
      ContestStart = rules->StartTime;
      Runtime = rules->RunTime;
 
      if(CNr != CQWAEDC)
         ButtonQtc->hide();
      
      hqsos.clear();                
      hpoints.clear();              
      hmult1.clear();               
      hmult2.clear();               
      hqsosh.clear();               
      hscore.clear();               
      hbtime.clear();               
      workedList->clear();
      
      bqsos = 0;                    
      bpoints = 0;                  
      bmulti1p = 0;                 
      bmulti2p = 0;                 
      bqsosh = 0;                   
      bscore = 0;                   
      tqsos = 0;                    
      tpoints = 0;                  
      tmulti1p = 0;                 
      tmulti2p = 0;                 
      tqsosh = 0;                   
      tscore = 0;                   
      tbandmin = 0;                 
      
      setBandValues("");            
      
      hashBandPoint.clear();        
      QSqlQuery query;              
      qy = "SELECT method,exchp,cprefm,offband,pointsA,pointsB,pointsC,pointsD,pointsE,pointsF,multiA,multiB,bpts160,bpts80,bpts40,bpts20,bpts15,bpts10 FROM wCruls WHERE contnr="+s.setNum(CNr);
      query.exec(qy);
      query.next();
      n = 0;
      methode = query.value(n++).toInt();                   
      exchp = query.value(n++).toInt();                     
      prefMode = query.value(n++).toInt();                  
      offband = query.value(n++).toInt();                   
      
      setPointA = query.value(n++).toInt();                 
      setPointB = query.value(n++).toInt();
      setPointC = query.value(n++).toInt();
      setPointD = query.value(n++).toInt();
      setPointE = query.value(n++).toInt();
      setPointF = query.value(n++).toInt();
      setMultiA = query.value(n++).toInt();
      setMultiB = query.value(n++).toInt();
      
      hashBandPoint.insert("b160",query.value(n++).toInt()); 
      hashBandPoint.insert("b80",query.value(n++).toInt());
      hashBandPoint.insert("b40",query.value(n++).toInt());
      hashBandPoint.insert("b20",query.value(n++).toInt());
      hashBandPoint.insert("b15",query.value(n++).toInt());
      hashBandPoint.insert("b10",query.value(n).toInt());
     
       qy = "SELECT view,qsoid,mode,srst,rrst,sent,freqband FROM wContest WHERE contestid='"+ContestID+"'";  
       query.exec(qy);
       query.next();
       n = 0;
       viewtiming = query.value(n++).toInt();                 
       qsoRow = query.value(n++).toInt();                     
       i = modeBox->findText(query.value(n++).toString());
       modeBox->setCurrentIndex( i );                         
       
       if(modeBox->currentText().compare("CW") == 0) {
          s = query.value(n++).toString();                     
	  s = "599";
          lineEditSrst->setText(s);                            
	  
	  s = query.value(n++).toString();                     
	  s = "599";
          lineEditRrst->setText(s);                            
	  prepCwdaemon();                                      
       }
       else {
	 lineEditSrst->setText(query.value(n++).toString());  
	 lineEditRrst->setText(query.value(n++).toString());  
       }
       
       
       createContestClass(1,CNr);                             
       
       QTimer::singleShot(20, this, SLOT(infoSetExchange())); 
       
       lineEditSent->setText(query.value(n++).toString());    
       freq = query.value(n++).toInt();                       
   
       setNewMasterTable();                                   
       
       lineLastHour->setText("-");
       lineLastMin->setText("-");
       lineBandMin->setText("-");
       Qpts = 0;                                
       Mul1p = 0;
       Mul2p = 0;
       
       bflg = 0;                                
       bQso = 0;
       bMin = 0;
       freqBox->setValue(freq);                 
       lastFreq = "";
       bandruning = 1;
       myContest->setQsoBand(band);                                
       myContest->setHomeDxcc(settings.value("Dxcc").toString());  
       qy = "SELECT lcn FROM tla WHERE la='"+settings.value("Dxcc").toString()+"'";
       query.exec(qy);
       query.next();
       myContest->setHomeContinent(query.value(0).toString());     
       
       myContest->setqsoPoints(setPointA,setPointB,setPointC,setPointD,setPointE,setPointF); 
       myContest->setmul1p(setMultiA);                             
       myContest->setmul2p(setMultiB);                             
       
       stackedWidget->setCurrentIndex(0);          
       rigpolling = runflg;
       if( rigpolling ) {                          
         QTimer::singleShot(10, this, SLOT(starthamlibServer())); 
         modeBox->setEnabled(false);               
       }
       lineCallsign->setFocus();
   }
   else
   stackedWidget->setCurrentIndex(2);              
}




void contLog::continueAc()
{
    settings.setValue("Val",CONTINUE);
    if(CNr) {
        saveAll();                         
	myContest->clearAllTable();        
	delete myContest;                  
        CNr = 0;
        labelContestName->setText("No Name");
        labelContestId->setText("No Name");
    }
    stackButtonOk->setText("             Continue                ");
    selectContestList();
}


void contLog::sentLogAc()
{
    settings.setValue("Val",SENTLOG);
    if(CNr) {
     saveAll();
     CNr = 0;
     labelContestName->setText("No Name");
     labelContestId->setText("No Name");
    }
    selectContestList();
    stackButtonOk->setText("   Sent Log - STF or Cabrillo file      ");
}


void contLog::deleteContestAc()
{
    settings.setValue("Val",DELETELOG);
    if(CNr) {
     saveAll();
     CNr = 0;
     labelContestName->setText("No Name");
     labelContestId->setText("No Name");
    }
    selectContestList();
    stackButtonOk->setText("             DELETE LOG               ");
}


void contLog::convertToQtLogAc()
{
    settings.setValue("Val",CONVQTLOG);
    selectContestList();
    stackButtonOk->setText("            Convert to QtLog           ");
}



void contLog::selectContestList()
{
     contestWorkedList->clear();
     QSqlQuery query; 
     qy = "SELECT contestid,name,startday,mode,expFile,expLogbook FROM wContest ORDER BY startday,contestid"; 
     query.exec(qy);
     while(query.next()) {
      i = 0;
      n = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(contestWorkedList);
      item->setText(i++,query.value(n++).toString());      
      item->setText(i++,query.value(n++).toString());      
      item->setText(i++,query.value(n++).toString());      
      item->setText(i++,query.value(n++).toString());      
      item->setText(i++,query.value(n++).toString());      
      item->setText(i++,query.value(n++).toString());      
    }
    stackedWidget->setCurrentIndex(1);                     
}



void contLog::stackButtonOkCb()
{
     if(Item == NULL) return;                               
     switch( settings.value("Val").toString().toInt()) {
       case CONTINUE :
                        contestContinue();                  
                        break;
       case SENTLOG  : {                      
                          settings.setValue("Val",labelContestName->text()+","+ labelContestId->text());
                          logexp *logx = new logexp();
                          logx->exec();
                          stackedWidget->setCurrentIndex(2);
                        }
                        break;
       case DELETELOG : {                     
                         deleteContest();
                         CNr = 0;
                         labelContestName->setText("No Name");
                         labelContestId->setText("No Name");
                         stackedWidget->setCurrentIndex(2);
                        }
                        break;
       case CONVQTLOG : {  
                          s = labelContestId->text()+",1";     
                          settings.setValue("Val",s);
                          editLog * eLog = new editLog();
                          eLog->exec();
                          CNr = 0;
                          labelContestName->setText("No Name");
                          labelContestId->setText("No Name");
                          stackedWidget->setCurrentIndex(2);
                        }
                        break;
     }
     contestWorkedList->clearSelection();
     Item = NULL;
     stackButtonOk->setText("                Ok                ");
}




//--------------------------------------------------------------------------
void contLog::contestContinue()
{
      //CNr = Item->text(6).toInt();                   
      ContestID = Item->text(0);                     
      Contestday = Item->text(2);                    
      ContestStart = Item->text(3);                  
      Runtime = Item->text(4);                       
      
      setContinueValues();                           
}



void contLog::setContinueValues()
{
      QString sband;
      if(CNr != CQWAEDC)
         ButtonQtc->hide();
      
      hashBandPoint.clear();
      QSqlQuery query; 
      //qy = "SELECT method,exchp,cprefm,offband,contnr,pointsA,pointsB,pointsC,pointsD,pointsE,pointsF,multiA,multiB,bpts160,bpts80,bpts40,bpts20,bpts15,bpts10,increm FROM wCruls WHERE contnr="+s.setNum(CNr);
      qy = "SELECT method,exchp,cprefm,offband,contnr,pointsA,pointsB,pointsC,pointsD,pointsE,pointsF,multiA,multiB,bpts160,bpts80,bpts40,bpts20,bpts15,bpts10,increm FROM wCruls WHERE namid='"+ContestID+"'";
      query.exec(qy);
      query.next();
      n = 0;
      methode = query.value(n++).toInt();          
      exchp = query.value(n++).toInt();            
      prefMode = query.value(n++).toInt();         
      offband = query.value(n++).toInt();          
      CNr = query.value(n++).toInt();              
      setPointA = query.value(n++).toInt();        
      setPointB = query.value(n++).toInt();        
      setPointC = query.value(n++).toInt();        
      setPointD = query.value(n++).toInt();        
      setPointE = query.value(n++).toInt();        
      setPointF = query.value(n++).toInt();        
      
      setMultiA = query.value(n++).toInt();        
      setMultiB = query.value(n++).toInt();        
      
      hashBandPoint.insert("b160",query.value(n++).toInt()); 
      hashBandPoint.insert("b80",query.value(n++).toInt());
      hashBandPoint.insert("b40",query.value(n++).toInt());
      hashBandPoint.insert("b20",query.value(n++).toInt());
      hashBandPoint.insert("b15",query.value(n++).toInt());
      hashBandPoint.insert("b10",query.value(n++).toInt());
      incNr = query.value(n).toInt();
      
      qy = "SELECT view,qsoid,mode,srst,sent,rrst,freqband FROM wContest WHERE contestid='"+ContestID+"'";  
      query.exec(qy);
      query.next();
      n = 0;
      viewtiming = query.value(n++).toInt();               
      qsoRow = query.value(n++).toInt();                   
      
      i = modeBox->findText(query.value(n++).toString());
      modeBox->setCurrentIndex( i );                       
      
      lineEditSrst->setText(query.value(n++).toString());  
      lineEditSent->setText(query.value(n).toString());    
      
      s = query.value(n).toString();
      if( incNr ) {                                        
         i = 0;
         while(1) {
           if(s.at(i) >= QChar('0') && s.at(i) <= QChar('9')) { 
             i++;                                     
	   }
           else
            break;
         }
        if(i) {
          if(s.at(i) == QChar('/')) {                 
	     stxNr = s.left(i).toInt();               
             sentVal = s.mid(i +1);                   
          }
          else {
	     stxNr = s.left(i).toInt();               
             sentVal = s.mid(i);                      
	  }
	}
      }
      //qDebug() << lineEditSent->text() << sentVal;
      
      n++;
      lineEditRrst->setText(query.value(n++).toString());  
      freq = query.value(n++).toInt();                     
     
      createContestClass(0,CNr);               
      
      bqsos = 0;            
      bpoints = 0;          
      bmulti1p = 0;         
      bmulti2p = 0;         
      bqsosh = 0;           
      bscore = 0;           
      
      tqsos = 0;            
      tpoints = 0;          
      tmulti1p = 0;         
      tmulti2p = 0;         
      tqsosh = 0;           
      tscore = 0;           
      tbandmin = 0;         
     
      makePoinsValueList();                    
      refreshMasterTable();                    
      
      lineLastHour->setText("-");
      lineLastMin->setText("-");
      lineBandMin->setText("-");
      Qpts = 0;                                
      Mul1p = 0; 
      Mul2p = 0;
      bflg = 0;
      bQso = 0;
      bMin = 0;
      freqBox->setValue(setfreqUp(freq/1000)); 
      
      lastFreq = "";
      bandruning = 1;
      
      myContest->setQsoBand(band);                                      
      myContest->setqsoPoints(setPointA,setPointB,setPointC,setPointD,setPointE,setPointF); 
      myContest->setmul1p(setMultiA);                                   
      myContest->setmul2p(setMultiB);                                   
      
      myContest->setHomeDxcc(settings.value("Dxcc").toString());        
      qy = "SELECT lcn FROM tla WHERE la='"+settings.value("Dxcc").toString()+"'";
      query.exec(qy);
      query.next();
      myContest->setHomeContinent(query.value(0).toString());           
      
      stackedWidget->setCurrentIndex(0);                
      showAllQso();  
      
      if(modeBox->currentText().compare("CW") == 0)
        prepCwdaemon();
      
      //zeige die letzten QSOs, falls vorhanden
      rigpolling = runflg;
      if( rigpolling ) {                              
        QTimer::singleShot(10, this, SLOT(starthamlibServer())); 
        modeBox->setEnabled(false);                   
      }
      lineCallsign->setFocus();
}



void contLog::refreshMasterTable()
{
     
      QSqlQuery query;
      qy = "UPDATE wCmaster SET b160=0,b80=0,b40=0,b20=0,b15=0,b10=0"; 
      query.exec(qy);
      
      qy = "SELECT qcall,band FROM wCqso WHERE contid='"+ContestID+"' AND band='160' AND pts !=0";
      query.exec(qy);
      if(query.size())
	setWorkdeBand(query);
      qy = "SELECT qcall,band FROM wCqso WHERE contid='"+ContestID+"' AND band='80' AND pts !=0";
      query.exec(qy);
      if(query.size())
	setWorkdeBand(query);
      
      qy = "SELECT qcall,band FROM wCqso WHERE contid='"+ContestID+"' AND band='40' AND pts !=0";
      query.exec(qy);
      if(query.size())
	setWorkdeBand(query);
      
      qy = "SELECT qcall,band FROM wCqso WHERE contid='"+ContestID+"' AND band='20' AND pts !=0";
      query.exec(qy);
      if(query.size())
	setWorkdeBand(query);
      
      qy = "SELECT qcall,band FROM wCqso WHERE contid='"+ContestID+"' AND band='15' AND pts !=0";
      query.exec(qy);
      if(query.size())
	setWorkdeBand(query);
      
      qy = "SELECT qcall,band FROM wCqso WHERE contid='"+ContestID+"' AND band='10' AND pts !=0";
      query.exec(qy);
      if(query.size())
	setWorkdeBand(query);
}

void contLog::setWorkdeBand(QSqlQuery query)
{
      QString q, call, band;
      int n;
      QSqlQuery bquery;
      while(query.next()) {
	n = 0;
	call = query.value(n++).toString();
	band = "b"+query.value(n++).toString();
	
	q = "SELECT callsign FROM wCmaster WHERE callsign='"+call+"'";
	bquery.exec(q);
	if(!bquery.size()) {
	  q = "INSERT INTO wCmaster VALUE ('"+call+"',0,0,0,0,0,0)";
	  bquery.exec(q);
	}
	q = "UPDATE wCmaster SET "+band+"=1 WHERE callsign='"+call+"'";
	bquery.exec(q);
      }
}



void contLog::makePoinsValueList()
{ 
      hqsos.clear();                                
      hpoints.clear();
      hmult1.clear();
      hmult2.clear();
      hqsosh.clear();
      hscore.clear();
 
      qy = "SELECT pts,multi1,mul1,multi2,mul2 FROM wCqso WHERE contid='"+ContestID+"' AND band='160'";
      countPoints(1,"b160");                        
      qy = "SELECT pts,multi1,mul1,multi2,mul2 FROM wCqso WHERE contid='"+ContestID+"' AND band='80'";
      countPoints(2,"b80");                         
      qy = "SELECT pts,multi1,mul1,multi2,mul2 FROM wCqso WHERE contid='"+ContestID+"' AND band='40'";
      countPoints(3,"b40");                         
      qy = "SELECT pts,multi1,mul1,multi2,mul2 FROM wCqso WHERE contid='"+ContestID+"' AND band='20'";
      countPoints(4,"b20");                         
      qy = "SELECT pts,multi1,mul1,multi2,mul2 FROM wCqso WHERE contid='"+ContestID+"' AND band='15'";
      countPoints(5,"b15");                         
      qy = "SELECT pts,multi1,mul1,multi2,mul2 FROM wCqso WHERE contid='"+ContestID+"' AND band='10'";
      countPoints(6,"b10");                         
      
                                                    
      QTreeWidgetItem * qItem;                      
      qItem = qsoPointsList->topLevelItem(0);       
      qItem->setText(7,s.setNum(tqsos));            
      qItem->setTextAlignment(7,Qt::AlignCenter);
      hqsos.insert("total",tqsos);
	  
      qItem = qsoPointsList->topLevelItem(1);       
      qItem->setText(7,s.setNum(tpoints));          
      qItem->setTextAlignment(7,Qt::AlignCenter);
      hpoints.insert("total",tpoints);
	  
      qItem = qsoPointsList->topLevelItem(2);       
      qItem->setText(7,s.setNum(tmulti1p));         
      qItem->setTextAlignment(7,Qt::AlignCenter);
      hmult1.insert("total",tmulti1p);  
      
      qItem = qsoPointsList->topLevelItem(3);       
      qItem->setText(7,s.setNum(tmulti2p));         
      qItem->setTextAlignment(7,Qt::AlignCenter);
      hmult2.insert("total",tmulti2p);  
      
      qItem = qsoPointsList->topLevelItem(4);       
      qItem->setText(7,s.setNum(0));                
      qItem->setTextAlignment(7,Qt::AlignCenter);
      hqsosh.insert("total",0);
      
      qItem = qsoPointsList->topLevelItem(5);       
      qItem->setText(7,s.setNum(0));                
      qItem->setTextAlignment(7,Qt::AlignCenter);
      hscore.insert("total",0);
}



void contLog::countPoints(int col, QString wband)           
{
  int cnt;
  
       Qpts = 0;
       Mul1p = 0;
       Mul2p = 0;
       Multi1 = "";
       Multi2 = "";
 
       bpoints = 0;
       bmulti1p = 0;
       bmulti2p = 0;
       
       QSqlQuery query; 
       query.exec(qy);
       cnt = query.size();
       while(query.next()) {                                 
	   n = 0;
	   Qpts = query.value(n++).toInt();                  
	   //-
	   Multi1 = query.value(n++).toString();             
           Mul1p = query.value(n++).toInt();                 
	  
	   if(Mul1p) {                                       
             if(offband)                                     
	       myContest->insertMulti1(Multi1,Multi1);       
	     else
               myContest->insertMulti1(Multi1+wband,Multi1); 
	   }
	   Multi2 = query.value(n++).toString();             
	   Mul2p = query.value(n++).toInt();                 
	   
	   if(Mul2p) {                                       
             if(offband)
	       myContest->insertMulti2(Multi2,Multi2);       
	     else
               myContest->insertMulti2(Multi2+wband,Multi2); 
	   }
	   
	   hqsos.insert(wband,cnt);
	   bpoints += Qpts;
	  
	   hpoints.insert(wband,bpoints);
	   bmulti1p += Mul1p;
	  
	   hmult1.insert(wband,bmulti1p);
	   bmulti2p += Mul2p;
	   hmult2.insert(wband,bmulti2p);
	}
	
	QTreeWidgetItem * qItem;                       
        qItem = qsoPointsList->topLevelItem(0);        
	
	if(cnt) {
          qItem->setText(col,s.setNum(cnt));           
	  qItem->setTextAlignment(col,Qt::AlignCenter);
	}
	qItem = qsoPointsList->topLevelItem(1);        
	if(bpoints) {
          qItem->setText(col,s.setNum(bpoints));
	  qItem->setTextAlignment(col,Qt::AlignCenter);
	}
	qItem = qsoPointsList->topLevelItem(2);        
	if(bmulti1p) {
          qItem->setText(col,s.setNum(bmulti1p)); 
	  qItem->setTextAlignment(col,Qt::AlignCenter);
	}
	qItem = qsoPointsList->topLevelItem(3);        
	if(bmulti2p) {
          qItem->setText(col,s.setNum(bmulti2p)); 
	  qItem->setTextAlignment(col,Qt::AlignCenter);
	}
	                                               
	qItem = qsoPointsList->topLevelItem(5);        
        qItem->setText(col,s.setNum(0));      
	qItem->setTextAlignment(col,Qt::AlignCenter);
	
	tqsos += cnt;                                  
	tpoints += bpoints;
	tmulti1p += bmulti1p;
	tmulti2p += bmulti2p;
}




void contLog::infoSetExchange()
{
    QString f;
          switch(CNr) {
          case 0 : 
    case CQWAEDC : break;
       case XMAS : contestInfo(CNr);
	           return;
    case DARC10M :
     case HHCont :
        case WAG :
        case HNY : 
	           break;
    case AGCWHTP : //f = "\nCheck and modify sent exchange,\nif needed click Info Button";
                   break;
       case SPDX : contestInfo(CNr);
                   return;
       case REFC : contestInfo(CNr);
                   return;
       case HADX : contestInfo(CNr);
                   return;
       case PACC : contestInfo(CNr);
	           return;
      case CQWPX : contestInfo(CNr);
		   return;
  case OCEANIADX : contestInfo(CNr);
		   return;
     case RUSSDX : contestInfo(CNr);
                   return;
   case CQWW160M :
     case ARRLDX : 
                   break;
      case UBALB : contestInfo(CNr);
	           return;
      case UBADX :
       case CQPTY:
   case NASPRINT :
     case CQWWDX :
                   break;
      }
      f = "\nCheck and modify sent exchange,\nif needed click the Info Button";
      QMessageBox::information(this,
      tr("Information"),f,QMessageBox::Ok);
      lineEditSent->setFocus();
}



void contLog::ButtonInfoCb()
{
    contestInfo(CNr);
}

void contLog::contestInfo(int id)
{
    QString l,s;
    QSqlQuery query;
    qy = "SELECT startday,stime,runtime,mycall  FROM wContest WHERE contestid='"+labelContestId->text()+"'";
    query.exec(qy);
    query.next();
    QString date = query.value(0).toString();      
    QString cstart = query.value(1).toString();    
    QString runtime = query.value(2).toString();   
    QString mCall = query.value(3).toString();     
    l = "";
    if(getBandPoint("b160") != 0)
      l += "- 160m ";
    if(getBandPoint("b80") != 0)
       l += "- 80m ";
    if(getBandPoint("b40") != 0)
       l += "- 40m ";
    if(getBandPoint("b20") != 0)
       l += "- 20m ";
    if(getBandPoint("b15") != 0)
       l += "- 15m ";
    if(getBandPoint("b10") != 0)
       l += "- 10m ";
      
   switch(id) {
     case 0 :   break;
     
     case CQWAEDC : {
                      s = "\n\nExchannge: RS(T) +\nserial number starting with 001";
		      s += "\nRuntime :  "+runtime+" hour";
		      s += "\nBand   "+l;
		      s += "\nQTCs: only sent from DX to EU";
		      s += "\n\nObject : EU amateurs works as many dxcc countries of the world as possible";
		      s+= "\n\nLogfile: "+labelContestId->text();
                    }
                    break;
       case CHECK: {
                     s = "\n\nEintraege in Exchange werden als Received gespeichert\nQSOs werden gezaehlt.\n";
                     s += "\nBand   "+l;
		     s += "\n\nLogfile:  "+labelContestId->text();
                    }             
                    break;
       case XMAS : {
	             s = "\n\nExchange : RS(T) + DOK\nDL station without DOK : NM";
		     s += "\nDX station: RS(T) + serial number";
		     s += "\n\nRuntime  : "+runtime+" hour ";
		     s += "\nBand     "+l;
		     s += "\n\nLogfile:  "+labelContestId->text();
                    }
                    break;
		    
       case DARC10M : 
	
	            break;
		    
       case CQPTY : {
	             s = "\n\nExchange : RS(T)";
		     s += "\nLebensalter des OP";
		     s += "\nYLs : 00";
		     s += "\nWer sein Alter nicht angeben moechte : XX";
		     s += "\n\nRuntime  : "+runtime+" hour ";
		     s += "\nBand     "+l;
		     s += "\n\nLogfile:  "+labelContestId->text();
                    }
		    break;
		    
       case HHCont  : {
	               s = "\n\nExchange : RS(T) +\nDOK";
		       s += "\nRuntime  : "+runtime+" hour for any band";
		       s += "\nBand     "+l;
		       s += "\n\nLogfile:  "+labelContestId->text();
                    }
                    break;
       case WAG     : {
		       s = "\n\nExchange: DL      :  RS(T) + DOK.\nStations without DOK : RS(T) + serial number";
		       s += "\nRuntime :  "+runtime+" hour";; 
		       s += "\nBand       "+l;
		       s += "\n\nObject : Worked All Germany";
		       s += "\n\nLogfile:  "+labelContestId->text();
                    }
                    break;
		
        case HNY : {
		     s = "\nExchange:\n\nRS(T) + Serial number\nAGCW members add their membership number";
		     s += "\nExample :  002/6789";
		     s += "\n\nRuntime :  "+runtime+" hour";; 
		     s += "\nBand         "+l;
		     s += "\n\nObject   :   good luck for  "+h.date().toString("yyyy");
		     s += "\nLogfile:  "+labelContestId->text();
                    }
	            break;
	       
      case AGCWHTP : {
		       s = "\nExchange:\n\nRS(T) + Serial number\nAGCW members add /class/name/age";
		       s += "\nExample :  001/A/Tom/39";
		       s += "\n\nRuntime :  "+runtime+" hour";; 
		       s += "\nBand          "+l;
		       s += "\n\nLogfile:  "+labelContestId->text();
                    }
	            break;
   
       case CQWPX : {
	              s = "\nExchange:  RS(T) +\nSerial number starting with 001";
		      s += "\nRuntime : "+runtime+" hour";;
		      s += "\nBand : "+l+" hour";
		      s += "\nWorking all prefixes around the world";
		      s += "\n\nLogfile: "+labelContestId->text();
                  }
                  break;
                  
      case CQWWDX : {
	              s = "\nExchange:  RS(T) + WAZ Zone ( DL = 14 )";
		      s += "\nRuntime : "+runtime+" hour";;
		      s += "\nBand : "+l+" hour";
		      s += "\nObject : Working all amateurs around the world";
		      s += "\n\nLogfile: "+labelContestId->text();
                   }
                   break;
		   
    case ARRLDX : {
		    s = "\n\nExchange  : RS(T) +";
		    s += "\nK + VE station : state or province";
		    s += "\nOther       :   power as number";
		    s += "\nRuntime  :  "+runtime+" hour";
		    s += "\nBand         "+l;
		    s += "\n\nObject: W/VE amateurs works as many DXCC countries of the World\n as  possible";
		    s += "\n\nLogfile:     "+labelContestId->text();
                  }
	          break;
		  
     case REFC : {
		   s = "\n\nExchange   : RS(T) +";
		   s += "\nF-/TK           : Department number ( 2 letters )";
		   s += "\nF-overseas : Own prefix";
		   s += "\nOthers        : Serial number starting from 001";
		   s += "\n\nRuntime   :  "+runtime+" hour";
		   s += "\nBand          "+l;
		   s += "\n\nLogfile:  "+labelContestId->text();
                 }
	         break;
  case RSGB160M : {
		   s = "\n\nExchange  : RS(T) +";
		   s += "\nUK : Serial number starting from 001 + district code";
		   s += "\nOther : Serial number starting from 001";
		   s += "\nStart:  "+cstart+" UTC /  Runtime : "+runtime+" hour";
		   
		   //s += "\nRuntime    :  "+runtime+" hour";
		   
		   s += "\nBand       "+l;
		   s += "\n\nLogfile:  "+labelContestId->text();
                 }
	         break;
 
  case CQWW160M : {
		    s = "\n\nExchange  : RS(T) +";
		    s += "\nW / VE station : state / province";
		    s += "\nOther         : CQ zone";
		    s += "\nRuntime    :  "+runtime+" hour";
		    s += "\nBand           "+l;
		    s += "\n\nObject: Amateurs around the World contacted other amateurs\nin as any US-state, Canadian provinces and contries as possible";
		    s += "\n\nLogfile:    "+labelContestId->text();
                 }
		 break;
		 
  case NASPRINT : {
		    s = "\n\nExchange  : RS(T) +";
		    s += "\nW / VE station : Serial number/name/state or province";
		    s += "\nOther         : Serial number/name/dxcc country";
		    s += "\n- Example of US/VE : 002/BRUCE/CA";
		    s += "\n- Example of EU       : 001/DIEK/DL";
		    s += "\nRuntime    :  "+runtime+" hour";
		    s += "\nBand           "+l;
		    s += "\n\nObject : contact as many North American stations as possible";
		    s += "\n\nLogfile:    "+labelContestId->text();
                 }
		 break;
		 
      case PACC : {
		    s = "\n\nExchange  : RS(T) +";
		    s += "\nPA : Province abbreviation";
		    s += "\nOther : Serial number starting with 001";
		    s += "\nRuntime    :  "+runtime+" hour";
		    s += "\nBand           "+l;
		    s += "\n\nObject: Amateurs around the World contacted other amateurs\nin as any PA provinces as possible";
		    s += "\n\nLogfile:    "+labelContestId->text();
                 }
		 break;
		 
     case RUSSDX : {
		    s = "\n\nExchange  : RS(T)";
		    s += "\nRussian: Oblast code ( two letters )";
		    s += "\nOther : QSO number starting with 001";
		    s += "\nRuntime    :  "+runtime+" hour";
		    s += "\nBand           "+l;
		    s += "\n\nLogfile:    "+labelContestId->text();
                 }
		 break;
		 
   case OCEANIADX : {
		      s = "\n\nExchange  : RS(T) +";
		      s += "\nAll: Serial number starting  wirh 001";
		      s += "\nStart:  "+cstart+" UTC /  Runtime : "+runtime+" hour";
		      s += "\nBand           "+l;
		      s += "\n\nObject: Oceania transmitting station to contact as many stations as possible both inside and outside the Oceania region.";
		      s += "\n\nLogfile:  "+labelContestId->text();
                 }
		 break;
		 
       case UBADX : {
		    s = "\n\nExchange  : RS(T) +";
		    s += "\nON: additional province abbreviation";
		    s += "\nOther : Serial number starting with 001";
		    s += "\nRuntime    :  "+runtime+" hour";
		    s += "\nBand       "+l;
		    s += "\n\nLogfile:  "+labelContestId->text();
                  }
	          break;
      case ONCONT :
		      break;
		      
       case UBASP : {  
		      s = "\n\nExchange  : RS(T) +";
		      s += "\nUBA : Province abbreviation";
		      s += "\nOther : Serial number starting with 001";
		      s += "\nStart time:  "+cstart+" UBA local /  Runtime : "+runtime+" hour";
		      s += "\nBand           "+l;
		      s += "\n\nLogfile:    "+labelContestId->text();
                 }
		 break;
      case UBALB : {                              
		      s = "\n\nExchange  : RS(T) +";
		      s += "\nON : UBA section ( 3 characters )";
		      s += "\nOthers : Serial number starting with 001";
		      s += "\nStart time:  "+cstart+"  - Runtime : "+runtime+" hour";
		      s += "\nBand           "+l;
		      s += "\n\nLogfile:    "+labelContestId->text();
                 }
		 break;
		 
      case HADX : {                              
		      s = "\n\nExchange  : RS(T) +";
		      s += "\nHA : country code ( 2 letters )";
		      s += "\nOthers : Serial number starting with 001";
		      s += "\n\nStart time:  "+cstart+"  - Runtime : "+runtime+" hour";
		      s += "\nBand           "+l;
		      s += "\nObject : work all HA countries as possible";
		      s += "\n\nLogfile:    "+labelContestId->text();
                 }
		 break;
		 
     case SPDX : {                              
		      s = "\n\nExchange  : RS(T) +";
		      s += "\nSP : Voivodeship ( one letter )";
		      s += "\nOthers : Serial number from 001";
		      s += "\n\nStart time:  "+cstart+"  - Runtime : "+runtime+" hour";
		      s += "\nBand           "+l;
		      s += "\nObject : work all SP Voivodeship as possible";
		      s += "\n\nLogfile:    "+labelContestId->text();
                 }
		 break;
		 
		 
		 
     }
     QMessageBox::information(this,tr("Information"),s,QMessageBox::Ok);
}
    
    
    




void contLog::createContestClass(int newCont, int ContNr)
{ 
        switch(ContNr) {
        case 0 : break;
	case CHECK : {                                      
		      cBase * MyContest;                    
		      MyContest = (cBase*) new cCheck();    
		      myContest = MyContest;                
	            }
	      break;
      
         case XMAS : {                                      
		       cBase * MyContest;
		       MyContest = (cBase*) new cXmas();
		       myContest = MyContest;
		       if(newCont) {
		   	 lineEditSent->setText(settings.value("Kenner").toString()); 
		       }
                     }
                break;
	case HHCont : {                                     
		        cBase * MyContest; 
		        MyContest = (cBase*) new cHHcont();
		        myContest = MyContest;
		        if(newCont) {
			  lineEditSent->setText(settings.value("Kenner").toString()); 
		        }
	              }
	         break;
	       
	case DARC10M : {                                     
		        cBase * MyContest; 
		        MyContest = (cBase*) new cDARC10M();
		        myContest = MyContest;
		        if(newCont) {
			  sentVal = settings.value("Kenner").toString();   
			  lineEditSent->setText("001/"+sentVal);           
			  stxNr = 1;
		        }
	             }
	        break;
	 case CQPTY : {                                     
		        cBase * MyContest; 
		        MyContest = (cBase*) new cCQPTY();
		        myContest = MyContest;
	             }
	        break;
		 
	   case HNY : {                                      
		        cBase * MyContest; 
		        MyContest = (cBase*) new cHNY();
		        myContest = MyContest;
	             }
	        break;
	       
       case AGCWHTP : {                                      
		        cBase * MyContest; 
		        MyContest = (cBase*) new cAGCWHTP();
		        myContest = MyContest;
			
		        QString h = QDir::homePath();         
                        s = h+"/log/iniFiles/AgcwClassHTP.ini";
                        QFile file(s);
                        if(!file.open(QIODevice::ReadOnly)) {
                           qDebug() << "FEHLER - Klassen-Tabelle.ini fehlt";
                        }
                        QTextStream istream( &file); 
			s = istream.readLine(0);              
                        while(1) {                            
                          s = istream.readLine(0); 
                          if(s.count() == 0) break;
                          i = s.indexOf("=");
                          t = s.mid(i +1);
                          s = s.left(i);
			  myContest->insertRange(s,t);       
		        }
		        file.close();
	             }
	       break;
		 
	 case CQWPX : {                                      
		        cBase * MyContest; 
		        MyContest = (cBase*) new cCQWPX();
		        myContest = MyContest;
			QString h = QDir::homePath();         
                        s = h+"/log/iniFiles/CqwpxContDx.ini";
                        QFile file(s);
                        if(!file.open(QIODevice::ReadOnly)) {
                           qDebug() << "FEHLER - CqwpxContDx.ini fehlt";
                        }
                        QTextStream istream( &file); 
			s = istream.readLine(0);              
                        while(1) {                            
                          s = istream.readLine(0); 
                          if(s.count() == 0) break;
                          i = s.indexOf("=");
                          t = s.mid(i +1);
                          s = s.left(i);
			  myContest->insertRange(s,t);        
		        }
		        file.close();
		        h = QDir::homePath();                 
                        s = h+"/log/iniFiles/CqwpxContHome.ini";
                        QFile file2(s);
                        if(!file2.open(QIODevice::ReadOnly)) {
                           qDebug() << "FEHLER - CqwpxContHome.ini fehlt";
                        }
                        QTextStream istream2( &file2); 
			s = istream.readLine(0);              
                        while(1) {                            
                          s = istream2.readLine(0); 
                          if(s.count() == 0) break;
                          i = s.indexOf("=");
                          t = s.mid(i +1);
                          s = s.left(i);
			  myContest->insertRange2(s,t);       
		        }
		        file2.close();
	             }
	        break;
	       
       case CQWAEDC : {                                      
		       cBase * MyContest; 
		       MyContest = (cBase*) new cWAEDC();
		       myContest = MyContest;
		      
	               QString h = QDir::homePath();         
                       s = h+"/log/iniFiles/DistriktZifferList.ini";
                       QFile file(s);
                       if(!file.open(QIODevice::ReadOnly)) {
                           qDebug() << "FEHLER - DistriktZifferList.ini fehlt";
                       }
                       QTextStream istream( &file); 
		       s = istream.readLine(0);              
                       while(1) {                            
                          s = istream.readLine(0); 
                          if(s.count() == 0) break;
                          i = s.indexOf("=");
                          t = s.mid(i +1);
                          s = s.left(i);
			  myContest->insertRange(s,t);       
                       }
                       file.close();
	           }
	     break;
           case WAG : {                                      
		        cBase * MyContest; 
		        MyContest = (cBase*) new cWAG();
		        myContest = MyContest;
			if(newCont) {
			  lineEditSent->setText(settings.value("Kenner").toString()); 
	                }
	            }
	      break;
	       
	 case ARRLDX : {                                     
		        cBase * MyContest; 
		        MyContest = (cBase*) new cARRLDX();
		        myContest = MyContest;
                        
			QSqlQuery query;
                        qy = "SELECT cqarea,pcode FROM uswas WHERE sel=2";  
                        query.exec(qy);
                        while(query.next()) {
			  s = query.value(0).toString();
                          t = query.value(1).toString();
			  myContest->insertRange(s,t);        
			}
			/*
		       QString h = QDir::homePath();          
                       s = h+"/log/iniFiles/ArrlDxWrong.ini";
                       QFile file(s);
                       if(!file.open(QIODevice::ReadOnly)) {
                           qDebug() << "FEHLER - ARRL-DX-Wrong.ini fehlt";
                       }
                       QTextStream istream( &file); 
		       s = istream.readLine(0);                 
                       while(1) {                               
                          s = istream.readLine(0); 
                          if(s.count() == 0) break;
                          i = s.indexOf("=");
                          t = s.mid(i +1);
                          s = s.left(i);
			  myContest->insertRange2(s,t);         
                       }
                       file.close();
		       */
	            }
	      break;
	       
	 case UBADX : {                                       
	       	       cBase * MyContest; 
		       MyContest = (cBase*) new cUBADX();
		       myContest = MyContest;
		      
	               QString h = QDir::homePath();          
                       s = h+"/log/iniFiles/ONprovinces.ini";
                       QFile file(s);
                       if(!file.open(QIODevice::ReadOnly)) {
                           qDebug() << "FEHLER - ON ProvincesList.ini fehlt";
                       }
                       QTextStream istream( &file); 
		       s = istream.readLine(0);                 
                       while(1) {                               
                          s = istream.readLine(0); 
                          if(s.count() == 0) break;
                          i = s.indexOf("=");
                          t = s.mid(i +1);
                          s = s.left(i);
			  myContest->insertRange2(s,t);         
                       }
                       file.close();
		       
                       s = h+"/log/iniFiles/EUcountries.ini";
                       QFile eufile(s);
                       if(!eufile.open(QIODevice::ReadOnly)) {
                           qDebug() << "FEHLER - Distriktziffer.ini fehlt";
                       }
                       QTextStream istreameu( &eufile); 
		       s = istream.readLine(0);                 
                       while(1) {                               
                          s = istreameu.readLine(0); 
                          if(s.count() == 0) break;
                          i = s.indexOf("=");
                          t = s.mid(i +1);
                          s = s.left(i);
			  myContest->insertRange(s,t);          
                       } 
                       eufile.close();
	            }
	       break;
	       
        case CQWWDX : {                                         
		        cBase * MyContest; 
		        MyContest = (cBase*) new cCQWWDX();
		        myContest = MyContest;
			//if(newCont) {
			
	                //}
	            }
	       break;
	case UBASP : {                                          
		        cBase * MyContest; 
		        MyContest = (cBase*) new cUBASP();
		        myContest = MyContest;
	              }
	        break;
	case UBALB : {                                          
		        cBase * MyContest; 
		        MyContest = (cBase*) new cUBALB();
		        myContest = MyContest;
	              }
	        break;
		
	case ONCONT : {                                          
		        cBase * MyContest; 
		        MyContest = (cBase*) new cONCONT();
		        myContest = MyContest;
	              }
	        break;
	
	  case REFC : {                                          
	       		cBase * MyContest; 
		        MyContest = (cBase*) new cREFC();
		        myContest = MyContest;
		      
	                QString h = QDir::homePath();            
                        s = h+"/log/iniFiles/FrenchOverseasDepartments.ini";
                        QFile file(s);
                        if(!file.open(QIODevice::ReadOnly)) {
                            qDebug() << "FEHLER - FrenchOverseasList.ini fehlt";
                        }
                        QTextStream istream( &file); 
			s = istream.readLine(0);                 
                        while(1) {                               
                          s = istream.readLine(0); 
                          if(s.count() == 0) break;
                          i = s.indexOf("=");
                          t = s.mid(i +1);
                          s = s.left(i);
			  myContest->insertRange(s,t);           
                        }
                        file.close();
	            }
	       break;
 
      case RSGB160M : {                                          
	       		cBase * MyContest; 
		        MyContest = (cBase*) new cRSGB160M();
		        myContest = MyContest;
			
			QString h = QDir::homePath();            
                        s = h+"/log/iniFiles/RSGBprefList.ini";
                        QFile file(s);
                        if(!file.open(QIODevice::ReadOnly)) {
                            qDebug() << "FEHLER - RSGBprefList.ini fehlt";
                        }
                        QTextStream istream( &file); 
			s = istream.readLine(0);                 
                        while(1) {                               
                          s = istream.readLine(0); 
                          if(s.count() == 0) break;
                          i = s.indexOf("=");
                          t = s.mid(i +1);
                          s = s.left(i);
			  myContest->insertRange(s,t);           
                        }
                        file.close();
	            }
	        break;
 
         case CQWW160M:{                                         
	    	        cBase * MyContest; 
		        MyContest = (cBase*) new cCQWW160M();
		        myContest = MyContest;
	    
	                if(newCont) {
		          qy = "SELECT cqz FROM tla WHERE la='"+settings.value("Dxcc").toString()+"'";   
		          QSqlQuery query(qy);
                          query.next();
                          lineEditSent->setText(query.value(0).toString());
		        }
                        QSqlQuery query;
                        qy = "SELECT cqarea,pcode FROM uswas WHERE sel=2";  
                        query.exec(qy);
                        while(query.next()) {
			  s = query.value(0).toString();
                          t = query.value(1).toString();
			  myContest->insertRange(s,t);               
			}
	            }
	        break;
	case NASPRINT:{                                               
	    	        cBase * MyContest; 
		        MyContest = (cBase*) new cNASPRINT();
		        myContest = MyContest;
                        QSqlQuery query;
                        qy = "SELECT cqarea,pcode FROM uswas WHERE sel=2";  
                        query.exec(qy);
                        while(query.next()) {
			  s = query.value(0).toString();
                          t = query.value(1).toString();
			  myContest->insertRange(s,t);               
			}
	            }
	       break;
		
          case PACC : {                                              
		        cBase * MyContest; 
		        MyContest = (cBase*) new cPACC();
		        myContest = MyContest;
	             }
	        break;
		
         case RUSSDX : {                                             
		        cBase * MyContest; 
		        MyContest = (cBase*) new cRUSSDX();
		        myContest = MyContest;
	             }
	        break;
		
     case OCEANIADX : {                                              
		        cBase * MyContest; 
		        MyContest = (cBase*) new cOCEANIADX();
		        myContest = MyContest;
	             }
	        break;
		
          case HADX : {                                              
		        cBase * MyContest; 
		        MyContest = (cBase*) new cHADX();
		        myContest = MyContest;
			
                        QString h = QDir::homePath();                
                        s = h+"/log/iniFiles/HungarianCountry.ini";
                        QFile file(s);
                        if(!file.open(QIODevice::ReadOnly)) {
                            qDebug() << "FEHLER - Hungary Country List.ini fehlt";
                        }
                        QTextStream istream( &file); 
			s = istream.readLine(0);                     
                        while(1) { 
                          s = istream.readLine(0); 
                          if(s.count() == 0) break;
                          i = s.indexOf("=");
                          t = s.mid(i +1);
                          s = s.left(i);
			  myContest->insertRange(s,t);               
                        }
                        file.close();
	             }
	        break;
		
          case SPDX : {                                              
		        cBase * MyContest; 
		        MyContest = (cBase*) new cSPDX();
		        myContest = MyContest;
			
                        QString h = QDir::homePath();                
                        s = h+"/log/iniFiles/SPprovinzen.ini";
                        QFile file(s);
                        if(!file.open(QIODevice::ReadOnly)) {
                            qDebug() << "FEHLER - SP Provinzen List.ini fehlt";
                        }
                        QTextStream istream( &file); 
			s = istream.readLine(0);                     
                        while(1) { 
                          s = istream.readLine(0); 
                          if(s.count() == 0) break;
                          i = s.indexOf("=");
                          t = s.mid(i +1);
                          s = s.left(i);
			  myContest->insertRange(s,t);                
                        }
                        file.close();
	             }
		
		
       }
}


void contLog::cwkeyerAc()
{
   i = system("keyerdb &");
}

void contLog::hlibAc()
{
    settings.setValue("Val",rigpolling);
    hlib * nhlib = new hlib();
    nhlib->exec(); 
    
    stimer->stop();                                 
    retry = nhlib->nRetry.toInt();
    stimer->start(retry);                           
    runflg = nhlib->run;                            
    rigpolling = runflg;
    
    if(rigpolling) {                                
      modeBox->setEnabled(false);                   
      int z = freqBox->value();                     
      QString c;
      cmdStr = "F "+c.setNum(z)+"000";              
      cmdFlg = 1;
    }
    else 
     modeBox->setEnabled(true);                     
}


void contLog::deleteContest()
{
     QSqlQuery query;                   
     qy = "SELECT expLogbook,expFile FROM wContest WHERE contestid='"+labelContestId->text()+"'";
     query.exec(qy);
     query.next();
     s = query.value(0).toString();     
     t = query.value(1).toString();     
     
     if(s.count() == 0) 
       s = "The Contest is not saved to QtLog";
     else 
       s = "Saved to QtLog : "+query.value(0).toString();
     if(t.count() == 0) 
       t = "The Contest is not exported";
     else 
       t = "Exported : "+query.value(1).toString();
     int status = QMessageBox::question(this,
     tr("Question"),
     tr("\nAre you shure to DELETE all logged data of contest\n%1 - %2\n\n%3\n%4\n")
     .arg(labelContestId->text(),labelContestName->text(),s,t),
					
     QMessageBox::Yes,QMessageBox::No);
     if(status != QMessageBox::Yes)
        return;
     qy = "DELETE FROM wCband WHERE contid='"+labelContestId->text()+"'";  
     query.exec(qy);
     qy = "DELETE FROM wContest WHERE contestid='"+labelContestId->text()+"'";
     query.exec(qy);
     qy = "DELETE FROM wCqso WHERE contid='"+labelContestId->text()+"'";
     query.exec(qy);
     if(CNr) {                          
       clearAll();
       labelContestId->setText("No Name");
       labelContestName->setText("No Name");
       myContest->clearAllTable();
     }
     stackedWidget->setCurrentIndex(2);
}
