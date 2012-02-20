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
#include <QString>
#include <QtNetwork>
#include "qslPr.h"
#include "postFile.h"
#include "translate.h"
#include "../qtlogDiag/dirmngr.h"
extern DirMngr dirMngr;
struct tm * tm;
time_t caltime;
#define DATUM 60


void qslPr::callEditedCb()
{
     qy = "SELECT cept,rufz,name,qslmgr,qsls,qslr,band,day,btime,mode,txpwr,rig,id ";
     qy += "FROM "+logFile+"om LEFT JOIN "+logFile+" ON (omid=oid) ";
     qy += "WHERE rufz LIKE '"+CallEdit->text()+"%' AND prflg = 'B' ORDER BY ";
     if(checkBoxOrder->isChecked() == false)
       qy += "day";
     else
       qy += "rufz";
     loadQsls(qy,0);
     CallEdit->setFocus();
}


void qslPr::loadQsls(QString q, int add)
{
    QString cept, rufz;
    translate tr;
    tr.installHashTableBand();     
    if( !add )
      prList->clear();
    QSqlQuery query;
    query.exec(q);
    cnt = query.size();
    while(query.next()) {                                   
        n = 0;
        i = 0;
        QTreeWidgetItem *item = new QTreeWidgetItem(prList);
        cept = query.value(n++).toString();
        rufz = query.value(n++).toString();
        cept += rufz;
        item->setText(i++,cept);                           
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i,query.value(n++).toString());      
        item->setTextAlignment(i++,Qt::AlignCenter);
        item->setText(i,query.value(n++).toString());      
        item->setTextAlignment(i++,Qt::AlignCenter);
        item->setText(i++,tr.getMyband(query.value(n++).toString()));  
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
     }
     s = s.setNum(cnt)+QString::fromUtf8("  QSO.s nicht bestätigt");
     labelQsos->setText(s);
     CallEdit->setFocus();
}



void qslPr::loadPrtMedium()
{
  QString s, b, c;
  int cn, n;
    cn = 0;
    qy = "SELECT traeger,style,prtype,zuf FROM prmedium ";
    qy += "WHERE sel=1";
    QSqlQuery query;
    query.exec(qy);
    n = 0;
    cn = query.size();
    if( cn ) {
      query.next();
      Labtype->setText(query.value(n++).toString());        
      strcpy(style,query.value(n++).toString().toAscii());  
      if(strcmp("K",style) == 0) {
        s = tr("Karte ");
        prtype = 1;                                         
      }
      else
        if(strcmp("L",style) == 0) {
         s = tr("Label ");                                  
         prtype = 0;
        }
      else                                                 
        if(strcmp("Lswl",style) == 0) {
          s = tr("Label-SWL ");
          prtype = 0;
        }
      else                                                 
       if(strcmp("Kswl",style) == 0) {
         s = tr("Karte-SWL ");
         prtype = 1;
      }
    MediumType->setText(s);                                
    lprtype = query.value(n++).toString();                 
    zuf = query.value(n++).toString();                     
  }
  if( prtype == 0 ) {                                      
   labelOld = Lab_1;
   Lab_1->setPalette( QPalette(QColor(180, 210, 200)));    
   labelPos = 1;
  }
  else
   labelOld->setPalette( QPalette(QColor(238, 238, 238))); 
}



