/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.01.2009 / 17.08.09
********************************************************************** */

#include <QSettings>
#include <QString>
#include <QTabWidget>
#include <QtGui>
#include <QtSql>

#include "query.h"
#include "../qtlogDiag/translate.h"

#include "dirmngr.h"
#include "quAdif.h"

query::query(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
  setupUi(this);
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                           
   setFont(font);
   connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(queryList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(queryListDoubleClicked(QTreeWidgetItem*,int)));
   connect(queryList, SIGNAL(itemClicked(QTreeWidgetItem *,int)), this, SLOT(queryListClicked(QTreeWidgetItem*,int)));
   connect(logList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(logListClickedCb(QTreeWidgetItem*,int)));
   connect(dataList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(dataListClickedCb(QTreeWidgetItem*,int)));
   connect(lineEditQname, SIGNAL(textEdited(QString)), this, SLOT(textEditedQnameCb(QString)));
   connect(ButtonSave, SIGNAL(clicked()), this, SLOT(buttonSaveCb()));
   connect(ButtonMkText, SIGNAL(clicked()), this, SLOT(buttonMkTextCb()));
   connect(ButtonMkAdif, SIGNAL(clicked()), this, SLOT(buttonMkAdifCb()));
   connect(ButtonBack, SIGNAL(clicked()), this, SLOT(buttonBackCb()));
   connect(ButtonClear, SIGNAL(clicked()), this, SLOT(buttonClearCb()));
   connect(ButtonCopy, SIGNAL(clicked()), this, SLOT(buttonCopyDataCb()));
   connect(ButtonHilfe, SIGNAL(clicked()), this, SLOT(showHilfeCb()));
   
   connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabCurrentChangedCb(int)));
   
   queryList->setColumnWidth(0,80);                  // name  
   queryList->setColumnWidth(1,65);                  // sep
   queryList->setColumnWidth(2,35);                  // eor
   queryList->setColumnWidth(3,1200);                // query
   queryList->setColumnWidth(4,40);                  // no
   
   logList->setColumnWidth(0,30);                    
   logList->setColumnWidth(1,90);     
   
   dataList->setColumnWidth(0,80);        
   
   logFile = settings.value("logFile").toString();
   transl.installHashTable();                        //-Hash_tabellen zur NamenConvertierung anlegen
   transl.installHashTableBand();                    // -.-           Bandtabelle
   QSqlQuery query; 
   qy = "SELECT refnam FROM refnamen WHERE type='2'  OR type='3' ORDER BY idn";
   query.exec(qy);
   while(query.next()) {                             // Die restlichen RefNamen ohne *
      i = 0;
      n = 0;
      s = query.value(n).toString();                 // refnam
      if(s.compare("Cept") != 0) {
        QTreeWidgetItem *item = new QTreeWidgetItem(logList);
        item->setText(i++,"");                       // sel keine '*'
        item->setText(i++,s);                        // refnam
      }
   }
   
   qy = "SELECT * FROM wquery";
   query.exec(qy);
   n = 0;
   while(query.next()) {                                      // Tabell    
      i = 0;
      n = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(queryList);
      item->setText(i++,query.value(n++).toString());         // name
      item->setText(i,query.value(n++).toString());           // separator
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(i,query.value(n++).toString());           // eor
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(i++,query.value(n++).toString());         // query
      item->setText(i++,"");                                  // No     
   }
   
   AWD = settings.value("Val").toString();                    // AWAD-type
   showTable = 0;
   upItem = NULL;
   cntdpos = 0;
}

// ----------------
query::~query()
{
}

// ----------------
void query::goExit() 
{
  accept();                              
}

// schalte auf Page 0
// ------------------
void query::buttonBackCb()
{
   tabWidget->setCurrentIndex(0);
}

// ---------------------
void query::showHilfeCb()
{
   settings.setValue("Val","DataExport");
   StartProcess("hilfedb &");
}

