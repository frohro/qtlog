/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.08.2009
********************************************************************** */
#include <QtGui>
#include <QtSql>
#include <QMessageBox>
#include "editLog.h"
#include "defs.h"

editLog::editLog(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
    s = settings.value("Val").toString();
    i = s.indexOf(",");
    QString t = s.left(i);            
    a = t;
    eflg = s.mid(i +1).toInt();       
    s = s.remove(0,i +3);
    i = s.indexOf(",");               
    CNr = s.left(i).toInt();
    s = s.remove(0,i +1);
    band = s;                         
    if(eflg == 0) {
       labelEdit->setText("  Edit ContestLog ");
       ButtonConvert->hide();
    }
    else
     labelEdit->setText("  ContestLog ");
    t += "  ";
    labelContest->setText(t);
    if(! CNr ) {
      qy = "SELECT ctype FROM wContest WHERE contestid='"+a+"'";
      QSqlQuery query;
      query.exec(qy);
      query.next();
      CNr = query.value(0).toInt();
    }
    
    s="Contest,contid;Trs,trans;Date,cdate;Time,ctime;Band,band;Freq,freq;Mode,mode;Call,qcall;sRST,srst;Sent,sent;Sent2,sent2;rRST,rrst;Rcvd,rcvd;Rcvd2,rcvd2;Pts,pts;WAE,wae;Dxcc,dxcc;Cnt,cnt;Multi1,multi1;Mul1,mul1;Multi2,multi2;Mul2,mul2;Qtcflg,qtcflg;";
    while(1) {
      i = s.indexOf(";");
      if( i  != -1) {
         t = s.left(i);      
         s.remove(0, ++i);
         i = t.indexOf(",");
         a = t.left(i);
         t.remove(0, ++i);
         toSysName.insert(a,t);
      }
      else 
       break;                
    }
/*
    
    QHashIterator<QString, QString> i(toSysName);
    while (i.hasNext()) {
       i.next();
       qDebug()  << i.key() << ":" << i.value();
    }
*/

    tableLogEdit->setColumnWidth(0,110);  
    tableLogEdit->setColumnWidth(1,60);   
    tableLogEdit->setColumnWidth(2,100);  
    tableLogEdit->setColumnWidth(3,80);   
    tableLogEdit->setColumnWidth(4,50);   
    tableLogEdit->setColumnWidth(5,65);   
    tableLogEdit->setColumnWidth(6,60);   
    tableLogEdit->setColumnWidth(7,100);  
    tableLogEdit->setColumnWidth(8,60);  
    tableLogEdit->setColumnWidth(9,60);
    tableLogEdit->setColumnWidth(10,60);
    tableLogEdit->setColumnWidth(11,60);
    tableLogEdit->setColumnWidth(12,60);
    tableLogEdit->setColumnWidth(13,60);
    tableLogEdit->setColumnWidth(14,60);
    tableLogEdit->setColumnWidth(15,60);
    tableLogEdit->setColumnWidth(16,60);
    tableLogEdit->setColumnWidth(17,50);
    
    tableLogEdit->setColumnWidth(18,50);  
    tableLogEdit->setColumnWidth(19,50);  
    tableLogEdit->setColumnWidth(20,50);  
    tableLogEdit->setColumnWidth(21,50);  
    tableLogEdit->setColumnWidth(22,50);  
    tableLogEdit->setColumnWidth(23,60);  
    
    readLogs(eflg);                       
    connect(ButtonConvert, SIGNAL(clicked()), this, SLOT(convertToQtlog()));
    connect (tableLogEdit, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateQsoItem(QTableWidgetItem * )));
    connect(ButtonDeleteQso, SIGNAL(clicked()), this, SLOT(deleteQsoCb()));
}

editLog::~editLog()
{
}
//--------------------------------------------------------------
void editLog::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_Escape :
            goExit();
            break;
   }
   tableLogEdit->clearSelection(); 
}

void editLog::goExit()
{
   accept();
}



void editLog::readLogs(int eflg)
{
    i = eflg;
    qy = "SELECT cqsoid,contid,trans,cdate,ctime,band,freq,mode,qcall,srst,sent,sent2,rrst,rcvd,rcvd2,pts,wae,dxcc,cnt,cpref,multi1,mul1,multi2,mul2,qtcflg";
    qy += " FROM wCqso WHERE contid='"+labelContest->text();
    qy +="' ORDER BY cdate ,ctime";                        
    QSqlQuery query;
    query.exec(qy);
    i = query.size();                                            
    s = "     "+s.setNum(i)+" Logs";
    labelRowCnt->setText(s);
    col = query.record().count();                                
    n = tableLogEdit->columnCount();
    tableLogEdit->setRowCount(i);  
      row = 0;                                                   
      while(query.next()) {                                      
        z = 0;                                                   
        r = 0;                                                   
        i = 0;                                                   
        s = query.value(i++).toString();                         
        QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((r++)*(z++))); //Id_item erzeugen
        rowItem->setText(s); 
        tableLogEdit->setVerticalHeaderItem(row,rowItem);        
        c = 0;
        while( c != col -1 ) {                                   
          QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(c))); 
          newItem->setText(query.value(i++).toString());         
         if( c > 7 )
            newItem->setTextAlignment(Qt::AlignCenter);          
          tableLogEdit->setItem(row,c++,newItem);                
       }
       row++;                                                    
     }
}




