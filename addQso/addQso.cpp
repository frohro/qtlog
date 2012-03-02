/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.05.2007
********************************************************************** */

// addQso.cpp  01.05.07 / 20.11.08 / 02.07.09 / 10-03-10

#include <QApplication>
#include <QtGui>
#include <QtSql>
#include <QToolButton>
#include <QHashIterator>
#include <QCheckBox>
#include <QDial>
#include <QtNetwork>

#include "addQso.h"
#include "serve.h"
#include "../qtlogDiag/version.h"
//#include "../qtlogDiag/dirmngr.h"
#include "rigctl.h"
#include "uploadR.h"


// ------------------------------------------------------------------
addQso::addQso(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                             
   setFont(font);
   readSettings();
   
   connect(actionENDE, SIGNAL(triggered(bool)), this, SLOT(goExit()));
   
   connect(actionQsoVerwaltung, SIGNAL(triggered(bool)), this, SLOT(qsoVerwaltungAc()));
   connect(actionQslBearbeitung, SIGNAL(triggered(bool)), this, SLOT(qslBearbeitungAc()));
   connect(actionDxSpot, SIGNAL(triggered(bool)), this, SLOT(dxSpotAc()));
   connect(actionRealtimeUpload, SIGNAL(triggered(bool)), this, SLOT(realtimeUpAc()));
   connect(actionLogbook_Sicherung, SIGNAL(triggered(bool)), this, SLOT(dumpLogbookAc()));
    // Ac Remote
   connect(actionRigCtl, SIGNAL(triggered(bool)), this, SLOT(rigctlAc()));
    // Ac Hilfe
   connect(actionHilfeQsoEingabe, SIGNAL(triggered(bool)), this, SLOT(hilfeQsoEingabeAc()));
   connect(actionHilfeRigControl, SIGNAL(triggered(bool)), this, SLOT(hilfeRigCtlAc()));
   connect(actionHilfeDxSpot, SIGNAL(triggered(bool)), this, SLOT(hilfeDxSpotAc()));
 // Ac Hilfe
   connect(ButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(wCall, SIGNAL(textEdited(QString)), this, SLOT(callEdited(QString)));
   connect(wAwd, SIGNAL(textEdited(QString)), this, SLOT(awdCb(QString)));
   connect(wIota, SIGNAL(textEdited(QString)), this, SLOT(iotaCb(QString)));
   connect(wQslMgr, SIGNAL(textEdited(QString)), this, SLOT(qslMgrCb(QString)));
   
   connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
   connect(wLocator, SIGNAL(textEdited(QString)), this, SLOT(locatorCb(QString)));
   connect(wZielLocator, SIGNAL(textEdited(QString)), this, SLOT(locatorZielCb(QString)));
   
   connect(checkBoxUhr, SIGNAL(stateChanged(int)), this, SLOT(uhrStop(int)));
   connect(checkBoxRSTe, SIGNAL(stateChanged(int)), this, SLOT(checkRSTeChanged(int)));
   connect(checkBoxRSTg, SIGNAL(stateChanged(int)), this, SLOT(checkRSTgChanged(int)));

   //connect(checkBoxStop, SIGNAL(pressed()), this, SLOT(checkBoxStopPolling()));
   connect(checkBoxKom, SIGNAL(toggled(bool)), this, SLOT(checkKomChanged()));
   connect(checkBoxEnd, SIGNAL(toggled(bool)), this, SLOT(checkQsoEndeChanged()));
   connect(checkBoxLocator, SIGNAL(toggled(bool)), this, SLOT(checkLocatorChanged()));
 
   connect(comboBoxRSTe, SIGNAL(currentIndexChanged(QString)), this, SLOT (comboBoxRSTeChanged(QString)));
   connect(comboBoxRSTg, SIGNAL(currentIndexChanged(QString)), this, SLOT (comboBoxRSTgChanged(QString)));
   connect(comboBoxMax, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxMaxCb(QString)));

   connect(ButtonHistory, SIGNAL(clicked()), this, SLOT(changeHistoryCb()));
   connect(ButtonReset, SIGNAL(clicked()), this, SLOT(resetAddMask()));
   connect(ButtonAdd, SIGNAL(clicked()), this, SLOT(addNewQso()));
   connect(ButtonBack, SIGNAL(clicked()), this, SLOT(buttonBackCb()));
   connect(ButtonUpdate, SIGNAL(clicked()), this, SLOT(buttonUpdateCb()));
   connect(ButtonDel, SIGNAL(clicked()), this, SLOT(buttonDeleteCb()));
   // QSO
   connect(qsoList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(qsoListClicked(QTreeWidgetItem*,int)));
   connect(wNotizEdit, SIGNAL(returnPressed()), this, SLOT(notizCheckCR()));
   connect(wKomEdit, SIGNAL(returnPressed()), this, SLOT(whileCRgoTab0()));
   connect(wCustom2, SIGNAL(returnPressed()), this, SLOT(whileCRgoTab0()));
   connect(wCustom1, SIGNAL(returnPressed()), this, SLOT(whileCRgoTab0()));
   connect(wEditContestId, SIGNAL(returnPressed()), this, SLOT(whileCRgoTab0()));
   connect(wQslMgr, SIGNAL(returnPressed()), this, SLOT(whileCRgoTab0()));
   connect(wOpCall, SIGNAL(textEdited(QString)), this, SLOT(wOpCallCb(QString)));
   
   logFile = settings.value("logfile").toString();
   wLogFile->setText(logFile);
   
   labelVersion->setText(VERSION);
   s = "[ "+settings.value("dbname").toString()+" ]";
   labelDatabase->setText(s);
// -
   QDate heute = QDate::currentDate();                  
   s = tr(" Login :  ");
   s += t.setNum(heute.day());
   s += "."+t.setNum(heute.month());
   s += "."+t.setNum(heute.year());
   rtime = QDateTime::currentDateTime().toUTC().time(); 
// -
   QDateTime h = QDateTime::currentDateTime().toUTC();
   dateEditStart->setDate(h.date());
   timeEditStart->setTime (h.time());
   s += " - "+h.time().toString();
   wStartLogin->setText(s);                             
   toDay = h.date().toString("yyyy-MM-dd");
   
   udpSocket = new QUdpSocket(this);                     //udpSocket
   udpSocket->bind(QHostAddress::LocalHost,45450);
   connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
// -   
   tcpSocket = new QTcpSocket(this);                     // tspSocket
   connect(tcpSocket, SIGNAL(connected()), this, SLOT(sentCommand()));
   connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readRigReply()));
   connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                                            this, SLOT(displayError(QAbstractSocket::SocketError)));
   //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
   db = QSqlDatabase::addDatabase("QMYSQL");
   db.setHostName(settings.value("host").toString());   
   db.setDatabaseName(settings.value("dbname").toString());
   db.setUserName(settings.value("dbuser").toString());
   db.setPassword(settings.value("dbpasswd").toString());
   if(! db.open()) {
     QSqlError err = db.lastError ();                   
     s = err.databaseText();
     s += tr("\n\nFehlermeldung vom MySQL-Server !\nDie Zugangsberechtigung ist beschaedigt\nBetrieb ist nicht moeglich");
     QMessageBox::information( this,
         tr("Server-Datenbank INFO"),
         tr(s.toAscii()),
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
     exit(0);                                          
   }
   omId = 0;
// -
   if(checkBoxRSTe->checkState())
       wRSTe->setText(comboBoxRSTe->currentText());
   if(checkBoxRSTg->checkState())
      wRSTg->setText(comboBoxRSTg->currentText());

   if(settings.value("Endtime").toString().toInt() == 1)
      checkBoxEnd->setChecked(TRUE);
   checkQsoEndeChanged();
// -
   s = tr("QTH - Locator ");                      // - geog.Koordinaten
   s += settings.value("Locator").toString();
   labelQthLocator->setText(s);
   i = settings.value("gKoordinaten").toInt();
   if( i ) {
      checkBoxLocator->setChecked(true);          // 1 = HOME_Breite/Laenge aus Locator berechnen
      s = settings.value("Locator").toString();
      distanz.setHomeCoordinatenFromLoc(s);
    }
    else {
      checkBoxLocator->setChecked(false);         // 0 = HOME_Breit/Laenge gem. Config
      s = settings.value("Breite").toString();    // Breite + Laenge setzen
      t = settings.value("Laenge").toString();
      distanz.setHomeCoordinaten(s,t);
    }

    QSqlQuery query;                              // Standort_Box fuellen
    HomeBox->clear();
    i = 0;
    qy = "SELECT logging FROM tlocation";
    query.exec(qy);
    while(query.next()) {
       s = query.value(0).toString();
       if(s.count() != 0)
         HomeBox->insertItem (i++,s);
    }
    HidW->setText(settings.value("QthId").toString());
    i = settings.value("QthId").toInt();
    i--;
    HomeBox->setCurrentIndex (i);
    connect(HomeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(HomeBoxCb(int)));
    
    operatorId = 0;
    qy = "SELECT opid FROM toperator WHERE ocall='"+settings.value("Call").toString()+"'";
    query.exec(qy);
    if(query.size()) {
      query.next();
      operatorId = query.value(0).toInt();
    }
// -
    serv.installHashTableBand();                  // Band_uebersetzungs_tabelle füllen
// -
    BandBox->clear();
    QString mBand, mFreq;
    i = 0;                                        // Band_Box fuellen
    qy = "SELECT mband,hfreq FROM wband WHERE work!=0";
    query.exec(qy);
    while(query.next()) {
      mBand = query.value(0).toString();
      BandBox->insertItem(i++,mBand);            // BandBox aufbauen
      mFreq = query.value(1).toString();
      FreqToBand.insert(mFreq,mBand);             // hashBand aufbauen
    }
    
    BandBox->setCurrentIndex(BandBox->findText(settings.value("Band").toString()));
    BandIdx = BandBox->currentIndex();           // store run_Band
    
    qy = "SELECT hfreq FROM wband WHERE mband='"+settings.value("Band").toString()+"'";
    query.exec(qy);
    query.next();
    lastBand = query.value(0).toString();
    connect(BandBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(BandBoxChangedCb(QString)));
// --
    ModeBox->clear();
    i = 0;                                        // Mode_Box fuellen
    qy = "SELECT mode FROM wmode WHERE work!=0";
    query.exec(qy);
    while(query.next()) 
       ModeBox->insertItem (i++, query.value(0).toString());
    ModeBox->setCurrentIndex(ModeBox->findText(settings.value("Mode").toString()));
    ModeIdx = ModeBox->currentIndex();            // store Mode
    connect(ModeBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ModeBoxChangedCb(QString)));
// --
    LineBox->clear();                             // Line_Box fuellen
    i = 0;
    qy = "SELECT line FROM wline ORDER BY id";
    query.exec(qy);
    while(query.next())
       LineBox->insertItem (i++, query.value(0).toString());
    LineBox->setCurrentIndex(settings.value("Line").toInt());
// --
    AwdBox->clear();
    i = 0;
    qy = "SELECT atype FROM wawdlist WHERE aset !='0' ORDER BY id";
    query.exec(qy);
    while(query.next())
       AwdBox->insertItem (i++, query.value(0).toString());      // ++ hash_tabelle aufbauen
    AwdBox->setCurrentIndex(settings.value("Awd").toInt());      // Awd_box current
    qsoList->headerItem()->setText(10,AwdBox->currentText());    // AWD_SpaltenNamen im Header setzen
    connect (AwdBox, SIGNAL(activated(int)), this, SLOT(awdBoxChanged(int)));
// --
    qy = "SELECT watt FROM wpwr WHERE work!=0 ORDER BY work";    // pwr
    PwrBox->clear();
    i = 0;                                                       // PWR_Box fuellen
    query.exec(qy);
    while(query.next())
       PwrBox->insertItem (i++, query.value(0).toString());
    PwrBox->setCurrentIndex(settings.value("Pwr").toInt());      // PWR_box current

    qy = "SELECT rigtype FROM wrig";                // RIG
    RigBox->clear();
    i = 0;                                          // RIG_Box fuellen
    query.exec(qy);
    while(query.next()) 
       RigBox->insertItem (i++, query.value(0).toString());
    s =  BandBox->currentText();
    qy = "SELECT brig FROM wband WHERE mband='"+s+"'";   // hole RigType - Band wenn angegeben
    query.exec(qy);
    query.next();
    t = query.value(0).toString();
    if(t.count() != 0)                                  // setze RigBox
        RigBox->setCurrentIndex(RigBox->findText(t) );
    else
     RigBox->setCurrentIndex(settings.value("Rig").toString().toInt());  // RIG_box current
