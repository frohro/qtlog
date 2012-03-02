/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n 
********************************************************************** */
// dxspot.cpp  02-04-2010

#include <QtGui>
#include <QtSql>
#include <QTimer>
#include <QNetworkAccessManager>

#include "../addQso/destination.h"
#include "dxspot.h"

dxspot::dxspot(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    
    connect(actionChose, SIGNAL(triggered(bool)), this, SLOT(getNodes()));
    connect(ButtonReturn, SIGNAL(clicked()), this, SLOT(buttonReturnCb()));
    connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
    connect(ButtonSendSpot, SIGNAL(clicked()), this, SLOT(sendSpotCb()));
    connect(ButtonSpot, SIGNAL(clicked()), this, SLOT(sendSpotTextCb()));
    connect(ButtonAnnounce, SIGNAL(clicked()), this, SLOT(sendAnnounceTextCb()));
    
    stackedWidget->setCurrentIndex(0);
    spotList->setColumnWidth(0,95);  // Call
    spotList->setColumnWidth(1,90);  // Freq
    spotList->setColumnWidth(2,100); // dx_call
    spotList->setColumnWidth(3,300); // spot
    spotList->setColumnWidth(4,100); // time + day + dx
 
    
    //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("host").toString());                         
    db.setDatabaseName(settings.value("dbname").toString());
    db.setUserName(settings.value("dbuser").toString());
    db.setPassword(settings.value("dbpasswd").toString());
    if(!db.open()) {
       qDebug() << db.lastError();           
    }
    
    //s = settings.value("Breite").toString();       
    //t = settings.value("Laenge").toString();
    //distanz.setHomeCoordinaten(s,t);
    distanz.setHomeCoordinaten(settings.value("Breite").toString(),settings.value("Laenge").toString());
    
    BoxSpots->clear();
    QSqlQuery query;  
    qy = "SELECT rangeval,descript FROM wspotsparm WHERE sel=1";
    query.exec(qy);
    i = 0;
    while(query.next()) {
       s = query.value(0).toString();        // val
       t = query.value(1).toString();        // description
       BoxSpots->insertItem(i++,t);
       RangeDes.insert(t,s);                 // description, val
    }
    
    BoxCustom->clear();
    qy = "SELECT rangeval,descript FROM wspotsparm WHERE sel=2";
    query.exec(qy);
    i = 0;
    while(query.next()) {
       s = query.value(0).toString();        // val
       t = query.value(1).toString();        // description
       BoxCustom->insertItem(i++,t);
       CustomDes.insert(t,s);                // custom_description, val
    }
    
    qy = "SELECT urlstr,fetchd,count,ranged FROM wspotnodes WHERE run = 1";
    query.exec(qy);
    query.next();
    urlSpot = query.value(0).toString();                          // urlstr
    urlSpot += "/"+query.value(1).toString();                     // fetchd
    urlSpot += "?count="+query.value(2).toString();               // count
    urlSpot += "&range="+getRangeVal(query.value(3).toString());  // range
    BoxSpots->setCurrentIndex(BoxSpots->findText(query.value(3).toString()));
     
    if(query.value(1).toString().indexOf("DxSpots") != -1) {
       BoxSpots->setPalette( QPalette(QColor(180, 210, 200)));   // grün
       BoxCustom->setCurrentIndex(BoxCustom->findText("-"));     // -.-
    }
    connect(BoxSpots, SIGNAL(currentIndexChanged(QString)), this, SLOT(spotsCb(QString)));
    connect(BoxCustom, SIGNAL(currentIndexChanged(QString)), this, SLOT(customCb(QString)));
    connect(spotList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(spotListClicked(QTreeWidgetItem*,int)));
    connect(ButtonF2, SIGNAL(pressed()), this, SLOT(writeForAddQso()));
    connect(lineEditSpotCall, SIGNAL(textEdited(QString)), this, SLOT(callUpperCb(QString)));
    
    udpSocket = new QUdpSocket(this);                    // datagram_UDP_net
    
    udpSocket = new QUdpSocket(this);                    // datagram_UDP_net
    udpSocket->bind(QHostAddress::LocalHost,45464);      // read from getitMgr
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT (processPendingDatagram()));
    
    connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
    
    dxtimer = new QTimer(this);              // DXCluster ( dxCall übernehmen )
    connect(dxtimer, SIGNAL(timeout()), this, SLOT(scanDxCall()));
   
    ntimer = new QTimer(this); 
    connect(ntimer,SIGNAL(timeout()), this, SLOT(getSpots()));    // 1.download
    ntimer->start(100);                                           // inital zeit
    xcnt = 0;
    timerflg = 1;                                                 // inital läuft
    urlFlg = 0;                                                   // 0=Spot_url, 1=QRZ_url
}

