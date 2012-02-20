/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.05.2007
********************************************************************** */

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
#include "../qtlogDiag/dirmngr.h"
#include "rigctl.h"
#include "uploadR.h"

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
   
   
   
   connect(actionRigCtl, SIGNAL(triggered(bool)), this, SLOT(rigctlAc()));
   
   connect(actionHilfeQsoEingabe, SIGNAL(triggered(bool)), this, SLOT(hilfeQsoEingabeAc()));
   connect(actionHilfeRigControl, SIGNAL(triggered(bool)), this, SLOT(hilfeRigCtlAc()));
   connect(actionHilfeDxSpot, SIGNAL(triggered(bool)), this, SLOT(hilfeDxSpotAc()));

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

   QDate heute = QDate::currentDate();                  
   s = tr(" Login :  ");
   s += t.setNum(heute.day());
   s += "."+t.setNum(heute.month());
   s += "."+t.setNum(heute.year());
   rtime = QDateTime::currentDateTime().toUTC().time(); 

   QDateTime h = QDateTime::currentDateTime().toUTC();
   dateEditStart->setDate(h.date());
   timeEditStart->setTime (h.time());
   s += " - "+h.time().toString();
   wStartLogin->setText(s);                             
   toDay = h.date().toString("yyyy-MM-dd");
   
   udpSocket = new QUdpSocket(this);                    
   udpSocket->bind(QHostAddress::LocalHost,45450);
   connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
   
   tcpSocket = new QTcpSocket(this);                    
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

   if(checkBoxRSTe->checkState())
       wRSTe->setText(comboBoxRSTe->currentText());
   if(checkBoxRSTg->checkState())
      wRSTg->setText(comboBoxRSTg->currentText());

   if(settings.value("Endtime").toString().toInt() == 1)
      checkBoxEnd->setChecked(TRUE);
   checkQsoEndeChanged();

   s = tr("QTH - Locator ");                     
   s += settings.value("Locator").toString();
   labelQthLocator->setText(s);
   i = settings.value("gKoordinaten").toInt();
   if( i ) {
      checkBoxLocator->setChecked(true);         
      s = settings.value("Locator").toString();
      distanz.setHomeCoordinatenFromLoc(s);
    }
    else {
      checkBoxLocator->setChecked(false);        
      s = settings.value("Breite").toString();   
      t = settings.value("Laenge").toString();
      distanz.setHomeCoordinaten(s,t);
    }

    QSqlQuery query;                             
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

    serv.installHashTableBand();                  

    BandBox->clear();
    QString mBand, mFreq;
    i = 0;                                        
    qy = "SELECT mband,hfreq FROM wband WHERE work!=0";
    query.exec(qy);
    while(query.next()) {
      mBand = query.value(0).toString();
      BandBox->insertItem(i++,mBand);            
      mFreq = query.value(1).toString();
      FreqToBand.insert(mFreq,mBand);            
    }
    
    BandBox->setCurrentIndex(BandBox->findText(settings.value("Band").toString()));
    BandIdx = BandBox->currentIndex();           
    
    qy = "SELECT hfreq FROM wband WHERE mband='"+settings.value("Band").toString()+"'";
    query.exec(qy);
    query.next();
    lastBand = query.value(0).toString();
    connect(BandBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(BandBoxChangedCb(QString)));

    ModeBox->clear();
    i = 0;                                        
    qy = "SELECT mode FROM wmode WHERE work!=0";
    query.exec(qy);
    while(query.next()) 
       ModeBox->insertItem (i++, query.value(0).toString());
    ModeBox->setCurrentIndex(ModeBox->findText(settings.value("Mode").toString()));
    ModeIdx = ModeBox->currentIndex();           
    connect(ModeBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ModeBoxChangedCb(QString)));

    LineBox->clear();                            
    i = 0;
    qy = "SELECT line FROM wline ORDER BY id";
    query.exec(qy);
    while(query.next())
       LineBox->insertItem (i++, query.value(0).toString());
    LineBox->setCurrentIndex(settings.value("Line").toInt());

    AwdBox->clear();
    i = 0;
    qy = "SELECT atype FROM wawdlist WHERE aset !='0' ORDER BY id";
    query.exec(qy);
    while(query.next())
       AwdBox->insertItem (i++, query.value(0).toString());     
    AwdBox->setCurrentIndex(settings.value("Awd").toInt());     
    qsoList->headerItem()->setText(10,AwdBox->currentText());   
    connect (AwdBox, SIGNAL(activated(int)), this, SLOT(awdBoxChanged(int)));

    qy = "SELECT watt FROM wpwr WHERE work!=0 ORDER BY work";   
    PwrBox->clear();
    i = 0;                                                      
    query.exec(qy);
    while(query.next())
       PwrBox->insertItem (i++, query.value(0).toString());
    PwrBox->setCurrentIndex(settings.value("Pwr").toInt()); 

    qy = "SELECT rigtype FROM wrig";              
    RigBox->clear();
    i = 0;                                        
    query.exec(qy);
    while(query.next()) 
       RigBox->insertItem (i++, query.value(0).toString());
    s =  BandBox->currentText();
    qy = "SELECT brig FROM wband WHERE mband='"+s+"'";   
    query.exec(qy);
    query.next();
    t = query.value(0).toString();
    if(t.count() != 0)                                  
        RigBox->setCurrentIndex(RigBox->findText(t) );
    else
     RigBox->setCurrentIndex(settings.value("Rig").toString().toInt());         

    wCallEdit->setText(settings.value("Call").toString());                      
    comboBoxRSTe->setCurrentIndex(settings.value("RSTe").toInt());   
    comboBoxRSTg->setCurrentIndex(settings.value("RSTg").toInt());   
    comboBoxSuche->setCurrentIndex(settings.value("Suche").toInt()); 
    lineEditOM->setText(settings.value("Salutation").toString());
    qy = "SELECT refnam FROM refnamen WHERE dbfield LIKE 'custom%'";            
    query.exec(qy);
    query.next();
    s = query.value(0).toString();       
    if(s.count() != 0)
      label_custo1->setText(s);
   
    query.next();
    s = query.value(0).toString();       
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
    
    retry = 0;                           
    rigpolling = 0;                      
    runing = 0;                          
    ctl = 0;                             
    slotFlg = 1;
    qy = "SELECT owner,host,port,retry,runing FROM wproz WHERE proz='HamLib'";
    query.exec(qy);
    if(query.next()) {
      owner = query.value(0).toString();        
      tcphost = query.value(1).toString();      
      tcpport = query.value(2).toString();      
      retry = query.value(3).toInt();           
      ctl = query.value(4).toString().toInt();  
      runing = ctl;                             
    }
    
    qsoList->setColumnWidth(0,110); 
    qsoList->setColumnWidth(1,70);  
    qsoList->setColumnWidth(2,110); 
    qsoList->setColumnWidth(3,100); 
    qsoList->setColumnWidth(4,120); 
    qsoList->setColumnWidth(5,50);  
    qsoList->setColumnWidth(6,50);  
    qsoList->setColumnWidth(7,50);  
    qsoList->setColumnWidth(8,46);  
    qsoList->setColumnWidth(9,50);  
    qsoList->setColumnWidth(10,60); 
    qsoList->setColumnWidth(11,70); 
    uscountyList->setColumnWidth(0,80);
    uscountyList->setColumnWidth(1,200);
    upMgr = 0;                      
    qslflg = 1;                     
    rmess = 1;                      
    s = comboBoxMax->currentText();
    s = s.mid(5);
    Last = s.trimmed();
    id = 0;
    tflg = 0;                       
    hisflg = 0;                     
    uaflg = 0;                      
    editflg = 0;                    
    updateflg = 0;                  
    flgf6 = 0;                      
    omId = 0;                       
    Rufz = "";                      
    tabWidget ->setCurrentIndex(0);
    tflg = tabWidget ->currentIndex();
    last = tr("(F5) Historie");
    qso  = tr("(F5) Eingabe");
    ButtonHistory->setText(last);
    wCall->setFocus();
    ButtonUpdate->hide();
    ButtonBack->hide();
    ButtonDel->hide();
    getHistoryLast();                        
    dial->setSliderPosition(180);
    setQslTabel();                           
    setAwdOmList();
    setQslDefault();                         
    
    connect(checkBoxEqsl, SIGNAL(clicked()), this, SLOT(qslviaChanged()));
    connect(checkBoxBuero, SIGNAL(clicked()), this, SLOT(qslviaChanged()));
    
    timer = new QTimer(this);                
    connect(timer,SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    showTime();                              
 
    QTimer::singleShot(100, this, SLOT(timerService()));      
    
    dxtimer = new QTimer(this);                               
    connect(dxtimer, SIGNAL(timeout()), this, SLOT(putDxCall()));
    ptimer = new QTimer(this);                                
    connect(ptimer, SIGNAL(timeout()), this, SLOT(requestRigNew()));
    QTimer::singleShot(10, this, SLOT(starthamlibServer()));  
   
}  

