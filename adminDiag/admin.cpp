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
#include <QtNetwork>
#include "admin.h"
#include "../qtlogDiag/dirmngr.h"
#include "../qtlogDiag/version.h"

extern DirMngr dirMngr;

// ---------------------------------------------
admin::admin(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                         
   setFont(font);
   connect(actionEnde, SIGNAL(triggered(bool)), this, SLOT(goExit()));
   connect(actionAdif_Export, SIGNAL(triggered(bool)), this, SLOT(adifExpCb()));
   connect(actionAdif_Import, SIGNAL(triggered(bool)), this, SLOT(adifImpCb()));
   connect(actionDump, SIGNAL(triggered(bool)), this, SLOT(dumpCb()));
   connect(actionRefNamen, SIGNAL(triggered(bool)), this, SLOT(refNamenCb()));
   connect(actionHilfe, SIGNAL(triggered(bool)), this, SLOT(getHilfeCb()));
   connect(pushButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(stackedWidgetCb(int)));
   connect(checkBoxLog, SIGNAL(pressed()), this, SLOT(toggelLogBoxCb()));
   connect(checkBoxDb, SIGNAL(pressed()), this, SLOT(toggelDbBoxCb()));
   connect(checkBoxQslMode, SIGNAL(pressed()), this, SLOT(toggelQslModeCb()));
   
   connect(logList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(logListCb(QTreeWidgetItem*,int)));
   connect(sysList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(sysListCb(QTreeWidgetItem*,int)));
   connect(RefTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateRefItemCb(QTableWidgetItem*)));
   connect(RefTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(saveValCb(QTableWidgetItem*)));
   connect(customsTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateCustomItemCb(QTableWidgetItem*)));
   connect(customsTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(saveValCb(QTableWidgetItem*)));
   connect(ButtonDumpDir, SIGNAL(clicked()), this, SLOT(getSaveDirCb()));
   connect(ButtonAdifDir, SIGNAL(clicked()), this, SLOT(getAdifDirCb()));
   connect(ButtonDump, SIGNAL(clicked()), this, SLOT(dbDump()));
   connect(ButtonRestore, SIGNAL(clicked()), this, SLOT(dbRestore()));
   connect(ButtonAdifExport, SIGNAL(clicked()), this, SLOT(adifExport()));
   connect(ButtonAdifImport, SIGNAL(clicked()), this, SLOT(adifImport()));
   s = settings.value("dbname").toString();
   s += " Rel.: ";
   s += RELEASE;
   
   label_db->setText(s);
   label_dbase->setText(s);
   dirsel = 0;
   setupAction();
   lItem = 0;
   sItem = 0;
   udpSocket = new QUdpSocket(this);      // Datagram_server
   ltreeWidget = logList;
}

