/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QSettings>
#include <QString>
#include <QTabWidget>
#include <QtGui>
#include <QtSql>
#include <stdlib.h>
#include <QtNetwork>
#include "confDiag.h"
#include "version.h"
#include "dirmngr.h"

confDialog::confDialog(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
  setupUi(this);
  n = settings.value("FontSize").toString().toInt();
  QFont font;
  font.setPointSize(n); 			
  setFont(font);

  LogLabel->setText(settings.value("logfile").toString());

  wCall->setText(settings.value("Call").toString());
  wName->setText(settings.value("Name").toString());
  wQth->setText(settings.value("Qth").toString());
  wDxcc->setText(settings.value("Dxcc").toString());
  wLocator->setText(settings.value("Locator").toString());
  wUtdif->setText(settings.value("Utdif").toString());
  wLaenge->setText(settings.value("Laenge").toString());
  wBreite->setText(settings.value("Breite").toString());
  wMail->setText(settings.value("Mail").toString());
  wWww->setText(settings.value("www").toString());

  wClub->setText(settings.value("Club").toString());
  wKennerType->setText(settings.value("KennerType").toString());
  wKenner->setText(settings.value("Kenner").toString());
  wAwd->setText(settings.value("ClubAwd").toString());
  wQslMgr->setText(settings.value("QslMgr").toString());
  
  // setup SIGNAL and SLOT connection
  connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
  
  // Standort - Bei jedem Tastedruck wird ein Flag gesetzt - Text verändert.
  connect(wCall, SIGNAL(textChanged(const QString)), this, SLOT(wCallCb(const QString&)));
  connect(wName, SIGNAL(textChanged(const QString)), this, SLOT(textMoved(const QString&)));
  connect(wQth, SIGNAL(textChanged(const QString)), this, SLOT(textMoved(const QString&)));
  connect(wDxcc, SIGNAL(textChanged(const QString)), this, SLOT(textMoved(const QString&)));
  connect(wLocator, SIGNAL(textChanged(const QString)), this, SLOT(wLocatorCb(const QString&)));
  connect(wUtdif, SIGNAL(textChanged(const QString)), this, SLOT(textMoved(const QString&)));
  connect(wLaenge, SIGNAL(textChanged(const QString)), this, SLOT(textMoved(const QString&)));
  connect(wBreite, SIGNAL(textChanged(const QString)), this, SLOT(textMoved(const QString&)));
  connect(wMail, SIGNAL(textChanged(const QString)), this, SLOT(textMoved(const QString&)));
  connect(wWww, SIGNAL(textChanged(const QString)), this, SLOT(textMoved(const QString&)));
  // save CLUB
  connect(ClubButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
  connect(NewLogButton, SIGNAL(clicked()), this, SLOT(makeLogFile()));
  connect(DelLogButton, SIGNAL(clicked()), this, SLOT(deleteLogFile()));
  connect(dblogsList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(changeLogCb(QTreeWidgetItem *, int)));
   if( state == 0 )
      tabWidget->setCurrentIndex ( 5 );        // Config DB zeigen
   else
      tabWidget->setCurrentIndex ( 0 );        // normal - config LOG's default
   dblogsList->setColumnWidth(2,60);
   QSqlQuery query;
   qy = "SELECT * FROM dblogs";                // dblogs laden
   query.exec(qy);
   while(query.next()) {
      i = 0;
      n = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(dblogsList);
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i,query.value(n++).toString());      
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(i++,query.value(n++).toString());    
   }
   work = 0;
   workLog->setFocus();
   tabWidget->setCurrentIndex(settings.value("Val").toInt()); // setze Page
   qy = "SELECT * FROM tlocation";                            // Standort-Tabelle laden
   query.exec(qy);
   row = query.size();                                         // 12 Sätze
   locationTable->setRowCount(row);             // TabellenLänge setzen - col ist schon gesetzt
   locationTable->setColumnWidth(0,70);
   locationTable->setColumnWidth(1,80);
   row = 0;
   while(query.next()) {
      col = 0;
      i = 0;
      QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((0)*(0)));      // id
      rowItem->setText(query.value(i++).toString());
      locationTable->setVerticalHeaderItem(row,rowItem);
      QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(col))); //logging
      s = query.value(i++).toString();
      newItem->setText(s);
      locationTable->setItem(row,col++,newItem);
      if(s.count() != 0)
        comboBoxHome->addItem(newItem->text());                          // comboBoxHOME füllen
      newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));       // Standort
      newItem->setText(query.value(i++).toString());
      locationTable->setItem(row,col++,newItem);
      row++;
   }
   s = settings.value("qthId").toString();
   wId->setText(s);                                                      // default: HOME i == 1
   i = s.toInt();
   i--;
   comboBoxHome->setCurrentIndex(i);                                     // index_HOME i = 0;

   lotwUser->setText(settings.value("LotUser").toString());
   lotwPasswd->setText(settings.value("LotPasswd").toString());
   eqslUser->setText(settings.value("EqslUser").toString());
   eqslPasswd->setText(settings.value("EqslPasswd").toString());
   connect(comboBoxHome, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxHomeCb(int)));
   connect(locationTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateLocationItemCb(QTableWidgetItem*)));
   connect(locationTable, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(saveColValCb(QTableWidgetItem*)));
   connect(cButtonHilfe, SIGNAL(clicked()), this, SLOT(getHilfeCb()));
   
   //connect(checkBoxRealtimeUp, SIGNAL(pressed()), this, SLOT(internetBoxChangedCb()));
   connect(ButtonInternet, SIGNAL(clicked()), this, SLOT(internetParamSaveCb()));
}

