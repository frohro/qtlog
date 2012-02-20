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

getMgr::getMgr()
{
    udpSocket = new QUdpSocket(this);                    
    udpSocket->bind(QHostAddress::LocalHost,45460);      
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT (processPendingDatagram()));
    
    http = new QHttp(this);
    connect(http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
    
    //cntVal = 0;
    //timer = new QTimer(this);                          
    //connect(timer,SIGNAL(timeout()), this, SLOT(timerpollRig()));
    //timer->start(1000);                                
}

getMgr::~getMgr()
{ }

void getMgr::execute()
{
     
}

void getMgr::getit()
{
     website = "www.qrz.com";
     callsign = Call.toUpper();
     urlstr = "/callsign/"+callsign;
     
     http->setHost(website);
     http->get(urlstr);
}

void getMgr::httpDone(bool error)
{
     if(error) {
       ErrorStr = http->errorString();
       processWriteDatagram(11, ErrorStr, rport);      
       return;
     }
     parseit();
}

void getMgr::parseit()
{
  QString call,name,street,qth,state;
  int i, n;
 
     QTextStream stream(http->readAll());
     flag = FALSE;
     str = "";
     while(!stream.atEnd()) {
       lineData = stream.readLine();                    
       if(lineData.contains(NOT_FOUND)) {               
	 flag = TRUE;
	 result = "-- Callsign not found --";           
	 processWriteDatagram(11, result, 45450);       
       }
       
       if(flag && (lineData.contains(END_NOT_FOUND))) { 
	 flag = FALSE;
       }
       
       if(lineData.contains(BEGIN_CALLSIGN)) {          
	 flag = TRUE;
       }
       
       if(flag && (lineData.contains(END_CALLSIGN))) {  
	 flag = FALSE;
       }
       
       if(flag && (lineData.contains("<img src="))) {
	 lineData = "";
       }
       
       if(flag) {
	 str = str + lineData;
       }
     }
     if(str.count() != 0 ) {                     
       result = str;                             
       s = ">"+Call;
       i= result.indexOf(s);                     
       result.remove(0,i);
       i = result.indexOf("</b");
       s = result.left(i);
       n = s.count();
       call = s.mid(1);
       result.remove(0,i);
       i = result.indexOf("<p>");
       result.remove(0,i+3);
       i = result.indexOf("<");
       name = result.left(i);                    
       result.remove(0,i+7);
       i = result.indexOf("<");
       street = result.left(i);                  
       result.remove(0,i+7);
       i = result.indexOf("<");
       qth = result.left(i);                     
       result.remove(0,i+7);
       i = result.indexOf("<");
       state = result.left(i);                   
       str = call+"|"+name+"|"+street+"|"+qth+"|"+state;
       processWriteDatagram(10, str, rport);     
     }
     else {
       str = " -- Call not found --";
       processWriteDatagram(11, str, rport);     
     }
}



void getMgr::processWriteDatagram(int dNr, QString mss, int rPort)
{
    datagram = QByteArray::number(dNr);          
    datagram.append(",");
    datagram.append(mss);
    
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
               QHostAddress::LocalHost, rPort);
}




void getMgr::processPendingDatagram()
{
   while(udpSocket->hasPendingDatagrams()) {
     datagram.resize(udpSocket->pendingDatagramSize());
     QHostAddress sender;
     quint16 senderPort;
     udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
     
     int i = datagram.indexOf (",");
     QByteArray Left = datagram.left(i);     
     QByteArray Mid = datagram.mid(i+1);     
     Com = Left.toInt();                     
     Call = Mid;                             
    
     if(Com == -1) {                         
       QCoreApplication::instance()->quit(); 
       return;
     }
     if(Com == 1)
       rport = 45450;                        
     else
       rport = 45464;                        
     getit();                                
   }
}