// -----------------------------
void admin::setupAction()
{
   readSettings();
   logList->setColumnWidth(0,60);          // Log_name
   logList->setColumnWidth(1,60);          // Einträge
   logList->setColumnWidth(2,40);          // Rel
   
   sysList->setColumnWidth(0,120);
   
   RefTable->setColumnWidth(0,100);        // sys.Name
   RefTable->setColumnWidth(1,100);        // User.Name
   RefTable->setColumnWidth(2,100);        // User_en
   RefTable->setColumnWidth(3,135);        // Adif.Name
   
   //RefTable->setColumnWidth(4,60);
   //RefTable->setColumnWidth(5,60);
   
   customsTable->setColumnWidth(0,80);
   customsTable->setColumnWidth(1,190);
   
   if(settings.value("dbconf").toInt() == 0) {      // liegt ein Fehler vor ?
     s = tr("\nDas Runtime-Flag 'dbconf' steht auf Null.\nDas Flag ist beschaedigt, ");
     s += tr("oder QtLog ist nicht initialisiert\n");
     s += tr("Das Programm schaltet ab");
     QMessageBox::information( this,
         tr("QtLog INFO"),
         tr(s.toAscii()),
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
     exit(0);                                       // Abbruch       
   }
   else {
     openDatabase();                                // normaler db_connect   
     showTables(1);                                 // Log + SysTabellen zeigen
   }
   if(settings.value("Val").toInt() == 1) {         // ADIF_Export
       dirsel = 0;                                  // dirselector auf Pfad stellen
       adifExpCb();
   }
   else
    if(settings.value("Val").toInt() == 2) {       // ADIF_Import
       dirsel = 1;                                 // dirselector auf Datei waehlen stellen
       adifImpCb();
    }
   QSqlQuery query;
   qy = "SELECT rigtype FROM wrig";
   query.exec(qy);
   while(query.next()) {
       n = 0;
       comboBoxRig->addItem(query.value(n++).toString());
   }
   qy = "SELECT lid,logging FROM tlocation WHERE logging != ''";
   query.exec(qy);
   while(query.next()) {
       i = 0;
       n = 0;
       i = query.value(n++).toInt();                       // id   
       comboBoxHome->addItem(query.value(n++).toString()); // logging_text
   }
   
   comboBoxHome->setCurrentIndex(1);                          
   SavePathEdit->setText(settings.value("dumpPath").toString());
}

// ---------------------------
void admin::openDatabase()
{
    //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("host").toString());              // oeffne die Datenbank           
    db.setDatabaseName(settings.value("dbname").toString());        // logdb        
    db.setUserName(settings.value("dbuser").toString());                       
    db.setPassword(settings.value("dbpasswd").toString());          // z.B "dieter";        
    if(!db.open()) {
       qDebug() << "usrDB_FEHLER - " << db.lastError();     // sollte hier nicht vorkommen
    }
}

// ==========================================================================
// Tabellen_Namen und Größen in logList u. sysList darstellen
// ==========================================================================
void admin::showTables(int f)
{
    i = f;                                              // dummy
    QSqlQuery query;
    qy = "SELECT * FROM dblogs";                        // alle logtabellen laden
    query.exec(qy);
    while(query.next()) {
       i = 0;
       n = 0;
       QTreeWidgetItem *item = new QTreeWidgetItem(logList);
       item->setText(i++,query.value(n++).toString());  // logFile
       item->setText(i++,query.value(n++).toString());  // QSO's
       item->setText(i++,query.value(n++).toString());  // Rel.
       item->setText(i++,query.value(n++).toString());  // letzte Sicherung
    }

    int row = logList->topLevelItemCount();            // Anzahl Logtabellen
    QTreeWidgetItem *Item;                             // Arbeitsitem bilden
    qy = "SHOW TABLES FROM "+settings.value("dbname").toString();
    query.exec(qy);                                    // hole alle Tabellen_Namen von der db
    while(query.next()) {
       n = 0;
       s = query.value(n++).toString();                // 1.Tabellen_name
       i = 0;
       while(row != i ) {                              // row = Anzahl logtabellen
         Item =  logList->topLevelItem (i++);          // Item von Loglist ( log_Basisnamen )
         b = Item->text(0);                            // hole Name
         x = b.size();                                 // Namenlänge basisnamen
         b = s.left(x);                                //
         if((b.compare(Item->text(0)) == 0)) {         // Vergleiche mit Name aus der db
             x = 0;                                    // wenn gefunden x = 0;
           break;                                      // ist fun oder funom, funawd, fundx
         }
       }
       if( x == 0 )                                    // wenn x=0 nichts -tun
        ;                                              // ist log_name
       else {                                          // sonst:
        QTreeWidgetItem *item = new QTreeWidgetItem(sysList); // item bilden
        item->setText(0,s);                            // und name in sysList eintragen
       }
    }
     // Alle Tabellen_Größen der system_tabellen ermitteln und in sysList eintragen
    row = sysList->topLevelItemCount();                // Anzahl_tabellen in sysList
    QTreeWidgetItem *sItem;                            // arbeits_system_item erzeugen
    i = 0;
    while( row != i ) {
       sItem =  sysList->topLevelItem (i);             // 1.item
       s = sItem->text(0);                             // name holen
       qy = "SELECT COUNT(*) FROM "+s;
       query.exec(qy);                                 // Tabellen_länge aus der db holen
       query.next();
       sItem->setText(1,query.value(0).toString());    // laenge eintragen
       i++;
    }
}

// ---------------------
admin::~admin()
{
    writeSettings();
}

// -----------------------
void admin::writeSettings()
{
  settings.setValue("admin/Size",size());
  settings.setValue("admin/Properties",saveState());
  settings.setValue("dumpPath",SavePathEdit->text());
}

// -------------------------
void admin::readSettings()
{
  resize(settings.value("admin/Size",sizeHint()).toSize());
  restoreState(settings.value("admin/Properties").toByteArray());
}

// ------------------------
void admin::goExit()                           
{
    db.close();                                
    qApp->quit();
}

// ------------------------
void admin::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                       
             goExit();
         break;
   }
}