// --
    wCallEdit->setText(settings.value("Call").toString());               // Sations_rufzeichen
    comboBoxRSTe->setCurrentIndex(settings.value("RSTe").toInt());       // RSTe
    comboBoxRSTg->setCurrentIndex(settings.value("RSTg").toInt());   
    comboBoxSuche->setCurrentIndex(settings.value("Suche").toInt());     // Suche set default
    lineEditOM->setText(settings.value("Salutation").toString());
    
    qy = "SELECT refnam FROM refnamen WHERE dbfield LIKE 'custom%'";     // ist custom1 configuriert ?
    query.exec(qy);
    query.next();
    s = query.value(0).toString();        // custom1
    if(s.count() != 0)
      label_custo1->setText(s);
   
    query.next();
    s = query.value(0).toString();        // custom2
    if(s.count() != 0)
       label_custo2->setText(s);
    ModeToMod.insert("LSB","SSB");
    ModeToMod.insert("USB","SSB");
    ModeToMod.insert("SSB","LSB");
    ModeToMod.insert("CW","CW");
    ModeToMod.insert("RTTY","RTTY");
    ModeToMod.insert("PKTUSB","PSK31");
    
    wLineEdit.insert("Name",wName);      
    wLineEdit.insert("Qth",wQth);
    wLineEdit.insert("Kommentar",wKomEdit);
    wLineEdit.insert("Notiz",wNotizEdit);
    wLineEdit.insert("QslMgr",wQslMgr);
    
    retry = 0;                            // hamlib - rig_ctl values laden
    rigpolling = 0;                       // nein
    runing = 0;                           // nein
    ctl = 0;                              // status
    slotFlg = 1;
    qy = "SELECT owner,host,port,retry,runing FROM wproz WHERE proz='HamLib'";
    query.exec(qy);
    if(query.next()) {
      owner = query.value(0).toString();        // hamlib
      tcphost = query.value(1).toString();      // host
      tcpport = query.value(2).toString();      // port
      retry = query.value(3).toInt();           // retry
      ctl = query.value(4).toString().toInt();  // hamlib wird gestartet
      runing = ctl;                             // status
    }
    
    qsoList->setColumnWidth(0,110);  // Datum  80 alle Spaltenbreiten ueberarbeiten
    qsoList->setColumnWidth(1,70);   // Start
    qsoList->setColumnWidth(2,110);  // Station 90
    qsoList->setColumnWidth(3,100);  // Name 70
    qsoList->setColumnWidth(4,120);  // Qth 90
    qsoList->setColumnWidth(5,50);   // Band
    qsoList->setColumnWidth(6,50);   // Mode
    qsoList->setColumnWidth(7,50);   // RSTe 40
    qsoList->setColumnWidth(8,46);   // QSLe
    qsoList->setColumnWidth(9,50);   // qslb gesendet 60
    qsoList->setColumnWidth(10,60);  // AWD 50
    qsoList->setColumnWidth(11,70);  // Locator 60
    
    uscountyList->setColumnWidth(0,80);
    uscountyList->setColumnWidth(1,200);
    
    upMgr = 0;                      // realtime_upload_Mgr
    qslflg = 1;                     // wird mit F3 verändert
    rmess = 1;                      // gibt Realtime Upload 1 x message aus
    s = comboBoxMax->currentText();
    s = s.mid(5);
    Last = s.trimmed();
    id = 0;
    tflg = 0;                       
    hisflg = 0;                     // die Letzten max 100 QSO's zeigen
    uaflg = 0;                      // RDA_Awd_flg
    editflg = 0;                    // wird bei Rufzeichen-Aenderung = 1 gesetzt
    updateflg = 0;                  // wird durch qsoListClicked 1 gesetzt
    flgf6 = 0;                      // anfrage qrz.com
    omId = 0;                       // Id Om_Datensatz
    Rufz = "";                      // Rufzeiche_str leer
    
    tabWidget ->setCurrentIndex(0);
    tflg = tabWidget ->currentIndex();
    
    last = tr("(F5) Historie");
    qso  = tr("(F5) Eingabe");
    ButtonHistory->setText(last);
    
    wCall->setFocus();
    ButtonUpdate->hide();
    ButtonBack->hide();
    ButtonDel->hide();
    
    getHistoryLast();                         // die letzten QSOs zeigen
    dial->setSliderPosition(180);
    
    setQslTabel();                            // setze die qslTabelle
    setAwdOmList();
    setQslDefault();                          // qsl defaults setzen
    
    connect(checkBoxEqsl, SIGNAL(clicked()), this, SLOT(qslviaChanged()));
    connect(checkBoxBuero, SIGNAL(clicked()), this, SLOT(qslviaChanged()));
    
    timer = new QTimer(this);                 // UHR_Timer
    connect(timer,SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    showTime();                               // Uhr laeuft mit UTC_zeit
 
    QTimer::singleShot(100, this, SLOT(timerService()));      // set QSL defaults
    
    dxtimer = new QTimer(this);                               // DXCluster ( dxCall übernehmen )
    connect(dxtimer, SIGNAL(timeout()), this, SLOT(putDxCall()));
    ptimer = new QTimer(this);                                // DXCluster ( dxCall übernehmen )
    connect(ptimer, SIGNAL(timeout()), this, SLOT(requestRigNew()));
    QTimer::singleShot(10, this, SLOT(starthamlibServer()));  // starte Server + getmgrdb
   
}  
// -------------
addQso::~addQso()
{
   writeSettings();
   processWriteDatagram(-1,"-1",45458);      // Eqsl
   processWriteDatagram(-1,"",45460);        // getMgr
}

// ================================================================================
// Cursor_Steuerung durch F_Keys
//--------------------------------------------------------------
void addQso::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F1 : {                               // Start_ZEIT setzen
                     if(updateflg)
                       break;                          // Zeit nicht verändern
                     startQso();                       // QSO Start-Zeit:  setzen ---
                  }
            break;
     case Qt::Key_F2 : {
                     if(tabWidget->currentIndex() == 6) { 
                        getQrzComData();               // Daten von QRZ.COM übernehmen
                        setColor(0);
                     }
                     else {
                       if(updateflg)
                         buttonUpdateCb();            // wenn updateflg = 1 UPDATE durchführen
                       else 
                        addNewQso();                  // wenn updateflg = 1 UPDATE durchführen
                     }
                     tabWidget ->setCurrentIndex(0);   // set QSO_page
                  }
            break;
     case Qt::Key_F3 :                                // toggle QSLs: via Buero / eQSL / keine QSL
                 toggelQslsent();
            break;
     case Qt::Key_F4 : {                              // toggeln QSO <-> QSL,Notizen,Kom
                     if( tflg ) {
                       tabWidget ->setCurrentIndex(0);// Karte QSO_liste
                       wCall->setFocus();
                       tflg = 0;
                     }
                     else {
                      tabWidget ->setCurrentIndex(1); // Karte QSL_Notizen_Kom.
                      wNotizEdit->setFocus();
                      tflg = 1;
                     }
                     setQslDefault();
                  }
            break;
     case Qt::Key_F5 : {                              // History toggeln
                     tabWidget->setCurrentIndex(0);
                     qsoList->clear();
                     if( hisflg ) {
                       ButtonHistory->setText(last);  // Historie
                       getHistoryLast();
                       hisflg = 0;
                     }
                     else {
                      ButtonHistory->setText(qso);     // Qso Eingabe
                      getHistoryPref();
                      hisflg = 1;
                    }
                  }
            break;
     case Qt::Key_F6 : {                                    // Rufzeichen bei QRZ.com abfragen
                     if(comboBoxSuche->currentIndex() == 0) {
		       if( !flgf6 ) {                       // toggel F6; falls 0, dann Abfrage
		          if(wCall->text().count() != 0) {  // kein leeres Rufzeichen_feld abfragen
			    flgf6 = 1;                      // Abfrage läuft jetzt
			    processWriteDatagram(1, wCall->text(), 45460);
			    setColor(1);                    // rot setzen, Abfrage aktiv
			  }
		       }
		       else {
			 flgf6 = 0;                         // sonst, Ergebnis löschen
			 setColor(0);
			 clearQrzData(); 
			 tabWidget->setCurrentIndex(0);
		       }
		     }
                     else
                     if(comboBoxSuche->currentIndex() == 1) // QRZ_CD_Datenbank
                       getQrzDbCall(wCall->text());         // wenn von CD geladen
                  }
            break;
     case Qt::Key_F7 : {                                    // toggel AWD status
                     if(tflg) {
                       tabWidget->setCurrentIndex(0);      // toggel zurück
                       tflg = 0;
                       wCall->setFocus(); 
                     }
                     else {
                      tabWidget->setCurrentIndex(2);        // toggel vor
                      tflg = 2;
                      wAwd->setFocus(); 
                      getAdwKennerList();
                     }
                  }
            break;
     case Qt::Key_F8 : {                                   // toggel QSL status
                     if(tflg) {
                       tabWidget->setCurrentIndex(0);      // zurück
                       tflg = 0;
                     }
                     else {
                      tabWidget->setCurrentIndex(3);       // vor
                       tflg = 3;
                       getQslStatus();
                     }
                  }
            break;
     case Qt::Key_F9 : {                                   // reset _Add_Mask
                     buttonBackCb();
                     resetAddMask();                       // eingabe_maske loeschen
                     tabWidget ->setCurrentIndex(0);
                  }
            break;
     case Qt::Key_F10 :
                      goExit();
            break;
     case Qt::Key_Escape : {                              // zurueck nach Page_0 -Last Qsos
                         tabWidget ->setCurrentIndex(0);  // und alle Eingabe_daten löschen
                         tflg = 0;
                         buttonBackCb();
                         wCall->setFocus();               // Cursor auf Station setzen
                         if(flgf6) {
                           setColor(0);                   // falls Abfrage läuft - rot_aktiv
                           clearQrzData();                // QRZ_daten löschen
                           flgf6 = 0;                     // frei für nächste Abfrage
                         }
                  }
            break;
   }
}

// ------------------------------------------------------------------------
void addQso::goExit()                                     
{
   db.close();
   qApp->quit();
}

// QSO Verwaltung
// --------------------------
void addQso::qsoVerwaltungAc()
{
    i = system("qtlog &");
}

// Aufruf QSL Bearbeitung
// ---------------------------------
void addQso::qslBearbeitungAc()
{
    i = system("qsldb &");
}
// ----------------------------
void addQso::dxSpotAc()
{
    i = system("dxspotdb &");
}
// ---------------------------
void addQso::dumpLogbookAc()
{
    i = system("admindb &");
}

// HILFE QsoEingabe
// --------------------------------------------------------
void addQso::hilfeQsoEingabeAc()
{
    settings.setValue("Val","Qso Eingabe");
    i = system("hilfedb &");
}

// Hilfe RigCtl
// --------------------------------------------------------
void addQso::hilfeRigCtlAc()
{
    settings.setValue("Val","Rig Control-Hamlib");
    i = system("hilfedb &");
}

// Hilfe DxSpot
// --------------------------------------------------------
void addQso::hilfeDxSpotAc()
{
    settings.setValue("Val","Dx Spot");
    i = system("hilfedb &");
}

// UHR ticker - Aufruf Sekunden_takt
// --------------------------------------------------------
void addQso::showTime()                     // die Uhr wird eine Sek
{                                           // weitergestellt
  rtime = QDateTime::currentDateTime().toUTC().time();
  timeEditEnd->setTime(rtime);
  if(rtime.hour() == 24) {                  // wenn runtime 24 Uhr UTC
     QDateTime h = QDateTime::currentDateTime().toUTC();
     dateEditStart->setDate(h.date());      // Datum stellen
  }
}

// UHR - STOP    + checkBoxStopPolling()
// --------------------------------------------------------
void addQso::uhrStop(int state)
{
  if( state ) {                                  // == 2 uhr anhalten
    timer->stop();                                        // UHR halt
    checkBoxUhr->setPalette(QPalette(QColor(255,90,14))); // rot
    checkBoxUhr->setText(tr("Uhr..steht"));
    ModeBox->setEnabled(true);                            // frei geben
    rigpolling = 0;                                       // stop               ku
    checkBoxStop->setPalette(QPalette(QColor(232,58,5))); // weil stop dann rot ku
    //checkBoxStop->setChecked(TRUE);                     // Haken setztn       ku
    checkBoxStop->setText("Rctl off");
   }
  else {
    timer->start(1000);                                   // UHR wieder einschalten
    checkBoxUhr->setPalette(QPalette(QColor(224,224,224)));
    checkBoxUhr->setText(tr("Uhr..aktiv"));
    QDate heute = QDate::currentDate();                   // erzeuge das Datum von heute
    dateEditStart->setDate(heute);                         // ausgeben
    
    ModeBox->setEnabled(false);                                      // sperren
    rigpolling = runing;                                             // freigeben  
    
    slotFlg = 1;                                                     // evtl wurde das Band verändert
    BandBox->setCurrentIndex(BandBox->findText(getBand(lastBand)));  // auf lastBand setztn       ku
    
    checkBoxStop->setPalette(QPalette(QColor(224,224,224)));         // grau - freigebeb          ku
    checkBoxStop->setText("Rctl on");
  }
}

// --------------------------------
void addQso::timerService()
{
    setQslDefault();                            // setze default qslflg
    startUploadMgrR(1);                         // starte uploadMgrR
}

// Settings
// ---------------------------------------------------------
void addQso::writeSettings()
{
    settings.setValue("Suche",comboBoxSuche->currentIndex());
    settings.setValue("RSTe",comboBoxRSTe->currentIndex());
    settings.setValue("RSTg",comboBoxRSTg->currentIndex());
    settings.setValue("AWD",AwdBox->currentIndex());
    settings.setValue("Mode",ModeBox->currentText());
    settings.setValue("Band",BandBox->currentText());
    settings.setValue("Salutation",lineEditOM->text());
    if(settings.value("EqslRealtime").toString().toInt())
      settings.setValue("EqslRealtime","1");
    settings.setValue("Line",LineBox->currentIndex());
    settings.setValue("Max",comboBoxMax->currentIndex());
    settings.setValue("Rig",RigBox->currentIndex());
    settings.setValue("Pwr",PwrBox->currentIndex());
    settings.setValue("addQso/Size",size());
    settings.setValue("addQso/Properties",saveState());
}

// Settings
// ---------------------------------------------------------
void addQso::readSettings()
{
    resize(settings.value("addQso/Size",sizeHint()).toSize());
    restoreState(settings.value("addQso/Properties").toByteArray());
    comboBoxMax->setCurrentIndex(settings.value("Max").toString().toInt());
}

// TAB_Widget veraendert
// ---------------------------------------------------------
void addQso::tabChanged(int index)         // Tab_Widget Cursor_Steuerung durch F_Keys
{
     tflg = index;                         // Dummy
     if( tflg == 0 ) {
        tabWidget ->setCurrentIndex(0);
        wCall->setFocus();
        return;
     }
     if( tflg == 1 ) {                      // F4 - QSL
         wNotizEdit->setFocus();
        return;
     }
     if( tflg == 2 ) {                     // F7 - Kenner_status
        wAwd->setFocus(); 
        getAdwKennerList();
        wEditContestId->setFocus();
     }
     if( tflg == 3 ) {                     // F8 - QSL_status
        getQslStatus();
        wZielLocator->setFocus();
     }
}// -------------------------------
void addQso::clearQrzData()
{
     lineEditQrzCall->setText("");
     lineEditQrz_1->setText("");
     lineEditQrz_2->setText("");
     lineEditQrz_3->setText("");
     lineEditQrz_4->setText("");
     labelError->setText("");
}

// --------------------------------
void addQso::setColor(int n)
{
     QPalette palette5;
     if( n ) {                               // setzte ROT - frage QRZ.com
        QBrush brush7(QColor(255,0,0,255));
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        labelSuche->setPalette(palette5);
        labelSuche->setText("QRZ.com");
     }
     else {                                 // SCHWARZ
        QBrush brush7(QColor(48,48,48,255));
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        labelSuche->setPalette(palette5);
        labelSuche->setText("(F6) Suche");
     }
}

