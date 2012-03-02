/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n - 21.01.2010
********************************************************************** */

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QtNetwork>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <qdebug.h>
#include <QtGui>
#include <stdio.h>

#include "getmgr.h"

#define BEGIN_CALLSIGN "begin calldata"
#define END_CALLSIGN   ">Email: "
#define NOT_FOUND      "<div style=\"margin:lem\">"
#define END_NOT_FOUND  "<span style="

// *********************************************************************

getMgr::getMgr()
{
    udpSocket = new QUdpSocket(this);                     // datagram_UDP_net
    udpSocket->bind(QHostAddress::LocalHost,45460);       // read from addqsodb
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT (processPendingDatagram()));
    
    http = new QHttp(this);
    connect(http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
    
    //cntVal = 0;
    //timer = new QTimer(this);                           // Timer time_out einbauen
    //connect(timer,SIGNAL(timeout()), this, SLOT(timerpollRig()));
    //timer->start(1000);                                 // jede sek. ein ticker
}

// -----------------------
getMgr::~getMgr()
{ }

// -----------------------
void getMgr::execute()
{
     // warte auf Datagramme
}

// ----------------------
void getMgr::getit()
{
     website = "www.qrz.com";
     callsign = Call.toUpper();
     urlstr = "/callsign/"+callsign;
     
     http->setHost(website);
     http->get(urlstr);
}

// -----------------------------
void getMgr::httpDone(bool error)
{
     if(error) {
       ErrorStr = http->errorString();
       processWriteDatagram(11, ErrorStr, rport);      // ERROR_Meldung senden # 45450
       return;
     }
     parseit();
}

// -----------------------------
void getMgr::parseit()
{
  QString call,name,street,qth,state;
  int i, n;
 
     QTextStream stream(http->readAll());
     flag = FALSE;
     str = "";
     while(!stream.atEnd()) {
       lineData = stream.readLine();                    // ERROR_Meldung senden # 45450
       if(lineData.contains(NOT_FOUND)) {               // "<div style=\"margin:lem\">"
	 flag = TRUE;
	 result = "-- Callsign not found --";           // kein Mitglied bei QRZ
	 processWriteDatagram(11, result, 45450);       // INFO sent to addqso
       }
       
       if(flag && (lineData.contains(END_NOT_FOUND))) { // "<span style="
	 flag = FALSE;
       }
       
       if(lineData.contains(BEGIN_CALLSIGN)) {           // "begin calldata"
	 flag = TRUE;
       }
       
       if(flag && (lineData.contains(END_CALLSIGN))) {  // ">Email: "
	 flag = FALSE;
       }
       
       if(flag && (lineData.contains("<img src="))) {
	 lineData = "";
       }
       
       if(flag) {
	 str = str + lineData;
       }
     }
     if(str.count() != 0 ) {                     // found
       result = str;                             // Abfrage_Ergebnis auswerten
       s = ">"+Call;
       i= result.indexOf(s);                     // Call
       result.remove(0,i);
       i = result.indexOf("</b");
       s = result.left(i);
       n = s.count();
       call = s.mid(1);
       result.remove(0,i);
       i = result.indexOf("<p>");
       result.remove(0,i+3);
       i = result.indexOf("<");
       name = result.left(i);                    // name
       result.remove(0,i+7);
       i = result.indexOf("<");
       street = result.left(i);                  // street
       result.remove(0,i+7);
       i = result.indexOf("<");
       qth = result.left(i);                     // qth
       result.remove(0,i+7);
       i = result.indexOf("<");
       state = result.left(i);                   // state
       str = call+"|"+name+"|"+street+"|"+qth+"|"+state;
       processWriteDatagram(10, str, rport);     // Ergebnis an addqso senden # 45450
     }
     else {
       str = " -- Call not found --";
       processWriteDatagram(11, str, rport);      // Ergebnis an addqso senden #  45450
     }
}


// ---------------------------------------------------------------------
// Datagram : WRITE datagram_message
// ---------------------------------------------------------------------
void getMgr::processWriteDatagram(int dNr, QString mss, int rPort)
{
    datagram = QByteArray::number(dNr);          // message an qsldb.proz. -> data incoming
    datagram.append(",");
    datagram.append(mss);
    
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
               QHostAddress::LocalHost, rPort);
}



// -----------------------------------------------------------------------
// Datagram von addQso eingegangen
// Ein Call bei qrz.com abfragen
// -----------------------------------------------------------------------
void getMgr::processPendingDatagram()
{
   while(udpSocket->hasPendingDatagrams()) {
     datagram.resize(udpSocket->pendingDatagramSize());
     QHostAddress sender;
     quint16 senderPort;
     udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
     
     int i = datagram.indexOf (",");
     QByteArray Left = datagram.left(i);   // Funktion
     QByteArray Mid = datagram.mid(i+1);   // Parameter_Liste
     
     Com = Left.toInt();                   // funktions_no 1 = Auftrag von addqso, 2 = Auftrag von dxspot
     Call = Mid;                           // parameter, Rufzeiceen
    
     if(Com == -1) {                       // getmgr abschalten
       QCoreApplication::instance()->quit(); 
       return;
     }
     if(Com == 1)
       rport = 45450;                        // Antwort an addqso senden
     else
       rport = 45464;                        // Antwort an dxspot senden
     getit();                                // anfrage qrz.com durchführen
   }
}