// ----------------------
dxspot::~dxspot()
{
   QSqlQuery query; 
   qy = "UPDATE wspotnodes SET ranged='"+BoxSpots->currentText();
   qy += "' WHERE node='OH8'";
   query.exec(qy);
}

// ---------------------
void dxspot::getNodes()
{
  stackedWidget->setCurrentIndex(1);
}

// --------------------
void dxspot::goExit()
{
   qApp->quit();
}


// -------------------------------------------
void dxspot::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F2 :
                writeForAddQso();                        // data übernehmen - und an addqso senden
             break;
     case Qt::Key_Escape :
                if(stackedWidget->currentIndex() == 2) { // wenn spot_page
                   s = "last ";
                   datagram = QByteArray::number(3);     // Datagramm an addqso - stelle TRx wieder
                   datagram.append(",");
                   datagram.append(s);
                   udpSocket->writeDatagram(datagram.data(),datagram.size(),
                   QHostAddress::LocalHost, 45450);      // datagram_port
		   
                   lineEditDxCall->setText("");          // alles löschen
                   lineEditName->setText("");
                   lineEditQth->setText("");
                   lineEditQslMgr->setText("");
                   lineEditFreq->setText("");
                   stackedWidget->setCurrentIndex(0);
                }
                else
                  if(stackedWidget->currentIndex() == 1) {
                    stackedWidget->setCurrentIndex(0);
                  }
             break;
   }
}


// ----------------------------------------------------------
// return von getNotes
// ----------------------------------------------------------
void dxspot::buttonReturnCb()
{
    // "http://www.dxsummit.fi/DxSpots.aspx?count=50&range=2";
    QSqlQuery query;  
    qy = "SELECT urlstr,fetchd FROM wspotnodes WHERE run = 1";
    query.exec(qy);
    query.next();
    urlSpot = query.value(0).toString();             // http://www.dxsummit.fi
    urlSpot += "/"+query.value(1).toString();        // DxSpots.aspx
    urlSpot += "?count="+BoxCount->currentText();    // query.value(2).toString();
    // unterscheide zwischen BoxSpots und BoxCustom je nach Selektion !!
    if(BoxSpots->currentText().compare("-") == 0)
       urlSpot += "&range="+getCustomVal(BoxCustom->currentText()); 
    else
       urlSpot += "&range="+getRangeVal(BoxSpots->currentText());
    
    ntimer->stop();
    ntimer->start(100);
    timerflg = 1;                                    // flg inital start
    stackedWidget->setCurrentIndex(0);
}


// -------------------------------------
// BoxSpots changed
// -------------------------------------
void dxspot::spotsCb(QString str)
{
    if(str.compare("-") == 0)
       return;
    BoxSpots->setPalette( QPalette(QColor(180, 210, 200)));   // grün
    BoxCustom->setPalette( QPalette(QColor(224, 224, 224)));  // grün
    BoxCustom->setCurrentIndex(BoxCustom->findText("-"));
  
    QSqlQuery query;       // "http://www.dxsummit.fi/DxSpots.aspx?count=50&range=2";
    qy = "SELECT urlstr,fetchd FROM wspotnodes WHERE run = 1";
    query.exec(qy);
    query.next();
    
    urlSpot = query.value(0).toString();            // http://www.dxsummit.fi
    urlSpot += "/"+query.value(1).toString();       // DxSpots.aspx
    urlSpot += "?count="+BoxCount->currentText();   // count
    urlSpot += "&range="+getRangeVal(str);          // rangeVal
    ntimer->stop();
    ntimer->start(100);
    timerflg = 1;                                                
}