addQso::~addQso()
{
   writeSettings();
   processWriteDatagram(-1,"-1",45458);     
   processWriteDatagram(-1,"",45460);       
}


//--------------------------------------------------------------
void addQso::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F1 : {                              
                     if(updateflg)
                       break;                         
                     startQso();                      
                  }
            break;
     case Qt::Key_F2 : {
                     if(tabWidget->currentIndex() == 6) { 
                        getQrzComData();              
                        setColor(0);
                     }
                     else {
                       if(updateflg)
                         buttonUpdateCb();            
                       else 
                        addNewQso();                  
                     }
                     tabWidget ->setCurrentIndex(0);  
                  }
            break;
     case Qt::Key_F3 :                                
                 toggelQslsent();
            break;
     case Qt::Key_F4 : {                              
                     if( tflg ) {
                       tabWidget ->setCurrentIndex(0);
                       wCall->setFocus();
                       tflg = 0;
                     }
                     else {
                      tabWidget ->setCurrentIndex(1); 
                      wNotizEdit->setFocus();
                      tflg = 1;
                     }
                     setQslDefault();
                  }
            break;
     case Qt::Key_F5 : {                              
                     tabWidget->setCurrentIndex(0);
                     qsoList->clear();
                     if( hisflg ) {
                       ButtonHistory->setText(last);  
                       getHistoryLast();
                       hisflg = 0;
                     }
                     else {
                      ButtonHistory->setText(qso);    
                      getHistoryPref();
                      hisflg = 1;
                    }
                  }
            break;
     case Qt::Key_F6 : {                                   
                     if(comboBoxSuche->currentIndex() == 0) {
		       if( !flgf6 ) {                      
		          if(wCall->text().count() != 0) { 
			    flgf6 = 1;                     
			    processWriteDatagram(1, wCall->text(), 45460);
			    setColor(1);                   
			  }
		       }
		       else {
			 flgf6 = 0;                        
			 setColor(0);
			 clearQrzData(); 
			 tabWidget->setCurrentIndex(0);
		       }
		     }
                     else
                     if(comboBoxSuche->currentIndex() == 1) 
                       getQrzDbCall(wCall->text());         
                  }
            break;
     case Qt::Key_F7 : {                                   
                     if(tflg) {
                       tabWidget->setCurrentIndex(0);      
                       tflg = 0;
                       wCall->setFocus(); 
                     }
                     else {
                      tabWidget->setCurrentIndex(2);       
                      tflg = 2;
                      wAwd->setFocus(); 
                      getAdwKennerList();
                     }
                  }
            break;
     case Qt::Key_F8 : {                                  
                     if(tflg) {
                       tabWidget->setCurrentIndex(0);     
                       tflg = 0;
                     }
                     else {
                      tabWidget->setCurrentIndex(3);      
                       tflg = 3;
                       getQslStatus();
                     }
                  }
            break;
     case Qt::Key_F9 : {                                  
                     buttonBackCb();
                     resetAddMask();                      
                     tabWidget ->setCurrentIndex(0);
                  }
            break;
     case Qt::Key_F10 :
                      goExit();
            break;
     case Qt::Key_Escape : {                              
                         tabWidget ->setCurrentIndex(0);  
                         tflg = 0;
                         buttonBackCb();
                         wCall->setFocus();               
                         if(flgf6) {
                           setColor(0);                   
                           clearQrzData();                
                           flgf6 = 0;                     
                         }
                  }
            break;
   }
}

void addQso::goExit()                                     
{
   db.close();
   qApp->quit();
}


void addQso::qsoVerwaltungAc()
{
    StartProcess("qtlog &");
}


void addQso::qslBearbeitungAc()
{
    StartProcess("qsldb &");
}

void addQso::dxSpotAc()
{
    StartProcess("dxspotdb &");
}

void addQso::dumpLogbookAc()
{
  StartProcess("admindb &");
}


void addQso::hilfeQsoEingabeAc()
{
    settings.setValue("Val","Qso Eingabe");
    StartProcess("hilfedb &");
}


void addQso::hilfeRigCtlAc()
{
    settings.setValue("Val","Rig Control-Hamlib");
    StartProcess("hilfedb &");
}


void addQso::hilfeDxSpotAc()
{
    settings.setValue("Val","Dx Spot");
    StartProcess("hilfedb &");
}


void addQso::showTime()                    
{                                          
  rtime = QDateTime::currentDateTime().toUTC().time();
  timeEditEnd->setTime(rtime);
  if(rtime.hour() == 24) {                 
     QDateTime h = QDateTime::currentDateTime().toUTC();
     dateEditStart->setDate(h.date());     
  }
}


void addQso::uhrStop(int state)
{
  if( state ) {                                 
    timer->stop();                                        
    checkBoxUhr->setPalette(QPalette(QColor(255,90,14))); 
    checkBoxUhr->setText(tr("Uhr..steht"));
    ModeBox->setEnabled(true);                            
    rigpolling = 0;                                       
    checkBoxStop->setPalette(QPalette(QColor(232,58,5))); 
    //checkBoxStop->setChecked(TRUE);                     
    checkBoxStop->setText("Rctl off");
   }
  else {
    timer->start(1000);                                   
    checkBoxUhr->setPalette(QPalette(QColor(224,224,224)));
    checkBoxUhr->setText(tr("Uhr..aktiv"));
    QDate heute = QDate::currentDate();                   
    dateEditStart->setDate(heute);                        
    
    ModeBox->setEnabled(false);                                      
    rigpolling = runing;                                             
    slotFlg = 1;                                                     
    BandBox->setCurrentIndex(BandBox->findText(getBand(lastBand)));  
    
    checkBoxStop->setPalette(QPalette(QColor(224,224,224)));         
    checkBoxStop->setText("Rctl on");
  }
}

void addQso::timerService()
{
    setQslDefault();                           
    startUploadMgrR(1);                        
}


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


void addQso::readSettings()
{
    resize(settings.value("addQso/Size",sizeHint()).toSize());
    restoreState(settings.value("addQso/Properties").toByteArray());
    comboBoxMax->setCurrentIndex(settings.value("Max").toString().toInt());
}


void addQso::tabChanged(int index)         
{
     tflg = index;                         
     if( tflg == 0 ) {
        tabWidget ->setCurrentIndex(0);
        wCall->setFocus();
        return;
     }
     if( tflg == 1 ) {                     
         wNotizEdit->setFocus();
        return;
     }
     if( tflg == 2 ) {                     
        wAwd->setFocus(); 
        getAdwKennerList();
        wEditContestId->setFocus();
     }
     if( tflg == 3 ) {                     
        getQslStatus();
        wZielLocator->setFocus();
     }
}

void addQso::clearQrzData()
{
     lineEditQrzCall->setText("");
     lineEditQrz_1->setText("");
     lineEditQrz_2->setText("");
     lineEditQrz_3->setText("");
     lineEditQrz_4->setText("");
     labelError->setText("");
}

void addQso::setColor(int n)
{
     QPalette palette5;
     if( n ) {                              
        QBrush brush7(QColor(255,0,0,255));
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        labelSuche->setPalette(palette5);
        labelSuche->setText("QRZ.com");
     }
     else {                                 
        QBrush brush7(QColor(48,48,48,255));
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush7);
        labelSuche->setPalette(palette5);
        labelSuche->setText("(F6) Suche");
     }
}


void addQso::getQrzComData()
{                                                                         
       QLineEdit * wLine = getEditLine(qBox_1->currentText().trimmed());  
       wLine->setText(lineEditQrz_1->text());
       
       if(qBox_2->currentText().trimmed().compare("-") != 0) {            
          wLine = getEditLine(qBox_2->currentText().trimmed());
          wLine->setText(lineEditQrz_2->text());
       }
       
       wLine = getEditLine(qBox_3->currentText().trimmed());              
       wLine->setText(lineEditQrz_3->text());
       if(qBox_4->currentText().trimmed().compare("-") != 0) {            
          wLine = getEditLine(qBox_4->currentText().trimmed());
          wLine->setText(lineEditQrz_4->text());
       }
}


void addQso::comboBoxMaxCb(QString str)
{
    s = str;
    s = s.mid(5);
    Last = s.trimmed();
    if( ! hisflg )
      getHistoryLast();                              
}


void addQso::BandBoxChangedCb(QString name)          
{
 QString f, b;
 
   if(slotFlg) {
     QSqlQuery query;
     qy = "SELECT freq,brig FROM wband WHERE mband='"+name+"'";
     query.exec(qy);
     query.next();
     f = query.value(0).toString();                  
     b = query.value(1).toString();                  
     
     if(b.count() != 0)
       RigBox->setCurrentIndex(RigBox->findText(b)); 
       if(rigpolling) {                              
         f += "000";
         if(f.toInt() / 1000000 < 10 )
            cmdStr = "F "+f+" M LSB 2400";
         else 
           cmdStr = "F "+f+"M USB 2400";
         cmdFlg = 2;
       }
       wFreq->setText(f);                            
   }
   slotFlg = 1;
}