// F2 gedrückt -  Daten von QRZ.Com übernehmen
// ---------------------------------------------
void addQso::getQrzComData()
{                                                                          // QRZ.com Data_set
       QLineEdit * wLine = getEditLine(qBox_1->currentText().trimmed());   // name
       wLine->setText(lineEditQrz_1->text());
       
       if(qBox_2->currentText().trimmed().compare("-") != 0) {             // street
          wLine = getEditLine(qBox_2->currentText().trimmed());
          wLine->setText(lineEditQrz_2->text());
       }
       
       wLine = getEditLine(qBox_3->currentText().trimmed());               // qth
       wLine->setText(lineEditQrz_3->text());
       if(qBox_4->currentText().trimmed().compare("-") != 0) {             // state     
          wLine = getEditLine(qBox_4->currentText().trimmed());
          wLine->setText(lineEditQrz_4->text());
       }
}

// BoxMax verändert
// --------------------------------------
void addQso::comboBoxMaxCb(QString str)
{
    s = str;
    s = s.mid(5);
    Last = s.trimmed();
    if( ! hisflg )
      getHistoryLast();                               // Last_QSO's anzeigen
}

// Band_Box verändert
// ----------------------------------------
void addQso::BandBoxChangedCb(QString name)          // z.B name = '80 m'
{
 QString f, b;
 
   if(slotFlg) {
     QSqlQuery query;
     qy = "SELECT freq,brig FROM wband WHERE mband='"+name+"'";
     query.exec(qy);
     query.next();
     f = query.value(0).toString();                  // default Frequenz
     b = query.value(1).toString();                  // band -> rig_type
     
     if(b.count() != 0)
       RigBox->setCurrentIndex(RigBox->findText(b)); // setze RigBox RIG
       if(rigpolling) {                              // ku 22.08.10
         f += "000";
         if(f.toInt() / 1000000 < 10 )
            cmdStr = "F "+f+" M LSB 2400";
         else 
           cmdStr = "F "+f+"M USB 2400";
         cmdFlg = 2;
       }
       wFreq->setText(f);                             // zeige Ergebnis
   }
   slotFlg = 1;
}

// ---------------------------------------
void addQso::ModeBoxChangedCb(QString name)
{
  QString n = name;
}

// QTH_Box_id verändert
// -------------------------------------
void addQso::HomeBoxCb(int id)
{
   id++;
   s.setNum(id);
   HidW->setText(s);
   wCall->setFocus();
}

// Box_Suche verändert
// -------------------------------------
void addQso::comboBoxSucheCb(int idx)
{
     switch(idx) {
        case 1 : //getQrzDbCall(Rufz);         // in QRZ.db suchen
                 break;
       default : break;                        // ist QSO_historie
    }
}

// --- checkBox RSTe: -----------------------------------------
void addQso::checkRSTeChanged(int state)
{
   if(state) {                                  // RSTe eintragen
     wRSTe->setText(comboBoxRSTe->currentText());
     checkBoxRSTe->setText(tr("eintragen"));
   }
   else {
    wRSTe->setText("");
    checkBoxRSTe->setText("....");
   }
}

// --- checkBox RSTg: -----------------------------------------
void addQso::checkRSTgChanged(int state)
{
   if(state) {                                  // RSTe eintragen
     wRSTg->setText(comboBoxRSTg->currentText());
     checkBoxRSTg->setText("eintragen");
   }
   else {
    wRSTg->setText("");
    checkBoxRSTg->setText("....");
   }
}

// --- comboBox RSTe:  erhalten -----------------------------
void addQso::comboBoxRSTeChanged(QString s)
{
   if(checkBoxRSTe->isChecked() != false)
      wRSTe->setText(s);
}

// --- comboBox RSTg:  gegeben ------------------------------
void addQso::comboBoxRSTgChanged(QString s)
{
   if(checkBoxRSTg->isChecked() != false)
      wRSTg->setText(s);
}

// -----------------------------------------------------------
// --- checkBox_QSO_ende:
void addQso::checkQsoEndeChanged()
{
   if(checkBoxEnd->isChecked() == true) {    // Haken gesetzt
     checkBoxEnd->setPalette(QPalette(QColor(158,221,157)));
     labelEnd->setText("F2 - Start");
     labelStart->setText("");
     checkBoxEnd->setText(tr(" und  Ende  ->"));
     settings.setValue("endTime","1");
   }
   else {
     labelEnd->setText("Ende");
     labelStart->setText("( F1 ) Start");
     checkBoxEnd->setText(tr("      Uhrzeit ->"));
     checkBoxEnd->setPalette(QPalette(QColor(224,224,224)));
     settings.setValue("EndTime","0");
   }
}



// ---------------------------------------------------------
// F3 - QSL_default Einstellung für ein einzelnes QSO ändern 
// ( wird von save und update wieder auf default gestellt )
// ---------------------------------------------------------
void addQso::toggelQslsent()
{ 
     switch( qslflg ) {                                     // qslflg
      case 0 : break;
      case 1 :                                              // wenn  1 = Büro
	       qslflg = 2;                                  // jetzt 2 = --
               checkBoxBuero->setChecked(FALSE);            // 
               checkBoxEqsl->setChecked(FALSE);             // 
               labelQslVia->setText(QString::fromUtf8("(F3) QSL via --")); 
	       upMgr = 0;
               break;
      case 2 :                                              // wenn  2 = --
	       qslflg = 3;                                  // jetzt 3 = BE
               checkBoxBuero->setChecked(TRUE);             // 
               checkBoxEqsl->setChecked(TRUE);              // 
               labelQslVia->setText(QString::fromUtf8("(F3) QSL via BE")); 
	       upMgr = settings.value("EqslRealtime").toString().toInt(); 
               break;
      case 3 :                                              // wenn  3 = BE
	       qslflg = 4;                                  // jetzt 4 = E
               checkBoxBuero->setChecked(FALSE);            //
               checkBoxEqsl->setChecked(TRUE);              //
               labelQslVia->setText(QString::fromUtf8("(F3) QSL via E"));
	       upMgr = settings.value("EqslRealtime").toString().toInt(); 
               break;
      case 4 :                                              // wenn  4 = E
	       qslflg = 1;                                  // jetzt 1 = B
               checkBoxBuero->setChecked(TRUE);             //
               checkBoxEqsl->setChecked(FALSE);             //
               labelQslVia->setText(QString::fromUtf8("(F3) QSL via Büro")); 
	       upMgr = 0;
               break;
     }
}


// Qsl via wurde durch F3 verändert
// stelle QSL_flgs wieder auf default zurück ( erfolgt von: start(über timer), oder clear (ESC)
// -------------------------------------------------------------------------------------------
void addQso::setQslDefault()
{ 
   if(settings.value("QSLvia").toInt() == 1) {                        // B
     qslflg = 1;
     checkBoxBuero->setChecked(TRUE);
     checkBoxEqsl->setChecked(FALSE);
     labelQslVia->setText(QString::fromUtf8("(F3) Qsl Via Büro"));
     upMgr = 0;
   }
   else
     if(settings.value("QSLvia").toInt() == 2) {                      // -
       qslflg = 2;
       checkBoxBuero->setChecked(FALSE);
       checkBoxEqsl->setChecked(FALSE);
       labelQslVia->setText(QString::fromUtf8("(F3) Qsl Via --"));
       upMgr = 0;
     }
   else
     if(settings.value("QSLvia").toInt() == 3) {                      // B+E
       qslflg = 3;
       checkBoxBuero->setChecked(TRUE);
       checkBoxEqsl->setChecked(TRUE);
       labelQslVia->setText(QString::fromUtf8("(F3) Qsl Via B+E"));
       upMgr = settings.value("EqslRealtime").toString().toInt();     
     }
   else
     if(settings.value("QSLvia").toInt() == 4) {                      // E
       qslflg = 4;
       checkBoxBuero->setChecked(FALSE);
       checkBoxEqsl->setChecked(TRUE);
       labelQslVia->setText(QString::fromUtf8("(F3) Qsl via eQsl"));
       upMgr = settings.value("EqslRealtime").toString().toInt();     
     }
}
   

// SIGNAL clicked  
// schreibe neuen default QSLvia_status in settings.value("QSLvia")
// -------------------------------------------------------------------
void addQso::qslviaChanged()
{
  if((checkBoxBuero->isChecked() == TRUE) && (checkBoxEqsl->isChecked() == TRUE)) {    // 3 = B+E
    settings.setValue("QSLvia",3);
  }
  else
    if((checkBoxBuero->isChecked() == FALSE) && (checkBoxEqsl->isChecked() ==FALSE)) { // = --
      settings.setValue("QSLvia",2);
    }
  else
    if((checkBoxBuero->isChecked() == FALSE) && (checkBoxEqsl->isChecked() == TRUE)) {  // 4 = Eqsl
      settings.setValue("QSLvia",4);
    }
  else
    if((checkBoxBuero->isChecked() == TRUE) && (checkBoxEqsl->isChecked() == FALSE)) {  // 1 = B
      settings.setValue("QSLvia",1);
    }
}
/*

void addQso::checkBoxStopPolling()
{
   if(checkBoxStop->isChecked() == FALSE) { 
     if(retry == 0) {
       checkBoxStop->setChecked(true); //return;
     }
     rigpolling = 0;
     checkBoxStop->setPalette(QPalette(QColor(232,58,5)));     
     checkBoxStop->setText("stoped");
     ModeBox->setEnabled(true);
   }    
   else {
     rigpolling = runing;
     checkBoxStop->setText("set stop");
     checkBoxStop->setPalette(QPalette(QColor(224,224,224)));  
     ModeBox->setEnabled(false);
   }
}
*/

// --- Kommentar eintragen / global immer eintragen
void addQso::checkKomChanged()
{
   if(checkBoxKom->isChecked() == false)               
    checkBoxKom->setText(tr("Nur zu diesem QSO"));
   else
     checkBoxKom->setText(tr("Globaler Kommentar"));   
}

// --- QTH koordinaten aus Locator berechnen oder aus Config uebernehmen --
void addQso::checkLocatorChanged()
{
 QString s;
   if(checkBoxLocator->isChecked() == true) {
     checkBoxLocator->setText(tr("Aus Locator berechnen"));
     s = "1";
   }
   else {
    checkBoxLocator->setText(tr("Breite/Laenge aus Config"));
    s = "0";
   }
   settings.setValue(tr("gKoordinaten"),s);
}

// go Tab 0
// ---------------------------------------
void addQso::whileCRgoTab0()
{
   tabWidget->setCurrentIndex(0);
}

// notiz returnpressed, remove cr, go page 0
// -----------------------------------------
void addQso::notizCheckCR()
{
    wNotizEdit->text() =  wNotizEdit->text().simplified();
    if(updateflg)
       buttonUpdateCb();
    else
    tabWidget->setCurrentIndex(0);
}

// operator_call immer groß
// -------------------------------------
void addQso::wOpCallCb(QString str)
{
   s = str.toUpper();
   wOpCall->setText(s);
}



// ----------------------------------------------------------
// --- AWD_Editline:  Kenner ( DOK oder andere ) eingegeben
//                    SIGNAL( textEdited( QString ));
// ----------------------------------------------------------
void addQso::awdCb(QString si)
{
  int x, e;
  QString a;
     t = si.toUpper();                            // immer groß
     wAwd->setText(t);
     s = AwdBox->currentText();                   // AWD_name holen
     if(s.compare("US-CA") == 0) {                // falls USA-country -gibt es unterstuetzung
       if(t.count() == 2)                         // falls state eingegeben
          awdhash.insert(s,t);                    // state eintragen
       else {
         e = t.indexOf('-',0);                    // suche flush
         if(e == 2) {                             // '-' gefunden
           tabWidget ->setCurrentIndex(2);
           wAwd->setFocus();
            a = t.right(t.count() -e-1);
            if(a.count() >= 0) {                  // hole zum State passende counties
              qy = "SELECT state,county FROM uscounty WHERE state='"+si.left(e)+"' AND county LIKE '"+a+"%'";
              QSqlQuery query;
              query.exec(qy);
              i = query.size();                                    // id > 0 Prefix gefunden
              uscountyList->clear();
              while(query.next()) {
                n = 0;
                x = 0;
                QTreeWidgetItem *item = new QTreeWidgetItem(uscountyList);
                item->setText(x++,query.value(n++).toString());    // state
                f = query.value(n).toString();                     // county
                item->setText(x++,f);                              // county
                if( i == 1 ) {                                     // county ist jetzt gueltig
                   awdhash.insert(s,si.left(e+1)+f);               // state updaten
                   QPalette palette8;                              // Widget auf grüen stellen
                   QBrush brush10(QColor(212, 255, 211, 255));
                   brush10.setStyle(Qt::SolidPattern);
                   palette8.setBrush(QPalette::Active, QPalette::Base, brush10);
                   palette8.setBrush(QPalette::Inactive, QPalette::Base, brush10);
                   QBrush brush11(QColor(244, 244, 244, 255));
                   brush11.setStyle(Qt::SolidPattern);
                   palette8.setBrush(QPalette::Disabled, QPalette::Base, brush11);
                   wAwd->setPalette(palette8);
                }
                else {                                              // mehr als 1 county gefunden => ungültig
                 QPalette palette11;                                // setze Farbe weiß
                 QBrush brush11(QColor(244, 244, 244, 255));
                 brush11.setStyle(Qt::SolidPattern);
                 palette11.setBrush(QPalette::Disabled, QPalette::Base, brush11);
                 wAwd->setPalette(palette11);
                }
              }
            }
         }
       }
     }
     else {
      awdhash.insert(s,t);            // falls key schon vorhanden; update value 
      if(t.count() >= 3) {            // check ob schon gearbeitet
        x = 0;
        e = 0;
        QSqlQuery query;
        qy = "SELECT qslr FROM "+logFile+","+logFile+"awd WHERE id=qid ";
        qy += "AND awtype='"+AwdBox->currentText();
        qy += "' AND awkenner='"+t+"'";
        query.exec(qy);
        i = query.size();              // id > 0 Prefix gefunden
        if( i ) {                      // wenn id = 1 kenner vorhanden
          while(query.next()) {
            s = query.value(0).toString();
            if(s.compare("R") == 0)
              x++;
            if(s.compare("Y") == 0)
              e++;
          }
          a = t+tr("  gearbeitet ")+s.setNum(i);
          a += tr("  -  Nicht bestaetigt:  ")+s.setNum(x);
          a += tr("      Bestaetigt:  ")+s.setNum(e)+"   ";
          labelAwdInfo->setText(a);
        }
      }
      else {
        labelAwdInfo->setText("");
        uscountyList->clear();
      }
    }
}

// --- AWD_BOX changed:   AWD neu_gestellt
// -----------------------------------------------------------
void addQso::awdBoxChanged(int n)
{
    n = 0;
    uscountyList->clear();                                     // evtl. inhalt ist jetzt ungültig
    t = awdhash.value(AwdBox->currentText());                  // neuer AWD_type gesetzt
    wAwd->setText(t);                                          // Kenner anzeigen wenn vorhanden
    qsoList->headerItem()->setText(10,AwdBox->currentText());  // neuen SpaltenNamen im Header setzen
    getHistoryLast();                                         
}