confDialog::~confDialog()
{
}

// Locator immer GroßBuchstaben
//--------------------------------------------------
void confDialog::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
   }
}

// *********************************************

// Rufzeichen immer Großbuchstaben
// ------------------------------------
void confDialog::wCallCb(QString call)
{
    call = call.toUpper();
    wCall->setText(call);                       
    work++;
}

// Locator immer GroßBuchstaben
// ----------------------------------------------
void confDialog::wLocatorCb(QString loc)
{
    loc = loc.toUpper();
    wLocator->setText(loc);                     
    work++;
}

// text_widget wurden editiert
// ----------------------------------------------
void confDialog::textMoved(const QString &str)
{
   QString st;
   st += str;                                   
   work++;                                      
}

// config für update einstellen ( daten speichern )
// ----------------------------------------------
void confDialog::buttonClicked()
{
   work++;                                      
}

// comboBox_home_id verändert
// ----------------------------------------------
void confDialog::comboBoxHomeCb(int idx)
{
   s.setNum(++idx);
   wId->setText(s);
}
/*

void confDialog::internetBoxChangedCb()
{
     if(checkBoxRealtimeUp->isChecked() == FALSE)
       settings.setValue("EqslRealtime","1");
     else 
       settings.setValue("EqslRealtime","0");
}
*/

void confDialog::internetParamSaveCb()
{
    settings.setValue("LotUser",lotwUser->text());
    settings.setValue("LotPasswd",lotwPasswd->text());
    settings.setValue("EqslUser",eqslUser->text());
    settings.setValue("EqslPasswd",eqslPasswd->text());
    
    if(lotwPasswd->text().count() != 0) {    // check DIR lotw
      QString h = QDir::homePath();
      s = h+"/lotw";
      QFile file(s);
      if(file.exists(s) == false) {         // wenn DIR lotw nicht vorhanden, anlegen  
        QDir dir(s);
        dir.mkpath(s);
        s += "/tmp";
        dir.mkpath(s);
      }
    }
    
    if(eqslPasswd->text().count() != 0) {   // check DIR eQSL
      QString h = QDir::homePath();
      s = h+"/eQsl";
      QFile file(s);
      if(file.exists(s) == false) {         
        QDir dir(s);
        dir.mkpath(s);
      }
    }
}

// sichere Celltext
// ------------------------------------------------------
void confDialog::saveColValCb(QTableWidgetItem * item)
{
     colval = item->text();
}

