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
#include "awdst.h"


void awdst::BoxAwdCb(QString awd)
{
    if(awd.compare("DLD") == 0) {
       if(checkBoxBand->isChecked() == true)
          getAWDband("DLD");                       
       else 
       awStatusDLD("DLD");
    }
    else
     if(awd.compare("IOTA") == 0) {
       if(checkBoxBand->isChecked() == true)
          getAWDband("IOTA");                      
       else 
        awStatusIOTA("IOTA");
     }
    else {
       if(checkBoxBand->isChecked() == true)
          getAWDband(awd);                         
       else 
       awStatusDIV(awd);
    }
    stackedWidget->setCurrentIndex(3);
}



void awdst::awStatusIOTA(QString iota)
{
      qy = "SELECT awkenner,iconty,";
      qy += "(SELECT qslr FROM "+settings.value("logfile").toString()+" WHERE qid=id)";
      qy += " FROM ("+settings.value("logfile").toString()+"awd,tiota) WHERE awtype='";
      qy += iota;
      qy += "'AND iota=awkenner ORDER BY awkenner";
      awdStatus(qy,1);
}



void awdst::awStatusDLD(QString dld)
{
      qy = "SELECT awkenner,ovnam,";
      qy += "(SELECT qslr FROM "+settings.value("logfile").toString()+" WHERE qid=id)";
      qy += " FROM ("+settings.value("logfile").toString()+"awd,tdok) WHERE awtype='";
      qy += dld;
      qy += "'AND dok=awkenner ORDER BY awkenner";
      awdStatus(qy,1);
}



void awdst::awStatusWAS(QString awd)
{
     QString s, kenner, qslr, pcode;
     int x, eCnt, kenCnt, pcodeCnt, k, rows, row, id, qid, cnt, workedCnt, qslrbCnt, qslrlCnt;
     QTreeWidgetItem * litem;
     QString log, qslrb, qslrl;
      
        log = settings.value("logfile").toString();
        litem = 0;
        stackedWidget->setCurrentIndex(3);
        QSqlQuery query;
        pcodeCnt = 0;
        kenCnt = 0;
        eCnt = 0;
        x = 0;
        k = 0;
        rows = 0;
        row = 0;
        id = 0;
        qid = 0;
        cnt = 0;
      
        AwdList->clear();
        AwdList->setColumnCount(6);                 
      
        AwdList->setColumnWidth(0,80);              
        AwdList->setColumnWidth(1,160);             
        AwdList->setColumnWidth(2,80);              
        AwdList->setColumnWidth(3,60);              
        AwdList->setColumnWidth(4,60);              
        AwdList->setColumnWidth(5,60);              
        i = 0;
        AwdList->headerItem()->setText(i++,"PostCode");
        AwdList->headerItem()->setText(i++,"State");
        AwdList->headerItem()->setText(i++,"Worked");
        AwdList->headerItem()->setText(i++,"QslrB ");
        AwdList->headerItem()->setText(i++,"QslrLotw ");
        AwdList->headerItem()->setText(i++," *");
     
        
        qy = "SELECT pcode,state FROM uswas WHERE sel='1' ORDER BY pcode";
        query.exec(qy);
        pcodeCnt = query.size();
        while(query.next()) {
          n = 0;
          i = 0;
          QTreeWidgetItem * item = new QTreeWidgetItem(AwdList); 
          item->setText(i,query.value(n++).toString());          
          item->setTextAlignment(i++,Qt::AlignCenter);
          item->setText(i++,query.value(n++).toString());        
          item->setText(i++,"");                                 
          item->setText(i++,"");                                 
          item->setText(i++,"");                                 
          item->setText(i++,"");                                 
        }
 
        rows = AwdList->topLevelItemCount();      
        QTreeWidgetItem * item1 = new QTreeWidgetItem(AwdList);
	item1->setText(0,"------");
	item1->setTextAlignment(0,Qt::AlignCenter);
	item1->setText(1,"------");
	item1->setText(2,"------");
	item1->setTextAlignment(2,Qt::AlignCenter);
	item1->setText(3,"------");
	item1->setTextAlignment(3,Qt::AlignCenter);
	item1->setText(4,"------");
	item1->setTextAlignment(4,Qt::AlignCenter);
        QTreeWidgetItem *cItem;
	workedCnt = 0;
	qslrbCnt = 0;
	qslrlCnt = 0;
        row = 0;
        while(rows != row) {
          cItem = AwdList->topLevelItem(row++);
          pcode = cItem->text(0);
  
          qy = "SELECT id,qid,qslr,lotqslr FROM "+log+" LEFT JOIN ("+log+"qsl,"+log+"awd) ON (qsoid=id AND qid=id) ";
          qy += "WHERE awtype='"+awd+"' AND awkenner LIKE '"+pcode+"%'";
          query.exec(qy);
          cnt = query.size();
          while(query.next()) {
            n = 0;
            id = query.value(n++).toString().toInt();
            qid = query.value(n++).toString().toInt();
      
	    cItem->setText(2,s.setNum(cnt)); 
	    cItem->setTextAlignment(2,Qt::AlignCenter);
            qslrb = query.value(n++).toString();
            x = cnt; 
            while( x-- ) {
              cItem->setText(3,qslrb);                  
	      cItem->setTextAlignment(3,Qt::AlignCenter);
              if(qslrb.indexOf("Y") != -1) {            
		qslrbCnt++;
                break;
	      }
            }  
            qslrl = query.value(n++).toString();        
            x = cnt;
            while( x-- ) {
              cItem->setText(4,qslrl);                  
              cItem->setTextAlignment(4,Qt::AlignCenter);
              if(qslrl.indexOf("Y") != -1) {            
		qslrlCnt++;
                break;
              }
            }
            workedCnt++; 
          }
	}
	
	QTreeWidgetItem * item2 = new QTreeWidgetItem(AwdList);
	
	item2->setText(0,s.setNum(rows));
        item2->setTextAlignment(0,Qt::AlignCenter);
	
	item2->setText(2,s.setNum(workedCnt));
        item2->setTextAlignment(2,Qt::AlignCenter);
	
	item2->setText(3,s.setNum(qslrbCnt));
        item2->setTextAlignment(3,Qt::AlignCenter);
	
	item2->setText(4,s.setNum(qslrlCnt));
        item2->setTextAlignment(4,Qt::AlignCenter);
}