// ------------------------------------------------------
// currentPageChanged - nach config query schaltet
//------------------------------------------------------
void query::tabCurrentChangedCb(int index)
{
   QString range, token, t;
   int len, idx, pos;
   if(index == 1) {                                    // logbook_header_page einstellen
     // Datum, Start, Call, Mode, RSTg, RSTe, AWD, Locator range <2007-11-01> to <2008-01-30> ORDER BY Dxcc
     s = lineEditQuery->text();
     cnt = s.count();  
     
     if(!cnt) {
       QMessageBox::information(this,tr(" Info"),
         tr("Es ist praktischer  ein vorhandenes query zu modefizieren und unter einem neuen Namen zu speichern."),
       QMessageBox::Ok); 
     }
     
     pos = 0;
     if(cnt != 0) {                                   // check token_str vorhanden
       buttonClearCb();
       i = s.indexOf("range");                        // range_index ermitteln
       token = s.left(i);
       token = token.simplified();                    // token_str aufbereiten
       range = s.mid(i);                              // range_str aufbereiten
       range = range.simplified();
       len = token.count();                           // token_str einlesen
      
       pos = 1;
       idx = 0;
       while( 1 ) {                                  // datastream
        while(len != idx +1) {                       // header_str lesen
          t = "";
          while((token[idx] != QChar(',')) && (idx != len)) {  // ein Token holen
               t += token[idx]; 
               idx++;                               // char +1 
          }
          t = t.simplified();
          insertTok(t,pos);                         // füge token in logList und dataList ein
          pos++;
          if(idx  == len) break;                    // das letzte Feld
          idx++;                                    // ',' überspringen n. token holen
        }                                           // das n.Feld
        if(idx  == len) break;
      }                                             // ENDE FELD_str
      cntdpos = pos;
                                                    // -- Separatoren
      s = lineEditTrenner->text();                  // Feld_Separatoren
      s = s.simplified();  
      if(s.compare(";") == 0)
         comboBoxSep->setCurrentIndex(0);
      else
      if(s.compare(",") == 0)
          comboBoxSep->setCurrentIndex(1);
      else
      if(s.compare("+") == 0)
         comboBoxSep->setCurrentIndex(2);
      else
      if(s.compare(":") == 0) 
         comboBoxSep->setCurrentIndex(3);
      
      // -- zeilenumbruch
      s = lineEditEor->text();                       // Zeilenumbruch
      s = s.simplified();  
      if(s.compare("L") == 0)
         comboBoxEor->setCurrentIndex (0);
      else
       if(s.compare("W") == 0)
          comboBoxEor->setCurrentIndex (1);
     
      len = range.count();
      i = range.indexOf("range");                    // range <2008-01-01> to <2009-01-11>"
      range = range.mid(i +5,len);                   // <2008-01-01> to <2009-01-11> ORDER BY Dxcc
      idx = 0;                                       // range - qday: "<2008-01-01> to <2009-01-11>"
      while(range[idx] != QChar('<')) range[idx++];         // suche '<'
      idx++;                                                // '<' überspringen
      whv = "";
      while(range[idx] != QChar('>')) whv += range[idx++];  // von_datum übernehmen
      idx++;
      whb = "";
      while(range[idx] != QChar('<')) range[idx++];         // suche '<'
      idx++;
      while(range[idx] != QChar('>')) whb += range[idx++];  //  bis_datum übernehmen
      
      
      QDate d = QDate::fromString (whv,"yyyy-MM-dd");
      dateEditFrom->setDate(d);                            // vom
      d = QDate::fromString (whb,"yyyy-MM-dd");
      dateEditTo->setDate(d);                              // bis
      
      labelQuery->setText(lineEditQname->text());
    }
  }
}

// füge token in logList und dataList ein
// ----------------------------------------
void query::insertTok(QString tok, int pos)
{
  int count;
      count = logList->topLevelItemCount();          // Listenlaenge
      lItem = logList->currentItem();                // arbeits_item
      i = 0;
      while(i != count ) {                           // suche tok in logList
        lItem = logList->topLevelItem (i);
        if(lItem->text(1).compare(tok) == 0)
           break;                                    // gefunden
        i++;
      }
      s = s.setNum(pos);                             // tok_pos setzen
      lItem->setText(0,s);
      QTreeWidgetItem *itemN = new QTreeWidgetItem(dataList);
      itemN->setText(0,lItem->text(1));              // tok in dataList einfügen
}