void qslPr::printQSL()
{
  int sheet;
  int label;
  int qn, id, err, row, samelqsl, qslcnt;
  char datum[30], zeit[20];
  QString s, rufz, phead;
 
  PostFile post;
  QSqlQuery query;
  caltime = time(NULL);                        
  tm = gmtime(&caltime);
  strftime(datum,DATUM,"%Y-%m-%d",tm);         
  strftime(zeit,DATUM,"%M%S",tm);
  id = 0;
  err = 0;
  samelqsl = 0;                                
  if(checkBoxSqsl->isChecked() == TRUE ) {     
     samelqsl = 1;                             
  }
  post.LoadMedium(Labtype->text());            
  QTreeWidgetItem *Item;                       
  row = prList->topLevelItemCount();           
  sheet = 0;                                   
  qn = 0;                                      

  if( row ) {                                  
    Item = prList->topLevelItem(qn);           
    sheet = 1;                                 
#ifdef Q_WS_WIN
    QString qfname=QString("%1-%2.ps").arg(QDateTime::currentDateTime().toString("ddmmyyy_hhMMss")).arg(sheet);
    qfname=dirMngr.CreatePath(qfname,DIR_TEMP);  //oder DIR_DATA1?
    tempfile=fopen(qfname.toAscii(),"wt");       
#else
    
    
    s =  QDir::homePath();
    s += "/tmp";
    QDir dir(s);
    if( !dir.exists() ) {                           
         dir.mkdir(s);                              
    }
    
    QString qfname=QString("%1%2-%3.ps").arg(style).arg(QDateTime::currentDateTime().toString("ddmmyyy_hhMMss")).arg(sheet);
    qfname=dirMngr.CreatePath(qfname,DIR_TEMP);     
    pf = QDir::homePath()+qfname;                   
    QFile pfile(pf);
    if (!pfile.open(QIODevice::Append | QIODevice::Text)) { 
         qDebug() << "ERROR " << pf;                        
       return;
    }
#endif
    QTextStream out(&pfile);                        
    phead = "%!PS-Adobe-2.0\n";                     
    phead += "72 2.54 div dup scale\n";             
    if( prtype ) {                                  
       if(zuf.compare("0") != 0) {                  
         phead += zuf+" rotate\n";                  
         phead += " 0 -23 translate\n";
       }
       else
        phead += "0 23 translate\n";                
    }
    else
      phead += "0 23 translate\n";                  
    label = labelPos;                               
    qslcnt = 0;                                     
    if( prtype )                                    
      label = 1;   
                                                    

    while( Item ) {                                 
       out << phead;                                
       pfile.close();

       do {
          rufz = Item->text(0);                      
          id = Item->text(11).toInt();               
          
          post.Label( pf, id, logFile, label, samelqsl, qslcnt, datum);

          if( samelqsl ) {                           
            row--;                                   
            if( row == 0 ) break;                    
            qslcnt++;                                
            if(qslcnt == 4)                          
               break;                                
            else {                                            
              Item = prList->topLevelItem(++qn);
              if(QString::compare(Item->text(0),rufz) != 0) { 
                qslcnt = 0;                                   
                if( prtype )                                  
                   break;
                if(label == 8) break;
                label++;
              }
            }
          }
          else {                                
            row--;                              
            if(prtype) break;                   
            if(label == 8) break;               
            label++;                            
            Item = prList->topLevelItem(++qn);  
         }
         if( row == 0 ) break;                  
       } while( row != 0 );                     

       
       QFile pfile(pf);
       if (!pfile.open(QIODevice::Append | QIODevice::Text)) {
         qDebug() << "ERROR_2 " << pf;
        return;
       }
       QTextStream out(&pfile);
       out << "showpage\n%% EOF\n";  
       pfile.close();
       
       if(checkBoxLpr->isChecked() == TRUE) {   
#ifdef Q_WS_WIN
           StartProcess(qfname);                //.ps muss auf Ghostview registriert sein
					        
#else
         if(checkBoxForm->isChecked() == TRUE) {
           QString h;
           h = "ps2pdf ";
           h += pf;                            
           h += " ";
           n = pf.indexOf(".");
           QString f;
           f = pf.left(n);                     
           f += ".pdf";                        
           h += f;
           StartProcess(h);                    
           s = lprtype+" "+f;
           StartProcess(s);                    
         }
         else {
           s = lprtype+" "+pf;                 
           StartProcess(s);                    
         }
#endif
        }
        
        if( row != 0 ) {                        
          sheet++;                              
          QString qfname=QString("%1%2-%3.ps").arg(style).arg(QDateTime::currentDateTime().toString("ddmmyyy_hhMMss")).arg(sheet);
          qfname=dirMngr.CreatePath(qfname,DIR_TEMP);  //oder DIR_DATA1?
          pf = QDir::homePath()+qfname;                
          QFile pfile(pf);
          if (!pfile.open(QIODevice::Append | QIODevice::Text)) { 
            qDebug() << "ERROR_3 " << pf;                         
            return;
          }

        QTextStream out(&pfile);               
        out << phead;                          
        pfile.close();
        if( !samelqsl ) {                      
          if( qslcnt == 0)
            ;
          else 
           Item = prList->topLevelItem(++qn);
         Item = prList->topLevelItem(++qn); 
        }
        else {                                 
          if( qslcnt == 0)
            ;
          else
           Item = prList->topLevelItem(++qn);     
        }
        label = 1;                             
        qslcnt = 0;                            
      } 
      if( row == 0 ) break;                    
     }  
    }   

  
  s += QString::fromUtf8("\nWurde der Druck korrekt durchgeführt ?\n");
  s += QString::fromUtf8("Bei Bestätigung wird 'QSL-Sent' auf < Y > gesetzt");
  if( post.GetState() == 1)
      s += QString::fromUtf8("\n\n*RIG ! nicht configuriert **");
  int status = QMessageBox::question( this,tr("DRUCK Erfolg ?"),s,
      QMessageBox::Yes|QMessageBox::Default,
      QMessageBox::No|QMessageBox::Escape,
      QMessageBox::NoButton);
      if(status == QMessageBox::Yes) {
         row = prList->topLevelItemCount();               
         qn = 0;
         while(row != 0) {
            Item = prList->topLevelItem(qn++);             
            Item->setText(3,"Y");                   
            qy = "SELECT qslSvia FROM "+logFile+" WHERE id="+Item->text(11);
            query.exec(qy);
            query.next();
            s = query.value(0).toString();
            if(s.count() != 0) {                           
              if(s.indexOf("B") == -1)                     
                 s += "B";                                 
            }
            else 
              s = "B";                                     
            qy = "UPDATE "+logFile+" SET qsls='Y',qslsd='"+datum+"',prflg='',qslSvia='"+s+"' WHERE id="+Item->text(11);
            query.exec(qy);
            row--;
         }
         s.setNum(n);
         s += QString::fromUtf8("  QSO's   bestätigt");
         labelQsos->setText(s);
         processWriteDatagram(2, 0);                
      }
}



void qslPr::processWriteDatagram(int dNr, QString idNr)
{
    
    
    
    datagram = QByteArray::number(dNr);         
    datagram.append(",");
    datagram.append(idNr);
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, 45454); 
}