void awdst::awStatusDIV(QString awd)
{
    if(awd.compare("US-CA") == 0)
      awStatusWAS(awd);
    else {
      qy = "SELECT awkenner,";
      qy += "(SELECT qslr FROM "+settings.value("logfile").toString()+" WHERE qid=id)";
      qy += " FROM "+settings.value("logfile").toString()+"awd WHERE awtype='";
      qy += awd;
      qy += "' ORDER BY awkenner";
      awdStatus(qy,0);
    }
}



void awdst::awdStatus(QString q, int fl)
{
  QString s, ov, ken, kenner, qslr;
  int x, eCnt, kenCnt, k;
  QTreeWidgetItem * litem;
  
      litem = 0;
      stackedWidget->setCurrentIndex(3);
      QSqlQuery query;
      kenner = "";
      kenCnt = 0;
      eCnt = 0;
      x = 0;
      k = 0;
      AwdList->clear();
      AwdList->setColumnCount(4);
      AwdList->setColumnWidth(0,120);               
      AwdList->setColumnWidth(1,100);               
      AwdList->setColumnWidth(2,110);               
      i = 0;
      AwdList->headerItem()->setText(i++,"AWD Kenner");
      AwdList->headerItem()->setText(i++,"Gearbeitet");
      AwdList->headerItem()->setText(i++,"QSL erhalten");
      AwdList->headerItem()->setText(i++,"Name");
      query.exec(q);
      while(query.next()) {
         n = 0;
         i = 0;
         ov = "-";
         ken = query.value(n++).toString();          
         if( fl )                                    
            ov = query.value(n++).toString();        
         qslr = query.value(n++).toString();         
         if(ken.compare(kenner) != 0) {              
              QTreeWidgetItem * item = new QTreeWidgetItem(AwdList);
              item->setText(i,ken);                  
              item->setTextAlignment(i++,Qt::AlignCenter);
              x = 1;                                 
              item->setText(i,s.setNum(x));          
              item->setTextAlignment(i++,Qt::AlignCenter);
              item->setText(i,qslr);                 
              item->setTextAlignment(i++,Qt::AlignCenter);
              if(qslr.compare("Y") == 0)
                 eCnt++;                             
              item->setText(i++, ov);                
              kenner = ken;                          
              litem = item;                          
              k++;                                   
          }
          else {                                     
            x++;                                     
            litem->setText(1,s.setNum(x));           
            if(qslr.compare("Y") == 0) {
              litem->setText(2,qslr);                
              eCnt++;                                
            }
         }
         kenCnt++;                                   
      }
      QTreeWidgetItem * item = new QTreeWidgetItem(AwdList);
      item->setText(0,"----------");
      item->setTextAlignment(0,Qt::AlignCenter);
      item->setText(1,"----------");
      item->setTextAlignment(1,Qt::AlignCenter);
      item->setText(2,"----------");
      item->setTextAlignment(2,Qt::AlignCenter);
      item->setText(3,"---------------------------");
      QTreeWidgetItem * eitem = new QTreeWidgetItem(AwdList);
      eitem->setText(0,s.setNum(k));
      eitem->setTextAlignment(0,Qt::AlignCenter);
      eitem->setText(1,s.setNum(kenCnt));
      eitem->setTextAlignment(1,Qt::AlignCenter);
      eitem->setText(2,s.setNum(eCnt));
      eitem->setTextAlignment(2,Qt::AlignCenter);
      if(BoxAwd->currentText().compare("DLD") == 0 )
         eitem->setText(3,tr("SONDER_DOKs werden z.Z. nicht beruecksichtigt !!"));
      QTreeWidgetItem * enditem = new QTreeWidgetItem(AwdList);
      enditem->setText(0,"");
}