// Hilfe
// --------------------------
void admin::getHilfeCb()
{
   settings.setValue("Val","DB-Administration");
   StartProcess("hilfedb &");
}

// --------------------------------------------
void admin::stackedWidgetCb( int page )
{
     switch(page) {
     case 0:                                             // Datensicherung DUMP / RESTORE
              logList->setEnabled(true);                 // freigeben
              sysList->setEnabled(true);                 // freigeben
             break;
     case 1:                                             // Daten ADIF Im/Export
              logList->setEnabled(true);                 // freigeben
              sysList->setEnabled(false);                // sperren
              getCustomsfields();                        // tabelle awd laden
             break;
     case 2:                                  // Daten_Convertierung von V 1.4/1.4 -> 1.5
              sysList->setEnabled(false);               // sperren
              logList->setEnabled(true);                // freigeben
             break;
     case 3 :                                           // Referenz-Namen verwalten
              logList->setEnabled(false);               // sperren
              sysList->setEnabled(false);               // sperren
              getRefnamen();                            // Tabelle Ref.Namen laden
             break;
     }
}

// ADIF - Export
// --------------------------------
void admin::adifExpCb()
{
   ButtonAdifExport->setEnabled(true);                              // freigeben
   ButtonAdifExport->setPalette( QPalette(QColor(180, 210, 200)));  // gruen
   
   ButtonAdifImport->setEnabled(false);                             // sperren
   ButtonAdifImport->setPalette( QPalette(QColor(238, 238, 238)));  // grau
   checkBoxQslMode->setEnabled(false);                              // sperren
   checkBoxQslMode->setPalette( QPalette(QColor(238,238,238)));     // grau
   comboBoxRig->setEnabled(false);                                  // sperren
   comboBoxRig->setPalette( QPalette(QColor(238,238,238)));         // grau
   comboBoxHome->setEnabled(false);                                 // sperren
   comboBoxHome->setPalette( QPalette(QColor(238,238,238)));        // grau
   
   SavePathAdif->setText("");
   dirsel = 0;
   ButtonAdifDir->setText(tr("Waehle ADIF Pfad"));
   stackedWidget->setCurrentIndex(1);
}

// ADIF - IMPORT
// --------------------------------
void admin::adifImpCb()
{
   ButtonAdifExport->setEnabled(false);                             // sperren
   ButtonAdifExport->setPalette( QPalette(QColor(238, 238, 238)));  // grau
   
   ButtonAdifImport->setEnabled(true);                              // freigeben
   ButtonAdifImport->setPalette( QPalette(QColor(180, 210, 200)));  // grün
   
   checkBoxQslMode->setEnabled(true);
   checkBoxQslMode->setPalette( QPalette(QColor(180, 210, 200)));   // grün
   
   comboBoxRig->setEnabled(true);                                   // freigeben
   comboBoxRig->setPalette( QPalette(QColor(180, 210, 200)));       // grün
   comboBoxHome->setEnabled(true);                                  // freigeben
   comboBoxHome->setPalette( QPalette(QColor(180, 210, 200)));      // grün
   SavePathAdif->setText("");
   dirsel = 1;
   ButtonAdifDir->setText(tr("Waehle ADIFdatei"));
   stackedWidget->setCurrentIndex(1);
}

// Datensicherung DUMP / RESTORE
// ---------------------------------
void admin::dumpCb()
{
   stackedWidget->setCurrentIndex(0);
}

// ReferenzNamen verwalten
// ----------------------------------
void admin::refNamenCb()
{
   stackedWidget->setCurrentIndex(3);
}

// toggel checkBoxLog
// -------------------------------------------------------
void admin::toggelLogBoxCb()
{
    if(checkBoxLog->isChecked() == FALSE)
       checkBoxDb->setChecked(FALSE);
    else
       checkBoxDb->setChecked(TRUE);
}

// toggel checkBoxDB
// -------------------------------------------------------
void admin::toggelDbBoxCb()
{
    if(checkBoxDb->isChecked() == FALSE)
       checkBoxLog->setChecked(FALSE);
    else
       checkBoxLog->setChecked(TRUE);
}