// clear datalist und 1.spalte in logList ( sel )
// ------------------------------------------------
void query::buttonClearCb()
{
     dataList->clear();                         // clear sel 'sel' in logList löschen
     i = logList->topLevelItemCount();          // Listenlaenge
     lItem = logList->currentItem();            // 1.pos einehmen
     n =  logList->indexOfTopLevelItem(lItem);  // nr layout_item
     n = 0;
     while( n != i ) {
       lItem = logList->topLevelItem (n);
       lItem->setText(0,"");
       n++;
     }
     cntdpos = 0;
}

// Button Übernehmen  geklickt - config_data übernehmen (in lineEditQuery eintragen )
// ----------------------------------------------------------------------------------
void query:: buttonCopyDataCb()
{
     //Muster: Datum,Start, Ende, Call, Mode, RSTg, RSTe, AWD, Locator range <2007-11-01> to <2008-01-30> ORDER BY Dxcc
     QString q;
     i = dataList->topLevelItemCount();           // Listenlaenge
     lItem = dataList->currentItem();             // arbeits_item
     n =  dataList->indexOfTopLevelItem(lItem);   // anzahl
     n = 0;
     q = "";
     while( n != i ) {
       lItem = dataList->topLevelItem (n);
       q += lItem->text(0);
       n++;
       if(n != i) 
          q += ",";
     }
     
     QDate d = dateEditFrom->date();               // Vom
     whv = t.setNum(d.year());
     whv += "-";
     t.setNum(d.month());
     if(t.count() == 1)
       whv += "0";
     whv += t+"-";  
     t.setNum(d.day());
     if(t.count() == 1)
        whv += "0";
     whv += t;
     
     d = dateEditTo->date();                       // Bis
     whb = t.setNum(d.year());
     whb += "-";
     t.setNum(d.month());
     if(t.count() == 1)
       whb += "0";
     whb += t+"-";  
     t.setNum(d.day());
     if(t.count() == 1)
        whb += "0";
     whb += t;
 
     t = comboBoxSep->currentText();
     t = t.simplified(); 
     lineEditTrenner->setText(t);
     
     s = comboBoxEor->currentText();
     s = s.simplified(); 
     t = s.left(1);
     lineEditEor->setText(t);
     
     q += " range <"+whv+"> to <"+whb+"> ORDER BY "+comboBoxOrder->currentText();
     lineEditQuery->setText(q);
     tabWidget->setCurrentIndex (0);
}

// ---------------------------------------------------------
void query::queryListDoubleClicked(QTreeWidgetItem *item, int col)
{
    int x = col;
    upItem = item;
    lineEditQname->setText(item->text(0));
    lineEditTrenner->setText(item->text(1));
    lineEditEor->setText(item->text(2));
    lineEditQuery->setText(item->text(3));
    lineEditOutfile->setText(item->text(0));
    tabWidget->setCurrentIndex(1);
}

// -------------------------------------------------------
void query::queryListClicked(QTreeWidgetItem *item, int col)
{
    int x;
    x = col;
    upItem = item;
    lineEditQname->setText(item->text(0));
    lineEditTrenner->setText(item->text(1));
    lineEditEor->setText(item->text(2));
    lineEditQuery->setText(item->text(3));
    lineEditOutfile->setText(item->text(0));
}

// -----------------------------------------------
void query::textEditedQnameCb(QString str)
{
   lineEditQname->setText(str.toUpper());
}

// kopiere headername: logHeader -> dataHeader, setze cntNr
// --------------------------------------------------------------
void query::logListClickedCb(QTreeWidgetItem *item,int col)
{
    int x;
    x = col;
    if(item->text(0).count() == 0) {
      item->setText(0,s.setNum(cntdpos));
      QTreeWidgetItem *itemnew = new QTreeWidgetItem(dataList);
      itemnew->setText(0,item->text(1));
      cntdpos++;
    }
    else {                                          // name ist schon belegt - löschen
       item->setText(0,"");                         // Listenlaenge
       lItem = dataList->currentItem();             // 1.pos einehmen
       n =  dataList->indexOfTopLevelItem(lItem);   // nr layout_item
       n = 0;
       while( n != i ) {
         lItem = dataList->topLevelItem (n);
         s = lItem->text(0);
         x = s.compare(item->text(1));              // Vergleich item(1) / Item(0)
         if( !x ) {
           dataList->takeTopLevelItem(n);           // löschen layout_item
           break;
         }
         n++;
       }
       if( x ) {
         qDebug() << "Grober FEHLER";               // sollte nicht vorkommen
        return;
       }
    }
}

