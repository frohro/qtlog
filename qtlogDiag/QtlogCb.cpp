/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

// QtLOg_CB    01.05.2007 dl1hbd 

#include <QtGui>
#include <QSettings>
#include <QtSql>
#include <QMessageBox>
#include "Qtlog.h"
#include "confDiag.h"
#include "version.h"

// -------------------------------------------------------------------
// Query default 
// -------------------------------------------------------------------
// example
// SELECT id,cept,rufz,band,mode,name,qth,day,btime,(SELECT awkenner FROM funawd WHERE awtype='DLD' AND id=qid),dxcc FROM funom LEFT JOIN (funqsl,fun) ON (omid=oid AND qsoid=id) WHERE lotqsls='Y'
// default
void QtLog::buildQuery()
{
   i = qystr.indexOf("AWD");                                                // get Index for AWD
   qy = qystr.left(i);                   
   if( i != -1 ) {                       
      s = "(SELECT awkenner FROM "+logFile+"awd WHERE awtype='"+BoxAwd->currentText()+"' AND id=qid)";
      qy += s;
      s = qystr.mid(i +3);                                                  // skip AWD in the string        
      qy += s;
   }
   qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
   qy += " WHERE ";
   s = transl.getDbField(BoxGroup->currentText());
   if(s.compare("band") == 0) {
     p = "SELECT mband FROM wband WHERE mband='"+groupEditLine->text()+"'"; // check band format
     QSqlQuery query;
     query.exec(p);
     if(!query.size()) {                                                    // report bugs
        QMessageBox::information( this,
        tr("Band Fehler"),tr("\nBandnamen immer kompl. eingeben"),
        QMessageBox::Ok | QMessageBox::Default,
        QMessageBox::NoButton, QMessageBox::NoButton);
        groupEditLine->setFocus(); 
        return;
     }
     query.next();
     qy += "band ='"+transl.getSysband(query.value(0).toString());
     qy += "'";                          
   }
   else
     qy += s+" LIKE '"+groupEditLine->text()+"%'";
     if(checkBoxDateEpoch->isChecked() == true) {                           // if set epoch
       qy += " AND day BETWEEN "; 
       QDate d = dateEditBis->date();                    
       qy += "'"+s.setNum(d.year());
       qy += "-"+s.setNum(d.month());
       qy += "-"+s.setNum(d.day());
       qy += "' AND ";
       d = dateEditVon->date();                          
       qy += "'"+s.setNum(d.year());
       qy += "-"+s.setNum(d.month());
       qy += "-"+s.setNum(d.day());
       qy += "'";
     }
     else {                                                
       if(groupEditLine->text().count() == 0) {            
	 n = BoxLimit->currentText().toInt();              
	 if( n != 0) {
           qy += " AND day BETWEEN ";                      
           QDate d = QDate::currentDate();                 
           d = d.addMonths(-n);                            
           qy += "'"+s.setNum(d.year());
           qy += "-"+s.setNum(d.month());
           qy += "-"+s.setNum(d.day());
           qy += "' AND ";
           d = dateEditVon->date();                        
           qy += "'"+s.setNum(d.year());
           qy += "-"+s.setNum(d.month());
           qy += "-"+s.setNum(d.day());
           qy += "'";
	 }
       }
     }
     qy += " ORDER BY ";
     s = transl.getDbField(BoxSort->currentText());
     qDebug() << "sort:" << BoxSort->currentText();
     qDebug() << s; 
     
     if(s.indexOf("day") != -1) {
        qy += "day DESC,btime DESC";
     }
     else
       qy += transl.getDbField(BoxSort->currentText())+" DESC";
     qDebug() << "qy is:"<< qy <<"\n";
     readData(qy,awdPos);
     groupEditLine->setFocus();
}

// ------------------------------------------------------------------------
// Query for BoxAWD - ohne Kenner
//------------------------------------------------------------------------
void QtLog::awdQuery()
{
  if( awdPos != -1 ) {
    i = qystr.indexOf("AWD");                       // get AWD_index    
    qy = qystr.left(i);                    
    qy += "awkenner";
    qy += qystr.mid(i+3);
    qy += " FROM ("+logFile+"om,"+logFile+"awd)";
    qy += " LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
    qy += " WHERE (awtype='"+BoxAwd->currentText();
    qy += "' AND id=qid) ORDER BY "+transl.getDbField(BoxSort->currentText());
    qy += " DESC";
    readData(qy,awdPos);
    awdEditLine->setFocus();
  }
  else {
    QMessageBox::information( this,
    tr("   INFO    "),tr("\nAWD-Auswertung ist im Layout nicht selektiert"),
    QMessageBox::Ok | QMessageBox::Default,
    QMessageBox::NoButton, QMessageBox::NoButton);
  }
}