// -------------------------------------
// BoxCustom changed
// -------------------------------------
void dxspot::customCb(QString str)
{
    if(str.compare("-") == 0) 
      return;
    
    BoxCustom->setPalette( QPalette(QColor(180, 210, 200))); // grün
    BoxSpots->setPalette( QPalette(QColor(224, 224, 224)));  // grün
    BoxSpots->setCurrentIndex(BoxSpots->findText("-"));
    
    QSqlQuery query;      
    qy = "SELECT urlstr FROM wspotnodes WHERE run = 1";
    query.exec(qy);
    query.next();
    urlSpot = query.value(0).toString();                 // http://www.dxsummit.fi
    urlSpot += "/CustomFilter.aspx";                     // CustomFilter.aspx
    urlSpot += "?customCount="+BoxCount->currentText();  // count
    urlSpot += "&customRange="+getCustomVal(str);        // customRangeVal
 
    ntimer->stop();
    ntimer->start(100);
    timerflg = 1;                                       
}


// ---------------------------------------------------
// get Call + Frequenz : sent data -> qsologging
// itemClicked(QTreeWidgetItem(int)
// ---------------------------------------------------
void dxspot::spotListClicked(QTreeWidgetItem *item, int col)
{
      if((col == 1) || (col == 2)) {                   // Frequenz setzen & QRZ.com abfragen
        bItem = item;                                  // --------          -------
        
        lineEditFreq->setText(item->text(1));          // neue Freq setzen und zeigen
        s = item->text(1);
        s = s.trimmed();
        i = s.indexOf(".");
        s = s.remove(i,1);
        s += "00 ";                                    // Frequenz in Hz
        
        dxCall = item->text(2).trimmed();
        
        datagram = QByteArray::number(3);              // Datagramm an addqso - stelle TRx auf neue
        datagram.append(",");
        datagram.append(s);
	//datagram.append(" ");
	//datagram.append(dxCall);
	// starte qrz_abfrage schalte addqso_dialog # zeige ergebnis
        udpSocket->writeDatagram(datagram.data(),datagram.size(),
                    QHostAddress::LocalHost, 45450);  
        
        dxCall = item->text(2).trimmed();
        xcnt = 1;
        dxtimer->start(10);                 // starte Berechnung -> Entfernung u. Richtung ( zeige an )
        
        
        // sende Anfrage an getitMgr / QSZ.COM
        // ------------------------------------
        datagram = QByteArray::number(2);
        datagram.append(",");
        datagram.append(dxCall);
        udpSocket->writeDatagram(datagram.data(),datagram.size(),
                    QHostAddress::LocalHost, 45460);  
        timerflg = 1; 
        ntimer->stop();                    // alle aktivitäten einstellen
        ntimer->start(40);                 // in 40 mS Anforderung starten
	
	stackedWidget->setCurrentIndex(2);
     }
}


// -----------------------------------------------------
// QSO-daten von QRZ.com übernehmen und an addqso senden
// -----------------------------------------------------
void dxspot::writeForAddQso()
{
       datagram = QByteArray::number(3);         // Datagramm an addqso senden - com = 3;
       datagram.append(",");
       
       // die Rx/Freq ist schon umgeschaltet     - baue data_string
       t = lineEditDxCall->text();               // Call
       s = "<call>"+t.setNum(t.count())+" "+lineEditDxCall->text();
        
       t = lineEditName->text();                 // Name ...
       if(t.count() != 0)                        // kein leeres Feld
         s += "<"+BoxName->currentText()+">"+t.setNum(t.count())+" "+lineEditName->text();
       
       t = lineEditQth->text();                  // QTH ...
       if(t.count() != 0)                        // kein leeres Feld
         s += "<"+BoxQth->currentText()+">"+t.setNum(t.count())+" "+lineEditQth->text().trimmed();
       
       if(lineEditQslMgr->text().count() != 0) { // QslMgr ...
         t = lineEditQslMgr->text();
         s += "<"+BoxQslMgr->currentText()+">"+t.setNum(t.count())+" "+lineEditQslMgr->text();
       }
       
       datagram.append(s);                       // sende jetzt an addQso
       udpSocket->writeDatagram(datagram.data(),datagram.size(),
       QHostAddress::LocalHost, 45450);          // datagram_port
      
       lineEditDxCall->setText("");              // alle Einträge löschen
       lineEditName->setText("");
       lineEditQth->setText("");
       lineEditQslMgr->setText("");
       lineEditFreq->setText("");
       stackedWidget->setCurrentIndex(0);
}


// ------------------
// SendSpot
// ------------------
void dxspot::sendSpotCb()
{
    lineEditOwnerCall->setText(settings.value("Call").toString());
    stackedWidget->setCurrentIndex(1);
}