//--- IOTA_Box:  Eingabe ( AWD ) -  SIGNAL(textEdited(QString));
// ------------------------------------------------------------
void addQso::iotaCb(QString s)
{
    t = s.toUpper();
    wIota->setText(t);
    s = IotaBox->currentText();                  // IOTA_Land holen
    s += t;                                      // LandNumer anhängen
    awdhash.insert("IOTA",s);                    // IOTA_value eitragen
}

// QslMgr eintragen
// -----------------------------------------------------------
void addQso::qslMgrCb(QString s)
{
    t = s.toUpper();
    wQslMgr->setText(t);
}

// --- Locator_EditLine:  Locator eingetragen -> Entferung berechnen
// -----------------------------------------------------------------
void addQso::locatorCb(QString s)
{
    t = s.toUpper();
    wLocator->setText(t);
    if(t.count() == 6) {     // wenn str_len == 6 : Locator gültig - Entfernung berechen
      labelDistanz->setText(distanz.calculateToLL(t));  
    }
    else
      labelDistanz->setText("");                          // FormatFehler
}

// Ziel-Locator:  Entfernung und Richtung berechnen
// ---------------------------------------------------------
void addQso::locatorZielCb(QString l)
{
    t = l.toUpper();
    wZielLocator->setText(t);
    if(t.count() == 6) {                              // wenn str_len == 6  - Entfernung berechen
      wZielEntfRichtung->setText(distanz.calculateToLL(t));
      dial->setSliderPosition(distanz.getRichtung()); // Kompas_Nadel setzen
    }
    else
     wZielEntfRichtung->setText("");
}

// --- QSO Start-Zeit:  setzen ---
// ----------------------------------------------------------
void addQso::startQso()                                    // Start_Zeit setzen
{
    rtime = QDateTime::currentDateTime().toUTC().time();   // erzeuge UT_zeit
    timeEditStart->setTime(rtime);                         // setze neue Start_zeit
}