int awdst::getBandIndex(QString sband)
{
   int id;
     id = 0;
     for(int i = 0; i < bandlist.size(); ++i) { 
       if (bandlist.at(i) == sband) {
          id = i;
          break;
       }
     }
     return id;
}



void awdst::getAWDband(QString awd)
{
    qy = "SELECT awkenner,(SELECT qslr FROM "+logFile+" WHERE qid=id),";
    qy += "(SELECT band FROM "+logFile+" WHERE qid=id) ";
    qy += "FROM "+logFile+","+logFile+"awd ";
    qy += "WHERE qid=id AND awtype='";
    qy += awd+"' ORDER BY awkenner";
    showAwdData(qy);
}



void awdst::showAwdData(QString q)
{
  QString s, ken, kenner, qslr, bqslr, band;
  int x, eCnt, kenCnt, gCnt, k, ii, bindx;
  QTreeWidgetItem * litem;
      stackedWidget->setCurrentIndex(3);
      litem = 0;
      i = 0;
      k = 0;
      kenCnt = 0;
      gCnt = 0;
      kenner = "";
      bqslr = "";
      eCnt = 0;
      x = 0;
      
      AwdList->clear();                             
      n = bandlist.size();                          
      AwdList->setColumnCount(n +3);                
      AwdList->headerItem()->setText(i++,tr("AWD Kenner"));
      AwdList->setColumnWidth(0,120);
      AwdList->headerItem()->setText(i++,tr("Gearbeitet"));
      AwdList->setColumnWidth(1,90);
      AwdList->headerItem()->setText(i++,tr("QSL erhalten"));
      AwdList->setColumnWidth(2,110);
      n = 3;                                        
      for(int i = 0; i < bandlist.size(); ++i) {    
        AwdList->headerItem()->setText(n,getMyband(bandlist.at(i)));
        AwdList->setColumnWidth(n++,45);            
      }
      QSqlQuery query;                              
      query.exec(q);
      while(query.next()) {
         n = 0;
         ii = 0;
         ken  = query.value(n++).toString();         
         qslr = query.value(n++).toString();         
         band = query.value(n++).toString();         
         bindx = getBandIndex(band);                 
         if(ken.compare(kenner) != 0) {              
              QTreeWidgetItem * item = new QTreeWidgetItem(AwdList); 
              item->setText(ii,ken);                 
              item->setTextAlignment(ii++,Qt::AlignCenter);
              x = 1;                                 
              bqslr = "";
              item->setText(ii,s.setNum(x));         
              item->setTextAlignment(ii++,Qt::AlignCenter);
              eCnt = 0;
              if(qslr.compare("Y") == 0) {           
                 eCnt = 1;                           
                 item->setText(ii,s.setNum(eCnt));   
                 item->setTextAlignment(ii,Qt::AlignCenter);
                 bqslr = qslr;                       
              }
              ii++;                                  
              if(bqslr.compare("Y") == 0)            
                  item->setText(ii + bindx,bqslr);   
               else
                 item->setText(ii + bindx,qslr);     
              item->setTextAlignment(ii + bindx,Qt::AlignCenter);
              kenner = ken;                          
              litem = item;                          
              k++;                                   
          }
          else {                                     
            x++;                                     
            litem->setText(1,s.setNum(x));           
            if(qslr.compare("Y") == 0) {             
              litem->setText(3 +bindx,qslr);         
              litem->setTextAlignment(3 +bindx,Qt::AlignCenter);
              eCnt++;                                
              litem->setText(2,s.setNum(x));         
              litem->setTextAlignment(2,Qt::AlignCenter);
              bqslr = qslr;                          
            }
            if(bqslr.compare("Y") == 0)              
                litem->setText(3 + bindx,bqslr);     
            else
               litem->setText(3 + bindx,qslr);       
            litem->setTextAlignment(3 +bindx,Qt::AlignCenter);
         }
         kenCnt++;                                   
         gCnt = gCnt +eCnt;                          
      }
      QTreeWidgetItem * item = new QTreeWidgetItem(AwdList);
      item->setText(0,"----------");
      item->setTextAlignment(0,Qt::AlignCenter);
      item->setText(1,"----------");
      item->setTextAlignment(1,Qt::AlignCenter);
      item->setText(2,"----------");
      item->setTextAlignment(2,Qt::AlignCenter);
      QTreeWidgetItem * eitem = new QTreeWidgetItem(AwdList);
      eitem->setText(0,s.setNum(k));                      
      eitem->setTextAlignment(0,Qt::AlignCenter);
      eitem->setText(1,s.setNum(kenCnt));                 
      eitem->setTextAlignment(1,Qt::AlignCenter);
      eitem->setText(2,s.setNum(gCnt));                   
      eitem->setTextAlignment(2,Qt::AlignCenter);
      QTreeWidgetItem * enditem = new QTreeWidgetItem(AwdList);
      enditem->setText(0,tr("ACHTUNG -TEST-version. Die Ergebnisse sind nicht geprueft !"));  
}
