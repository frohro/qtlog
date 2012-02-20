/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007 / 08.07.09
********************************************************************** */

#include <QApplication>
#include <QtGui>
#include <QtSql>
#include "admin.h"
#include "wadif.h"


void admin::getRefnamen()
{
  QString y;
  QSqlQuery query;
  qy = "SELECT idn,dbfield,refnam,adifnam FROM refnamen WHERE type != 0 AND idn < 60 ORDER BY idn"; 
  query.exec(qy);
  
  QBrush brush(QColor(217,207,196));  
  row = query.size();              
  col = query.record().count();    
  RefTable->setRowCount(row);      
  row = 0;
  while(query.next()) {
    z = 0;
    r = 0;
    col = 0;
    i = 0;
    y = query.value(i++).toString();
    y += "";
    QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((r++)*(z++))); //idn
    rowItem->setText(y);
    RefTable->setVerticalHeaderItem(row,rowItem);
    QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(col)));//dbFeld
    newItem->setText(query.value(i++).toString());
    RefTable->setItem(row,col++,newItem);
    newItem->setBackground(brush);
    
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                 
    newItem->setText(query.value(i++).toString());
    RefTable->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                 
    newItem->setText(query.value(i++).toString());
    RefTable->setItem(row,col++,newItem);
    row++;
  }
}


void admin::getCustomsfields()
{
    QString y;
    QSqlQuery query;
    qy = "SELECT id,atype,adiftype FROM wawdlist WHERE aset !='0' ORDER BY id";
    query.exec(qy);
    row = query.size();                     
    customsTable->setRowCount(row);         
    row = 0;
    customsTable->setColumnWidth(0,63); 
    customsTable->setColumnWidth(1,160); 
    QBrush brush(QColor(217,207,196)); 
    while(query.next()) {
      z = 0;
      r = 0;
      col = 0;
      i = 0;
      y = query.value(i++).toString();
      QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((r++)*(z++))); //idn
      rowItem->setText(y);
      customsTable->setVerticalHeaderItem(row,rowItem);
      QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(col)));//dbFeld
      newItem->setText(query.value(i++).toString());
      customsTable->setItem(row,col++,newItem);
      newItem->setBackground(brush);
      newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                
      newItem->setText(query.value(i).toString());
      customsTable->setItem(row,col++,newItem);
      row++;
    }
}




//-------------------------------------------------------------------
void admin::updateCustomItemCb( QTableWidgetItem *item )
{
    if (item != customsTable->currentItem())     
        return;
        row = customsTable->row( item );         
        col = customsTable->column ( item );     
        if(col == 0) {
          item->setText(val);                    
          return;                                
        }
        QSqlQuery query; 
        QTableWidgetItem * v = customsTable->verticalHeaderItem ( row );
        if( col == 1) {
            qy = "UPDATE wawdlist SET adiftype='"+item->text()+"' WHERE id="; 
            qy += v->text();
            query.exec(qy);
        }
}


void admin::saveValCb(QTableWidgetItem * item)
{
     val = item->text();
}






void admin::updateRefItemCb( QTableWidgetItem *item )
{
    if (item != RefTable->currentItem())         
        return;
        row = RefTable->row( item );             
        col = RefTable->column ( item );         
        if(col == 0) {
          item->setText(val);                    
          return;                                
        }
        QSqlQuery query; 
        QTableWidgetItem * v = RefTable->verticalHeaderItem ( row );
        switch( col ) {
          case 1: s = "refnam";
                  break;
          case 2: s = "adifnam";
                  break;
        }
        qy = "UPDATE refnamen SET "+s+"='"+item->text()+"' WHERE idn="; 
        qy += v->text();
        query.exec(qy);
}

void admin::getAdifDirCb()
{
   SavePathAdif->setText(DirSelector());
}