// --- Alle Eingabe_Felder:  löschen - Cursor positionieren ---
//------------------------------------------------------------
void addQso::resetAddMask()
{
    omId = 0;
    wCall->setText("");
    wName->setText("");
    wQth->setText("");
    wOpCall->setText("");
    wIota->setText("000");
    wAwd->setText("");
    uscountyList->clear();
    wLocator->setText("");
    wCntry->setText("");
    wRxPwr->setText("");
    wSwl->setText("");                                 // ContestId
    wQslMgr->setText("");                              // QslManager
    wEditContestId->setText("");
    
    wCustom1->setText("");
    wCustom2->setText("");
    labelDistanz->setText("");
    labelTdif->setText("");
    tQsofound->setText("");
    labelAwdInfo->setText("");
    Cept = "";
    Rufz = "";
    La = "";
    //editflg = 0;
    uaflg = 0;
    wNotizEdit->setText("");
    if(checkBoxKom->isChecked() == false)              // falls kein globaler Kommentar
       wKomEdit->setText("");                          // löschen
       
    dial->setSliderPosition(180);
    i = 0;
    if(checkBoxRSTe->checkState())
        wRSTe->setText(comboBoxRSTe->currentText());
    if(checkBoxRSTg->checkState())
        wRSTg->setText(comboBoxRSTg->currentText());
    
    QHashIterator<QString,QString> i(awdhash);         // AWD.Hash.Tabelle leeren
    while(i.hasNext()) {
       i.next();
       awdhash.insert(i.key(),"");                     // clear_value
    }
    labelAWD->setText("0");                            // awd_einträge = 0
    
    setQslDefault();                                   // wieder zurückstellen         
    id = 0;
    if(editflg) { 
      checkBoxUhr->setPalette(QPalette(QColor(224,224,224)));  // UHR als aktiv anzeigen
      checkBoxUhr->setChecked(FALSE);
    
      checkBoxStop->setChecked(FALSE);
      checkBoxStop->setPalette(QPalette(QColor(224,224,224))); // Frequenz_Box wieder aktiv
      editflg = 0;
    }
    
    QPalette palette2;
    QBrush brush2(QColor(0, 0, 0, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Active, QPalette::WindowText, brush2);
    palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
    QBrush brush3(QColor(119, 119, 119, 255));
    brush3.setStyle(Qt::SolidPattern);
    palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
    label_Station->setPalette(palette2);
    QPalette palette11;
    QBrush brush11(QColor(244,244,244,255));
    brush11.setStyle(Qt::SolidPattern);
    palette11.setBrush(QPalette::Disabled, QPalette::Base,brush11);
    wAwd->setPalette(palette11);
    
    // qsoList->clear();                    // History_List loeschen
    ButtonHistory->setText(qso);            // auf Qso Eingabe schalten
    hisflg = 1;
    wCall->setFocus();                       // Cursor_Fokus setzen
}


// ---------------------------------------------------------------
// Button_History toggeln:  < Last-QSO's>  /  < Prefix-QSO's >
// ---------------------------------------------------------------
void addQso::changeHistoryCb()
{
    qsoList->clear();
    if( hisflg == 1 ) {                           
       hisflg = 0;                                
       ButtonHistory->setText(last);              
       getHistoryLast();
    }
    else {
       hisflg = 1;
       ButtonHistory->setText(qso);               
       getHistoryPref();
    }
}


// -----------------------------------------------------
// Suche nach Prefix_La und gebe Ergebnis zurück
// -----------------------------------------------------
int addQso::getla(QString str)
{
  int i,n,index;
    n = 0;
    i = 0;
    QSqlQuery query;                                       // setze query_instanz
    qy = "SELECT pref FROM tpref WHERE spref='"+str+"'";   // suche Land zum Prefix
    query.exec(qy);
    i = query.size();                                      // id > 0 Prefix gefunden
    if( i ) {                                              // wenn id = 1 Land gefunden
       query.next();
       s = query.value(0).toString();
       qy = "SELECT lname,lcn,br,lg,ldxcc,timez FROM tla WHERE la='"+s+"'"; // alle la_Angaben
       query.exec(qy);
       
       while(query.next()) {
          s = query.value(0).toString();
          wCntry->setText(s);                              //  Land_Name anzeigen
          s = query.value(1).toString();                   //  continent
          s +="-";
          index = IotaBox->findText(s);
          IotaBox->setCurrentIndex(index);                 //  stelle IOTA_box
	  
          s = query.value(2).toString();                   //  Breite
          f = query.value(3).toString();                   //  Laenge
          labelDistanz->setText(distanz.getDisRicht(s,f)); // -Entfernung + Richtung berechen
          dial->setSliderPosition(distanz.getRichtung());  //  Kompas_Nadel setzen
          La = query.value(4).toString();                  //  dxcc_prefix sichern
          labelTdif->setText(query.value(5).toString());   //  +/-UTC eintragen
          n++;
         }
      }
      return i;
}



// ###########################################################################################
//  Rufzeichen_EINGABE  - suche: Prefix, dann Rufzeichen_vergangenheit - frage Datenbanken ab
//  connect(wCall, SIGNAL(textEdited(QString)), this, SLOT(callEdited(QString)));
// ===========================================================================================
void addQso::callEdited(QString str)
{
    if(str.isEmpty() == true) {
      if( hisflg) {
         qsoList->clear();                   // History_List loeschen
         resetAddMask();                     // Eingabemaske loeschen
      }
      id = 0;                                // Land_Id loeschen
      return;                                // falls Eingabe leer -> return
    }
    if(editflg == 1) {          // editflg = 1 - INFO zur Rufzeichen_Aenderung ausgeben
        QMessageBox::information( this,
        tr("Rufzeichen Aenderung"),
        tr("Wird ein Rufzeichen veraendert,\noder auf einen CEPT-Prefix umgestellt.\nMuss es komplett neu eingegeben werden.\nSonst ist die Aenderung ohne Wirkung"),
        QMessageBox::Default,
        QMessageBox::NoButton, QMessageBox::NoButton);
        Cept = "";
        editflg = 2;
    }
    
    QTime timeN = timeEditEnd->time();       // Uhrzeit wird bei jedem Tastclick aktualisiere
    s = t.setNum(timeN.hour());              // hole die Zeit aus timeEdit_END ( laufende uhr )
    s += ":"+t.setNum(timeN.minute());
    s += ":"+t.setNum(timeN.second());
    timeN = QTime::fromString(s,"H:m:s");
    timeEditStart->setTime(timeN);           // trage diese in timeEditStart ein
    
    strlen = str.count();
    if( strlen == 1 ) {                      // Name 'Station' auf aktiv setzen = rot
       QPalette palette2;
       QBrush brush2(QColor(255, 85, 0, 255));
       brush2.setStyle(Qt::SolidPattern);
       palette2.setBrush(QPalette::Active, QPalette::WindowText, brush2);
       palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
       QBrush brush3(QColor(119, 119, 119, 255));
       brush3.setStyle(Qt::SolidPattern);
       palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
       label_Station->setPalette(palette2);
       qsoList->clear();                     // History_List loeschen
       ButtonHistory->setText(qso);          // auf Qso Eingabe schalten
       hisflg = 1;
       id = 0;
    }
    t = str.toUpper();
    wCall->setText(t);                         // Call_Echo immer Großbuchstaben

    x = t.indexOf('/',0);      // suche flush
    if( x == -1 ) {            // flush nicht gefunden
       Rufz = t;               // rufzeichen übernehmen
       id = getla(Rufz);       // suche Landes_prefix
    }
    else {                     // ja: flush gefunden
     if( x <= 3 ) {
      Cept = t.left(x +1);     // kopiere nach Cept, mit '/'
      Rufz = t.mid(x +1);      // kopiere rufz nach Rufzeichen
      wQslMgr->setText(Rufz);  // bei Cept auch gleich QSL via Call übernehmen
     }
     else {
       if(Cept.count() == 0)
        Rufz = t;
     }
    }
// --
    if( !id ) {                 // falls kein prefix
        // labelDistanz->setText("");
        // wCntry->setText("");
        // labelAdif->setText("");
        qsoList->clear();
    }
    
    if( hisflg )                // path für Ausgabe
       getHistoryPref();        // hisflg = 1 Ausgabe / hisflg = 0 keine Ausgabe -> Update()
       
    if( strlen >= 4 ) {                   // ab 4 char. Datenbanken abfragen
      if(wName->text().count() == 0) {    // wenn Name nicht vorhanden - suche in ..
         switch(comboBoxSuche->currentIndex()) {
            case 1 :                      //qDebug() << "suche in QRZ_db";
	             getQrzDbCall(Rufz);  // in QRZ.db suchen
                     break;
           default : break;               // ist 0  QSO_historie
         }
      }
    }
    
    // entwicklung ---------
    // if(strlen == 4 & uaflg == 1)                  // check RDA_AWARD_prefix, bei Rufz_länge == 4
    //   awdLabel->setText(getRdaDistrictKenner(Rufz));
    // else {
    //  if(strlen == 2 & La.compare("UA") == 0)     // check UA
    //       uaflg = 1;                             // gefunden
    // }
}


// --------------------------------------------------------------------------
// Anzeige - Historie auf Prefix_Basis -> Historie zum Rufzeichen
// --------------------------------------------------------------------------
void addQso::getHistoryPref()
{
  int cnt;
     cnt = 0;
     if(!Rufz.count()) {              // kein Rufzeichen, keine Anzeige
        tQsofound->setText(s.setNum(cnt));
       return;
     }
     qsoList->clear();
     // QApplication::beep();           // geht nicht oder ausgeshaltet
     QSqlQuery query;
     qy = "SELECT omid,name FROM "+logFile+"om WHERE rufz='"+Rufz+"'";
     query.exec(qy);
     n = query.size();
     if( n ) {
        while(query.next()) {
            omId = query.value(0).toInt();
            wName->setText(query.value(1).toString());
        }
     }
     // else {   // nicht löschen, es könnte bereits ein gültiger Name eingetragen sein.
     //   omId = 0;
     //   wName->setText("");
     //   wQth->setText("");
     // }
     t =  AwdBox->currentText();
     
     qy = "SELECT id,oid,day,btime,cept,rufz,name,qth,band,mode,rstr,qslr,qsls,(SELECT awkenner FROM ";
     qy += logFile+"awd WHERE awtype = '"+t+"' AND id=qid),loc,ntext,contestid";
     qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
     qy += " WHERE rufz LIKE '"+Rufz+"%' AND dxcc='"+La+"' ORDER by day DESC,btime DESC LIMIT "+Last;
     query.exec(qy);
     x = query.size();
     while(query.next()) {                                 // Liste aufbauen
        n = 0;
        i = 0;
        QTreeWidgetItem *item = new QTreeWidgetItem(qsoList);
        x = query.value(n++).toInt();                      // QSO_Id
        Id = query.value(n++).toInt();                     // QSO_oId
        item->setText(i++,query.value(n++).toString());    // Datum
        item->setText(i++,query.value(n++).toString());    // Startzeit
        s = query.value(n++).toString();                   // Cept
        s += query.value(n++).toString();
        item->setText(i++,s);                              // Rufzeichen
        item->setText(i++,query.value(n++).toString());    // Name
        item->setText(i++,query.value(n++).toString());    // Qth
        item->setText(i++,serv.getMyband(query.value(n++).toString())); 
        item->setText(i++,query.value(n++).toString());    // Mode
        item->setText(i,query.value(n++).toString());      // RST erhalten
        item->setTextAlignment(i++,Qt::AlignCenter);
        item->setText(i,query.value(n++).toString());      // QSL erhalten
        item->setTextAlignment(i++,Qt::AlignCenter);
        item->setText(i,query.value(n++).toString());      // QSL gesendet
        item->setTextAlignment(i++,Qt::AlignCenter);
        item->setText(i++,query.value(n++).toString());    // AWD
        item->setText(i++,query.value(n++).toString());    // Locator
        item->setText(i++,query.value(n++).toString());    // Notiz
        item->setText(i++,query.value(n++).toString());    // ContestId
        cnt++;
     }
     
     if(omId) {                                           // falls OM gefunden - hole QTH
         wQth->setText("");
         qy = "SELECT qth FROM "+logFile+","+logFile+"om ";
	 qy += "WHERE omid="+s.setNum(omId)+" AND id="+f.setNum(x);
         query.exec(qy);
         n = query.numRowsAffected();
         if( n > 0 ) {
           query.next();
           wQth->setText(query.value(0).toString());
         }
     }
     if( cnt == Last.toInt())
        tQsofound->setText(" max");
     else
      tQsofound->setText(s.setNum(cnt)+tr(" gefunden"));
}


// --------------------------------------------------------------------
// Historie - die letzten 0 - Last  QSO's ausgeben
// --------------------------------------------------------------------
void addQso::getHistoryLast()
{
 int n, i, cnt;
   cnt = 0;
   qsoList->clear();
   t =  AwdBox->currentText();
   
   qy = "SELECT day,btime,cept,rufz,name,qth,band,mode,rstr,qslr,qsls,(SELECT awkenner FROM ";
   qy += logFile+"awd WHERE awtype = '"+t+"' AND id=qid),loc,ntext,contestid ";
   qy += "FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
   qy += " WHERE day <= '"+toDay+"' ORDER BY day DESC,btime DESC LIMIT "+Last;
   QSqlQuery query(qy);
   while(query.next()) {
      n = 0;
      i = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(qsoList);
      item->setText(i++,query.value(n++).toString());    // Datum
      item->setText(i++,query.value(n++).toString());    // Startzeit
      s = query.value(n++).toString();                   // Cept
      s += query.value(n++).toString();                  // cept/Call
      item->setText(i++,s);                              // Rufzeichen
      item->setText(i++,query.value(n++).toString());    // Name
      item->setText(i++,query.value(n++).toString());    // Qth
      item->setText(i++,serv.getMyband(query.value(n++).toString())); // band
      item->setText(i++,query.value(n++).toString());    // Mode
      item->setText(i,query.value(n++).toString());      // RST erhalten
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(i,query.value(n++).toString());      // QSL erhalten
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(i,query.value(n++).toString());      // QSL gesendet
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(i++,query.value(n++).toString());    // AWD (zB. DOK)
      item->setText(i++,query.value(n++).toString());    // Locator
      item->setText(i++,query.value(n++).toString());    // Notiz
      item->setText(i++,query.value(n++).toString());    // Contest
      cnt++;
  }
  if(cnt == Last.toInt())
    tQsofound->setText(" max");
  else
   tQsofound->setText(s.setNum(cnt)+tr(" gefunden"));
}

// suche in der QRZ.db - gebe Namen und QTH zurück
// -------------------------------------------------------
void addQso::getQrzDbCall(QString call)
{
 QString c, ruf;
 int i = 0;
   c = call;
   i = c.indexOf("/",0);                // suche flush
   if( i <= 3 )                         // wenn i kleiner = 3 ist es Cept
      ruf = c.mid(i +1);                // separiere rufzeichen
   else
      ruf = c.left(i);                  // es ist suf - separiere rufzeichen vom suf
   qy = "SELECT fname,mcity FROM qrzbkc WHERE icall='"+ruf+"'"; // ruf ist immer ohne Cept und suf
   QSqlQuery query;
   query.exec(qy);
   while(query.next()) {
     wName->setText(query.value(0).toString());   // Name
     wQth->setText(query.value(1).toString());    // Qth
   }
}
/*

QString addQso::getRdaDistrictKenner(QString rdaPref)
{
   QSqlQuery query;
   qy = "SELECT rdxc FROM rutable WHERE pref='"+rdaPref+"'";
   query.exec(qy);
   query.next();
   t = query.value(0).toString();
   s = AwdBox->currentText();    
   awdhash.insert(s,t);          
   return t;                     
}
*/

// *************************************************************************
// INSERT: neues QSO speichern
//*************************************************************************
void addQso::addNewQso()
{
 QString xband, pr;
   i = 0;                                     // Call aufbereiten
   t = wCall->text();
   if(t.count() == 0)                         // nichts eingetragen, geht nicht
     return;
   pr = "";
   QSqlQuery query;
   QSqlField field("feld",QVariant::String);  // arbeits_var. 'field'
   
   // -- OM_tabelle -----------------------------------------
   if( ! omId ) {                                   // wenn kein OM vorhanden - OM anlegen
      qy = "INSERT INTO "+logFile+"om VALUES (0,'";
      qy += Rufz+"',";                              // rufzeichen
      
      if(wName->text().count() == 0) {              // falls Name nicht bekannt
	if(lineEditOM->text().indexOf("CALL") != -1)
	 wName->setText(Rufz);
	else
         wName->setText(lineEditOM->text());        // setze pseudo_Anrede
      }
      
      field.setValue(wName->text());                // Name
      qy += db.driver()->formatValue(field,false);  // Name - escape_field
      qy += ")";
      query.exec(qy);
      omId = query.lastInsertId().toInt();          // sichere neue omId
   }
   
   // -- QSO_tabelle -----------------------------------------
   qy = "INSERT INTO "+logFile+" VALUES (0,";    // Id=0: ist qso_index_Nr
   qy += t.setNum(omId);                         // om_Id übernehmen
   qy += ",'"+Cept+"',";                         // Cept
   field.setValue(wQth->text());                 // QTH
   qy += db.driver()->formatValue(field,false);  // QTH - escape_field
   qy += ",'"+wLocator->text();                  // Locator
   qy += "','"+serv.getSysband(BandBox->currentText()); // Band
   qy += "','"+wFreq->text();                    // Frequenz
   qy += "','"+ModeBox->currentText();           // Mode
   qy += "','";
   
   QDate d;
   if(checkBoxUhr->isChecked() == FALSE) {       // QSO_erfassung - UHR steht
     QDateTime h = QDateTime::currentDateTime().toUTC(); // day - laufendes Tages_Datum BASIS UT_time
     dateEditStart->setDate(h.date());
     d = h.date();                               // ab 24Uhr ist nach UT_Zeit ein neuer Tag
   }
   else 
     d = dateEditStart->date();                  // nachträgliche QSO_erfassung  
   qy += d.toString("yyyy-MM-dd"); 
   qy += "','";
   
   QTime timeN;
   if( checkBoxEnd->isChecked() == true ) {      // Startzeit == Endzeit
     timeN = timeEditEnd->time();                // eTime für Zeit QSO_Start
     s = timeN.toString("hh:mm:ss");
     timeN = QTime::fromString(s,"H:m:s");
     timeEditStart->setTime(timeN);              // trage diese in timeEditStart ein
   }
   else                                          // Startzeit ist älter als Endzeit
     timeN = timeEditStart->time();              // btime - Zeit QSO_Start
   qy += timeN.toString("hh:mm:ss");
   qy += "','";
   timeN = timeEditEnd->time();                  // etime - Zeit QSO_Ende
   qy += timeN.toString("hh:mm:ss");
   
   // --
   s = wRSTe->text();
   qy += "','"+s.trimmed();                     // RSTr erhalten
   s = wRSTg->text();
   qy += "','"+s.trimmed();                     // RSTs gegeben
   qy += "','"+wRxPwr->text();                  // Sendeleistung der Empfangs-Station

   // -- QSL via Büro
   if(checkBoxBuero->isChecked() == true) {     // via Büro - Haken gesetzt ?
     qy += "','R','R";                          // qslr=R, qsls=R
     pr = "B";                                  // prflg setzen
   }
   else {
      qy += "','-','-";                         // nein
      pr = "";                                  // prflg nein
   }
// --
   qy += "','0000-00-00";                       // qslrd  Datum QSL_erhalten
   qy += "','0000-00-00";                       // qslsd  Datum QSL gesendet
   qy += "','"+wQslMgr->text();                 // qslmgr QSL_manager
   qy += "','"+RigBox->currentText();           // Rig
   qy += "','"+PwrBox->currentText();           // TxPWR
// --
     // hole die Landes_Daten für den Prefix -> La
   t = "SELECT ldxcc,lwae,lwpx,ituz,cqz,lcn FROM tla WHERE la = '"+La+"'";
   query.exec(t);
   if(query.size()) {                            // falls gültiger Prefix vorhanden
      while(query.next()) {
        n = 0;
        //dxcc = query.value(n++).toString();    
        qy += "','"+query.value(n++).toString(); // Dxcc
        qy += "','"+query.value(n++).toString(); // Wae
        qy += "','"+query.value(n++).toString(); // Wpx
        qy += "','"+query.value(n++).toString(); // itu_zone
        qy += "','"+query.value(n++).toString(); // Cq_zone
        qy += "','"+query.value(n++).toString(); // Cn
      }
    }
    else {                               // keinen gültigen Prefix für Call gefunden
        qy += "','";                             // Dxcc
        qy += "','";                             // Wae
        qy += "','";                             // Wpx
        qy += "','";                             // itu_zone
        qy += "','";                             // Cq_zone
        qy += "','";                             // Cn
   }
   qy += "','','','";                           // qslRvia, sendVia
   qy += pr;                                    // prflg
   field.setValue(wNotizEdit->text());               // Notiz
   qy += "',"+db.driver()->formatValue(field,false); // escape
   field.setValue(wKomEdit->text());                 // Kommemtar
   qy += ","+db.driver()->formatValue(field,false);  // escape
   qy += ",'";                                  // qslmsg
   qy += "','"+LineBox->currentText();          // Qline
   
   qy += "',"+HidW->text();               // quarter, + operator_Id
   
   qy += ","+s.setNum(operatorId)+")";
   
   query.exec(qy);                               // query durchfüheren
   id = query.lastInsertId().toInt();            // Id vom neuen QSO holen
  
// --- QSL-datensatz **************************
   qy = "INSERT INTO "+logFile+"qsl VALUES (0,";
   qy += t.setNum(id);                          // qso_Id übernehmen
   qy += ",'"+Rufz;                             // QSO_rufzeichen aus Qso_satz
   
   d = dateEditStart->date();                   // day -laufendes Tages_Datum BASIS UT_time
   qy += "','"+t.setNum(d.year());
   qy += "-"+t.setNum(d.month());
   qy += "-"+t.setNum(d.day());                 // ab 24Uhr ist nach UT_Zeit ein neuer Tag
   qy += "','"+wEditContestId->text();          // CONTEST_ID QSO Contest Identifier
   qy += "','";                                 // Authenticity Guaranteed
   
// -- QSLvia eQsl
   qy += "','','";                        // EQSL_QSL_RCVD status e_qsl: Y, N, R, I, V
   if(checkBoxEqsl->isChecked() == true)  // wenn gesetzt
     qy += "R";                           // dann: EQSL_QSL_SEND status e_qsl: Y, N, R, Q, I - wird immer gesetzt
    else 
     qy += "-";                           // nicht gesetzt - kein eQSL
   
   qy += "','";                      // EQSL_QSLRDATE
   qy += "','";                      // EQSL_QSLSDATE
   
// -- LotW
   qy += "','";                      // LOTW_QSL_RCVD status Logbook_over_the_world: Y, N, R, I, V 
   qy += "','";
   qy += "R";                        // immer LOTW_QSL_SEND status Logbook_over_the_world: Y, N, R, Q,
   qy += "','";                      // LOTW_QSLRDATE
   qy += "','";                      // LOTW_QSLSDATE
// --
   qy += "','"+wOpCall->text();      // CONTACTED_OP individual operatig callsign to contacted st.
   qy += "','";                      // FREQ_RX the logging station received freq in MHz
   qy += "','";                      // SRX contest QSO received serial number
   qy += "','";                      // SRX_STRING  contest QSO received serial information
   qy += "','";                      // TRX  contest QSO transmitted serial number
   qy += "','";                      // TRX_STRING  contest QSO transmitted serial information
   qy += "','"+wSwl->text();         // qinfo (SWL indicates) -the QSO information pertains to an SWL
   qy += "','";                      // COMMENT
   qy += "','"+wCustom1->text();     // costom1
   qy += "','"+wCustom2->text();     // custom2
   qy += "','')";                    // für besondere Anwendungen
   query.exec(qy);
   
     // -- AWD_tabelle   anlegen -------------------
   i = 0;
   QHashIterator<QString,QString> i(awdhash);
   while(i.hasNext()) {                         // seek hash_tabelle und INSERT AWD
     i.next();
     s = i.value();
     if(s.count()) {                            // falls Eintrag vorhanden
        qy = "INSERT INTO "+logFile+"awd VALUES (0,"; // Datensatz anlegen
        qy += t.setNum(id);                     // qso_Id
        qy += ",'"+Rufz;                        // Rufzeichen
        qy += "','"+i.key();                    // AWD.Type
        qy += "','"+i.value();                  // Kenner  z.B DOK E33
        qy += "')";
        query.exec(qy);                          // anlegen
     }
   }
   
   wIota->setText("000");
   qy = "SELECT COUNT(*) FROM "+logFile;
   query.exec(qy);
   query.next();
   t = query.value(0).toString();                     // Anzahl_QSOs
   
   qy = "UPDATE dblogs SET qsocnt="+t+" WHERE logname='"+logFile+"'";
   query.exec(qy);
   
   if((upMgr) && (checkBoxEqsl->isChecked() == TRUE)) {   // falls upload freigegeben - upMgr=1
     processWriteDatagram(1, s.setNum(id), 45458);        // Eqsl_RealtimeUp - upload
     labelUp->setText("Up");
   }
   processWriteDatagram(1, s.setNum(id), 45454);    
   
// -- Die neue QSO_Zeile in 'qsoList' einfügen ---
   QTreeWidgetItem *item = new QTreeWidgetItem(qsoList);
   n = 0;
   d = dateEditStart->date();                       // QSO_Tag
   s = d.toString("yyyy-MM-dd"); 
   item->setText(n++,s);
   
   timeN = timeEditStart->time();                   // Zeit QSO_Start
   s = timeN.toString("hh:mm:ss");
   item->setText(n++,s);
   
   item->setText(n++,wCall->text());                // Rufzeichen
   item->setText(n++,wName->text());                // Name
   item->setText(n++,wQth->text());                 // Qth
   item->setText(n++,BandBox->currentText());       // Band
   item->setText(n++,ModeBox->currentText());       // Mode
   item->setText(n,wRSTe->text().simplified());     // RST erhalten
   item->setTextAlignment(n++,Qt::AlignCenter);
   item->setText(n,"R");                            // QSL gewünscht 'x' erh. 'e'
   item->setTextAlignment(n++,Qt::AlignCenter);
   
   if(checkBoxBuero->isChecked() == TRUE)           // gewünscht
     item->setText(n,"R");                          // QSL gewünscht 'x' erh. 'e'
   else
     item->setText(n,"-");                          // QSL nicht gewünscht
     
   item->setTextAlignment(n++,Qt::AlignCenter);
   item->setText(n++,wAwd->text());                 // DOK
   item->setText(n++,wLocator->text());             // Locator
   item->setText(n++,wNotizEdit->text());           // Notiz
   item->setText(n++,wEditContestId->text());       // Contest
   id = 0;
   omId = 0;
   
   // -- dxcc_pref_tabelle füllen --------------------------
   // nur wenn gültiger Landes_Prefix vorhanden
   if(La.count()) {                                 // gültiger Prefix ?
     qy = "SELECT cntx,b"+serv.getSysband(BandBox->currentText())+" FROM "+logFile+"dx ";
     qy += "WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
     query.exec(qy);
     query.next();
     if(query.size() != 0) {                         // neues QSO
         n = query.value(0).toInt();                 // cntx holen
         xband = query.value(1).toString();          // Eintrag in band x oder e
         n++;
         qy = "UPDATE "+logFile+"dx SET cntx=";
         qy += t.setNum(n);
         if(xband.compare("e") != 0) {                          // wenn Eintrag Band_status nicht 'e'
           qy += ", b"+serv.getSysband(BandBox->currentText()); // dann 'x' eintragen
           qy += "='x' WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
         }
         else {                                      // sonst Band_status 'e' nicht überschreiben
           qy += " WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
         }
         query.exec(qy);
     }
     else {
       qy = "INSERT INTO "+logFile+"dx VALUE ('";
       qy += La;                                     // Prefix
       qy += "','"+ModeBox->currentText();           // mode
       qy += "',1,0,";                               // cnt
       qy += "'','','','','','','','','','','','','','','','','','','')";
       query.exec(qy);
       qy = "UPDATE "+logFile+"dx SET b"+serv.getSysband(BandBox->currentText());
       qy += "='x' WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
       query.exec(qy);
     }
   }
   resetAddMask();                   // alle Variablen im QSO_Dialog löschen
   getHistoryLast();                 // 10-03-10
}


// -------------------------------------------------------------------------------------------
// QSO_EDIT: - Doppel_click in eine QSO_Zeile - Das QSO wird in die Datenfelder geladen
// Suche und Anzeige auf Prefix_Ebene gestoppt, Freq_update stop, Uhr stop
//--------------------------------------------------------------------------------------------
void addQso::qsoListClicked(QTreeWidgetItem *item, int col)
{
      i = col;                                              // ist dummy, wird nicht benötigt
      uhrStop(1);                                           // UHR stop
      rigpolling = 0;                                       // rig_polling stop
      checkBoxStop->setPalette(QPalette(QColor(232,58,5))); // Farbe rot
      checkBoxStop->setChecked(TRUE);                       // Haken setzen
      //stimer->stop();
      
      ButtonAdd->hide();                               // addQso ausblenden
      ButtonReset->hide();                             // Reset_Button ausblenden
      ButtonUpdate->show();                            // Update anzeigen
      ButtonBack->show();                              // Zurück_Button anzeigen
      ButtonDel->show();                               // Del_Button zeigen
      hisflg = 0;                  // Suche und Anzeige auf Prefix_ebene  verhindern
      editflg = 1;
      updateflg = 1;                                   // update_modus aktive
      
      QString cept;
      QString ruf,xrufz;
      
      
      // 1. Datensatz ohne AWD lesen
      // 2. AWD - alle Einträge zum Datensatz lesen und hash_tabelle aufbauen.
      //    AWD_Box auf den 1.Eintrag setzen
      qsoItem = item;
      xrufz = item->text(2);
      i = xrufz.indexOf('/',0);
      if( i != -1 ) {                       // wenn flush gefunden
        if( i <= 3 ) {
          cept = xrufz.left(i +1);          // kopiere nach cept, '/' abhängen
          ruf = xrufz.mid(i +1);            // sichere Rufzeichen
        }
        else
         ruf = xrufz;
      }
      else {
        ruf = xrufz;
      }
      BandIdx = BandBox->currentIndex();    // store Band merken
      ModeIdx = ModeBox->currentIndex();    // store Mode merken
      
      qy = "SELECT id,oid,day,btime,etime,cept,rufz,name,qth,band,mode,";
      qy += "freq,rstr,rsts,rxpwr,qslr,qsls,contactdOp,loc,ntext,ktext,qslmgr,contestid,qinfo,custom1,custom2";
      qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
      qy += " WHERE band = '"+serv.getSysband(item->text(5));
      qy += "' AND rufz = '"+ruf+"' AND day = '";
      qy += item->text(0)+"' AND btime = '"+item->text(1)+"'";
      QSqlQuery query(qy);                                // query durchführen
      query.next();                                       // Datensatz lesen
      
      n = 0;
      Id = query.value(n++).toInt();                      // QSO_Datensatz sichern
      omId = query.value(n++).toInt();                    // OM_datensatz sichern
 
      s = query.value(n++).toString();                    // day_Start
      QDate d = QDate::fromString (s,"yyyy-MM-dd");
      dateEditStart->setDate(d);                           // korr.Datum wenn nach 12 UR !!!!
      
      s = query.value(n++).toString();                    // time_Start
      QTime timeold = QTime::fromString(s,"HH:mm:ss");
      timeEditStart->setTime(timeold);
      
      s = query.value(n++).toString();                    // time_End
      timeold = QTime::fromString(s,"HH:mm:ss");
      timeEditEnd->setTime(timeold);
      
      Cept = query.value(n++).toString();
      Rufz = query.value(n++).toString();
      f = Cept + Rufz;
      wCall->setText(f);                                       // 3 Rufzeichen
      wName->setText(query.value(n++).toString());             // 4 Name
      wQth->setText(query.value(n++).toString());              // 5 Qth
      // --
      i = BandBox->findText(serv.getMyband(query.value(n++).toString())); // band
      BandBox->setCurrentIndex(i);                                        // - schalte Band_Box
      
      i = ModeBox->findText(query.value(n++).toString());                 // 7 Mode
      ModeBox->setCurrentIndex(i);                                        // - schalte Mode_Box
      
      wFreq->setText(query.value(n++).toString());             //  8 Frequenz
      wRSTe->setText(query.value(n++).toString());             //  9 RSTe
      wRSTg->setText(query.value(n++).toString());             // 10 RSTg
      wRxPwr->setText(query.value(n++).toString());            // 11 RxPwr
      Qslr = query.value(n++).toString();                      //    Qslr
      Qsls = query.value(n++).toString();                      //    Qsls
      wOpCall->setText(query.value(n++).toString());           // 12 Operator_call
      wLocator->setText(query.value(n++).toString());          // 13 Locator
      wNotizEdit->setText(query.value(n++).toString());        // 14 Notiz
      wKomEdit->setText(query.value(n++).toString());          // 15 Kommentar

      wQslMgr->setText(query.value(n++).toString());           // 16 QSLmanager
 
      wEditContestId->setText(query.value(n++).toString());    // 16 ContestID
      wSwl->setText(query.value(n++).toString());              // 16 SWL
      wCustom1->setText(query.value(n++).toString());          // 17 custom1
      wCustom2->setText(query.value(n++).toString());          // 18 custom2
      
      QHashIterator<QString, QString> i(awdhash);        // hash.Tabelle value löschen
      while(i.hasNext()) {
         i.next();
         n = QString::compare(t, i.key(), Qt::CaseInsensitive);
         awdhash.insert(i.key(),"");
      }
      
      qy = "SELECT awtype,awkenner FROM "+logFile+"awd WHERE qid = "+t.setNum(Id);
      query.exec(qy);                                    // query durchführen
      while (query.next()) {                             // AWD_Tabelle fuellen
        n = 0;
        t = query.value(n++).toString();                 // - type ( keys )
        s = query.value(n++).toString();                 // - kenner
        awdhash.insert(t,s);                             // - Type u. Kenner eintragen
      }
      
      wAwd->setText("");
      awCnt = 0;
      t = AwdBox->currentText();                         // AWD_Box auf Pos.1 stellen
      QHashIterator<QString, QString> m(awdhash);        // QHashIterator erzeugen
      while(m.hasNext()) {                      // awd_tabelle durchlaufen - AWD_keys zählen
        m.next();
        s = m.value();
        if(s.count()) {                         // check ob key.value vorhanden
           s = m.key();
           n = AwdBox->findText(s);             // Box_Name suchen
           AwdBox->setCurrentIndex(n);          // currentIndex box_Name setzen
           wAwd->setText(m.value());            // Kenner eintragen
           awCnt++;
        }
      }
      
      t.setNum(awCnt);
      labelAWD->setText(t);            // Anzahl der gefundenen AWD_keys zeigen
}

// --------------------------------------- TEST ----------
void addQso::getQrzCallsign()
{
                    if(comboBoxSuche->currentIndex() == 0) {
		       if( !flgf6 ) {                      // toggel F6; falls 0, dann Abfrage
		          if(wCall->text().count() != 0) { // kein leeres Rufzeichen_feld abfragen
			    flgf6 = 1;                     // Abfrage läuft jetzt
			    processWriteDatagram(1, wCall->text(), 45460);
			    setColor(1);                   // rot setzen, Abfrage aktiv
			  }
		       }
		       else {
			 flgf6 = 0;                        // sonst, Ergebnis löschen
			 setColor(0);
			 clearQrzData(); 
			 tabWidget->setCurrentIndex(0);
		       }
		     }
}

// Nach addQSO zurück_schalten ( Abbruch - kein update ) Uhr run, Freq_update run
// ------------------------------------------------------------------------------
void addQso::buttonBackCb()
{
   BandBox->setCurrentIndex(BandIdx);  // stelle default zurück
   ModeBox->setCurrentIndex(ModeIdx);  // stelle default zurück
   ButtonUpdate->hide();
   ButtonBack->hide();
   ButtonDel->hide();
   ButtonAdd->show();
   ButtonReset->show();
   uhrStop(0);                         // UHR wieder einschalten
   rigpolling = runing;                // hamlib Freq_update herstellen
   updateflg = 0;                      // update_modus verlassen
   resetAddMask();                     // loesche Eingabe_maske und awd_tabelle
}


// ---------------------------------------------------------------------------
// UPDATE: durchführen  - nach addQSO zurück schalten, Uhr run, hamlib run
// ---------------------------------------------------------------------------
void addQso::buttonUpdateCb()
{
      // prüfe ob Rufzeichen verändert wurde
      // -----------------------------------
      // Id   = QSO_Datensatz
      // omId = OM_Datensatz
      // werden von qsoListClicked zur Verfügung gestellt.
      
      QSqlQuery query;
      QSqlField field("feld",QVariant::String);
       // OM_Tabelle
      qy = "UPDATE "+logFile+"om SET rufz='"+Rufz+"',name=";
      field.setValue(wName->text());                  // Name
      qy += db.driver()->formatValue(field,false);    // Name - escape_field
      qy += " WHERE omid="+t.setNum(omId);
      query.exec(qy);
      
      // QSO_Tabelle
      qy = "UPDATE "+logFile+" SET ";                 // QSO_Tabelle
      if(editflg == 2) {                              // falls Rufzeichen veraendert
        if(Cept.count())
          Cept += "/";                                // flush anhaengen
        editflg = 0;
      }
      qy += "cept='"+Cept+"',";                       // Cept
      
      field.setValue(wQth->text());                   // QTH
      qy += "qth="+db.driver()->formatValue(field,false); // QTH - escape_field
      
      qy += ",loc='"+wLocator->text();                  // locator
      qy += "',band='"+serv.getSysband(BandBox->currentText()); // Band
      qy += "',freq='"+wFreq->text();                   // Frequenz
      qy += "',mode='"+ModeBox->currentText();          // Mode
      
      qy += "',day='";                                 
      QDate d = dateEditStart->date();                  //  // Datum
      qy += t.setNum(d.year());
      qy += "-"+t.setNum(d.month());
      qy += "-"+t.setNum(d.day());
      qy += "',btime='";                               // Start Zeit
      
      QTime timN = timeEditStart->time();
      qy += t.setNum(timN.hour());
      qy += ":"+t.setNum(timN.minute());
      qy += ":"+t.setNum(timN.second());
      
      qy += "',etime='";                                // ENDE zeit
      timN = timeEditEnd->time();
      qy += t.setNum(timN.hour());
      qy += ":"+t.setNum(timN.minute());
      qy += ":"+t.setNum(timN.second());
      
      qy += "',rstr='";                                 // RST erhalten
      s = wRSTe->text();
      qy += s.trimmed();
      
      qy += "',rsts='";                                // RST gegeben
      s = wRSTg->text();
      qy += s.trimmed();
      
      qy += "',rxpwr='"+wRxPwr->text();                // Rx _ PWR
      
      if(checkBoxBuero->isChecked() == TRUE) {
        qy += "',qslr='R',qsls='R";                    // QSL gewünscht
        qy += "',prflg='B";                            // Printflag
       }
       else {
        qy += "',qslr='-',qsls='-";                    // QSL nein
        qy += "',prflg='";                             // Printflag
	
      }
      qy += "',qslmgr='"+wQslMgr->text();
      qy += "',rig='"+RigBox->currentText();           // RIG
      qy += "',txpwr="+PwrBox->currentText();          // Trx PWR
      field.setValue(wNotizEdit->text());              // Notiz
      qy += ",ntext="+db.driver()->formatValue(field,false);     // escape_field
      field.setValue(wKomEdit->text());                 // Kommemtar
      qy += ",ktext="+db.driver()->formatValue(field,false);     // escape_field
      qy += " WHERE id="+t.setNum(Id);
      query.exec(qy);                                   // UPDATE Datensatz_Felder
// qsl_Tabelle --
      qy = "UPDATE "+logFile+"qsl SET ";
      qy += "contestid='"+wEditContestId->text();
      
      // eQSL 
      if(checkBoxEqsl->isChecked() == TRUE)            // wenn gesetzt
         qy += "',eqsls='R";
      else
         qy += "',eqsls='-";
      
      qy += "',contactdOp='"+wOpCall->text();
      qy += "',qinfo='"+wSwl->text();
      qy += "',custom1='"+wCustom1->text();
      qy += "',custom2='"+wCustom2->text();
      qy += "' WHERE qsoid="+t.setNum(Id);
      query.exec(qy);   
  
// AWD --
/*
      // TEST
      QHashIterator<QString,QString> ii(awdhash);       // AWD.Hash.Tabelle leeren
      while(ii.hasNext()) {
         ii.next();
         qDebug() << ii.key() << ii.value();            // clear_value
      }
*/
      QHashIterator<QString, QString> i(awdhash);       // AWD_Tabelle kompl.durcharbeiten
      while(i.hasNext()) {
	
         i.next();
         s = i.value();
         qy = "SELECT awtype FROM "+logFile+"awd WHERE awtype = '"+i.key()+"' AND qid="+t.setNum(Id);
         query.exec(qy);                            // suche nach dem passenden keyPressEvent
	 
         if( query.numRowsAffected() ) {            // wenn vorhanden -> updaten
             if(s.count()) {                        // nur wenn kenner_text vorhanden updaten
               qy = "UPDATE "+logFile+"awd SET awkenner='"+s+"' WHERE qid="+t.setNum(Id);
               qy += " AND awtype='"+i.key()+"'";
               query.exec(qy);
              }
              else {                               // loeschen - da i.value() geloescht wurde
                 qy = "DELETE FROM "+logFile+"awd WHERE ";
                 qy += "qid="+t.setNum(Id)+" AND awtype='"+i.key()+"'";
                 query.exec(qy);
              }
          }
           else {                                  // kein key mit Eintrag gefunden
              if(s.count()) {                      // falls Eintrag vorhanden -> AWDsatz anlegen
                 qy = "INSERT INTO "+logFile+"awd VALUES (0,";
                 qy += t.setNum(Id);               // qso_Id
                 qy += ",'"+wCall->text();         // Rufzeichen
                 qy += "','"+i.key();              // AWD.Type
                 qy += "','"+i.value();            // Kenner  z.B DOK E33
                 qy += "')";
                 query.exec(qy);
              }
          }
           // i.next(); falsch
     }
     processWriteDatagram(2, s.setNum(Id), 45454); // qtlog - message an qtlog.proz. -> data_UPDATE
     
     if( ! hisflg )                                // Update_Aufruf aus Last_Historie
       getHistoryLast();
     else
       getHistoryPref();                           // Updtate_Aufruf aus Pref_Historie
       
     BandBox->setCurrentIndex(BandIdx);            // stelle default zurück
     ModeBox->setCurrentIndex(ModeIdx);            // stelle default zurück_schalten
     
     ButtonUpdate->hide();
     ButtonBack->hide();
     ButtonDel->hide();
     ButtonAdd->show();
     ButtonReset->show();                // UHR wieder einschalten
     uhrStop(0);                         
     rigpolling = runing;
     resetAddMask();                     // lösche Eingabe_maske und awd_tabelle
     hisflg = 1;                         // suche auf Prefix_Ebene wieder zulassen
     updateflg = 0;                      // update_modus beendet
     tabWidget ->setCurrentIndex(0);
}


// ------------------------------------------------------------------
//  QSO löschen : durchführen und nach  addQSO zurück schalten
// ------------------------------------------------------------------
void addQso::buttonDeleteCb()
{
     // AWD -  prüfe ob AWD für QSO vorhanden - wenn ja: löschen
     // OM  -  prüfe ob OM mehrere QSO's hat  - wenn nein: löschen
     // QSO -  Datensatz_id löschen
     QSqlQuery query;
     // fun_awd
     qy = "DELETE FROM "+logFile+"awd WHERE qid="+s.setNum(Id);  
     query.exec(qy);
     
     // fun_om
     qy = "SELECT id FROM "+logFile+" WHERE oid="+s.setNum(omId);       //OM prüfe Anzahl OSO's
     query.exec(qy);                                                    //    für diesen OM
     n = query.size();                                                  //    falls nur 1 QSO
     if( n == 1 ) {                                                     //    dann lösche OM
       qy  = "DELETE FROM "+logFile+"om WHERE omid="+s.setNum(omId);    //OM loeschen
       query.exec(qy);
     }
     
     // cntx = -1 setzen
     qy = "SELECT mode,dxcc FROM "+logFile+" WHERE id="+s.setNum(Id);   // dxcc bearbeiten
     query.exec(qy);
     query.next();
     QString mode = query.value(0).toString(); 
     QString dxcc = query.value(1).toString(); 
     qy = "SELECT cntx FROM "+logFile+"dx ";
     qy += "WHERE dxpref='"+dxcc+"' AND mode='"+mode+"'";
     query.exec(qy);
     query.next();
     int i = query.value(0).toInt(); 
     i--;
     qy = "UPDATE "+logFile+"dx SET cntx="+s.setNum(i)+" WHERE dxpref='"+dxcc+"' AND mode='"+mode+"'";
     query.exec(qy);
   
      // fun_card löschen
     qy = "SELECT cid FROM "+logFile+"card WHERE qsocid="+s.setNum(Id)+" AND qsl='E'";
     query.exec(qy);
     if(query.size() != 0) {
       qy = "DELETE FROM "+logFile+"card WHERE qsocid="+s.setNum(Id)+" AND qsl='E'";
     query.exec(qy);
     }
     // fun_qsl loeschen
     qy = "DELETE FROM "+logFile+"qsl WHERE qsoid="+s.setNum(Id);       // QSL Datensatz loeschen
     query.exec(qy);
   
     // fun
     qy = "DELETE FROM "+logFile+" WHERE id="+s.setNum(Id);             // QSO-Datensatz löschen
     query.exec(qy);
   
     n = qsoList->indexOfTopLevelItem(qsoItem);
     qsoList->takeTopLevelItem ( n );                   // Eintrag in qsoList löschen
     
     processWriteDatagram(3, s.setNum(Id), 45454);      // Qtlog Eintrag in reportList löschen
     
    if( ! hisflg )                                      // Update_Aufruf aus Last_Historie
      getHistoryLast();
    else
     getHistoryPref();                                  // Updtate_Aufruf aus Pref_Historie
     
    ButtonUpdate->hide();
    ButtonBack->hide();
    ButtonDel->hide();
    
    ButtonAdd->show();
    ButtonReset->show();
    uhrStop(0);                                         // UHR wieder einschalten
    resetAddMask();                                     // lösche Eingabe_maske und awd_tabelle
    hisflg = 1;                                         // suche auf Prefix_Ebene wieder zulassen
    updateflg = 0;                                      // update_modus beendet
    rigpolling = runing;
    tabWidget ->setCurrentIndex(0);
    ptimer->start(retry);
}


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Datagram : messages lesen und agieren
// ------------------------------------------------------------
void addQso::processPendingDatagram()
{
   while(udpSocket->hasPendingDatagrams()) {
     datagram.resize(udpSocket->pendingDatagramSize());
     QHostAddress sender;
     quint16 senderPort;
     udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
     i = datagram.indexOf (",",0);
     QByteArray Left = datagram.left(i);           // Funktions_Nr
     QByteArray Mid = datagram.mid(i+1);           // Parameter_Liste
     
     int com = Left.toInt();                       // funktion
     QString t = Mid;                              // message
     QString f, q;
     
     if(com == 3) {  // -------------  // message von DxSpot erhalten
       //FORM: <call>6 dxcall <Name>15 Manfred Dauhrer <Qth>36 Neumayerstr. 3, D-83313 Siegsdorf, , "
       if(t.indexOf("<") != -1) {      // QSO_daten übernehmen
                                       // 1. dxCall übertragen
         t = t.remove(0,1);            // remove <
         i = t.indexOf(">");
         f = t.left(i);                // token_Call. ist immer eindeutig - könnte entfallen
         t = t.remove(0,i +1);         // remove >
         i = t.indexOf(" ");
         n = t.left(i).toInt();        // dxcall.len
         dxCall = t.mid(i+1,n);        // dxCall
         dxCall = dxCall.trimmed();
         dxtimer->start(20);           // schreibe dxCall in callEdited(str)
         
         t = t.remove(0,n);            // jetzt Name, Qth, Kommentar, Notiz ..
         while(1) {
            // <Name>15 Manfred Dauhrer <Qth>36 Neumayerstr. 3, D-83313 Siegsdorf, , "
            if(t.indexOf("<") == -1) return;
            i = t.indexOf("<");
            f = t.remove(0,i+1);
            i = t.indexOf(">");
            f = t.left(i);             // Feld_name
            t = t.remove(0,i +1);      // remove <
            
            i = t.indexOf(" ");
            n = t.left(i).toInt();     // text_len
            q = t.mid(i+1,n).trimmed();
            setText(f, q);
         }
        }
       else {                                  // Auftrag von Dxspot: Frequenz stellen
         buttonBackCb();                       // alte Eingaben löschen
         i = t.indexOf(" ");
         QString freq = t.left(i);             // prüfe: 'last ' oder neu '14222900 '
         if(freq.compare("last") == 0) {       // auf letzte Freq zurückstellen
           freq = lastFreq;
           i = freq.indexOf('.');
           freq = freq.remove(i,1);
           freq += "00";
         } 
         lastFreq = wFreq->text();             // letzte Freq retten   '14555.9'
         slotFlg = 1;
         cmdStr = "F "+freq;
         if(freq.toInt() / 1000000 < 10 )      // prüfe ob das Seitenband angepasst werden muss
          cmdStr += " M LSB 2400";             // und erweitere command_str
        else 
           cmdStr += "M USB 2400";
         cmdFlg = 2;
        }
      }
     else // ---------------------------
      if(com == 10) {                         // message vom getMgr - QRZ.com Abfrage erhalten
	i = t.indexOf("|");
	lineEditQrzCall->setText(t.left(i));  // Call
	
	t.remove(0,i+1);
	i = t.indexOf("|");
	lineEditQrz_1->setText(t.left(i));    // Name
	
	t.remove(0,i+1);
	i = t.indexOf("|");
	lineEditQrz_2->setText(t.left(i));   // street
	
	t.remove(0,i+1);
	i = t.indexOf("|");
	lineEditQrz_3->setText(t.left(i));   // QTH
	
	t.remove(0,i+1);
	lineEditQrz_4->setText(t);           // state
	
	tabWidget->setCurrentIndex(6);
      }
     else
       if(com == 11) {
	  // setColor(0);
	 labelError->setText(t);
	 tabWidget->setCurrentIndex(6);
       }
     else  // ----------------------------------
      if(com == 5) {                          // Auftrag von ReportList
        logFile = t;                          // neus logFile setzen
        wLogFile->setText(logFile);           // Name zeigen
        getHistoryLast();                     // data_Last zeigen
      }
     else  // ----------------------------------
      if(com == 6) {                          // von ReportList wawdlist verändert
        QSqlQuery query;
        AwdBox->clear();
        i = AwdBox->currentIndex();           // Pos Awd_Box sichern
        qy = "SELECT atype FROM wawdlist WHERE aset!='0' ORDER BY aset";
        query.exec(qy);
        while(query.next())
          AwdBox->insertItem (i++, query.value(0).toString());   // ++ hash_tabelle aufbauen  ???
       }
      else  // ------------------------------ // message vom realtime UploadMGR
       if(com == 1) {                         // qso ist von uploadMgrEqsl gesendet ?
         labelUp->setText(" ");               // lösche up-flg
         if(Mid.indexOf("ERROR") != -1) {     // check auf ERROR - ist ok - ignore
           QMessageBox::information( this,
           tr("UploadMgr Eqsl"),("ERROR: No match on eQSL_User/eQSL_Pswd\nUploadMgr is unload."),
           QMessageBox::Default); 
           qslViaEqslSetOff();                // set_buero qsl_via_buero
           upMgr = 0; 
         }
       }
      else   // --------------------------------------
       if(com == 9) {                         // Info vom uploadMgrEqsl - Netzwerk_Problem
          QMessageBox::information( this,
          tr("UploadMgr Eqsl"),
          tr("\nEin Netzwerk Timeout Signal ist eingegangen.\nDas Netzwerk ist gestoert, der eQSL.cc Server meldet sich nicht.\nDer Upload zum Server wurde eingestellt."),
          QMessageBox::Default);
       }
   }
}

// schreibe text in QLineEdit_widget
// -----------------------------------------------
void addQso::setText(QString token, QString text)
{
      if(token.compare("Name") == 0)
          wName->setText(text);
      else
        if(token.compare("Qth") == 0)
          wQth->setText(text);
      else
        if(token.compare("Kommentar") == 0)
          wKomEdit->setText(text);
      else
        if(token.compare("Notiz") == 0)
          wNotizEdit->setText(text);
      else
        if(token.compare("QslMgr") == 0)
          wQslMgr->setText(text);
}

// Aufuf vom timer_dxCall_Cluster : Call vom DX_Cluster in Rufz_str eintagen
// -------------------------------------------------------------------------
void addQso::putDxCall()
{
     QString f;
     if(dxCall.count() == 0)
       dxtimer->stop();
     else {
       f = wCall->text();
       f += dxCall.left(1);
       callEdited(f);
       dxCall = dxCall.remove(0,1);
     }
}


// -----------------------------------------------------------------------
// Datagram : WRITE datagram_message
// -----------------------------------------------------------------------
void addQso::processWriteDatagram(int dNr, QString idNr, int port)
{
     datagram = QByteArray::number(dNr);
     datagram.append(",");
     datagram.append(idNr);
     udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, port);       
}