// ------------------------------------------------------------------------
// Query für BoxAWD - mit Kenner
//------------------------------------------------------------------------
void QtLog::awdQuerykenner()
{
  if( awdPos != -1 ) {
    i = qystr.indexOf("AWD");              
    qy = qystr.left(i);                    
    qy += "awkenner";
    qy += qystr.mid(i+3);
    qy += " FROM ("+logFile+"om,"+logFile+"awd)";
    qy += " LEFT JOIN ("+logFile+"qsl,"+logFile+")";
    qy += " ON (omid=oid AND qsoid=id)";
    qy += " WHERE awtype='"+BoxAwd->currentText();
    qy += "' AND awkenner='"+awdEditLine->text();
    qy += "' AND id=qid ORDER BY "+transl.getDbField(BoxSort->currentText());
    qy += " DESC";
    readData(qy,awdPos);
    awdEditLine->setFocus();
  }
  else {
     QMessageBox::information( this,
     tr("   INFO    "),tr("\nAWD-Auswertung ist nicht selektiert"),
     QMessageBox::Ok | QMessageBox::Default,
     QMessageBox::NoButton, QMessageBox::NoButton);
  }
}

// -------------------------------------------------------------------------
// Query vom Text export Data
// -------------------------------------------------------------------------
void QtLog::textExportData(QString q, int awdp)
{
   QString h;
   s = q;
   i = s.indexOf("SELECT");
   h = "SELECT id,"+s.mid(i +6);
   readData(h,awdp);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// read data from DB and display
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void QtLog::readData(QString q, int awdpos)
{
      QString b;
      bflg = -1;
      QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
      reportTable->clear();                                
      QSqlQuery query; 
      query.exec(q);                                       
      //qDebug() << query.lastError();
      i = query.size();                                    // Tabellen_Laenge ( anzahl rows )          
      col = query.record().count();                        // Anzahl Header_Felder         
      reportTable->setColumnCount(col -1);                 // länge Horizontal_Header ohne Id_nr setzen        
      reportTable->setRowCount(i);                         // Tabellen Laenge setzen      
      n = 0;                                               
      s = query.record().fieldName(n++);                   
      //Pos '0' im Horizontal_header ist das Datum ( nicht die linke obere Ecke ! )
      reportTable->setColumnWidth(0,80);                   // Spaltenbreite für HeaderItem setzen
      while(n  != col) {                                   // HorizontalHeader - aufbauen          
         s = query.record().fieldName(n);                  // Feld_namen lesen         
         QTableWidgetItem * cItem = new QTableWidgetItem(tr("%1").arg((n)));
         if( !(QString::compare(s,"band"))) bflg = n -1;   // suche 'band'-spalte
         if(n == awdpos) {                                 // check AWD_Kenner_position im Header
             cItem->setText(BoxAwd->currentText());        // falls gefunden: BoxAwd_Text setzen
             b = "AWD";
         }
         else {
           b = transl.getUsrField(s);                       // Header_feld_namen übersetzen      
           cItem->setText(b);                               // in item eintragen   
         }
         
         reportTable->setHorizontalHeaderItem(n -1,cItem);       // und im Header eintragen     
         reportTable->setColumnWidth(n -1,transl.getFieldBr(b)); // Spaltenbreite für HeaderItem setzen
         n++;                                                    // nächstes Feld ( Spalte )
      }
    
      row = 0;                                                   // zeilen_zähler
      while(query.next()) {                                      // Die QSO_Daten lesen
        z = 0;                                                   // col
        r = 0;                                                   // row
        i = 0;                                                   // query_index
        s = query.value(i++).toString();                         // Id für vertikal_header holen
        QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((r++)*(z++))); //Id_item erzeugen
        rowItem->setText(s);                                     // text eintragen
        reportTable->setVerticalHeaderItem(row,rowItem);         // Id ( ist QSO_satz_nr ) setzen
        c = 0;
        while( c != col -1 ) {                                   // alle folgenden Spalten bearbeiten
          QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(c))); 
          if( c == bflg ) {                                      // check BAND
             newItem->setText(transl.getMyband(query.value(i++).toString())); // band_text
          }
          else
            newItem->setText(query.value(i++).toString());       // text eintragen
          reportTable->setItem(row,c++,newItem);                 // an row,col übergeben
        }
        row++;                                                   // nächster Satz
     }
     statusBar()->showMessage(tr("Fertig"));
     mStatLabel->setText(tr("Anzeige : %1 QSO's - %2 Spalten").arg(s.setNum(row),p.setNum(col))); 
     
     groupEditLine->setFocus();
     QApplication::restoreOverrideCursor();
}