// entferne name im dataList und cntNr im logList
// ---------------------------------------------------------
void query::dataListClickedCb(QTreeWidgetItem *item, int col)
{
     int x;
     x = col;
     i = logList->topLevelItemCount();             // Listenlaenge
     lItem = logList->currentItem();
     n = 0;
     while( n != i ) {
       lItem = logList->topLevelItem (n);
       s = lItem->text(1);
       x = s.compare(item->text(0));              // Vergleich item(0) / Item(1)
         if( !x ) {
           lItem->setText(0,"");
           break;
         }
         n++;
       }
       n =  dataList->indexOfTopLevelItem(item);  // nr layout_item
       dataList->takeTopLevelItem(n);             // löschen layout_item
}

//-------------------------------------------------------------------------------
// Update oder INSERT query_str ( falls name nicht vorhanden - INSERT neues query 
// ------------------------------------------------------------------------------

void query::buttonSaveCb()
{
     QSqlQuery query;               // prüfe ob name bereits vorhanden  ja: update - sonst NEU anlegen
     qy = "SELECT name FROM wquery WHERE name='"+lineEditQname->text()+"'";
     query.exec(qy); 
     if(query.size() == 1) {                                  // Update
       qy = "UPDATE wquery SET sep='"+lineEditTrenner->text();
       qy += "',eor='"+lineEditEor->text();
       qy += "',query='"+lineEditQuery->text();  
       qy += "' WHERE name='"+lineEditQname->text()+"'";
       query.exec(qy);
       
       i = 1;
       upItem->setText(i++,lineEditTrenner->text());          // sep
       upItem->setText(i++,lineEditEor->text());              // eor
       upItem->setText(i++,lineEditQuery->text());            // query
     }
     else {                                                   // neu           
       if(lineEditQuery->text().count() == 0) {               // kein query
          QMessageBox::information(this,tr(" Info"),tr("Kein Query konfiguriert"),
          QMessageBox::Ok); 
         return;
       }
       
       if(lineEditQname->text().count() == 0) {
          QMessageBox::information(this,tr(" Info"),tr("Query Name fehlt"),
          QMessageBox::Ok); 
         return;
       }
       
       qy = "INSERT INTO wquery VALUES ('"+lineEditQname->text();
       qy += "','"+lineEditTrenner->text();
       qy += "','"+lineEditEor->text();
       qy += "','"+lineEditQuery->text();
       qy += "')";
       query.exec(qy);
       i = 0;
       QTreeWidgetItem *item = new QTreeWidgetItem(queryList);
       item->setText(i++,lineEditQname->text());           // name
       item->setText(i,lineEditTrenner->text());           // sep
       item->setTextAlignment(i++,Qt::AlignCenter);
       item->setText(i,lineEditEor->text());               // eor
       item->setTextAlignment(i++,Qt::AlignCenter);
       item->setText(i++,lineEditQuery->text());           // query
       item->setText(i++,"");                              // No    
     }
}


// ------------------------------------------------------------------------
// Ein TEXT_file CSV_format erzeugen  - make_query, dann make_text
// ------------------------------------------------------------------------
void query::buttonMkTextCb()
{
     err = 0;
     createQuery();                            // query bilden
     if(err) 
        return;                                // Syntax_FEHLER festgestellt
     err = 0;
     if(lineEditOutfile->text().count() == 0) {
        int status = QMessageBox::information(this,tr("Parameter Check"),
              tr("Dateiname fehlt.\nAbfrage Ergebnis im Report-Dialog anzeigen ?"),
        QMessageBox::Yes,QMessageBox::No); 
       if(status != QMessageBox::Yes)
        return;
       else {
          showTable = 1;                        // flg für anzeigen
          goExit();
       }
     }
     else {                                     // Datei_name vorhanden
       QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
       mkText();                                // TextDatei erzeugen
       QApplication::restoreOverrideCursor();
       s = "\n"+t.setNum(cnt)+tr("  Datensaetze - erzeugt\nDatei:  ");
       s +=  QDir::homePath();                     
       s += "/";
       s += lineEditOutfile->text()+".csv  \n\n";
       s += tr("Zurueck zum ReportDialog");
       
       qDebug() << "str:" << s;
       
       int status = QMessageBox::information(this,tr("Ergebnis Check"),s,
                    QMessageBox::Yes,QMessageBox::No); 
       if(status == QMessageBox::Yes)
         goExit();
     }
}