// ==========================================================================================
// ==========================================================================================
// hamlib TCP_socket
// neue Rig_Abfrage initieren - wird vom 'p_timer' aufgerufen - poll_interVal default 1000 mS
// ------------------------------------------------------------------------------------------
void addQso::requestRigNew()
{
   if( rigpolling ) {             // wenn poling nicht gesperrt
     if(done) {                   // und done=1; alles erledigt ist
       if(cmdFlg) {               // SET_MODUS 1 = Frequenz vom DXspot
         i = cmdStr.indexOf("M"); // teile cmdStr F 14999200 /  M LSB/2400 
         if(i > 3) {                      // in diesem Fall geht es so !!
           cmd = cmdStr.left(i); 
           cmd = cmd.trimmed();           // Frequenz
           cmdStr = cmdStr.remove(0, i);  // Seitenband "M LSB/2400"
         }
         else {                           // i == 0    
          cmd = cmdStr;                   // setze neue Frequenz oder Seitenband
          cmdStr = "";
         }
         ptimer->stop();
         ptimer->start(200);              // kurze Laufzeit
       }
       else {                      // POLL_Mode, kein SET_flg gesetzt
         if(poll)                  // 1
           cmd = "f";              // poll = 1 frequenz
         else                      // poll = 0
           cmd = "m";              // 
       }
       done = 0;                   // neuer Abfrage- oder Set_zyklus läuft
     }
     //else
     
     slotFlg = 1;
     tcpSocket->abort();
     tcpSocket->connectToHost(tcphost,tcpport.toInt());
   }
}


