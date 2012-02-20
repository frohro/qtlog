/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n
********************************************************************** */

#include <QtGui>
#include <mysql.h>
#include "qsldb.h"
#include "../qtlogDiag/dirmngr.h"
//extern DirMngr dirMngr;
qsldb::qsldb(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
    connect(cardList, SIGNAL(itemClicked(QTreeWidgetItem *,int)), this, SLOT(cardListClicked(QTreeWidgetItem*,int)));
    connect(lineEdit, SIGNAL(textChanged(QString)), this, SLOT(searchQslCardCb(QString)));
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(searchQslCardCB()));
    path =  QDir::homePath();                      
    s = path+"/log/icon/AuthGuaranteed.gif";
    label->setPixmap(QPixmap(s));
    label->adjustSize();
    s = path+"/log/icon/dl1hbd.jpg";
    cardLabel->setPixmap(QPixmap(s));
    cardLabel->adjustSize();
    s = settings.value("host").toString();
    strcpy(host,s.toAscii());
    s = settings.value("dbname").toString();
    strcpy(dbname,s.toAscii());
    s = settings.value("dbuser").toString();
    strcpy(dbuser,s.toAscii());
    s = settings.value("dbpasswd").toString();
    strcpy(dbpasswd,s.toAscii());
    db = new Connection( host, dbname, dbuser, dbpasswd );
    labeldbInfo->setText(db->GetConnectInfo());
 
    cardList->setColumnWidth(0,80);   
    cardList->setColumnWidth(1,100);  
    cardList->setColumnWidth(2,60);   
    cardList->setColumnWidth(3,60);   
    cardList->setColumnWidth(4,50);   
    udpSocket = new QUdpSocket(this);                    
    udpSocket->bind(QHostAddress::LocalHost,45508);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
    pcount = 0;
    logFile = settings.value("Logfile").toString();
    qy = "SELECT qcall,qsoday,cband,cmode,qsl,qsocid FROM "+logFile+"card";
    showQslCards(qy);
    lineEdit->setFocus(); 
}

qsldb::~qsldb()
{
   processWriteDatagram(3, "xx", 45504);
}

void qsldb::goExit()
{
  db->Close();
  qApp->quit();
}


void qsldb::showQslCards(QString q)
{
    cardList->clear();
    res = db->Query(q);
    if(db->GetState()) { 
      qDebug() << db->GetError(); 
    }
    n = 0;
    while( res->Next() != 0) {
       r = 1;
       i = 0;
       rows = res->GetCurrentRow();
       QTreeWidgetItem *item = new QTreeWidgetItem(cardList);
       item->setText(i++,rows->GetField(r++));      
       item->setText(i++,rows->GetField(r++));      
       item->setText(i++,rows->GetField(r++));      
       item->setText(i++,rows->GetField(r++));      
       item->setText(i,rows->GetField(r++));        
       item->setTextAlignment(i++,Qt::AlignCenter);
       item->setText(i++,rows->GetField(r++));      
      n++;
    }
    res->Close();
    p.setNum(n);
    labeldbSize->setText("Qsl  Bestand: "+p);
}

void qsldb::cardListClicked(QTreeWidgetItem * item, int r)
{
     QString cid = item->text(5);
     viewCard( cid, r );
}

void qsldb::viewCard(QString qsoid, int r)
{
     QString len;
     unsigned long il;
     qy = "SELECT strlen FROM "+logFile+"card WHERE qsocid="+qsoid;
     len = db->GetOneField(qy);
     if(len.count()) {
       il = len.toLong();
       QPixmap bild;   
       qy = "SELECT qcard FROM "+logFile+"card WHERE qsocid="+qsoid;
       res = db->Real_Query(qy, qy.count());                
       res->Next();
       rows = res->GetCurrentRow();
       r = 1;
       bild.loadFromData((uchar*)rows->GetField(r),il);
       res->Close();
       cardLabel->setPixmap(bild);                          
       cardLabel->adjustSize();
     }
     else {
      s = path+"/log/icon/dl1hbd.jpg";
      cardLabel->setPixmap(QPixmap(s));
      cardLabel->adjustSize();
     }
}

void qsldb::insertQslCard()
{
}

void qsldb::searchQslCardCb(QString str) //searchQslCardCb(QString)));
{
   strqy = str;
   if(str.count() >= 3) {
     qy = "SELECT qcall,qsoday,cband,cmode,qsl,qsocid FROM "+logFile+"card ";
     qy += "WHERE qcall LIKE '"+str+"%'";
     showQslCards(qy);
   }
}

void qsldb::searchQslCardCB()
{
     qy = "SELECT qcall,qsoday,cband,cmode,qsl,qsocid FROM "+logFile+"card ";
     qy += "WHERE qcall LIKE '"+strqy+"%'";
     showQslCards(qy);
}



void qsldb::processPendingDatagram()
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
     s = Mid;                                      
     if(com == 1)                                  
        viewCard( s, 0 );
     else
      if(com == -1)
       goExit();
  }
}



void qsldb::processWriteDatagram(int dNr, QString idNr, int port)
{
    datagram = QByteArray::number(dNr);
    datagram.append(",");
    datagram.append(idNr);
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, port);       
}