// ------------------------------------------------------------------------
// Ein ADIF_file erzeugen  - make_query, dann make_adif
// ------------------------------------------------------------------------
void query::buttonMkAdifCb()
{
     err = 0;
     createQuery();                             // query bilden
     if(err) 
        return;                                 // Syntax_FEHLER festgestellt
     err = 0;
     if(lineEditOutfile->text().count() == 0) {
        int status = QMessageBox::information(this,tr("Parameter Check"),
              tr("      Dateiname fehlt.\n\nAbfrage Ergebnis im ReportDialog anzeigen ?"),
        QMessageBox::Yes,QMessageBox::No); 
       if(status != QMessageBox::Yes)
        return;
       else {
          showTable = 1;                       // flg für anzeigen
          goExit();
       }
     }
     else {                                   
       QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
       mkAdif();                              
       QApplication::restoreOverrideCursor();
       s = "\n"+t.setNum(cnt)+"   Datensaetze erzeugt\nDatei:  ";
       s +=  QDir::homePath();                     
       s += "/";
       s += lineEditOutfile->text()+".adi  \n\n";
       s += "Zurueck zum ReportDialog";
       int status = QMessageBox::information(this,tr("Ergebnis Check"),tr("%1").arg(s),
                    QMessageBox::Yes,QMessageBox::No); 
       if(status == QMessageBox::Yes)
         goExit();
     }
}