// Standorte editiert
// *******************************************************************
// Daten in der Standort_Tabelle editieren und in der DB updaten
// SIGNAL ( itemChanged(QTableWidgetItem *));
//-------------------------------------------------------------------
void confDialog::updateLocationItemCb( QTableWidgetItem *item )
{
 QString nam;
        if (item != locationTable->currentItem())    // fremde items nicht bearbeiten     
          return;
        row = locationTable->row( item );             
        col = locationTable->column ( item );         
        if(row == 0) {                                
           item->setText(colval);
           return;
        }
        if(col == 0)                                 // nicht editierbar
          nam = "logging";                           // spalten_name  
        else
          nam = "place";                             // spalten_name
        QSqlQuery query;
        QTableWidgetItem * v = locationTable->verticalHeaderItem ( row );
        qy = "UPDATE tlocation SET "+nam+"='"+item->text()+"' WHERE lid="+v->text();
        query.exec(qy);
        n = comboBoxHome->currentIndex();            // currentItem sichern         
        comboBoxHome->clear();
        QTableWidgetItem * sitem;                    // neues arbeits_item erzeugen     
        row = 0;
        col = 0;
        while(row != 12) {
           sitem = locationTable->item(row,0);
           if( sitem->text().count() == 0)
              break;
           comboBoxHome->addItem(sitem->text());
           row++;
        }
        comboBoxHome->setCurrentIndex(n);
}

// save settings
// ----------------------------------------------
void confDialog::writeSettings()
{
  settings.setValue("Logfile",LogLabel->text());
  settings.setValue("QthId",wId->text());
  settings.setValue("Call",wCall->text());
  settings.setValue("Name",wName->text());
  settings.setValue("Qth",wQth->text());
  settings.setValue("Dxcc",wDxcc->text());
  settings.setValue("Locator",wLocator->text());
  settings.setValue("Utdif",wUtdif->text());
  settings.setValue("Laenge",wLaenge->text());
  settings.setValue("Breite",wBreite->text());
  settings.setValue("Mail",wMail->text());
  settings.setValue("www",wWww->text());
  // Club
  settings.setValue("Club",wClub->text());
  settings.setValue("KennerType",wKennerType->text());
  settings.setValue("Kenner",wKenner->text());
  settings.setValue("ClubAwd",wAwd->text());
  settings.setValue("QslMgr",wQslMgr->text());
  
  QSqlQuery query;                                     // prüfe ob Operator angelegt ist
  n = 0;
  qy="SELECT ocall,logbook,logging FROM toperator WHERE ocall='"+wCall->text()+"' AND logbook='";
  qy += settings.value("Logfile").toString()+"'";
  query.exec(qy);
  i = query.size();
  if( !i ) {                                          // nicht vorhandn
    
    s = "INSERT INTO toperator VALUES ('";
    s += wCall->text();
    s += "',0";
    s += ",'";
    s += "','";
    s += "','"+wName->text();
    s += "','"+wQth->text();
    s += "','"; // street
    s += "','"+wMail->text();
    s += "')";
    query.exec(s);
  }
}

// Dialog_ENDE
// ----------------------
void confDialog::goExit()                
{
  if( work ) {                           
    writeSettings();
  }
  accept();                              
}

// Arbeitslog ändern - neu setzen
// ---------------------------------
void confDialog::changeLogCb(QTreeWidgetItem * item, int col)
{
   i = col;                              
   s = item->text(0);
   LogLabel->setText(s);
   state = 2;
}

// help
// ---------------------------------
void confDialog::getHilfeCb()
{
   settings.setValue("Val","Configuriere");
   StartProcess("hilfedb &");
}