void addQso::ModeBoxChangedCb(QString name)
{
  QString n = name;
}


void addQso::HomeBoxCb(int id)
{
   id++;
   s.setNum(id);
   HidW->setText(s);
   wCall->setFocus();
}


void addQso::comboBoxSucheCb(int idx)
{
     switch(idx) {
        case 1 : //getQrzDbCall(Rufz);        
                 break;
       default : break;                       
    }
}

void addQso::checkRSTeChanged(int state)
{
   if(state) {                                  
     wRSTe->setText(comboBoxRSTe->currentText());
     checkBoxRSTe->setText(tr("eintragen"));
   }
   else {
    wRSTe->setText("");
    checkBoxRSTe->setText("....");
   }
}

void addQso::checkRSTgChanged(int state)
{
   if(state) {                                  
     wRSTg->setText(comboBoxRSTg->currentText());
     checkBoxRSTg->setText("eintragen");
   }
   else {
    wRSTg->setText("");
    checkBoxRSTg->setText("....");
   }
}

void addQso::comboBoxRSTeChanged(QString s)
{
   if(checkBoxRSTe->isChecked() != false)
      wRSTe->setText(s);
}

void addQso::comboBoxRSTgChanged(QString s)
{
   if(checkBoxRSTg->isChecked() != false)
      wRSTg->setText(s);
}