// query erzeugen
// -----------------------------------------------------------------------
void query::createQuery()
{
    QString qtext, sep, s, q, qday;
    int err, cnt, len;
    qtext = lineEditQuery->text();
    if(!qtext.count()) {
       QMessageBox::information(this,tr("Parameter Check"),
         tr("Die Queryzeile enthaelt einen Fehler"),
       QMessageBox::Ok); 
       err = 1;
       return;
    }
    if((lineEditTrenner->text().count() == 0) || (lineEditEor->text().count() == 0)) {
      QMessageBox::information(this,tr("Parameter Check"),
         tr("Es gibt einen Fehler bei der Trennerangabe"),
      QMessageBox::Ok); 
      err = 1;
      return;
    }
    sep = lineEditTrenner->text();
    sep = sep.simplified();                   
    idx = 0;
    cnt = 0;
    len = qtext.count();
    // "Format: Call, Datum, Band, Ende,  RSTe, RSTg, SRX, AWD, dxcc range <2008-01-01> to <2009-01-11>"
    // Prüfe die syntax
    // -----------------
    while(idx !=  len)                        
       if(qtext[idx++] == QChar('<') ) cnt++;
    idx = 0;
    while(idx !=  len)                        
       if(qtext[idx++] == QChar('>') ) cnt++;
    if(cnt != 4) {
       QMessageBox::information(this,tr("Syntax Fehler"),tr("Im Datum Format fehlen '<' oder '>'"),
       QMessageBox::Ok); 
       err = 1;
     return;
    }
    idx = 0;
    cnt = 0;
    while(idx !=  len) 
       if(qtext[idx++] == QChar('-') ) cnt++;
    if(cnt != 4) {                              // check ob im Datum Format  '-' vorhanden
      QMessageBox::information(this,tr("Syntax Fehler"),tr("Im Datum Format fehlen  '-'"),
       QMessageBox::Ok); 
       err = 1;
     return;
    }
    
    // suche nach 'range' trenne data von Datum und ORDER BY
    idx = qtext.indexOf("range");              // range_index suchen
    if(idx == -1) {                            // range_index suchen
       QMessageBox::information(this,tr("Syntax Fehler"),tr("' range '  FEHLT !"),
       QMessageBox::Ok); 
       err = 1;
     return;
    }
    
    q = qtext.left(idx);                      // split qtext - kopiere Header_Felder
    qday = qtext.mid(idx +5,len);             // Datum_str + ORDER BY ( wenn vorhanden )
    qday = qday.simplified();                 // space entfernen
    q = q.simplified();                       // space entfernen
    // -- baue header_str
    qy = "SELECT cept,";                      // muss sein
    t = "";
    n = 0;
    err = 0;
    len = q.count();
    idx = 0;
    while( 1 ) {                              // datastream
         while(len != idx +1) {               // header_str lesen
           t = "";
           while((q[idx] != QChar(',')) && (idx != len)) {   // ein Token lesen
               t += q[idx]; 
               if(t.count() > 10) {
                 f = tr("Hier fehlt ein Komma ! ")+t;
                 QMessageBox::information(this,tr("Syntax Error"),(f),
                 QMessageBox::Ok);
                 err = 1;
                 return;
               }
               idx++;                                // char +1   
           }
       
           t = t.simplified();                       // Komma gefunden
           f = t;                                    // token sichern
           if(idx  == len) break;                    // ist das letzte Feld
           t = transl.getDbField(t);                 // Header_namen übersetzen - FEHLER prüfen
           if(t.compare("?") == 0) {
              t = f+" ?";                            // header_name unbekannt
              err = 1;
             break;                                  // Name falsch
           }
           idx++;                                    // ',' überspringen n. token holen
           if(idx  == len) break;                    // das letzte Zeichen ist ein komma
           qy += t+",";                              // qy_string
         }
         t = transl.getDbField(t);                   // das letzt Wort übersetzen - FEHLER prüfen
         if(t.compare("?") == 0) {
            t = f+" ?";                              // header_name unbekannt
            err = 2;
           break;                                    // Name falsch
         }
         qy += t;                                    // query_str
         if(idx  == len) break;
         if(err) break;
    }                                               // ENDE FELD_str
    
    if(err) {
      f = tr("Feldnamen Error : ")+t;
      QMessageBox::information(this,tr("Syntax Fehler"),(f),
        QMessageBox::Ok);
        err = 1;
      return;
    }
// --                                              // query aufbauen
    f = qy;
    idx = f.indexOf("AWD");                        // hole AWD_index, wenn vorhanden
    n = idx;
    awdPos = idx;                                  // Char_AWD_pos im header, nicht gefunden -1
    
    if( idx != -1 ) {                              // ist AWD vorhanden ?
      i = 0;                                       // ja;
      cnt = 0;
      while(i !=  n)
       if(f[i++] == QChar(',') ) cnt++;            // zähle kommata
      awdPos = cnt +1;                             // wort_AWD_position im query +1 für 'cept'
      qy = f.left(idx);                            // kopiere Header_Felder bis AWD_Feld
      if( idx != -1 ) {                            // da AWD gefunden - SUB_query einfügen
        s = "(SELECT awkenner FROM "+logFile+"awd WHERE awtype='";
        s += settings.value("Val").toString();
        s += "' AND id=qid)";
        qy += s;
        s = f.mid(idx +3);                         // jetzt: AWD überspringen und den Rest_Header anhängen
        qy += s;
      }
    }
    
    qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
    qy += " WHERE ";                                    // WHERE class
    
    idx = 0;                                            // range - qday: "<2008-01-01> to <2009-01-11>"
    while(qday[idx] != QChar('<')) qday[idx++];         // suche '<'
    idx++;                                              // '<' überspringen
    whv = "";
    while(qday[idx] != QChar('>')) whv += qday[idx++];  // von_datum übernehmen
    idx++;
    whb = "";
    while(qday[idx] != QChar('<')) qday[idx++];         // suche '<'
    idx++;
    while(qday[idx] != QChar('>')) whb += qday[idx++];  //  bis_datum übernehmen

    qy += " day BETWEEN '"; 
    qy += whv;
    qy += "' AND '";
    qy += whb;
    qy += "'";
    idx = qday.indexOf("ORDER BY");                    // suche ORDER BY
    if(idx != -1) {                                    // wenn gefunden füge ORDER BY  'Name' ein
      qy += " ORDER BY ";
      s = qday.mid(idx +8);  
      s = s.simplified(); 
      f = s;
      s = transl.getDbField(s);                        //
         if(s.compare("?") == 0) {
            t = f+" ?";                                // header_name unbekannt
            f = tr("Feldnamen Error : ")+t;
            QMessageBox::information(this,tr("Syntax Fehler"),(f),
            QMessageBox::Ok);
            err = 1;
          return;
         }
      qy += s;
    }
}