void editLog::updateQsoItem( QTableWidgetItem * item )
{
     if (item != tableLogEdit->currentItem())          
         return;
     QString datum;
     QString La, mod, band, Id, new_str, q;
       row = tableLogEdit->row( item );                  
       col = tableLogEdit->column ( item );              
       QTableWidgetItem * h = tableLogEdit->horizontalHeaderItem( col ); 
       QTableWidgetItem * v = tableLogEdit->verticalHeaderItem ( row );
       new_str = item->data(Qt::EditRole).toString();    
       QSqlQuery query; 
       qsoId = v->text();
       qy = "UPDATE wCqso SET ";
       qy += getSysName(h->text());
       qy += "='"+new_str+"' WHERE contid='"+labelContest->text().simplified ()+"' AND cqsoid="+v->text();
       query.exec(qy);
}



void editLog::deleteQsoCb()
{
  int id, row, qpts;
  QString q, Contid, band, qband, qmult, qn, Wae, Dxcc, Mult1;
  
       row = tableLogEdit->currentRow();   
       QTableWidgetItem * v = tableLogEdit->verticalHeaderItem ( row );
       id = v->text().toInt();             
       QTableWidgetItem * item;
       item = tableLogEdit->item(row,0);
       Contid = item->text();              
       item = tableLogEdit->item(row,4);
       qband = "b";
       band = item->text();                
       qband += item->text();              
       item = tableLogEdit->item(row,12);
       qpts = item->text().toInt();        
       item = tableLogEdit->item(row,13);
       Wae = item->text();                 
       item = tableLogEdit->item(row,14);
       Dxcc = item->text();                
       item = tableLogEdit->item(row,16);
       Mult1 = item->text();               
       item = tableLogEdit->item(row,7);
       q = item->text();                   
       s = "\n\nQSO number: "+q.setNum(id)+"  - "+item->text()+" remove ?\n";
       int status = QMessageBox::question( this,
           tr("INFO"),s,
           QMessageBox::Yes,QMessageBox::No);
       if(status != QMessageBox::Yes)
          return;
       
       
       QMessageBox::information(this,
       tr("Information"),
       tr("At this time, the dialog don`t have function DELETE"),
       QMessageBox::Ok);
       
     return;
       
       
       
       
       
     
       
       
     
     
     
       
       
}