// -----------------------------------------------------------------------------------------
// check dabase connection
// -----------------------------------------------------------------------------------------
void QtLog::CheckDBconnection()
{
   s = getenv("HOME");                         
   s += "/.qtlog/qlogfile";
   QFile lfile(s);
   if(! lfile.open(QIODevice::Append | QIODevice::Text)) {
         qDebug() << "FEHLER - qlogfile nicht vorhanden !! hier stimmt nichts";
       return;
   }
   QTextStream out(&lfile);
   if(! dbstat ) {                             
       QDateTime heute = QDateTime::currentDateTime();
       out << "\nqtlog - DB Info - "+heute.toString (Qt::TextDate)+"\n";
       out << "settings: \n";
       out << "dbname = "+settings.value("dbname").toString()+"\n";
       out << "dbpasswd = "+settings.value("dbpasswd").toString()+"\n";
       out << "dbuser = "+settings.value("dbuser").toString()+"\n";
       out << "host = "+settings.value("host").toString()+"\n";
       out << "port = "+settings.value("port").toString()+"\n";
       out << "mySQLPath = "+settings.value("mySQLPath").toString()+"\n";
       out << "Usermeldung: ";
       s = tr("QtLog System Info ");
       p = tr("\nDas Datenbank runtime_Flag steht auf: 0 \nQtLog wurde noch nicht Initialisiert !!\nOder dieses Flag wurde beschaedigt.\n\nIn diesem Fall ist ein groesserer Schaden entstanden.\nSchaue in '.config/Qtlog/qtlog.ini'.\nLese mit einem Browser im doc_Ordner INSTALL.\n\nWird keine Abweichung gefunden, sende mir eine eMail mit dem '.qtlog/qlogfile' an diek@dl1hbd.de.\n");
       out << p;
       QMessageBox::information( this,s,p,
             QMessageBox::Ok | QMessageBox::Default,
             QMessageBox::NoButton, QMessageBox::NoButton);
       lfile.close();
      exit(0);
  }
  else {
    db.setHostName(settings.value("host").toString());             // Datenbank oeffnen
    db.setDatabaseName(settings.value("dbname").toString());
    db.setUserName(settings.value("dbuser").toString());
    db.setPassword(settings.value("dbpasswd").toString());
  }
  if( !db.open() ) {
     err = db.lastError ();
     QDateTime heute = QDateTime::currentDateTime();
     out << "\n\nqtlog - DB Info - "+heute.toString (Qt::TextDate)+"\n";
     out << "Server: "+ err.databaseText()+"\n";
     out << "settings :\n";
     out << "dbpasswd = "+settings.value("dbpasswd").toString()+"\n";
     out << "dbuser = "+settings.value("dbuser").toString()+"\n";
     out << "host = "+settings.value("host").toString()+"\n";
     out << tr("Meldung an User:");
     s = tr("\nFehlermeldung vom MySQL-Server !\nDie Zugangsberechtigung ist beschaedigt\nBetrieb ist nicht moeglich. Pruefe:\n");
     s += tr("UserName = ")+ settings.value("dbuser").toString();
     s += tr("\nPasswort = ") + settings.value("dbpasswd").toString();
     s += tr("\ndbname = ")+settings.value("dbname").toString();
     s += "\n";
     out << s;
     QMessageBox::information( this,
         tr("Server-Datenbank INFO"),
         tr(s.toAscii()),
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
     lfile.close();
     exit(0);                              // Abbruch
  }
   // check db_Release_Nr
   QDateTime heute = QDateTime::currentDateTime();
   QSqlQuery query;                           
   qy = "SELECT relnr FROM dblogs";
   query.exec(qy);
   query.next();
   s = query.value(0).toString();
   if(s.compare(RELEASE) != 0) {          // check db_release_nr identisch mit Programm_Version        
     s = RELEASE;
     out << tr("Rel.Nr : ")+s+"\n";
     out << tr("\nqtlog - Versions Info - ")+heute.toString (Qt::TextDate)+"\n";
     s = tr("\nDie DB_Release_Nr ist ungueltig.\n\nEs muss mit 'setupqtlog' eine Datenbank-Korrektur durchgefuehrt werden.\nInformiere dich auf www.dl1hbd.de, oder sende eine Anfrage. Betrieb ist nicht moeglich !");
     out << s;
     QMessageBox::information( this,
         tr("\nDatenbank - Info\n"),
         tr(s.toAscii()),
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
     lfile.close();
     exit(0);                            // Abbruch
   }
   out << "Start : qtlog - "+heute.toString (Qt::TextDate)+"\n";
   out << VERSION << "Rel" << RELEASE"\n";
   s = "  "+settings.value("dbname").toString()+" ]";
   labelDatabase->setText(s);
   lfile.close();
}