// ===========================================================
// -----------------------------------------------------------
//  erzeuge CSV_Daten_file - verwende date und time FORMATE 
// -----------------------------------------------------------
void query::mkText()
{
    QString p, hfeld,rip;
    QString Cept, Call, val, id;
    int col, idx;
    cnt = 0;
    path =  QDir::homePath();                     
    path += "/";                                       // auch bei ADI_datei ändern !!!!
    p = path+lineEditOutfile->text()+".csv";           // file öffnen

    QFile file(p);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Dein Linux ist kaputt";
       return;
    }
    QTextStream out(&file);
    rip = lineEditTrenner->text();
    rip = rip.simplified();
    rip += " ";
    
    QSqlQuery query; 
    query.exec(qy);                                   // query ausführen
    n = query.size();                                 // Tabellen_größe
    col = query.record().count();                     // Anzahl Tabellen_felder
    while(query.next() ) {                            // db_loop
      idx = 0;
      p = "";
      Call = "";
      Cept = "";
      while( idx != col ) {                           // -- data_header_loop --
        n = 0;                                        // ----------------------
        hfeld = query.record().fieldName(idx);        // dbfeld_name pos(col)
        val = query.value(idx).toString();            // Feld_Data_value(col)
        if(hfeld.compare("id") == 0) {                // ist satz_ID
           id = val;
        }
       else 
        if(hfeld.compare("cept") == 0) {  
           Cept = val;                                // Cept
        }
       else
        if(hfeld.compare("rufz") == 0) {              // Call
          Call = val;
          if(Cept.count() != 0) {                     // ist cept schon da ?
              val = Cept+Call;                        // bilde cept/Call
              p += val+rip;
           }
           else 
             p += Call+rip;
        }
       else
        if(hfeld.compare("band") == 0) {
           val = transl.getMyband(val);               // Band getMyBand(QString sysband)
           p += val+rip;
        }
       else
        if(hfeld.compare("day") == 0) {              // datum_format einfügen
          if(comboBoxDform->currentIndex() == 0)     // 2000-02-01
            p += val+rip;
          else {                                     // 20000201
             val = val.remove(QChar('-'), Qt::CaseInsensitive);
             p += val+rip;
          }
       }
       else
        if(hfeld.compare("btime") == 0) {             // zeit_format einfügen
          if(comboBoxTform->currentIndex() == 1) {    // hhmmss
             val = val.remove(QChar(':'), Qt::CaseInsensitive);
          }
          else
           if(comboBoxTform->currentIndex() == 2) {  // hhmm
             val = val.remove(QChar(':'), Qt::CaseInsensitive);
             val = val.left(4);
           }
          p += val+=rip;
        }
       else 
        if(hfeld.compare("etime") == 0) {             // zeit_format
          if(comboBoxTform->currentIndex() == 1) {    // hhmmss
             val = val.remove(QChar(':'), Qt::CaseInsensitive);
          }
          else
           if(comboBoxTform->currentIndex() == 2) {  // hhmm
             val = val.remove(QChar(':'), Qt::CaseInsensitive);
             val = val.left(4);
           }
           p += val+rip;
        }
       else {                                        // default 
          p += val;
        if( idx +1  == col )
            ;
        else
          p += rip;
       }
       idx++;                                        // nächstes Feld
      }
      out << p << "\n";                              // ein Datensatz fertig
      cnt++;
    }
    file.close();
}

// ====================================================================
// erzeuge ADIF_daten_file
// --------------------------------------------------------------------
void query::mkAdif()
{
    QString fp;
    path =  QDir::homePath();                     
    path += "/";
    fp = path+lineEditOutfile->text()+".adi";         // file öffnen
   
    quadi adi;
    adi.createAdifTableHeader();
    adi.setBandTable();
   
    QSqlQuery query; 
    s = "SELECT atype,adiftype FROM wawdlist WHERE aset != ''";
    query.exec(s);
    while(query.next()) { 
       n = 0;
       t = query.value(n++).toString();         // atype
       s = query.value(n++).toString();         // adiftype
       adi.loadAdifAwdTable(t,s);               // in Tabelle eintragen
    }
    
    adi.setDayForm(comboBoxDform->currentIndex());
    adi.setTimeForm(comboBoxTform->currentIndex());
    
    s = qy;
    i = s.indexOf("SELECT");
    qy = "SELECT id,"+s.mid(i +6);
    adi.doExport( logFile, qy, fp,AWD, 0, 0 );
   
    cnt = adi.getCount();
    adi.clearExpHash();
}