// toggel QSL_Mode
// -------------------------------------------------------
void admin::toggelQslModeCb()
{
    if(checkBoxQslMode->isChecked() == FALSE) {
        checkBoxQslMode->setChecked(FALSE);
        checkBoxQslMode->setText("Default Import");
    }
    else {
      checkBoxQslMode->setChecked(TRUE);
      checkBoxQslMode->setText("Nicht setzen - Contest Import");
    }
}

// ----

// logList_Item changed
// ------------------------------------------------------
void admin::logListCb(QTreeWidgetItem *item,int)
{
   if( logList != ltreeWidget ) {
     ltreeWidget->clearSelection ();               // alte selection loescheen
   }
   ltreeWidget = logList;                          // jetzt logList selectiert
   sItem = 0;
   lItem = item;
   val = lItem->text(0);                           // jetzt logList selectiert
}

// ----

// // sysList_Item changed
// -------------------------------------------------------
void admin::sysListCb(QTreeWidgetItem *item,int)
{
   if( sysList != ltreeWidget ) {
     ltreeWidget->clearSelection ();               // alte selection loeschen
   }
   lItem = 0;
   ltreeWidget = sysList;                          // jetzt sysList selectiert
   sItem = item;
   val = item->text(0);                            // Tabellen_namen sichern
}


// -------------------------------------------------------
QString admin::DirSelector()
{
   if( !dirsel ) {
     QString directory = QFileDialog::getExistingDirectory(this,
             tr("Waehle einen Ordner"), QDir::homePath());
	     directory+="/";
     return directory;
   }
   else {
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                        "/home", tr(" *.adi *.adif *.ADI *.ADIF"));
     return fileName;
   }
}


// ------------------------------------------------------
// DUMP in Initialisierungs_Ordner  als TEST
// ------------------------------------------------------
void admin::getSaveDirCb()
{
   dirsel = 0;
   SavePathEdit->setText(DirSelector());
}


// =====================================================================
// DUMP alle Tabellen oder eine einzelne Tabelle
// ---------------------------------------------
void admin::dbDump()
{
    if(SavePathEdit->text().count() == 0) {       // ist ein Path gewaehlt ?
      QMessageBox::information( this,
        tr("INFO"),
        tr("\nBitte DUMP-Ordner waehlen !!"),
        QMessageBox::Ok);
      return;
    }
        QString pathToMysql="";
#ifdef Q_WS_WIN
    pathToMysql=settings.value("mySQLPath","").toString();
#endif
    if(checkBoxDb->isChecked() == TRUE ) {     // gesetzt - Alle Tabellen Dumpen
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        s = pathToMysql+"mysqldump -u"+settings.value("dbuser").toString()+" -p";
        s += settings.value("dbpasswd").toString();
        s += " --opt "+settings.value("dbname").toString()+" > "+SavePathEdit->text()+"backup-db.sql"; 
        StartProcess(s.toAscii());
        QApplication::restoreOverrideCursor();
    }
    else {                                   // LOG_tabelle dumpen
       QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
       if(lItem) {
         s = pathToMysql+"mysqldump -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" --opt "+settings.value("dbname").toString()+" ";
          s += lItem->text(0)+" ";          // fun
          s += lItem->text(0)+"qsl ";       // fun_qsl
          s += lItem->text(0)+"om ";        // fun_om
          s += lItem->text(0)+"awd ";       // fun_awd
          s += lItem->text(0)+"card ";      // fun_qslcard
          s += lItem->text(0)+"dx ";        // fun_dx
          s += "wawdlist > ";               
          s+= SavePathEdit->text()+"backup-"+lItem->text(0)+".sql";
          StartProcess(s.toAscii());         // DUMP
	  
          QString tag, monat, jahr, dform;
          QDate d = QDate::currentDate();   // Datum von heute
          tag = s.setNum(d.day());
          monat = s.setNum(d.month());
          jahr = s.setNum(d.year());
	  
          if(tag.count() < 2) tag = "0"+tag;
          if(monat.count() < 2) monat = "0"+monat;
	  
          // Datum_Datensicherung
          qy = "UPDATE dblogs SET saved='"+jahr+"-"+monat+"-"+tag;
          qy += "' WHERE logname='"+lItem->text(0)+"'";
          QSqlQuery query;
          query.exec(qy);
          s = jahr+"-"+monat+"-"+tag;
          lItem->setText(3,s);
       }
       else
         if(sItem) {                          // System_tabelle dumpen
            s = pathToMysql + "mysqldump -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" --opt "+settings.value("dbname").toString();
            s += " "+sItem->text(0)+" > ";
            s+= SavePathEdit->text()+"backup-"+sItem->text(0)+".sql";
            StartProcess(s.toAscii());        // DUMP
         }
       else {
          QApplication::restoreOverrideCursor();
          QMessageBox::information( this,
          tr("INFO"),
          tr("\nErst eine Tabelle im Logbook- oder QtLog-Ordner waehlen"),
          QMessageBox::Ok);
          return;
       }
    }
    
    s = "cp "+QDir::homePath();             // sichere immer auch qtlog.ini
    s += "/.config/QtLog/qtlog.ini "+SavePathEdit->text();
    StartProcess(s.toAscii());
    
    QApplication::restoreOverrideCursor();
}