void addQso::checkQsoEndeChanged()
{
   if(checkBoxEnd->isChecked() == true) {    
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




void addQso::toggelQslsent()
{ 
     switch( qslflg ) {                                     
      case 0 : break;
      case 1 :                                              
	       qslflg = 2;                                  
               checkBoxBuero->setChecked(FALSE);            
               checkBoxEqsl->setChecked(FALSE);             
               labelQslVia->setText(QString::fromUtf8("(F3) QSL via --")); 
	       upMgr = 0;
               break;
      case 2 :                                              
	       qslflg = 3;                                  
               checkBoxBuero->setChecked(TRUE);             
               checkBoxEqsl->setChecked(TRUE);              
               labelQslVia->setText(QString::fromUtf8("(F3) QSL via BE")); 
	       upMgr = settings.value("EqslRealtime").toString().toInt(); 
               break;
      case 3 :                                              
	       qslflg = 4;                                  
               checkBoxBuero->setChecked(FALSE);            
               checkBoxEqsl->setChecked(TRUE);              //
               labelQslVia->setText(QString::fromUtf8("(F3) QSL via E"));
	       upMgr = settings.value("EqslRealtime").toString().toInt(); 
               break;
      case 4 :                                              
	       qslflg = 1;                                  
               checkBoxBuero->setChecked(TRUE);             
               checkBoxEqsl->setChecked(FALSE);             
               labelQslVia->setText(QString::fromUtf8("(F3) QSL via Büro")); 
	       upMgr = 0;
               break;
     }
}



void addQso::setQslDefault()
{ 
   if(settings.value("QSLvia").toInt() == 1) {                        
     qslflg = 1;
     checkBoxBuero->setChecked(TRUE);
     checkBoxEqsl->setChecked(FALSE);
     labelQslVia->setText(QString::fromUtf8("(F3) Qsl Via Büro"));
     upMgr = 0;
   }
   else
     if(settings.value("QSLvia").toInt() == 2) {                      
       qslflg = 2;
       checkBoxBuero->setChecked(FALSE);
       checkBoxEqsl->setChecked(FALSE);
       labelQslVia->setText(QString::fromUtf8("(F3) Qsl Via --"));
       upMgr = 0;
     }
   else
     if(settings.value("QSLvia").toInt() == 3) {                      
       qslflg = 3;
       checkBoxBuero->setChecked(TRUE);
       checkBoxEqsl->setChecked(TRUE);
       labelQslVia->setText(QString::fromUtf8("(F3) Qsl Via B+E"));
       upMgr = settings.value("EqslRealtime").toString().toInt();     
     }
   else
     if(settings.value("QSLvia").toInt() == 4) {                      
       qslflg = 4;
       checkBoxBuero->setChecked(FALSE);
       checkBoxEqsl->setChecked(TRUE);
       labelQslVia->setText(QString::fromUtf8("(F3) Qsl via eQsl"));
       upMgr = settings.value("EqslRealtime").toString().toInt();     
     }
}
   



void addQso::qslviaChanged()
{
  if((checkBoxBuero->isChecked() == TRUE) && (checkBoxEqsl->isChecked() == TRUE)) {    
    settings.setValue("QSLvia",3);
  }
  else
    if((checkBoxBuero->isChecked() == FALSE) && (checkBoxEqsl->isChecked() ==FALSE)) { 
      settings.setValue("QSLvia",2);
    }
  else
    if((checkBoxBuero->isChecked() == FALSE) && (checkBoxEqsl->isChecked() == TRUE)) {  
      settings.setValue("QSLvia",4);
    }
  else
    if((checkBoxBuero->isChecked() == TRUE) && (checkBoxEqsl->isChecked() == FALSE)) {  
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

void addQso::checkKomChanged()
{
   if(checkBoxKom->isChecked() == false)               
    checkBoxKom->setText(tr("Nur zu diesem QSO"));
   else
     checkBoxKom->setText(tr("Globaler Kommentar"));   
}

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


void addQso::whileCRgoTab0()
{
   tabWidget->setCurrentIndex(0);
}


void addQso::notizCheckCR()
{
    wNotizEdit->text() =  wNotizEdit->text().simplified();
    if(updateflg)
       buttonUpdateCb();
    else
    tabWidget->setCurrentIndex(0);
}


void addQso::wOpCallCb(QString str)
{
   s = str.toUpper();
   wOpCall->setText(s);
}




void addQso::awdCb(QString si)
{
  int x, e;
  QString a;
     t = si.toUpper();                            
     wAwd->setText(t);
     s = AwdBox->currentText();                   
     if(s.compare("US-CA") == 0) {                
       if(t.count() == 2)                         
          awdhash.insert(s,t);                    
       else {
         e = t.indexOf('-',0);                    
         if(e == 2) {                             
           tabWidget ->setCurrentIndex(2);
           wAwd->setFocus();
            a = t.right(t.count() -e-1);
            if(a.count() >= 0) {                  
              qy = "SELECT state,county FROM uscounty WHERE state='"+si.left(e)+"' AND county LIKE '"+a+"%'";
              QSqlQuery query;
              query.exec(qy);
              i = query.size();                                    
              uscountyList->clear();
              while(query.next()) {
                n = 0;
                x = 0;
                QTreeWidgetItem *item = new QTreeWidgetItem(uscountyList);
                item->setText(x++,query.value(n++).toString());    
                f = query.value(n).toString();                     
                item->setText(x++,f);                              
                if( i == 1 ) {                                     
                   awdhash.insert(s,si.left(e+1)+f);               
                   QPalette palette8;                              
                   QBrush brush10(QColor(212, 255, 211, 255));
                   brush10.setStyle(Qt::SolidPattern);
                   palette8.setBrush(QPalette::Active, QPalette::Base, brush10);
                   palette8.setBrush(QPalette::Inactive, QPalette::Base, brush10);
                   QBrush brush11(QColor(244, 244, 244, 255));
                   brush11.setStyle(Qt::SolidPattern);
                   palette8.setBrush(QPalette::Disabled, QPalette::Base, brush11);
                   wAwd->setPalette(palette8);
                }
                else {                                             
                 QPalette palette11;                               
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
      awdhash.insert(s,t);         
      if(t.count() >= 3) {         
        x = 0;
        e = 0;
        QSqlQuery query;
        qy = "SELECT qslr FROM "+logFile+","+logFile+"awd WHERE id=qid ";
        qy += "AND awtype='"+AwdBox->currentText();
        qy += "' AND awkenner='"+t+"'";
        query.exec(qy);
        i = query.size();              
        if( i ) {                      
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


void addQso::awdBoxChanged(int n)
{
    n = 0;
    uscountyList->clear();                                    
    t = awdhash.value(AwdBox->currentText());                 
    wAwd->setText(t);                                         
    qsoList->headerItem()->setText(10,AwdBox->currentText()); 
    getHistoryLast();                                         
}
//--- IOTA_Box:  Eingabe ( AWD ) -  SIGNAL(textEdited(QString));

void addQso::iotaCb(QString s)
{
    t = s.toUpper();
    wIota->setText(t);
    s = IotaBox->currentText();                 
    s += t;                                     
    awdhash.insert("IOTA",s);                   
}


void addQso::qslMgrCb(QString s)
{
    t = s.toUpper();
    wQslMgr->setText(t);
}


void addQso::locatorCb(QString s)
{
    t = s.toUpper();
    wLocator->setText(t);
    if(t.count() == 6) {    
      labelDistanz->setText(distanz.calculateToLL(t));  
    }
    else
      labelDistanz->setText("");                         
}


void addQso::locatorZielCb(QString l)
{
    t = l.toUpper();
    wZielLocator->setText(t);
    if(t.count() == 6) {                              
      wZielEntfRichtung->setText(distanz.calculateToLL(t));
      dial->setSliderPosition(distanz.getRichtung()); 
    }
    else
     wZielEntfRichtung->setText("");
}


void addQso::startQso()                                    
{
    rtime = QDateTime::currentDateTime().toUTC().time();    
    timeEditStart->setTime(rtime);                          
}

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
    wSwl->setText("");                                
    wQslMgr->setText("");                             
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
    if(checkBoxKom->isChecked() == false)             
       wKomEdit->setText("");                         
    dial->setSliderPosition(180);
    i = 0;
    if(checkBoxRSTe->checkState())
        wRSTe->setText(comboBoxRSTe->currentText());
    if(checkBoxRSTg->checkState())
        wRSTg->setText(comboBoxRSTg->currentText());
    QHashIterator<QString,QString> i(awdhash);        
    while(i.hasNext()) {
       i.next();
       awdhash.insert(i.key(),"");                    
    }
    labelAWD->setText("0");                           
    setQslDefault();                                  
    id = 0;
    if(editflg) { 
      checkBoxUhr->setPalette(QPalette(QColor(224,224,224))); 
      checkBoxUhr->setChecked(FALSE);
    
      checkBoxStop->setChecked(FALSE);
      checkBoxStop->setPalette(QPalette(QColor(224,224,224))); 
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
    
    ButtonHistory->setText(qso);            
    hisflg = 1;
    wCall->setFocus();                      
}



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



int addQso::getla(QString str)
{
  int i,n,index;
    n = 0;
    i = 0;
    QSqlQuery query;                                       
    qy = "SELECT pref FROM tpref WHERE spref='"+str+"'";   
    query.exec(qy);
    i = query.size();                                      
    if( i ) {                                              
       query.next();
       s = query.value(0).toString();
       qy = "SELECT lname,lcn,br,lg,ldxcc,timez FROM tla WHERE la='"+s+"'"; 
       query.exec(qy);
       while(query.next()) {
          s = query.value(0).toString();
          wCntry->setText(s);                              
          s = query.value(1).toString();                   
          s +="-";
          index = IotaBox->findText(s);
          IotaBox->setCurrentIndex(index);                 
          s = query.value(2).toString();                   
          f = query.value(3).toString();                   
          labelDistanz->setText(distanz.getDisRicht(s,f)); 
          dial->setSliderPosition(distanz.getRichtung());  
          La = query.value(4).toString();                  
          labelTdif->setText(query.value(5).toString());   
          n++;
         }
      }
      return i;
}




void addQso::callEdited(QString str)
{
    if(str.isEmpty() == true) {
      if( hisflg) {
         qsoList->clear();                   
         resetAddMask();                     
      }
      id = 0;                                
      return;                                
    }
    if(editflg == 1) {         
        QMessageBox::information( this,
        tr("Rufzeichen Aenderung"),
        tr("Wird ein Rufzeichen veraendert,\noder auf einen CEPT-Prefix umgestellt.\nMuss es komplett neu eingegeben werden.\nSonst ist die Aenderung ohne Wirkung"),
        QMessageBox::Default,
        QMessageBox::NoButton, QMessageBox::NoButton);
        Cept = "";
        editflg = 2;
    }
    QTime timeN = timeEditEnd->time();        
    s = t.setNum(timeN.hour());               
    s += ":"+t.setNum(timeN.minute());
    s += ":"+t.setNum(timeN.second());
    timeN = QTime::fromString(s,"H:m:s");
    timeEditStart->setTime(timeN);            
    strlen = str.count();
    if( strlen == 1 ) {                       
       QPalette palette2;
       QBrush brush2(QColor(255, 85, 0, 255));
       brush2.setStyle(Qt::SolidPattern);
       palette2.setBrush(QPalette::Active, QPalette::WindowText, brush2);
       palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush2);
       QBrush brush3(QColor(119, 119, 119, 255));
       brush3.setStyle(Qt::SolidPattern);
       palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
       label_Station->setPalette(palette2);
       qsoList->clear();                      
       ButtonHistory->setText(qso);           
       hisflg = 1;
       id = 0;
    }
    t = str.toUpper();
    wCall->setText(t);                        

    x = t.indexOf('/',0);      
    if( x == -1 ) {            
       Rufz = t;               
       id = getla(Rufz);       
    }
    else {                     
     if( x <= 3 ) {
      Cept = t.left(x +1);     
      Rufz = t.mid(x +1);      
      wQslMgr->setText(Rufz);  
     }
     else {
       if(Cept.count() == 0)
        Rufz = t;
     }
    }

    if( !id ) {                
        
        
        
        qsoList->clear();
    }
    if( hisflg )                
       getHistoryPref();        
    if( strlen >= 4 ) {                   
      if(wName->text().count() == 0) {    
         switch(comboBoxSuche->currentIndex()) {
            case 1 :                      //qDebug() << "suche in QRZ_db";
	             getQrzDbCall(Rufz);  
                     break;
           default : break;               
         }
      }
    }
    
    
    
    
    
    
    
}



void addQso::getHistoryPref()
{
  int cnt;
     cnt = 0;
     if(!Rufz.count()) {             
        tQsofound->setText(s.setNum(cnt));
       return;
     }
     qsoList->clear();
     
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
    
     
     
     
     
     
     t =  AwdBox->currentText();
     
     qy = "SELECT id,oid,day,btime,cept,rufz,name,qth,band,mode,rstr,qslr,qsls,(SELECT awkenner FROM ";
     qy += logFile+"awd WHERE awtype = '"+t+"' AND id=qid),loc,ntext,contestid";
     qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
     qy += " WHERE rufz LIKE '"+Rufz+"%' AND dxcc='"+La+"' ORDER by day DESC,btime DESC LIMIT "+Last;
     query.exec(qy);
     x = query.size();
     while(query.next()) {                                 
        n = 0;
        i = 0;
        QTreeWidgetItem *item = new QTreeWidgetItem(qsoList);
        x = query.value(n++).toInt();                      
        Id = query.value(n++).toInt();                     
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        s = query.value(n++).toString();                   
        s += query.value(n++).toString();
        item->setText(i++,s);                              
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,serv.getMyband(query.value(n++).toString())); 
        item->setText(i++,query.value(n++).toString());    
        item->setText(i,query.value(n++).toString());      
        item->setTextAlignment(i++,Qt::AlignCenter);
        item->setText(i,query.value(n++).toString());      
        item->setTextAlignment(i++,Qt::AlignCenter);
        item->setText(i,query.value(n++).toString());      
        item->setTextAlignment(i++,Qt::AlignCenter);
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        cnt++;
     }
     
     if(omId) {                                           
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
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      s = query.value(n++).toString();                   
      s += query.value(n++).toString();                  
      item->setText(i++,s);                              
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,serv.getMyband(query.value(n++).toString())); 
      item->setText(i++,query.value(n++).toString());    
      item->setText(i,query.value(n++).toString());      
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(i,query.value(n++).toString());      
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(i,query.value(n++).toString());      
      item->setTextAlignment(i++,Qt::AlignCenter);
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      cnt++;
  }
  if(cnt == Last.toInt())
    tQsofound->setText(" max");
  else
   tQsofound->setText(s.setNum(cnt)+tr(" gefunden"));
}


void addQso::getQrzDbCall(QString call)
{
 QString c, ruf;
 int i = 0;
   c = call;
   i = c.indexOf("/",0);                
   if( i <= 3 )                         
      ruf = c.mid(i +1);                
   else
      ruf = c.left(i);                  
   qy = "SELECT fname,mcity FROM qrzbkc WHERE icall='"+ruf+"'"; 
   QSqlQuery query;
   query.exec(qy);
   while(query.next()) {
     wName->setText(query.value(0).toString());  
     wQth->setText(query.value(1).toString());   
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


//*************************************************************************
void addQso::addNewQso()
{
 QString xband, pr;
   i = 0;                                     
   t = wCall->text();
   if(t.count() == 0)                         
     return;
   pr = "";
   QSqlQuery query;
   QSqlField field("feld",QVariant::String);  
   
   if( ! omId ) {                                   
      qy = "INSERT INTO "+logFile+"om VALUES (0,'";
      qy += Rufz+"',";                              
      
      if(wName->text().count() == 0) {              
	if(lineEditOM->text().indexOf("CALL") != -1)
	 wName->setText(Rufz);
	else
         wName->setText(lineEditOM->text());        
      }
      
      field.setValue(wName->text());                
      qy += db.driver()->formatValue(field,false);  
      qy += ")";
      query.exec(qy);
      omId = query.lastInsertId().toInt();          
   }
   
   qy = "INSERT INTO "+logFile+" VALUES (0,";    
   qy += t.setNum(omId);                         
   qy += ",'"+Cept+"',";                         
   field.setValue(wQth->text());                 
   qy += db.driver()->formatValue(field,false);  
   qy += ",'"+wLocator->text();                  
   qy += "','"+serv.getSysband(BandBox->currentText()); 
   qy += "','"+wFreq->text();                    
   qy += "','"+ModeBox->currentText();           
   qy += "','";
   QDate d;
   if(checkBoxUhr->isChecked() == FALSE) {       
     QDateTime h = QDateTime::currentDateTime().toUTC(); 
     dateEditStart->setDate(h.date());
     d = h.date();                               
   }
   else 
     d = dateEditStart->date();                  
   qy += d.toString("yyyy-MM-dd"); 
   qy += "','";
   
   QTime timeN;
   if( checkBoxEnd->isChecked() == true ) {      
     timeN = timeEditEnd->time();                
     s = timeN.toString("hh:mm:ss");
     timeN = QTime::fromString(s,"H:m:s");
     timeEditStart->setTime(timeN);              
   }
   else                                          
     timeN = timeEditStart->time();              
   qy += timeN.toString("hh:mm:ss");
   qy += "','";
   timeN = timeEditEnd->time();                  
   qy += timeN.toString("hh:mm:ss");
   
   s = wRSTe->text();
   qy += "','"+s.trimmed();                     
   s = wRSTg->text();
   qy += "','"+s.trimmed();                     
   qy += "','"+wRxPwr->text();                  

   if(checkBoxBuero->isChecked() == true) {     
     qy += "','R','R";                          
     pr = "B";                                  
   }
   else {
      qy += "','-','-";                         
      pr = "";                                  
   }

   qy += "','0000-00-00";                       
   qy += "','0000-00-00";                       
   qy += "','"+wQslMgr->text();                 
   qy += "','"+RigBox->currentText();           
   qy += "','"+PwrBox->currentText();           

   
   t = "SELECT ldxcc,lwae,lwpx,ituz,cqz,lcn FROM tla WHERE la = '"+La+"'";
   query.exec(t);
   if(query.size()) {                           
      while(query.next()) {
        n = 0;
        //dxcc = query.value(n++).toString();    
        qy += "','"+query.value(n++).toString(); 
        qy += "','"+query.value(n++).toString(); 
        qy += "','"+query.value(n++).toString(); 
        qy += "','"+query.value(n++).toString(); 
        qy += "','"+query.value(n++).toString(); 
        qy += "','"+query.value(n++).toString(); 
      }
    }
    else {                              
        qy += "','";                            
        qy += "','";                            
        qy += "','";                            
        qy += "','";                            
        qy += "','";                            
        qy += "','";                            
   }
   qy += "','','','";                           
   qy += pr;                                    
   field.setValue(wNotizEdit->text());               
   qy += "',"+db.driver()->formatValue(field,false); 
   field.setValue(wKomEdit->text());                 
   qy += ","+db.driver()->formatValue(field,false);  
   qy += ",'";                                  
   qy += "','"+LineBox->currentText();          
   
   qy += "',"+HidW->text();               
   
   qy += ","+s.setNum(operatorId)+")";
   
   query.exec(qy);                              
   id = query.lastInsertId().toInt();           
  

   qy = "INSERT INTO "+logFile+"qsl VALUES (0,";
   qy += t.setNum(id);                          
   qy += ",'"+Rufz;                             
   d = dateEditStart->date();                   
   qy += "','"+t.setNum(d.year());
   qy += "-"+t.setNum(d.month());
   qy += "-"+t.setNum(d.day());                 
   qy += "','"+wEditContestId->text();          
   qy += "','";                                 

   qy += "','','";                        
   if(checkBoxEqsl->isChecked() == true)  
     qy += "R";                           
    else 
     qy += "-";                           
   
   qy += "','";                      
   qy += "','";                      
   

   qy += "','";                      
   qy += "','";
   qy += "R";                        
   qy += "','";                      
   qy += "','";                      

   qy += "','"+wOpCall->text();      
   qy += "','";                      
   qy += "','";                      
   qy += "','";                      
   qy += "','";                      
   qy += "','";                      
   qy += "','"+wSwl->text();         
   qy += "','";                      
   qy += "','"+wCustom1->text();     
   qy += "','"+wCustom2->text();     
   qy += "','')";                    
   query.exec(qy);
   
   
   i = 0;
   QHashIterator<QString,QString> i(awdhash);
   while(i.hasNext()) {                         
     i.next();
     s = i.value();
     if(s.count()) {                            
        qy = "INSERT INTO "+logFile+"awd VALUES (0,"; 
        qy += t.setNum(id);                     
        qy += ",'"+Rufz;                        
        qy += "','"+i.key();                    
        qy += "','"+i.value();                  
        qy += "')";
        query.exec(qy);                         
     }
   }
   wIota->setText("000");
   qy = "SELECT COUNT(*) FROM "+logFile;
   query.exec(qy);
   query.next();
   t = query.value(0).toString();                     
   qy = "UPDATE dblogs SET qsocnt="+t+" WHERE logname='"+logFile+"'";
   query.exec(qy);
   
   if((upMgr) && (checkBoxEqsl->isChecked() == TRUE)) {   
     processWriteDatagram(1, s.setNum(id), 45458);        
     labelUp->setText("Up");
   }
   processWriteDatagram(1, s.setNum(id), 45454);    

   QTreeWidgetItem *item = new QTreeWidgetItem(qsoList);
   n = 0;
   d = dateEditStart->date();                       
   s = d.toString("yyyy-MM-dd"); 
   item->setText(n++,s);
   timeN = timeEditStart->time();                   
   s = timeN.toString("hh:mm:ss");
   item->setText(n++,s);
   item->setText(n++,wCall->text());                
   item->setText(n++,wName->text());                
   item->setText(n++,wQth->text());                 
   item->setText(n++,BandBox->currentText());       
   item->setText(n++,ModeBox->currentText());       
   item->setText(n,wRSTe->text().simplified());     
   item->setTextAlignment(n++,Qt::AlignCenter);
   item->setText(n,"R");                            
   item->setTextAlignment(n++,Qt::AlignCenter);
   if(checkBoxBuero->isChecked() == TRUE)           
     item->setText(n,"R");                          
   else
     item->setText(n,"-");                          
     
   item->setTextAlignment(n++,Qt::AlignCenter);
   item->setText(n++,wAwd->text());                 
   item->setText(n++,wLocator->text());             
   item->setText(n++,wNotizEdit->text());           
   item->setText(n++,wEditContestId->text());       
   id = 0;
   omId = 0;
   
   
   if(La.count()) {                                 
     qy = "SELECT cntx,b"+serv.getSysband(BandBox->currentText())+" FROM "+logFile+"dx ";
     qy += "WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
     query.exec(qy);
     query.next();
     if(query.size() != 0) {                         
         n = query.value(0).toInt();                 
         xband = query.value(1).toString();          
         n++;
         qy = "UPDATE "+logFile+"dx SET cntx=";
         qy += t.setNum(n);
         if(xband.compare("e") != 0) {                          
           qy += ", b"+serv.getSysband(BandBox->currentText()); 
           qy += "='x' WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
         }
         else {                                      
           qy += " WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
         }
         query.exec(qy);
     }
     else {
       qy = "INSERT INTO "+logFile+"dx VALUE ('";
       qy += La;                                    
       qy += "','"+ModeBox->currentText();          
       qy += "',1,0,";                              
       qy += "'','','','','','','','','','','','','','','','','','','')";
       query.exec(qy);
       qy = "UPDATE "+logFile+"dx SET b"+serv.getSysband(BandBox->currentText());
       qy += "='x' WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
       query.exec(qy);
     }
   }
   resetAddMask();                  
   getHistoryLast();                
}



//--------------------------------------------------------------------------------------------
void addQso::qsoListClicked(QTreeWidgetItem *item, int col)
{
      i = col;                                              
      uhrStop(1);                                           
      rigpolling = 0;                                       
      checkBoxStop->setPalette(QPalette(QColor(232,58,5))); 
      checkBoxStop->setChecked(TRUE);                       
      //stimer->stop();
      
      ButtonAdd->hide();                               
      ButtonReset->hide();                             
      ButtonUpdate->show();                            
      ButtonBack->show();                              
      ButtonDel->show();                               
      hisflg = 0;                 
      editflg = 1;
      updateflg = 1;                                   
      
      QString cept;
      QString ruf,xrufz;
      
      
      
      qsoItem = item;
      xrufz = item->text(2);
      i = xrufz.indexOf('/',0);
      if( i != -1 ) {                       
        if( i <= 3 ) {
          cept = xrufz.left(i +1);          
          ruf = xrufz.mid(i +1);            
        }
        else
         ruf = xrufz;
      }
      else {
        ruf = xrufz;
      }
      BandIdx = BandBox->currentIndex();    
      ModeIdx = ModeBox->currentIndex();    
      qy = "SELECT id,oid,day,btime,etime,cept,rufz,name,qth,band,mode,";
      qy += "freq,rstr,rsts,rxpwr,qslr,qsls,contactdOp,loc,ntext,ktext,qslmgr,contestid,qinfo,custom1,custom2";
      qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
      qy += " WHERE band = '"+serv.getSysband(item->text(5));
      qy += "' AND rufz = '"+ruf+"' AND day = '";
      qy += item->text(0)+"' AND btime = '"+item->text(1)+"'";
      QSqlQuery query(qy);                                
      query.next();                                       
      n = 0;
      Id = query.value(n++).toInt();                      
      omId = query.value(n++).toInt();                    
 
      s = query.value(n++).toString();                    
      QDate d = QDate::fromString (s,"yyyy-MM-dd");
      dateEditStart->setDate(d);                          
      s = query.value(n++).toString();                    
      QTime timeold = QTime::fromString(s,"HH:mm:ss");
      timeEditStart->setTime(timeold);
      s = query.value(n++).toString();                    
      timeold = QTime::fromString(s,"HH:mm:ss");
      timeEditEnd->setTime(timeold);
      Cept = query.value(n++).toString();
      Rufz = query.value(n++).toString();
      f = Cept + Rufz;
      wCall->setText(f);                                       
      wName->setText(query.value(n++).toString());             
      wQth->setText(query.value(n++).toString());              
      
      i = BandBox->findText(serv.getMyband(query.value(n++).toString())); 
      BandBox->setCurrentIndex(i);                                        
      
      i = ModeBox->findText(query.value(n++).toString());                 
      ModeBox->setCurrentIndex(i);                                        
      
      wFreq->setText(query.value(n++).toString());             
      wRSTe->setText(query.value(n++).toString());             
      wRSTg->setText(query.value(n++).toString());             
      wRxPwr->setText(query.value(n++).toString());            
      Qslr = query.value(n++).toString();                      
      Qsls = query.value(n++).toString();                      
      wOpCall->setText(query.value(n++).toString());           
      wLocator->setText(query.value(n++).toString());          
      wNotizEdit->setText(query.value(n++).toString());        
      wKomEdit->setText(query.value(n++).toString());          
      wQslMgr->setText(query.value(n++).toString());           
      wEditContestId->setText(query.value(n++).toString());    
      wSwl->setText(query.value(n++).toString());              
      wCustom1->setText(query.value(n++).toString());          
      wCustom2->setText(query.value(n++).toString());          
      QHashIterator<QString, QString> i(awdhash);        
      while(i.hasNext()) {
         i.next();
         n = QString::compare(t, i.key(), Qt::CaseInsensitive);
         awdhash.insert(i.key(),"");
      }
      
      qy = "SELECT awtype,awkenner FROM "+logFile+"awd WHERE qid = "+t.setNum(Id);
      query.exec(qy);                                    
      while (query.next()) {                             
        n = 0;
        t = query.value(n++).toString();                 
        s = query.value(n++).toString();                 
        awdhash.insert(t,s);                             
      }
      wAwd->setText("");
      awCnt = 0;
      t = AwdBox->currentText();                         
      QHashIterator<QString, QString> m(awdhash);        
      while(m.hasNext()) {                      
        m.next();
        s = m.value();
        if(s.count()) {                         
           s = m.key();
           n = AwdBox->findText(s);             
           AwdBox->setCurrentIndex(n);          
           wAwd->setText(m.value());            
           awCnt++;
        }
      }
      
      t.setNum(awCnt);
      labelAWD->setText(t);            
}

void addQso::getQrzCallsign()
{
                    if(comboBoxSuche->currentIndex() == 0) {
		       if( !flgf6 ) {                      
		          if(wCall->text().count() != 0) { 
			    flgf6 = 1;                     
			    processWriteDatagram(1, wCall->text(), 45460);
			    setColor(1);                   
			  }
		       }
		       else {
			 flgf6 = 0;                        
			 setColor(0);
			 clearQrzData(); 
			 tabWidget->setCurrentIndex(0);
		       }
		     }
}


void addQso::buttonBackCb()
{
   BandBox->setCurrentIndex(BandIdx);  
   ModeBox->setCurrentIndex(ModeIdx);  
   ButtonUpdate->hide();
   ButtonBack->hide();
   ButtonDel->hide();
   ButtonAdd->show();
   ButtonReset->show();
   uhrStop(0);                         
   rigpolling = runing;                
   updateflg = 0;                      
   resetAddMask();                     
}



void addQso::buttonUpdateCb()
{
      
      
      
      
      
      QSqlQuery query;
      QSqlField field("feld",QVariant::String);
      
      qy = "UPDATE "+logFile+"om SET rufz='"+Rufz+"',name=";
      field.setValue(wName->text());                  
      qy += db.driver()->formatValue(field,false);    
      qy += " WHERE omid="+t.setNum(omId);
      query.exec(qy);
      
      qy = "UPDATE "+logFile+" SET ";                 
      if(editflg == 2) {                              
        if(Cept.count())
          Cept += "/";                                
        editflg = 0;
      }
      qy += "cept='"+Cept+"',";                        
      field.setValue(wQth->text());                       
      qy += "qth="+db.driver()->formatValue(field,false); 
      qy += ",loc='"+wLocator->text();                 
      qy += "',band='"+serv.getSysband(BandBox->currentText()); 
      qy += "',freq='"+wFreq->text();                  
      qy += "',mode='"+ModeBox->currentText();         
      qy += "',day='";                                 
      QDate d = dateEditStart->date();                 
      qy += t.setNum(d.year());
      qy += "-"+t.setNum(d.month());
      qy += "-"+t.setNum(d.day());
      qy += "',btime='";                               
      QTime timN = timeEditStart->time();
      qy += t.setNum(timN.hour());
      qy += ":"+t.setNum(timN.minute());
      qy += ":"+t.setNum(timN.second());
      qy += "',etime='";                               
      timN = timeEditEnd->time();
      qy += t.setNum(timN.hour());
      qy += ":"+t.setNum(timN.minute());
      qy += ":"+t.setNum(timN.second());
      qy += "',rstr='";                                
      s = wRSTe->text();
      qy += s.trimmed();
      qy += "',rsts='";                                
      s = wRSTg->text();
      qy += s.trimmed();
      qy += "',rxpwr='"+wRxPwr->text();                
      if(checkBoxBuero->isChecked() == TRUE) {
        qy += "',qslr='R',qsls='R";                    
        qy += "',prflg='B";                            
       }
       else {
        qy += "',qslr='-',qsls='-";                    
        qy += "',prflg='";                             
      }
      qy += "',qslmgr='"+wQslMgr->text();
      qy += "',rig='"+RigBox->currentText();           
      qy += "',txpwr="+PwrBox->currentText();          
      field.setValue(wNotizEdit->text());              
      qy += ",ntext="+db.driver()->formatValue(field,false);     
      field.setValue(wKomEdit->text());                
      qy += ",ktext="+db.driver()->formatValue(field,false);     
      qy += " WHERE id="+t.setNum(Id);
      query.exec(qy);                                  

      qy = "UPDATE "+logFile+"qsl SET ";
      qy += "contestid='"+wEditContestId->text();
      
      
      if(checkBoxEqsl->isChecked() == TRUE)           
         qy += "',eqsls='R";
      else
         qy += "',eqsls='-";
      
      qy += "',contactdOp='"+wOpCall->text();
      qy += "',qinfo='"+wSwl->text();
      qy += "',custom1='"+wCustom1->text();
      qy += "',custom2='"+wCustom2->text();
      qy += "' WHERE qsoid="+t.setNum(Id);
      query.exec(qy);   
  

/*
      
      QHashIterator<QString,QString> ii(awdhash);        
      while(ii.hasNext()) {
         ii.next();
         qDebug() << ii.key() << ii.value();             
      }
*/
      QHashIterator<QString, QString> i(awdhash);       
      while(i.hasNext()) {
	
         i.next();
         s = i.value();
         qy = "SELECT awtype FROM "+logFile+"awd WHERE awtype = '"+i.key()+"' AND qid="+t.setNum(Id);
         query.exec(qy);                           
         if( query.numRowsAffected() ) {           
             if(s.count()) {                       
               qy = "UPDATE "+logFile+"awd SET awkenner='"+s+"' WHERE qid="+t.setNum(Id);
               qy += " AND awtype='"+i.key()+"'";
               query.exec(qy);
              }
              else {                               
                 qy = "DELETE FROM "+logFile+"awd WHERE ";
                 qy += "qid="+t.setNum(Id)+" AND awtype='"+i.key()+"'";
                 query.exec(qy);
              }
          }
           else {                                  
              if(s.count()) {                      
                 qy = "INSERT INTO "+logFile+"awd VALUES (0,";
                 qy += t.setNum(Id);               
                 qy += ",'"+wCall->text();         
                 qy += "','"+i.key();              
                 qy += "','"+i.value();            
                 qy += "')";
                 query.exec(qy);
		 
              }
          }
          
     }
     processWriteDatagram(2, s.setNum(Id), 45454); 
     if( ! hisflg )                                
       getHistoryLast();
     else
       getHistoryPref();                           
     BandBox->setCurrentIndex(BandIdx);            
     ModeBox->setCurrentIndex(ModeIdx);            
     ButtonUpdate->hide();
     ButtonBack->hide();
     ButtonDel->hide();
     ButtonAdd->show();
     ButtonReset->show();
     uhrStop(0);                         
     rigpolling = runing;
     resetAddMask();                     
     hisflg = 1;                         
     updateflg = 0;                      
     tabWidget ->setCurrentIndex(0);
}



void addQso::buttonDeleteCb()
{
     
     
     
     QSqlQuery query;
     
     qy = "DELETE FROM "+logFile+"awd WHERE qid="+s.setNum(Id);  
     query.exec(qy);
     
     qy = "SELECT id FROM "+logFile+" WHERE oid="+s.setNum(omId);       //OM prüfe Anzahl OSO's
     query.exec(qy);                                                    
     n = query.size();                                                  
     if( n == 1 ) {                                                     
       qy  = "DELETE FROM "+logFile+"om WHERE omid="+s.setNum(omId);    //OM loeschen
       query.exec(qy);
     }
     
     qy = "SELECT mode,dxcc FROM "+logFile+" WHERE id="+s.setNum(Id);   
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
   
     
     qy = "SELECT cid FROM "+logFile+"card WHERE qsocid="+s.setNum(Id)+" AND qsl='E'";
     query.exec(qy);
     if(query.size() != 0) {
       qy = "DELETE FROM "+logFile+"card WHERE qsocid="+s.setNum(Id)+" AND qsl='E'";
     query.exec(qy);
     }
     
     qy = "DELETE FROM "+logFile+"qsl WHERE qsoid="+s.setNum(Id);      
     query.exec(qy);
   
     
     qy = "DELETE FROM "+logFile+" WHERE id="+s.setNum(Id);            
     query.exec(qy);
   
     n = qsoList->indexOfTopLevelItem(qsoItem);
     qsoList->takeTopLevelItem ( n );                   
     processWriteDatagram(3, s.setNum(Id), 45454);      
    if( ! hisflg )                                      
      getHistoryLast();
    else
     getHistoryPref();                                  
    ButtonUpdate->hide();
    ButtonBack->hide();
    ButtonDel->hide();
    ButtonAdd->show();
    ButtonReset->show();
    uhrStop(0);                                         
    resetAddMask();                                     
    hisflg = 1;                                         
    updateflg = 0;                                      
    rigpolling = runing;
    tabWidget ->setCurrentIndex(0);
    ptimer->start(retry);
}



void addQso::processPendingDatagram()
{
   while(udpSocket->hasPendingDatagrams()) {
     datagram.resize(udpSocket->pendingDatagramSize());
     QHostAddress sender;
     quint16 senderPort;
     udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
     i = datagram.indexOf (",",0);
     QByteArray Left = datagram.left(i);           
     QByteArray Mid = datagram.mid(i+1);           
     int com = Left.toInt();                       
     QString t = Mid;                              
     QString f, q;
     if(com == 3) {  
       //FORM: <call>6 dxcall <Name>15 Manfred Dauhrer <Qth>36 Neumayerstr. 3, D-83313 Siegsdorf, , "
       if(t.indexOf("<") != -1) {      
                                       
         t = t.remove(0,1);            
         i = t.indexOf(">");
         f = t.left(i);                
         t = t.remove(0,i +1);         
         i = t.indexOf(" ");
         n = t.left(i).toInt();        
         dxCall = t.mid(i+1,n);        
         dxCall = dxCall.trimmed();
         dxtimer->start(20);           
         
         t = t.remove(0,n);            
         while(1) {
            
            if(t.indexOf("<") == -1) return;
            i = t.indexOf("<");
            f = t.remove(0,i+1);
            i = t.indexOf(">");
            f = t.left(i);             
            t = t.remove(0,i +1);      
            
            i = t.indexOf(" ");
            n = t.left(i).toInt();     
            q = t.mid(i+1,n).trimmed();
            setText(f, q);
         }
        }
       else {                                 
         buttonBackCb();                      
         i = t.indexOf(" ");
         QString freq = t.left(i);            
         if(freq.compare("last") == 0) {      
           freq = lastFreq;
           i = freq.indexOf('.');
           freq = freq.remove(i,1);
           freq += "00";
         } 
         lastFreq = wFreq->text();            
         slotFlg = 1;
         cmdStr = "F "+freq;
         if(freq.toInt() / 1000000 < 10 )     
          cmdStr += " M LSB 2400";            
        else 
           cmdStr += "M USB 2400";
         cmdFlg = 2;
        }
      }
     else 
      if(com == 10) {                         
	i = t.indexOf("|");
	lineEditQrzCall->setText(t.left(i));  
	
	t.remove(0,i+1);
	i = t.indexOf("|");
	lineEditQrz_1->setText(t.left(i));    
	
	t.remove(0,i+1);
	i = t.indexOf("|");
	lineEditQrz_2->setText(t.left(i));    
	
	t.remove(0,i+1);
	i = t.indexOf("|");
	lineEditQrz_3->setText(t.left(i));    
	
	t.remove(0,i+1);
	lineEditQrz_4->setText(t);            
	
	tabWidget->setCurrentIndex(6);
      }
     else
       if(com == 11) {
	 
	 labelError->setText(t);
	 tabWidget->setCurrentIndex(6);
       }
     else  
      if(com == 5) {                          
        logFile = t;                          
        wLogFile->setText(logFile);           
        getHistoryLast();                     
      }
     else  
      if(com == 6) {                          
        QSqlQuery query;
        AwdBox->clear();
        i = AwdBox->currentIndex();           
        qy = "SELECT atype FROM wawdlist WHERE aset!='0' ORDER BY aset";
        query.exec(qy);
        while(query.next())
          AwdBox->insertItem (i++, query.value(0).toString());   
       }
      else  
       if(com == 1) {                         
         labelUp->setText(" ");               
         if(Mid.indexOf("ERROR") != -1) {     
           QMessageBox::information( this,
           tr("UploadMgr Eqsl"),("ERROR: No match on eQSL_User/eQSL_Pswd\nUploadMgr is unload."),
           QMessageBox::Default); 
           qslViaEqslSetOff();                
           upMgr = 0; 
         }
       }
      else  
       if(com == 9) {                         
          QMessageBox::information( this,
          tr("UploadMgr Eqsl"),
          tr("\nEin Netzwerk Timeout Signal ist eingegangen.\nDas Netzwerk ist gestoert, der eQSL.cc Server meldet sich nicht.\nDer Upload zum Server wurde eingestellt."),
          QMessageBox::Default);
       }
   }
}


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



void addQso::processWriteDatagram(int dNr, QString idNr, int port)
{
     datagram = QByteArray::number(dNr);
     datagram.append(",");
     datagram.append(idNr);
     udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, port);       
}





void addQso::requestRigNew()
{
   if( rigpolling ) {             
     if(done) {                   
       if(cmdFlg) {               
         i = cmdStr.indexOf("M"); 
         if(i > 3) {                      
           cmd = cmdStr.left(i); 
           cmd = cmd.trimmed();           
           cmdStr = cmdStr.remove(0, i);  
         }
         else {                           
          cmd = cmdStr;                   
          cmdStr = "";
         }
         ptimer->stop();
         ptimer->start(200);              
       }
       else {                      
         if(poll)                  
           cmd = "f";              
         else                      
           cmd = "m";              
       }
       done = 0;                   
     }
     //else
     
     slotFlg = 1;
     tcpSocket->abort();
     tcpSocket->connectToHost(tcphost,tcpport.toInt());
   }
}



void addQso::sentCommand()
{
    QString message;
    if(rigpolling) {
       message = cmd;
       tcpSocket->write(message.toAscii());
       tcpSocket->flush();
       
       if(cmdFlg) {
         done = 1;                  
         cmdFlg--;                  
       }
    }
}



void addQso::readRigReply()
{
    QString st;
    QString message(tcpSocket->readAll());     
    
    if(message.indexOf("RPRT") != -1) {
      ptimer->stop();
      rigpolling = 0;
      ModeBox->setEnabled(true);               
      checkBoxStop->setPalette(QPalette(QColor(224,224,224)));  
      QMessageBox::information(this,tr("Hamlib Client"),
                   tr("\nError ocuured timedout.\n"
                   "Check the transceiver connection.\n"
                   "Hardware unavailable !\n\n"
                   "* Server stopped. *"));
      return;
    }
    
    if( poll ) {                               
      message = message.simplified();          
      i = message.count();
      st = message.left(i -2);                 
      st.insert(i -3, QChar('.'));
      wFreq->setText(st);                      
      st = st.left(i -6);                      
      
      if(st.indexOf(lastBand) == -1) {         
         slotFlg = 0;
         BandBox->setCurrentIndex(BandBox->findText(getBand(st)));
         BandIdx = BandBox->currentIndex();
         
         lastBand = st;                        
      }
      checkBoxStop->setPalette(QPalette(QColor(164,179,167)));  
      poll = 0;                                
    }
    else {                                     
      i = message.indexOf("\n");               
      st = message.left(i);
      
      if(st.compare(lastMode) != 0) {
        n = ModeBox->findText(getMode(st));    
        ModeBox->setCurrentIndex(n);
        
        lastMode = st;
      }
      poll = 1;                                
      checkBoxStop->setPalette(QPalette(QColor(224,224,224)));  
    }
    
    done = 1; 
    ptimer->start(retry); 
}


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
  checkBoxStop->setPalette(QPalette(QColor(224,224,224)));  
  ModeBox->setEnabled(true);               
  checkBoxStop->setText("Rctl off");
}

void addQso::starthamlibServer()
{
     QString home, p, pr, q, rigPid;
     int ok, i, n, proz;
     ok = 1;
     
     home = getenv("HOME");                 
     p = "ps xa | grep rigctld > "+home+"/.qtlog/rigstatus";
     i = system(p.toAscii());               
     home += "/.qtlog/rigstatus";           
     QFile iniFile(home);
     iniFile.open(QIODevice::ReadOnly);
     QTextStream istream( &iniFile);        
     while(istream.atEnd() != true  ) { 
       pr = istream.readLine(0);            
       if(pr.indexOf("rigctld -m") != -1) break;
     }
     iniFile.close();
     
     proz = pr.indexOf("rigctld -m");
     if( proz != -1 ) {                      
       if( retry ) {                         
         if( runing ) {                      
           ModeBox->setEnabled(false);       
	   checkBoxStop->setText("Rctl on"); 
           rigpolling = runing;
           poll = 1;                         
           cmdFlg = 0;
           cmd = "f";
           done = 1;
           ptimer->start(retry);             
         }
       }
       else { 
         //usr/local/bin/rigctld -m 357 -r /dev/ttyUSB0 -s 19200 -p /dev/ttyUSB0 -C rts_state=OFF -C dtr_state=OFF"
          i = pr.indexOf("-m");             
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          q = "UPDATE wproz SET rigpid='";
          rigPid = pr.mid(i, n -i);         
          q += rigPid;
          
          i = pr.indexOf("-r");
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          q += "',rigdev='"+pr.mid(i, n -i); 
          
          i = pr.indexOf("-s");
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          q += "',baud='"+pr.mid(i, n -i);   
          
          i = pr.indexOf("-p");
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          q += "',pttdev='"+pr.mid(i, n -i); 
          
          i = pr.indexOf("-C");              
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          p = pr.mid(i, n -i); 
          n = p.count();
          i = p.indexOf("=");
          q += "',rts='"+p.mid(i +1, n -i);  
          
          i = pr.indexOf("-C", n);           
          n = pr.indexOf(" ",i +1);
          i = n+1;
          n = pr.indexOf(" ",i +1);
          p = pr.mid(i , n -i);
          n = p.count();
          i = p.indexOf("=");
          q += "',dtr='"+p.mid(i +1, n -i);  
          q += "',owner='hamlb";
          q += "',rig='"+getRigModelName(rigPid);
          q += "',retry='1000',runing=1 WHERE proz='HamLib'";
          QSqlQuery query(q);                
          ModeBox->setEnabled(false);        
	  checkBoxStop->setText("Rctl on");  
          runing = 1;
          rigpolling = runing;
          poll = runing;                     
          cmdFlg = 0;
          cmd = "f";                         
          done = 1;
          retry = 1000;                      
          ptimer->start(retry);              
     }
   }
   else {                                    
     
     if(owner.count() != 0) {  
       if( runing ) {          
         
          QSqlQuery query;     
          qy = "SELECT rigpid,rigdev,baud,pttdev,rts,dtr FROM wproz WHERE proz='HamLib'";
          query.exec(qy);
          query.next();
          n = 0;
          p = "rigctld -m "+query.value(n++).toString();     
       
          p += " -r "+query.value(n++).toString();           
          p += " -s "+query.value(n++).toString();           
          p += " -p "+query.value(n++).toString();           
       
          p += " -C rts_state="+query.value(n++).toString(); 
          p += " -C dtr_state="+query.value(n++).toString(); 
          p += " &";
          i = system(p.toAscii());  
        
          ModeBox->setEnabled(false);        
	  checkBoxStop->setText("Rctl on");  
          rigpolling = runing;
          poll = runing;                     
          cmdFlg = 0;
          cmd = "f";                         
          done = 1;
          ptimer->start(retry);              
       }
     }
   }
   checkBoxStop->setPalette(QPalette(QColor(164,179,167)));  
   i = system("getmgrdb &");
}


QString addQso::getRigModelName(QString rigId)
{
     QString home;
  
     home = getenv("HOME");              
     s = "rigctl -l > ";
     s += home;
     s +="/.qtlog/rigstatus";
     i = system(s.toAscii());
     home += "/.qtlog/rigstatus";        
     QFile inFile(home);
     inFile.open(QIODevice::ReadOnly);
 
     QTextStream istream( &inFile);      
     s = istream.readLine(0);            
     while(istream.atEnd() != true  ) {  
        x = 0;
        s = istream.readLine(0);         
        x = s.indexOf("\t");
        t = s.left(x);
        t = t.simplified();
        if(t.compare(rigId) == 0)        
           break;                        
     }
 
     s.remove(0,x);
     x = 16;
     t = s.left(x);
     t = t.simplified();                 
     s.remove(0,x);
     x = 24;
     t = s.left(x);
     t = t.simplified();                 
  return t;                              
}

void addQso::rigctlAc()
{
    rigctl * Rigctl = new rigctl();
    Rigctl->exec();   
    
    if(Rigctl->updateFlg == 1) {            
      ptimer->stop();                       
      
      ModeBox->setEnabled(false);           
      checkBoxStop->setText("Rctl on");
      checkBoxStop->setPalette(QPalette(QColor(164,179,167)));  
      runing = Rigctl->Runing;              
      rigpolling = runing;
      retry = Rigctl->Retry;
      ctl = 1;
      ptimer->start(retry);                 
      starthamlibServer();                  
    }
    else
    if(Rigctl->updateFlg == 2) {            
      rigpolling = 0;
      retry = 0;
      ptimer->stop();                       
      ModeBox->setEnabled(true);            
      checkBoxStop->setText("Rctl off");    
      checkBoxStop->setPalette(QPalette(QColor(224,224,224)));  
    }
}

void addQso::realtimeUpAc()
{
    uploadR * UploadR = new uploadR();
    UploadR->exec();   
    
    if(UploadR->upLd == 1) {                
      settings.setValue("QSLvia",4);        
      startUploadMgrR(1);
    }
    else {
      settings.setValue("QSLvia",1);        
    }
    setQslDefault();
}



int addQso::startUploadMgrR(int status)
{
  QString home, pr, p;
  int proz;
  
     if( status ) {                           
       home = getenv("HOME"); 
       p = "ps -e | grep uploadEqsldb > "+home+"/.qtlog/rigstatus";
       i = system(p.toAscii());               
       home += "/.qtlog/rigstatus";           
       QFile iniFile(home);
       iniFile.open(QIODevice::ReadOnly);
       QTextStream istream( &iniFile);        
       while(istream.atEnd() != true  ) { 
         pr = istream.readLine(0);            
         if(pr.indexOf("uploadEqsldb") != -1) break;
       }
       
       iniFile.close();
       proz = pr.indexOf("uploadEqsldb");
     
       if( proz != -1 ) {                     
         upMgr = 1;                           
       }
       else {                                 
	 if(settings.value("EqslRealtime").toString().toInt()) {
           if(settings.value("EqslPasswd").toString().count() != 0) { 
             s = "uploadEqsldb 45458 &";      
             StartProcess( s );               
             upMgr = 1;
             qslMsg(rmess);                   
           }
	 }
       }
     }
     else {                                   
       processWriteDatagram(-1, "-1", 45458); 
       upMgr = 0;
     }
     return upMgr;
}
     

void addQso::qslMsg(int mess)
{
   if(mess == 1) {
     rmess = 0;                                              
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



void addQso::qslViaEqslSetOff()
{
     checkBoxEqsl->setChecked(FALSE);       
     checkBoxBuero->setChecked(TRUE);
     settings.setValue("QSLvia",1);         
     //settings.setValue("QSLs",1);         
}