// -----------------------------------
void dxspot::callUpperCb(QString str)
{
    str = str.toUpper();
    lineEditSpotCall->setText(str);
}


// -----------------------------------
// Send SPOT
// -----------------------------------
void dxspot::sendSpotTextCb()
{
    QString s;
    s = "http://www.dxsummit.fi/SendSpot.aspx?callSign="+lineEditOwnerCall->text();
    s += "&dxCallSign="+lineEditSpotCall->text();
    s += "&frequency="+lineEditSpotFreq->text();
    s += "&info="+lineEditSpotInfo->text();
    urlQrz = s;
    
    urlFlg = 2;                                  // QRZ Abfrage durchführen
    timerflg = 1;                                // abbruch laufzeit 100 mS
    ntimer->stop(); 
    ntimer->start(40);                           // starte Anforderung
    stackedWidget->setCurrentIndex(0);
}


// ------------------------------------
// Send Announcement
// ------------------------------------
void dxspot::sendAnnounceTextCb()
{
    QString s;
    s = "http://www.dxsummit.fi/SendSpot.aspx?callSign="+lineEditOwnerCall->text();
    s += "&info="+lineEditSpotInfo->text();
    urlQrz = s;
 
    urlFlg = 2;                                  // QRZ Abfrage durchführen
    timerflg = 1;                                // abbruch laufzeit 100 mS
    ntimer->stop(); 
    ntimer->start(40);                           // starte Anforderung
    stackedWidget->setCurrentIndex(0);
}

// Aufuf vom timer_dxCall_Cluster : Call vom DX_Cluster übernehmen
// ---------------------------------------------------------------
void dxspot::scanDxCall()
{
     QString f;
     if(dxCall.count() == xcnt -1)
       dxtimer->stop();
     else {
       f = dxCall.left(xcnt);
       if(f.indexOf("/") != -1)     // falls gefunden
         dxtimer->stop();           // ende
       else
        getLaPrefix(f);
     }
     xcnt++;
}

// -------------------------------------------
void dxspot::getLaPrefix(QString callstr)
{
  QString dxcc, lname, cnt, cqz, br, lg;
     QSqlQuery query;                                   // check/select dxcc + WAE
     qy = "SELECT ldxcc,lname,lcn,ituz,cqz,br,lg FROM tpref,tla WHERE pref=la AND spref='"+callstr+"'";
     query.exec(qy);  
     if(query.size() == 1) {                           // wenn 1, land_pref gefunden
       n = 0;
       query.next();
       
       dxcc = query.value(n++).toString();             // DXCC
       lname = query.value(n++).toString();            // Landesname
       cnt = query.value(n++).toString();              // Continent
       
       s = dxcc+"  - "+lname+"   "+cnt+" /  ";         // Continent
       s += "ITU: "+query.value(n++).toString();       // ITU
       cqz = "  -  CQz: "+query.value(n++).toString(); // und CQZ
       s += cqz;
       labelInfo->setText(s); 
       
       br = query.value(n++).toString();               // breite
       lg = query.value(n++).toString();               // länge
       s = distanz.getDisRicht(br,lg);                 // Entfernung / Richting berechnen
       labelDistanz->setText(s);                       // INFO-string anzeigen 
     }
}



// -----------------------------------------------------------------------
// Datagram von getitMgr eingegangen
// ein Call bei qrz.com abgefragt
// -----------------------------------------------------------------------
void dxspot::processPendingDatagram()
{
     while(udpSocket->hasPendingDatagrams()) {
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
     
        i = datagram.indexOf (",");
        s = datagram.left(i);                 // Funktion
        t = datagram.mid(i +1);               // Parameter_Liste
        i = s.toInt();                        // funktions_no 1 = Auftrag von addqso, 2 = Auftrag von
     }
     i= t.indexOf("|");                       // Call
     lineEditDxCall->setText(t.left(i));
     t.remove(0,i+1);
       
     i = t.indexOf("|");
     lineEditName->setText(t.left(i));       // name
     t.remove(0,i +1);
       
     i = t.indexOf("|");
     t.remove(0,i +1);                       // street
     
     i = t.indexOf("|");
     lineEditQth->setText(t.left(i));        // qth
     t.remove(0, i +1);
      
     i = t.indexOf("|");
     lineEditQslMgr->setText(t.left(i));     // state
}
    
