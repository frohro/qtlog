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

// Referenz_Namen_Liste  laden und im Dialog anzeigen
// ---------------------------------------------------
void admin::getRefnamen()
{
  QString y;
  QSqlQuery query;
  qy = "SELECT idn,dbfield,refnam,ref_en,adifnam FROM refnamen WHERE type != 0 AND idn < 60 ORDER BY idn"; // !! < 60 !!
  qDebug() << qy;
  query.exec(qy);
  
  QBrush brush(QColor(217,207,196));  
  row = query.size();               // 34 rows ( qso_datensatz, ab ADIF.2 )
  col = query.record().count();     //  5 Spalten
  RefTable->setRowCount(row);       // TabellenLänge setzen - col ist schon gesetzt
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
    
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                 // ref_name
    newItem->setText(query.value(i++).toString());
    RefTable->setItem(row,col++,newItem);
    
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                 // ref_en
    newItem->setText(query.value(i++).toString());
    RefTable->setItem(row,col++,newItem);
    
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                 // adif_name
    newItem->setText(query.value(i++).toString());
    RefTable->setItem(row,col++,newItem);
    row++;
  }
}

// load AWD_customs_field_list und im Dialog anzeigen
// --------------------------------------------------
void admin::getCustomsfields()
{
    QString y;
    QSqlQuery query;
    qy = "SELECT id,atype,adiftype FROM wawdlist WHERE aset !='0' ORDER BY id";
    query.exec(qy);
    row = query.size();                     // anzahl aedTypen
    customsTable->setRowCount(row);         // TabellenLänge setzen - col ist schon gesetzt
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
      newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                 // ADIF_name
      newItem->setText(query.value(i).toString());
      customsTable->setItem(row,col++,newItem);
      row++;
    }
}


// customs_table editiert
// -------------------------------------------------------------------
// Daten in der custom_Tabelle editiert - jetzt in der DB updaten
// SIGNAL ( itemChanged(QTableWidgetItem *));
//-------------------------------------------------------------------
void admin::updateCustomItemCb( QTableWidgetItem *item )
{
    if (item != customsTable->currentItem())     // keine fremden items
        return;
        row = customsTable->row( item );         // zeile
        col = customsTable->column ( item );     // spalte
        if(col == 0) {
          item->setText(val);                    // original_text zurueckstellen
          return;                                // Spalte 0 : kein update
        }
        QSqlQuery query; 
        QTableWidgetItem * v = customsTable->verticalHeaderItem ( row );
        if( col == 1) {
            qy = "UPDATE wawdlist SET adiftype='"+item->text()+"' WHERE id="; 
            qy += v->text();
            query.exec(qy);
        }
}

// sichere Celltext
// -----------------------------------------------------------------
void admin::saveValCb(QTableWidgetItem * item)
{
     val = item->text();
}


// Ref_Namen editieren
// -------------------------------------------------------------------
// *******************************************************************
// Daten in der Refnamen_Tabelle editieren und in der DB updaten
// SIGNAL ( itemChanged(QTableWidgetItem *));
// -------------------------------------------------------------------
void admin::updateRefItemCb( QTableWidgetItem *item )
{
    if (item != RefTable->currentItem())         // fremde items nicht bearbeiten
        return;
        row = RefTable->row( item );             // zeile
        col = RefTable->column ( item );         // spalte
        if(col == 0) {
          item->setText(val);                    // original_text zurueckstellen
          return;                                // Spalte 0 : keine Änderungen !
        }
        QSqlQuery query; 
        QTableWidgetItem * v = RefTable->verticalHeaderItem ( row );
        switch( col ) {
          case 1: s = "refnam";
                  break;
		  
	  case 2: s = "ref_en";
                  break;
		  
          case 3: s = "adifnam";
                  break;
        }
        qy = "UPDATE refnamen SET "+s+"='"+item->text()+"' WHERE idn="; 
        qy += v->text();
        query.exec(qy);
}

// ---------------------------------------
void admin::getAdifDirCb()
{
   SavePathAdif->setText(DirSelector());
}