// -------------------------------------------------------
// tcpSocket SIGNAL connected()    - sende jetzt Command
// -------------------------------------------------------
void addQso::sentCommand()
{
    QString message;
    if(rigpolling) {
       message = cmd;
       tcpSocket->write(message.toAscii());
       tcpSocket->flush();
       
       if(cmdFlg) {
         done = 1;                  // set_cmd gesendet
         cmdFlg--;                  // bei 0, command erledigt
       }
    }
}


// -------------------------------------------------------------------
// Antwort vom Server lesen         - SIGNAL readyRead() eingegangen
// -------------------------------------------------------------------
void addQso::readRigReply()
{
    QString st;
    QString message(tcpSocket->readAll());      // hole message
    
    if(message.indexOf("RPRT") != -1) {
      ptimer->stop();
      rigpolling = 0;
      ModeBox->setEnabled(true);                // freigeben
      checkBoxStop->setPalette(QPalette(QColor(224,224,224)));   // grau
      QMessageBox::information(this,tr("Hamlib Client"),
                   tr("\nError ocuured timedout.\n"
                   "Check the transceiver connection.\n"
                   "Hardware unavailable !\n\n"
                   "* Server stopped. *"));
      return;
    }
    
    if( poll ) {                               // 1 = GOT Freq
      message = message.simplified();          // ------------
      i = message.count();
      st = message.left(i -2);                 // in kHz
      st.insert(i -3, QChar('.'));
      wFreq->setText(st);                      // Band in kHz anzeigen
      st = st.left(i -6);                      // Band in MHz
      
      if(st.indexOf(lastBand) == -1) {         // Band geändert
         slotFlg = 0;
         BandBox->setCurrentIndex(BandBox->findText(getBand(st)));
         BandIdx = BandBox->currentIndex();
         
         lastBand = st;                        // 14 MHz
      }
      checkBoxStop->setPalette(QPalette(QColor(164,179,167)));   // gruen
      poll = 0;                                 // next: is MODE
    }
    else {                                     // 0 = GOT Mode
      i = message.indexOf("\n");               // ------------
      st = message.left(i);
      
      if(st.compare(lastMode) != 0) {
        n = ModeBox->findText(getMode(st));    // stelle Mode_ Button
        ModeBox->setCurrentIndex(n);
        
        lastMode = st;
      }
      poll = 1;                                // next: is Freq
      checkBoxStop->setPalette(QPalette(QColor(224,224,224)));  // grau
    }
    
    done = 1; 
    ptimer->start(retry); 
}