// =================================================================
// create new LOG_table
//=================================================================
void confDialog::makeLogFile()
{
   if(workLog->text().count() == 0) {        // kein Name angegeben
    QMessageBox::information(this,
        tr("Vermisse den Lognamen"),
        tr("Bitte einen Logbookname angegeben"),
        QMessageBox::Ok|QMessageBox::Default,
        QMessageBox::NoButton, QMessageBox::NoButton);
     return;
   }
   
   // table OM's
   QSqlQuery query;
   qy = "DROP TABLE IF EXISTS `"+workLog->text()+"om`";
   query.exec(qy);
   qy = "CREATE TABLE `"+workLog->text()+"om` (";
   qy += "`omid` int(11) NOT NULL auto_increment,";
   qy += "`rufz` varchar(16) NOT NULL default '',";
   qy += "`name` varchar(128) default NULL,";
   qy += "PRIMARY KEY  (`omid`),";
   qy += "UNIQUE KEY `oidx` (`rufz`,`omid`)";
   qy += ")";
   query.exec(qy);
   
   // table fun
   qy = "DROP TABLE IF EXISTS `"+workLog->text()+"`";
   query.exec(qy);
   qy = "CREATE TABLE `"+workLog->text()+"` (";
   qy += "`id` int(11) NOT NULL auto_increment,";
   qy += "`oid` int(11) default NULL,";
   qy += "`cept` varchar(10) default NULL,";
   qy += "`qth` varchar(128) default NULL,";
   qy += "`loc` varchar(10) default NULL,";
   qy += "`band` varchar(10) default NULL,";
   qy += "`freq` varchar(8) default NULL,";
   qy += "`mode` varchar(8) default NULL,";
   qy += "`day` date default NULL,";
   qy += "`btime` time default NULL,";
   qy += "`etime` time default NULL,";
   qy += "`rstr` varchar(8) default NULL,";
   qy += "`rsts` varchar(8) default NULL,";
   qy += "`rxpwr` varchar(5) default NULL,";
   qy += "`qslr` char(2) default NULL,";
   qy += "`qsls` char(2) default NULL,";
   qy += "`qslrd` date default NULL,";
   qy += "`qslsd` date default NULL,";
   qy += "`qslmgr` varchar(18) default NULL,";
   qy += "`rig` varchar(12) default NULL,";
   qy += "`txpwr` varchar(5) default NULL,";
   qy += "`dxcc` varchar(6) default NULL,";
   qy += "`wae` varchar(6) default NULL,";
   qy += "`wpx` varchar(20) default NULL,";
   qy += "`ituz` varchar(4) default NULL,";
   qy += "`cqz` varchar(4) default NULL,";
   qy += "`cn` varchar(4) default NULL,";
   qy += "`qslRvia` varchar(8) default NULL,";
   qy += "`qslSvia` varchar(8) default NULL,";
   qy += "`prflg` varchar(8) default NULL,";
   qy += "`ntext` varchar(255) default NULL,";
   qy += "`ktext` varchar(255) default NULL,";
   qy += "`qslmsg` varchar(255) default NULL,";
   qy += "`qline` varchar(18) default NULL,";
   qy += "`quarter` tinyint(4) default NULL,";
   qy += "`op` int(11) default NULL,";
   qy += "PRIMARY KEY  (`id`),";
   qy += "KEY `qidx` (`oid`)";
   qy += ")";
   query.exec(qy);
   
   // table funawd
   qy = "DROP TABLE IF EXISTS `"+workLog->text()+"awd`";
   query.exec(qy);
   qy = "CREATE TABLE `"+workLog->text()+"awd` (";
   qy += "`aid` int(11) NOT NULL auto_increment,";
   qy += "`qid` int(11) default NULL,";
   qy += "`acall` varchar(16) default NULL,";
   qy += "`awtype` varchar(24) default NULL,";
   qy += "`awkenner` varchar(254) default NULL,";
   qy += "PRIMARY KEY  (`aid`),";
   qy += "KEY `aidx` (`aid`,`qid`)";
   qy += ")";
   query.exec(qy);
   
   // table funqsl
   qy = "DROP TABLE IF EXISTS "+workLog->text()+"qsl";
   query.exec(qy);
   qy = "CREATE TABLE `"+workLog->text()+"qsl` (";  // QSL_Datensatz anlegen
   qy += "`eid` int(11) NOT NULL auto_increment,";  // QSL_id
   qy += "`qsoid` int(11) NOT NULL,";               // QSO_id ( Verbindung zum QSO )
   qy += "`qcall` varchar(16) default NULL,";       // QSO_rufzeichen aus Qso_satz
   qy += "`qsoday` date default NULL,";             // Qso_Datum

   qy += "`contestid` varchar(80) default NULL,";  // 38 CONTEST_ID QSO Contest Identifier
   qy += "`ag` varchar(30) default NULL,";         //    Authenticity Guaranteed
   qy += "`eqslr` varchar(4) default NULL,";       // 39 EQSL_QSL_RCVD status e_qsl: Y, N, R, I, V
   qy += "`eqsls` varchar(4) default NULL,";       // 40 EQSL_QSL_SEND status e_qsl: Y, N, R, Q, I
   qy += "`eqslrd` date default NULL,";            // 41 EQSL_QSLRDATE
   qy += "`eqslsd` date default NULL,";            // 42 EQSL_QSLSDATE
   qy += "`lotqslr` varchar(4) default NULL,";     // 43 LOTW_QSL_RCVD status Logbook_over_the_world: Y, N, R, I, V 
   qy += "`lotqsls` varchar(4) default NULL,";     // 44 LOTW_QSL_SENT status Logbook_over_the_world: Y, N, R, Q, I 
   qy += "`lotqslrd` date default NULL,";          // 45 LOTW_QSLRDATE
   qy += "`lotqslsd` date default NULL,";          // 46 LOTW_QSLSDATE
   qy += "`contactdOp` varchar(12) default NULL,"; // CONTACTED_OP individual operatig callsign to contacted st.
   qy += "`freqrx` varchar(10) default NULL,";     // 50 FREQ_RX the logging station received freq in MHz
   qy += "`srx` varchar(16) default NULL,";        // 51 SRX contest QSO received serial number
   qy += "`srxstr` varchar(16) default NULL,";     // 52 SRX_STRING  contest QSO received serial information
   qy += "`stx` varchar(16) default NULL,";        // 53 TRX  contest QSO transmitted serial number
   qy += "`stxstr` varchar(16) default NULL,";     // 54 TRX_STRING  contest QSO transmitted serial information
   qy += "`qinfo` varchar(12) default NULL,";      // 55 SWL indicates -the QSO information pertains to an SWL repot
   qy += "`comment` varchar(128) default NULL,";   // 56 COMMENT comment field for QSO
   qy += "`custom1` varchar(80) default NULL,";    // 57 user
   qy += "`custom2` varchar(80) default NULL,";    // 58 user
   qy += "`sel` varchar(6) default NULL,";         // 59 besondere Anwendungen
   qy += "PRIMARY KEY  (`qsoid`),";
   qy += "KEY `eidx` (`eid`,`qsoid`))";
   query.exec(qy);
   
   // fundx
   qy = "DROP TABLE IF EXISTS `"+workLog->text()+"dx`";
   query.exec(qy);
   qy = "CREATE TABLE `"+workLog->text()+"dx` (";
   qy += "`dxpref` varchar(5) NOT NULL default '',";
   qy += "`mode` varchar(8) default NULL,";
   qy += "`cntx` smallint(6) default NULL,";       // bis 65.000   
   qy += "`cntb` smallint(6) default NULL,";       // bis 65.000
   qy += "`b160` char(2) default NULL,";
   qy += "`b80` char(2) default NULL,";
   qy += "`b60` char(2) default NULL,";
   qy += "`b40` char(2) default NULL,";
   qy += "`b30` char(2) default NULL,";
   qy += "`b20` char(2) default NULL,";
   qy += "`b17` char(2) default NULL,";
   qy += "`b15` char(2) default NULL,";
   qy += "`b12` char(2) default NULL,";
   qy += "`b10` char(2) default NULL,";
   qy += "`b6` char(2) default NULL,";
   qy += "`b2` char(2) default NULL,";
   qy += "`b070` char(2) default NULL,";
   qy += "`b023` char(2) default NULL,";
   qy += "`b013` char(2) default NULL,";
   qy += "`b009` char(2) default NULL,";
   qy += "`b006` char(2) default NULL,";
   qy += "`b003` char(2) default NULL,";
   qy += "`b0015` char(2) default NULL,";
   qy += "PRIMARY KEY (`dxpref`,`mode`))";
   query.exec(qy);

   // funqslcard
   qy = "DROP TABLE IF EXISTS `"+workLog->text()+"card`";
   query.exec(qy);
   qy = "CREATE TABLE `"+workLog->text()+"card` (";
   qy += "`cid` int(11) NOT NULL auto_increment,";
   qy += "`qsocid` int(11) default NULL,";
   qy += "`qcall` varchar(16) default NULL,";
   qy += "`qsoday` date default NULL,";
   qy += "`cband` varchar(10) default NULL,";
   qy += "`cmode` varchar(8) default NULL,";
   qy += "`cbtime` time default NULL,";
   qy += "`qsl` varchar(4) default NULL,";
   qy += "`strlen` int(11) default NULL,";
   qy += "`qcard` mediumblob default NULL,";
   qy += "PRIMARY KEY (`cid`),";
   qy += "KEY `qslidx` (`cid`,`qsocid`))";
   query.exec(qy);

   // neues log in log_dir eintragen
  i = 0;
  QTreeWidgetItem *item = new QTreeWidgetItem(dblogsList);
  item->setText(i++,workLog->text());    
  item->setText(i++,"0");                
  item->setText(i++,RELEASE);            
  item->setText(i++,"");                 
  qy = "INSERT INTO dblogs VALUES ('"+workLog->text()+"',0,'"+RELEASE+"','')";
  query.exec(qy);
}