void admin::adifExport()                
{
 int pref = 0;
 QString info;
   if(!SavePathAdif->text().size()) {
     QMessageBox::information( this,
        tr("INFO"),
        tr("\nBitte Ordner fuer ADIF-Exportdatei waehlen !!"),
        QMessageBox::Ok);
     return;
   }
   if(lItem)                             
     logbook = lItem->text(0);
   else {
        QMessageBox::information( this,
        tr("INFO"),
        tr("\nBitte erst Logbook waehlen !!"),
        QMessageBox::Ok);
     return;
   }
   if(checkBoxPref->isChecked() == true) 
      pref = 1;
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   wadif adif;
   adif.createAdifTableHeader();
   adif.setBandTable();
   
   QTableWidgetItem *awdItem;                  
   row = 0;
   col = 0;
   n = customsTable->rowCount();               
   while(n) {
      awdItem = customsTable->item(row,col++); 
      f = awdItem->text();                     
      awdItem = customsTable->item(row,col++);
      s  = awdItem->text();
      adif.loadAdifAwdTable(f,s);              
     n--;
   }
   adif.doExport(logbook,SavePathAdif->text(),pref);  
   if(adif.getState()) {
      QApplication::restoreOverrideCursor();
      QMessageBox::information( this,
      tr("INFO"),
      tr("\nADIF_FEHLER"),
      QMessageBox::Ok);
     return;
   }
   info = "\nLogbook: "+logbook+" - "+s.setNum(adif.getCount())+tr("  Datensaetze im Adif-Format V2.x exportiert\n\n");
   info += tr(" Es wurden keine Uebertragungsfehler festgestellt\n");
   InfoTextEdit->clear();
   InfoTextEdit->insertPlainText(info);
   stackedWidget->setCurrentIndex(4);
   adif.clearExpHash();                      
   QApplication::restoreOverrideCursor();
}



void admin::adifImport()
{
 QString info;
 int qslMode = 0;
 int homeId = 0;
   if(!SavePathAdif->text().size()) {
     QMessageBox::information( this,
        tr("INFO"),
        tr("\nBitte Ordner fuer ADIF-Importdatei waehlen !!"),
        QMessageBox::Ok);
     return;
   }
   if(lItem)                            
     logbook = lItem->text(0);
   else {
        QMessageBox::information( this,
        tr("INFO"),
        tr("\nBitte erst Logbook waehlen !!"),
        QMessageBox::Ok);
     return;
   }
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   wadif inadif;
   inadif.loadRefList();                
   inadif.loadRefListAwd();             
   inadif.setBandTable();               
      
   homeId = comboBoxHome->currentIndex();
   homeId++;

   if(checkBoxQslMode->isChecked() == TRUE)
     qslMode = 1;
   
   
   inadif.doImport(logbook,SavePathAdif->text(),qslMode,comboBoxRig->currentText(),homeId);
   if(inadif.getState()) {              //Fehler_meldung
      InfoTextEdit->clear();
      info = "\n "+s.setNum(inadif.getCount())+tr("  Datensaetze im Adif-Format V2.x importiert\n\n");
      info += tr(" Es wurden folgende Uebertragungsfehler festgestellt:\n\n");
      info += inadif.getErrorInfo();
      InfoTextEdit->insertPlainText(info);
      stackedWidget->setCurrentIndex(4);
   }
   else {
     info = "\n "+s.setNum(inadif.getCount())+tr("  Datensaetze im Adif-Format V2.x importiert\n\n");
     info += tr(" Es wurden keine Uebertragungsfehler festgestellt\n");
     InfoTextEdit->clear();
     InfoTextEdit->insertPlainText(info);
     stackedWidget->setCurrentIndex(4);
   }
   inadif.closeRefLists();                             
   inadif.clearBandHash();                             
   
   QSqlQuery query;
   qy = "SELECT COUNT(*) FROM "+logbook;               
   query.exec(qy);
   query.next();
   s = query.value(0).toString();                      
   qy = "UPDATE dblogs SET qsocnt="+s+" WHERE logname='"+logbook+"'";           
   query.exec(qy);
   n = logList->topLevelItemCount();      
   QTreeWidgetItem * Item;
   Item = NULL;
   i = 0;
   while(n != i) {
     Item = logList->topLevelItem(i++);
     if(Item->text(0).compare(logbook) == 0) break;
   }
   Item->setText(1,s);
   
   if(logbook.compare(settings.value("logfile").toString()) == 0) { //identisch ?
      datagram = QByteArray::number(3);               
      datagram.append(",");
      udpSocket->writeDatagram(datagram.data(),datagram.size(),
                    QHostAddress::LocalHost, 45454);  
   }
   
   qy = "SELECT id,atype FROM wawdlist WHERE aset != '0'";
   query.exec(qy);
   while(query.next()) {
      b = query.value(0).toString();                  
      s = query.value(1).toString();                  
      if(s.compare("IOTA") == 0)                      
        qy = "UPDATE wawdlist SET adiftype='"+s+"' WHERE id="+b; 
      else
       if(s.compare("US-CA") == 0)                    
         qy = "UPDATE wawdlist SET adiftype='STATE' WHERE id="+b; 
      else
         qy = "UPDATE wawdlist SET adiftype='APP_QTLOG_"+s+"' WHERE id="+b; 
      QSqlQuery query(qy);
   }
   QApplication::restoreOverrideCursor();
}