// ======================================================================
// RESTORE database kompl. oder eine einzelne Tabelle
// --------------------------------------------------------
void admin::dbRestore()
{
  int i;
  QString p;
  QFile file;
     i = 0;
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     QString pathToMysql="";
#ifdef Q_WS_WIN
    pathToMysql=settings.value("mySQLPath","").toString();
#endif
     if(checkBoxDb->isChecked() == TRUE ) {          // gesetzt, RESTORE alle Tabellen
        p += SavePathEdit->text()+"backup-db.sql";
        if(!file.exists(p)) {
           QApplication::restoreOverrideCursor();
           QMessageBox::information( this,
           tr("INFO"),
           tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
           QMessageBox::Ok);
          return;
        }
        QSqlQuery query;
        // hier müsste noch pathToMysql rein ! ?
        qy = "DROP DATABASE IF EXISTS "+settings.value("dbname").toString(); 
        query.exec(qy);
         // auch hier müsste noch pathToMysql rein ! ?
        qy = "CREATE DATABASE logdb";                                        
        query.exec(qy);
        qy = "USE logdb";
        query.exec(qy);
 
        // Windows: dbuser hinzugefuegt, sollte unter Linux auch gehen
        s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString();
        s += " "+ settings.value("dbname").toString()+" < ";
        s += p;
        StartProcess(s.toAscii());
        i = 1;
     }
     else {                                              // RESTORE eine log_Tabelle
        if( lItem) {
          p = SavePathEdit->text()+"backup-"+lItem->text(0)+".sql";
          if(!file.exists(p)) {
             QApplication::restoreOverrideCursor();
             QMessageBox::information( this,
             tr("INFO"),
             tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
             QMessageBox::Ok);
            return;
          }
         // fun, funqsl, funom, funawd, funcard, fundx, wawdlist
          s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" "+settings.value("dbname").toString()+" < ";
          s+= p;
          StartProcess(s.toAscii());
          QSqlQuery query;
          qy = "SELECT COUNT(*) FROM "+lItem->text(0);
          query.exec(qy);
          query.next();
          s = query.value(0).toString();
          qy = "UPDATE dblogs SET qsocnt='"+s+"' WHERE logname='"+lItem->text(0)+"'";
          query.exec(qy);
          i = 0;
       }
       else
          if( sItem ) {                                  // eine system_tabelle 
             p = SavePathEdit->text()+"backup-"+sItem->text(0)+".sql";
             if(!file.exists(p)) {
                QApplication::restoreOverrideCursor();
                QMessageBox::information( this,
                tr("INFO"),
                tr("\nRESTORE nicht moeglich - Datei nicht vorhanden !"),
                QMessageBox::Ok);
               return;
             }
             s = pathToMysql + "mysql -u"+settings.value("dbuser").toString()+" -p"+settings.value("dbpasswd").toString()+" "+settings.value("dbname").toString()+" < ";
             s += p;
             StartProcess(s.toAscii());
             i = 0;
          }
        else {
          QApplication::restoreOverrideCursor();
          QMessageBox::information( this,
          tr("INFO"),
          tr("\nErst eine Tabelle im Logbook- oder QtLog-Ordner waehlen"),
          QMessageBox::Ok);
          return;
       }
     }
     logList->clear();
     sysList->clear();
     showTables(i);                               // Ergebnis in logList und sysList anzeigen.
     QApplication::restoreOverrideCursor();
}

// ----------------------------
void admin::message()
{
     qDebug() << "FEHLER";
}