// ----------------------------------------------------
// delete LOG 
// -----------------------------------------------------
void confDialog::deleteLogFile()
{
  int x;
  QString s;
     if((workLog->text().count()) == 0) {
      s = tr("Kein Logbook - ")+workLog->text()+tr(" - eingetragen !");
      QMessageBox::information(this,
              tr("Info zum LogFile"),(s),
              QMessageBox::Ok|QMessageBox::Default,
              QMessageBox::NoButton, QMessageBox::NoButton);
       return;
    }
    x = 0;
    n = dblogsList->topLevelItemCount();
    QTreeWidgetItem *Item;
    x = 1;
    i = 0;
    while(n != i ) {
        Item =  dblogsList->topLevelItem (i);
        s = Item->text(0);
        x = s.compare(workLog->text());
        if( x == 0 ) break;                         // gefunden  
        i++;
    }
    if ( x == 0 ) {                                 // selectiertes logbook zeigen
       dblogsList->setCurrentItem(Item,i);        
       s = tr("\nLogbook  ' ")+workLog->text()+tr(" ' loeschen         ");
    }
    else {                                          // nicht gefunden
      s = tr("\nLogbook   ' ")+workLog->text()+tr(" '  nicht gefunden ! ?   ");
    }
    int status = QMessageBox::question( this,
       tr("LogBook loeschen"),s,
       QMessageBox::Yes|QMessageBox::Default,
       QMessageBox::No|QMessageBox::Escape,
       QMessageBox::NoButton);
       if(status != QMessageBox::Yes)
          return;
  dblogsList->takeTopLevelItem (i);                 // delete item

  QSqlQuery query;
  qy = "DROP TABLE IF EXISTS "+workLog->text();      
  query.exec(qy);
  qy = "DROP TABLE IF EXISTS "+workLog->text()+"om";
  query.exec(qy);
  qy = "DROP TABLE IF EXISTS "+workLog->text()+"awd";
  query.exec(qy);
  qy = "DROP TABLE IF EXISTS "+workLog->text()+"qsl";
  query.exec(qy);
  qy = "DROP TABLE IF EXISTS "+workLog->text()+"card";
  query.exec(qy);
  qy = "DROP TABLE IF EXISTS "+workLog->text()+"dx";
  query.exec(qy);
  qy = "DELETE FROM dblogs WHERE logname='"+workLog->text()+"'";
  query.exec(qy);
}