// ----------------------------------------------------------------------------
// ADIF EXPORT
// ----------------------------------------------------------------------------
void admin::adifExport()                 // LogDaten im ADIF_format exportieren
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
   if(lItem)                              // ku 29.08.10
     logbook = lItem->text(0);
   else {
        QMessageBox::information( this,
        tr("INFO"),
        tr("\nBitte erst Logbook waehlen !!"),
        QMessageBox::Ok);
     return;
   }
   if(checkBoxPref->isChecked() == true) // gesetzt - Export mit ADIF_nr im dxccprefix
      pref = 1;
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   wadif adif;
   adif.createAdifTableHeader();
   adif.setBandTable();
   
    // fuelle die customs (AWD) uebersetzungs_tabelle
   QTableWidgetItem *awdItem;                  // neues arbeits_item erzeugen
   row = 0;
   col = 0;
   n = customsTable->rowCount();               // Anzahl cstom_Eintraege ermitteln
   while(n) {
      awdItem = customsTable->item(row,col++); // positionieren
      f = awdItem->text();                     // text key_name row, col holen
      awdItem = customsTable->item(row,col++);
      s  = awdItem->text();
      adif.loadAdifAwdTable(f,s);              // in Tabelle eintragen
     n--;
   }
   
   adif.doExport(logbook,SavePathAdif->text(),pref);  // ADIF_Export logfile
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
   
   adif.clearExpHash();                       // Alle f. Export relevanten Speicher löschen
   QApplication::restoreOverrideCursor();
}


// ----------------------------------------------------------------------------
// ADIF IMPORT    -  LogDaten im ADIF_format importieren
// ----------------------------------------------------------------------------
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
   if(lItem)                            // ku 29.08.10
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
   inadif.loadRefList();                // lade Liste mit Referenz_Namen
   inadif.loadRefListAwd();             // AWD
   inadif.setBandTable();               // lade Übersetzungstabelle für Banddaten
      
   homeId = comboBoxHome->currentIndex();
   homeId++;
// -
   if(checkBoxQslMode->isChecked() == TRUE)
     qslMode = 1;
   
   // labelLogbook->text() SavePathAdif->text(),qslMode,comboBoxrig->currentText(),homeId
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
   
   inadif.closeRefLists();                              // Speicher löschen
   inadif.clearBandHash();                              // -.-
   
   // dblogs korrigieren - neue QSO_daten eingegangen
   QSqlQuery query;
   qy = "SELECT COUNT(*) FROM "+logbook;                // check neuen Datenbestand - ku 29.08
   query.exec(qy);
   query.next();
   s = query.value(0).toString();                      // Anzahl_QSOs
   
   qy = "UPDATE dblogs SET qsocnt="+s+" WHERE logname='"+logbook+"'";           // - ku 29.08
   query.exec(qy);
   n = logList->topLevelItemCount();           // neuen Datenbestand in logbook-List anzeigen
   QTreeWidgetItem * Item;
   Item = NULL;
   i = 0;
   while(n != i) {
     Item = logList->topLevelItem(i++);
     if(Item->text(0).compare(logbook) == 0) break;
   }
   Item->setText(1,s);
   
     // sende Message an report, neue QSOs eingegangen : Kenn_Nr, type
   if(logbook.compare(settings.value("logfile").toString()) == 0) { //identisch ?
      datagram = QByteArray::number(3);               
      datagram.append(",");
      udpSocket->writeDatagram(datagram.data(),datagram.size(),
                    QHostAddress::LocalHost, 45454);  
   }
   
   // wawdlist wieder herstellen
   qy = "SELECT id,atype FROM wawdlist WHERE aset != '0'";
   query.exec(qy);
   while(query.next()) {
      b = query.value(0).toString();                   // id
      s = query.value(1).toString();                   // atype
      
      if(s.compare("IOTA") == 0)                       // ist ARRL_type
        qy = "UPDATE wawdlist SET adiftype='"+s+"' WHERE id="+b; 
      else
       if(s.compare("US-CA") == 0)                     // ist ARRL_type
         qy = "UPDATE wawdlist SET adiftype='STATE' WHERE id="+b; 
      else
         qy = "UPDATE wawdlist SET adiftype='APP_QTLOG_"+s+"' WHERE id="+b; 
      QSqlQuery query(qy);
   }
   QApplication::restoreOverrideCursor();
}