void editLog::convertToQtlog()
{
  int rows, row, omId, id, in;
  QString contest, contestid, logFile, call, cept, name, dxcc, rcvd, rcvd2, trans,sent,sRX,strRX,sTX,strTX;
  
  
     in = 0;
     logFile = settings.value("Logfile").toString();
     s = "Convert Contestlog  ' "+labelContest->text()+"'  into Logbook : "+logFile+" ? \n";
     int status = QMessageBox::question( this,
          tr("INFO"),s,
          QMessageBox::Yes,QMessageBox::No);
     if(status != QMessageBox::Yes)
       return;

     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     QSqlQuery query;
     contestid = labelContest->text().simplified();
     qy = "SELECT name FROM wContest WHERE contestid='"+contestid+"'";
     query.exec(qy);
     query.next();
     contest = query.value(0).toString();           
     QDateTime h;
     h = QDateTime::currentDateTime();
     trans = h.date().toString("MM-yyyy");
     contest = contest+" "+trans.mid(3,4);
     rows = tableLogEdit->rowCount();
     row = 0;
     QTableWidgetItem * item;
     while(row != rows) {
        rcvd2 = "";
        cept = "";
        item = tableLogEdit->item(row,7);          
        call = item->text();
        i = call.indexOf("/");                     
        if( i != -1 ) {
          cept = call.left(i +1);                  
          call = call.mid(i +1);                   
        }
  //--- fun_om Datensatz **************************
        qy = "SELECT omid,name FROM "+logFile+"om WHERE rufz='"+call+"'";
        QSqlQuery query(qy);
        if( query.size()) {                        
          query.next();
          omId = query.value(0).toInt();           
          name = query.value(1).toInt();           
        }
        else {                                     
          qy = "INSERT INTO "+logFile+"om VALUES (0,'";
          qy += call+"','";                        
          qy += "OM')";                            
           QSqlQuery query(qy);
          omId = query.lastInsertId().toInt();     
        }
 //--- fun_qso-Datensatz **************************
       qy = "INSERT INTO "+logFile+" VALUES (0,";  
       qy += s.setNum(omId);                       
       qy += ",'"+cept;                            
       qy += "','";                                
       qy += "','";                                
       item = tableLogEdit->item(row,4);           
       qy += "','"+item->text();                   
       item = tableLogEdit->item(row,5);           
       qy += "','"+item->text();                   
       item = tableLogEdit->item(row,6);           
       qy += "','"+item->text();                   
       item = tableLogEdit->item(row,2);           
       qy += "','"+item->text();                   
       item = tableLogEdit->item(row,3);           
       qy += "','"+item->text();                   
       item = tableLogEdit->item(row,3);           
       qy += "','"+item->text();                   
       item = tableLogEdit->item(row,11);          
       qy += "','"+item->text();                   
       item = tableLogEdit->item(row,8);           
       qy += "','"+item->text();                   
       qy += "','";                                
       qy += "','R";                               
       qy += "','R";                               
       qy += "','0000-00-00";                      
       qy += "','0000-00-00";                      
       qy += "','";                                
       qy += "','";                                
       qy += "','";                                
       item = tableLogEdit->item(row,16);          
       dxcc = item->text();
       qy += "','"+dxcc;                           
  
       item = tableLogEdit->item(row,15);          
       qy += "','"+item->text();                   
       t = "SELECT lwpx,ituz,cqz,lcn FROM tla WHERE la = '"+dxcc+"'";
       query.exec(t);
       if(query.size()) {                           
         while(query.next()) {
           i = 0;
           qy += "','"+query.value(i++).toString(); 
           qy += "','"+query.value(i++).toString(); 
           qy += "','"+query.value(i++).toString(); 
           qy += "','"+query.value(i++).toString(); 
         }
       }
       else {                              
        qy += "','";                       
        qy += "','";                       
        qy += "','";                       
        qy += "','";                       
       }

       qy += "','";                        
       qy += "','";                        
       qy += "','B";                       
       qy += "','";                        
       //item = tableLogEdit->item(row,0);                    
       //contestid = item->text();                            
       qy += "','"+contest;                
       qy += "','";                        
       qy += "','";                        
       qy += "','";                        
       qy += "',0)";                       
       query.exec(qy);                     
       id = query.lastInsertId().toInt();  

       qy = "INSERT INTO "+logFile+"qsl VALUES (0,";
       qy += t.setNum(id);                 
       qy += ",'"+call;                    
       item = tableLogEdit->item(row,2);   
       qy += "','"+item->text();           
       item = tableLogEdit->item(row,0);   
       qy += "','"+item->text();           
       qy += "','";                        
       
       qy += "','";                   
       qy += "','R";                  
       qy += "','0000-00-00";         
       qy += "','0000-00-00";         
       
       qy += "','";                   
       qy += "','R";                  
       qy += "','0000-00-00";         
       qy += "','0000-00-00";         
       
       qy += "','";                   
       qy += "','";                   
       qy += "','";

       item = tableLogEdit->item(row,12);  
       rcvd = item->text();                
       qy += rcvd;                         
       qy += "','";
       item = tableLogEdit->item(row,13);  
       rcvd2 = item->text();
       qy += rcvd2;                        

       item = tableLogEdit->item(row,9);   
       qy += "','"+item->text();           
       item = tableLogEdit->item(row,10);  
       qy += "','"+item->text();           

       qy += "','";                        
       qy += "','";                        
       qy += "','";                        
       qy += "','";                        
       qy += "','')";                      
       query.exec(qy);
       
      
      switch(CNr) {
        case DARC10M : {
             if(rcvd2.count() > 1) {                          
                qy = "INSERT INTO "+logFile+"awd VALUES (0,"; 
                qy += s.setNum(id);                           
                qy += ",'"+call;                              
                qy += "','DLD";                               
                qy += "','"+rcvd2;                            
                qy += "')";
                 query.exec(qy);                               
             }
            }
            break;
        case XMAS : {                                         
             bool ok;
             in = rcvd.toInt(&ok, 10);                        
             if( ok == false ) {                              
                qy = "INSERT INTO "+logFile+"awd VALUES (0,"; 
                qy += s.setNum(id);                           
                qy += ",'"+call;                              
                qy += "','DLD";                               
                qy += "','"+rcvd;                             
                qy += "')";
                query.exec(qy);                               
             }
            }
            break;
        case ARRLDX : {                                       
             if(rcvd.count() > 1) {
                qy = "INSERT INTO "+logFile+"awd VALUES (0,"; 
                qy += s.setNum(id);                           
                qy += ",'"+call;                              
                qy += "','US-CA";      
                qy += "','"+rcvd;                             
                qy += "')";
                query.exec(qy);                               
             }
            }
            break;
        default :
            break;
      }
      row++;
     }
  
     qy = "UPDATE wCqso SET trans='"+trans+"' WHERE contid='"+contestid+"'"; 
     query.exec(qy);
     qy = "UPDATE wContest SET expLogbook='"+trans+"' WHERE contestid='"+contestid+"'"; 
     query.exec(qy);

     qy = "SELECT COUNT(*) FROM "+logFile;
     query.exec(qy);
     query.next();
     s = query.value(0).toString();
     qy = "UPDATE dblogs SET qsocnt='"+s+"' WHERE logname='"+logFile+"'";
     query.exec(qy);
     
     QApplication::restoreOverrideCursor();
     QMessageBox::information(this,
       tr("Information"),
       tr("\nConvert %1 QSOs").arg(row),
     QMessageBox::Ok);
}