// Client FEHLER Meldungen
// ----------------------------------------------------------------
void addQso::displayError(QAbstractSocket::SocketError socketError)
{
  rigpolling = 0;                 
  ptimer->stop();
  
  switch(socketError) {
    case QAbstractSocket::RemoteHostClosedError:
      break;
    case QAbstractSocket::HostNotFoundError:
         QMessageBox::information(this,tr("Hamlib Client"),
                                  tr("\nThe host was not found.\n"
                                  "Please check the host name and port setting."));
      break;
    case QAbstractSocket::ConnectionRefusedError:
         QMessageBox::information(this,tr("Hamlib Client"),
                                  tr("\nThe connection was refused by the peer.\n\n"
                                  "Make sure the hamlib server is running.\n"
                                  "and check that the host name and port settings\n"
                                  "are correct."));
      break;
    default: 
         QMessageBox::information(this,tr("Hamlib Client"),
                                  tr("\nThe folloing error ocuured: %1.")
                                  .arg(tcpSocket->errorString()));
  }
  checkBoxStop->setPalette(QPalette(QColor(224,224,224)));  // grau
  ModeBox->setEnabled(true);                                // freigeben
  checkBoxStop->setText("Rctl off");
}

// =============================================================================================
void addQso::starthamlibServer()
{
     QString home, p, pr, q, rigPid;
     int ok, i, n, proz;
     ok = 1;
     
     home = getenv("HOME");                 // prüfe ob rigctld bereits lauft - hole Prozess_pid
     p = "ps xa | grep rigctld > "+home+"/.qtlog/rigstatus";
     i = system(p.toAscii());               // suche rig_prozess
     home += "/.qtlog/rigstatus";           // prüfe rigstatus
     QFile iniFile(home);
     iniFile.open(QIODevice::ReadOnly);
     QTextStream istream( &iniFile);        // oeffne rigstatus
     
     while(istream.atEnd() != true  ) { 
       pr = istream.readLine(0);             // eine Zeile
       if(pr.indexOf("rigctld -m") != -1) break;
     }
     iniFile.close();
     
     proz = pr.indexOf("rigctld -m");
     if( proz != -1 ) {                      // gibt es einen rigCtl_proz ?
       if( retry ) {                         // ja; gibt es einen gültigen rig_diskriptoz
         if( runing ) {                      // ja; steht hamlib auf aktiv
           ModeBox->setEnabled(false);       // ja; Mode sperren ( wird vom Tx/Rx gesetzt )
	   checkBoxStop->setText("Rctl on"); // rigctld läuft
           rigpolling = runing;
           poll = 1;                         // 1 = poll Frequenz
           cmdFlg = 0;
           cmd = "f";
           done = 1;
           ptimer->start(retry);             // starte polling_zyklus
         }
       }
       else {  // rigCtl_discriptor ungültig, lade die Proz_parameter in den Diskriptor, starte polling
         //usr/local/bin/rigctld -m 357 -r /dev/ttyUSB0 -s 19200 -p /dev/ttyUSB0 -C rts_state=OFF -C dtr_state=OFF"
          i = pr.indexOf("-m");             // lese alle parameter vom rigCtl_proz
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          q = "UPDATE wproz SET rigpid='";
          rigPid = pr.mid(i, n -i);         // mPid = 357
          q += rigPid;
          
          i = pr.indexOf("-r");
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          q += "',rigdev='"+pr.mid(i, n -i); // /dev/ttyUSB0
          
          i = pr.indexOf("-s");
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          q += "',baud='"+pr.mid(i, n -i);   // 19200
          
          i = pr.indexOf("-p");
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          q += "',pttdev='"+pr.mid(i, n -i); // /dev/ttyUSB0
          
          i = pr.indexOf("-C");              // par_val
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          p = pr.mid(i, n -i); 
          n = p.count();
          i = p.indexOf("=");
          q += "',rts='"+p.mid(i +1, n -i);  // RTS
          
          i = pr.indexOf("-C", n);           // parm_val
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          p = pr.mid(i , n -i);
          n = p.count();
          i = p.indexOf("=");
          q += "',dtr='"+p.mid(i +1, n -i);   // DTR
          q += "',owner='hamlb";
          q += "',rig='"+getRigModelName(rigPid);
          q += "',retry='1000',runing=1 WHERE proz='HamLib'";
          QSqlQuery query(q);     // sichere rig_diskriptor
	  
          ModeBox->setEnabled(false);        // Mode sperren ( wird vom Tx/Rx gesetzt )
	  checkBoxStop->setText("Rctl on");  // rigctld läuft
          runing = 1;
          rigpolling = runing;
          poll = runing;                     // 1 = poll Frequenz
          cmdFlg = 0;
          cmd = "f";                         // Frequenz
          done = 1;
          retry = 1000;                      // default
          ptimer->start(retry);              // state polling 
     }
   }
   else {                                    // kein rigCtl_proz vorhanden
         // starte proz mit den parametern aus dem diskriptor
     if(owner.count() != 0) {  // gibt es einen gültigen rigCtl_diskriptor ?
       if( runing ) {          // ja; soll gepollt werden ?
    
          QSqlQuery query;     // starte rigctld ...... proz
          qy = "SELECT rigpid,rigdev,baud,pttdev,rts,dtr FROM wproz WHERE proz='HamLib'";
          query.exec(qy);
          query.next();
          n = 0;
          p = "rigctld -m "+query.value(n++).toString();     // RigPid
       
          p += " -r "+query.value(n++).toString();           // RigDev
          p += " -s "+query.value(n++).toString();           // Baud
          p += " -p "+query.value(n++).toString();           // PttDev
       
          p += " -C rts_state="+query.value(n++).toString(); // RTS=OFF
          p += " -C dtr_state="+query.value(n++).toString(); // DTR=OFF
          p += " &";
          i = system(p.toAscii());  // rigctld -m 357 -r /dev/ttyUSB0 -s 19200 -p /dev/ttyUSB0 -C  -C 
        
          ModeBox->setEnabled(false);         // Mode sperren ( wird vom Tx/Rx gesetzt )
	  checkBoxStop->setText("Rctl on");   // rigctld läuft
          rigpolling = runing;
          poll = runing;                      // 1 = poll Frequenz
          cmdFlg = 0;
          cmd = "f";                          // Frequenz
          done = 1;
          ptimer->start(retry);               // starte polling  
       }
     }
   }
   checkBoxStop->setPalette(QPalette(QColor(164,179,167)));  // gruen
   i = system("getmgrdb &");
}

// hole für rigPid den GeräteNamen
// -------------------------------------------
QString addQso::getRigModelName(QString rigId)
{
     QString home;
  
     home = getenv("HOME");              // suche nach rigctl
     s = "rigctl -l > ";
     s += home;
     s +="/.qtlog/rigstatus";
     i = system(s.toAscii());
     home += "/.qtlog/rigstatus";        // versuche die die Rig-Modelle zu laden
     QFile inFile(home);
     inFile.open(QIODevice::ReadOnly);
 
     QTextStream istream( &inFile);      // oeffne file, lade alle Rig-Modelle
     s = istream.readLine(0);            // 1.Zeile überspringen - ist header
     while(istream.atEnd() != true  ) {  // alle folgenden Zeilen bearbeiten
        x = 0;
        s = istream.readLine(0);         // 2.Zeile
        x = s.indexOf("\t");
        t = s.left(x);
        t = t.simplified();
        if(t.compare(rigId) == 0)        // suche rigpid
           break;                        // gefunden
     }
 
     s.remove(0,x);
     x = 16;
     t = s.left(x);
     t = t.simplified();                 // Mfg
     s.remove(0,x);
     x = 24;
     t = s.left(x);
     t = t.simplified();                 // rig_modell_name
  return t;                              // rig_name
}

// ==============================================
void addQso::rigctlAc()
{
    rigctl * Rigctl = new rigctl();
    Rigctl->exec();   
    
    if(Rigctl->updateFlg == 1) {            // server neu starten
      ptimer->stop();                       // polling_timer stop
      
      ModeBox->setEnabled(false);           // ModeBox sperren
      checkBoxStop->setText("Rctl on");
      checkBoxStop->setPalette(QPalette(QColor(164,179,167)));  // gruen
      runing = Rigctl->Runing;              // neuer Poll_Zyklus
      rigpolling = runing;
      retry = Rigctl->Retry;
      ctl = 1;
      ptimer->start(retry);                 // polling_timer neu starten
      starthamlibServer();                  // weil hamlb !
    }
    else
    if(Rigctl->updateFlg == 2) {            // server_polling einstellen/beenden
      rigpolling = 0;
      retry = 0;
      ptimer->stop();                       // polling_timer stop
      ModeBox->setEnabled(true);            // Mode freigeben
      checkBoxStop->setText("Rctl off");    // kein Rigctl
      checkBoxStop->setPalette(QPalette(QColor(224,224,224)));  // grau
    }
}

// -----------------------------------------
void addQso::realtimeUpAc()
{
    uploadR * UploadR = new uploadR();
    UploadR->exec();   
    
    if(UploadR->upLd == 1) {                // upload ermöglichen
      settings.setValue("QSLvia",4);        // E - starte server 
      startUploadMgrR(1);
    }
    else {
      settings.setValue("QSLvia",1);        // B
    }
    setQslDefault();
}


// ------------------------------------------------------
// realtime UploadMgr_server 100 mSek. verzögert starten
// ------------------------------------------------------
int addQso::startUploadMgrR(int status)
{
  QString home, pr, p;
  int proz;
  
     if( status ) {                           // status = 1;
       home = getenv("HOME"); 
       p = "ps -e | grep uploadEqsldb > "+home+"/.qtlog/rigstatus";
       i = system(p.toAscii());               // suche prozess
       home += "/.qtlog/rigstatus";           // prüfe status
       QFile iniFile(home);
       iniFile.open(QIODevice::ReadOnly);
       QTextStream istream( &iniFile);        // oeffne status_file
       while(istream.atEnd() != true  ) { 
         pr = istream.readLine(0);             // eine Zeile
         if(pr.indexOf("uploadEqsldb") != -1) break;
       }
       
       iniFile.close();
       proz = pr.indexOf("uploadEqsldb");
     
       if( proz != -1 ) {                     // gibt es bereits einen uploadEqsldb_proz ?
         upMgr = 1;                           // ja;
       }
       else {                                 // nein; uploadEqsldb nicht vorhanden
	 if(settings.value("EqslRealtime").toString().toInt()) {
           if(settings.value("EqslPasswd").toString().count() != 0) { // das passwd vorhanden is
             //s = "uploadEqsldb 45458 &";      // uploadMgrEqslR_ON - 45458 for bind
             i = system("uploadEqsldb 45458 &");    // uploadMgrEqsl start
             upMgr = 1;
             qslMsg(rmess);                   // ist gestartet - Message uploadEqslR_ON
           }
	 }
       }
     }
     else {                                   // proz löschen
       processWriteDatagram(-1, "-1", 45458); // uploadEqslR_OFF
       upMgr = 0;
     }
     return upMgr;
}
     
// --------------------------------------------
void addQso::qslMsg(int mess)
{
   if(mess == 1) {
     rmess = 0;                                               // info nur 1 x ausgeben
     QMessageBox::information(this, tr("RealTime EqslMgr"),
     tr("\neQSL Realtime UploadMgr gestartet\n\nIst Qsl via auf eQSL gestellt wird jedes gelogte QSO sofort an eQsl.cc gesendet.\nMit F3 kann der Modus fuer 1 Qso veraendert werden.\n\nFalls kein eQSL Realtimeupload erfogen soll,\nentferne das Internet Freigabeflag im Config.\n"),
     QMessageBox::Ok);
   }
   else 
   if(mess == 2) {
     QMessageBox::information(this, tr("RealTime EqslMgr"),  
     tr("\nRealtime Upload nicht moeglich\n\nDas Passwort fehlt"),
     QMessageBox::Ok);
   }
}


// überarbeiten !!!
// aufruf vom INSERT_qso - Upload_Fehler aufgetreten, UpMgr ist abgeschaltet
// -------------------------------------------------------------------------
void addQso::qslViaEqslSetOff()
{
     checkBoxEqsl->setChecked(FALSE);       // qsl_via Eqsl aus
     checkBoxBuero->setChecked(TRUE);
     settings.setValue("QSLvia",1);         // jetzt Qsl_via Büro
     //settings.setValue("QSLs",1);         // gesetzt R
}



