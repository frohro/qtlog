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
    spotList->setColumnWidth(0,95);  
    spotList->setColumnWidth(1,90);  
    spotList->setColumnWidth(2,100); 
    spotList->setColumnWidth(3,300); 
    spotList->setColumnWidth(4,100); 
 
    
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
       s = query.value(0).toString();        
       t = query.value(1).toString();        
       BoxSpots->insertItem(i++,t);
       RangeDes.insert(t,s);                 
    }
    
    BoxCustom->clear();
    qy = "SELECT rangeval,descript FROM wspotsparm WHERE sel=2";
    query.exec(qy);
    i = 0;
    while(query.next()) {
       s = query.value(0).toString();        
       t = query.value(1).toString();        
       BoxCustom->insertItem(i++,t);
       CustomDes.insert(t,s);                
    }
    
    qy = "SELECT urlstr,fetchd,count,ranged FROM wspotnodes WHERE run = 1";
    query.exec(qy);
    query.next();
    urlSpot = query.value(0).toString();                          
    urlSpot += "/"+query.value(1).toString();                     
    urlSpot += "?count="+query.value(2).toString();               
    urlSpot += "&range="+getRangeVal(query.value(3).toString());  
    BoxSpots->setCurrentIndex(BoxSpots->findText(query.value(3).toString()));
     
    if(query.value(1).toString().indexOf("DxSpots") != -1) {
       BoxSpots->setPalette( QPalette(QColor(180, 210, 200)));   
       BoxCustom->setCurrentIndex(BoxCustom->findText("-"));     
    }
    connect(BoxSpots, SIGNAL(currentIndexChanged(QString)), this, SLOT(spotsCb(QString)));
    connect(BoxCustom, SIGNAL(currentIndexChanged(QString)), this, SLOT(customCb(QString)));
    connect(spotList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(spotListClicked(QTreeWidgetItem*,int)));
    connect(ButtonF2, SIGNAL(pressed()), this, SLOT(writeForAddQso()));
    connect(lineEditSpotCall, SIGNAL(textEdited(QString)), this, SLOT(callUpperCb(QString)));
    
    udpSocket = new QUdpSocket(this);                           
    
    udpSocket = new QUdpSocket(this);                    
    udpSocket->bind(QHostAddress::LocalHost,45464);      
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT (processPendingDatagram()));
    
    connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
    
    dxtimer = new QTimer(this);              
    connect(dxtimer, SIGNAL(timeout()), this, SLOT(scanDxCall()));
   
    ntimer = new QTimer(this); 
    connect(ntimer,SIGNAL(timeout()), this, SLOT(getSpots()));   
    ntimer->start(100);                                          
    xcnt = 0;
    timerflg = 1;                                                
    urlFlg = 0;                                                  
}

dxspot::~dxspot()
{
   QSqlQuery query; 
   qy = "UPDATE wspotnodes SET ranged='"+BoxSpots->currentText();
   qy += "' WHERE node='OH8'";
   query.exec(qy);
}

void dxspot::getNodes()
{
  stackedWidget->setCurrentIndex(1);
}

void dxspot::goExit()
{
   qApp->quit();
}

void dxspot::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F2 :
                writeForAddQso();                        
             break;
     case Qt::Key_Escape :
                if(stackedWidget->currentIndex() == 2) { 
                   s = "last ";
                   datagram = QByteArray::number(3);     
                   datagram.append(",");
                   datagram.append(s);
                   udpSocket->writeDatagram(datagram.data(),datagram.size(),
                   QHostAddress::LocalHost, 45450);      
                   lineEditDxCall->setText("");          
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



void dxspot::buttonReturnCb()
{
    
    QSqlQuery query;  
    qy = "SELECT urlstr,fetchd FROM wspotnodes WHERE run = 1";
    query.exec(qy);
    query.next();
    urlSpot = query.value(0).toString();            
    urlSpot += "/"+query.value(1).toString();       
    urlSpot += "?count="+BoxCount->currentText();   
    
    if(BoxSpots->currentText().compare("-") == 0)
       urlSpot += "&range="+getCustomVal(BoxCustom->currentText()); 
    else
       urlSpot += "&range="+getRangeVal(BoxSpots->currentText());
    
    ntimer->stop();
    ntimer->start(100);
    timerflg = 1;                                   
    stackedWidget->setCurrentIndex(0);
}



void dxspot::spotsCb(QString str)
{
    if(str.compare("-") == 0)
       return;
    BoxSpots->setPalette( QPalette(QColor(180, 210, 200)));   
    BoxCustom->setPalette( QPalette(QColor(224, 224, 224)));  
    BoxCustom->setCurrentIndex(BoxCustom->findText("-"));
  
    QSqlQuery query;       
    qy = "SELECT urlstr,fetchd FROM wspotnodes WHERE run = 1";
    query.exec(qy);
    query.next();
    
    urlSpot = query.value(0).toString();            
    urlSpot += "/"+query.value(1).toString();       
    urlSpot += "?count="+BoxCount->currentText();   
    urlSpot += "&range="+getRangeVal(str);          
    ntimer->stop();
    ntimer->start(100);
    timerflg = 1;                                                
}



void dxspot::customCb(QString str)
{
    if(str.compare("-") == 0) 
      return;
    
    BoxCustom->setPalette( QPalette(QColor(180, 210, 200))); 
    BoxSpots->setPalette( QPalette(QColor(224, 224, 224)));  
    BoxSpots->setCurrentIndex(BoxSpots->findText("-"));
    
    QSqlQuery query;      
    qy = "SELECT urlstr FROM wspotnodes WHERE run = 1";
    query.exec(qy);
    query.next();
    urlSpot = query.value(0).toString();                 
    urlSpot += "/CustomFilter.aspx";                     
    urlSpot += "?customCount="+BoxCount->currentText();  
    urlSpot += "&customRange="+getCustomVal(str);        
 
    ntimer->stop();
    ntimer->start(100);
    timerflg = 1;                                       
}




void dxspot::spotListClicked(QTreeWidgetItem *item, int col)
{
      if((col == 1) || (col == 2)) {                  
        bItem = item;                                 
        
        lineEditFreq->setText(item->text(1));         
        s = item->text(1);
        s = s.trimmed();
        i = s.indexOf(".");
        s = s.remove(i,1);
        s += "00 ";                                   
        
        dxCall = item->text(2).trimmed();
        
        datagram = QByteArray::number(3);             
        datagram.append(",");
        datagram.append(s);
	//datagram.append(" ");
	//datagram.append(dxCall);
	
        udpSocket->writeDatagram(datagram.data(),datagram.size(),
                    QHostAddress::LocalHost, 45450);  
        
        dxCall = item->text(2).trimmed();
        xcnt = 1;
        dxtimer->start(10);                
        
        
        
        
        datagram = QByteArray::number(2);
        datagram.append(",");
        datagram.append(dxCall);
        udpSocket->writeDatagram(datagram.data(),datagram.size(),
                    QHostAddress::LocalHost, 45460);  
        timerflg = 1; 
        ntimer->stop();                   
        ntimer->start(40);                
	
	stackedWidget->setCurrentIndex(2);
     }
}



void dxspot::writeForAddQso()
{
       datagram = QByteArray::number(3);         
       datagram.append(",");
       
       t = lineEditDxCall->text();               
       s = "<call>"+t.setNum(t.count())+" "+lineEditDxCall->text();
        
       t = lineEditName->text();                 
       if(t.count() != 0)                        
         s += "<"+BoxName->currentText()+">"+t.setNum(t.count())+" "+lineEditName->text();
       
       t = lineEditQth->text();                  
       if(t.count() != 0)                        
         s += "<"+BoxQth->currentText()+">"+t.setNum(t.count())+" "+lineEditQth->text().trimmed();
       
       if(lineEditQslMgr->text().count() != 0) { 
         t = lineEditQslMgr->text();
         s += "<"+BoxQslMgr->currentText()+">"+t.setNum(t.count())+" "+lineEditQslMgr->text();
       }
       
       datagram.append(s);                       
       udpSocket->writeDatagram(datagram.data(),datagram.size(),
       QHostAddress::LocalHost, 45450);          
      
       lineEditDxCall->setText("");              
       lineEditName->setText("");
       lineEditQth->setText("");
       lineEditQslMgr->setText("");
       lineEditFreq->setText("");
       stackedWidget->setCurrentIndex(0);
}



void dxspot::sendSpotCb()
{
    lineEditOwnerCall->setText(settings.value("Call").toString());
    stackedWidget->setCurrentIndex(1);
}

void dxspot::callUpperCb(QString str)
{
    str = str.toUpper();
    lineEditSpotCall->setText(str);
}



void dxspot::sendSpotTextCb()
{
    QString s;
    s = "http://www.dxsummit.fi/SendSpot.aspx?callSign="+lineEditOwnerCall->text();
    s += "&dxCallSign="+lineEditSpotCall->text();
    s += "&frequency="+lineEditSpotFreq->text();
    s += "&info="+lineEditSpotInfo->text();
    urlQrz = s;
    
    urlFlg = 2;                                  
    timerflg = 1;                                
    ntimer->stop(); 
    ntimer->start(40);                           
    stackedWidget->setCurrentIndex(0);
}



void dxspot::sendAnnounceTextCb()
{
    QString s;
    s = "http://www.dxsummit.fi/SendSpot.aspx?callSign="+lineEditOwnerCall->text();
    s += "&info="+lineEditSpotInfo->text();
    urlQrz = s;
 
    urlFlg = 2;                                  
    timerflg = 1;                                
    ntimer->stop(); 
    ntimer->start(40);                           
    stackedWidget->setCurrentIndex(0);
}


void dxspot::scanDxCall()
{
     QString f;
     if(dxCall.count() == xcnt -1)
       dxtimer->stop();
     else {
       f = dxCall.left(xcnt);
       if(f.indexOf("/") != -1)     
         dxtimer->stop();           
       else
        getLaPrefix(f);
     }
     xcnt++;
}

void dxspot::getLaPrefix(QString callstr)
{
  QString dxcc, lname, cnt, cqz, br, lg;
     QSqlQuery query;                                  
     qy = "SELECT ldxcc,lname,lcn,ituz,cqz,br,lg FROM tpref,tla WHERE pref=la AND spref='"+callstr+"'";
     query.exec(qy);  
     if(query.size() == 1) {                           
       n = 0;
       query.next();
       
       dxcc = query.value(n++).toString();             
       lname = query.value(n++).toString();            
       cnt = query.value(n++).toString();              
       
       s = dxcc+"  - "+lname+"   "+cnt+" /  ";         
       s += "ITU: "+query.value(n++).toString();       
       cqz = "  -  CQz: "+query.value(n++).toString(); 
       s += cqz;
       labelInfo->setText(s); 
       
       br = query.value(n++).toString();               
       lg = query.value(n++).toString();               
       s = distanz.getDisRicht(br,lg);                 
       labelDistanz->setText(s);                       
     }
}




void dxspot::processPendingDatagram()
{
     while(udpSocket->hasPendingDatagrams()) {
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
     
        i = datagram.indexOf (",");
        s = datagram.left(i);              
        t = datagram.mid(i +1);            
        i = s.toInt();                     
     }
     i= t.indexOf("|");                      
     lineEditDxCall->setText(t.left(i));
     t.remove(0,i+1);
       
     i = t.indexOf("|");
     lineEditName->setText(t.left(i));       
     t.remove(0,i +1);
       
     i = t.indexOf("|");
     t.remove(0,i +1);                       
     
     i = t.indexOf("|");
     lineEditQth->setText(t.left(i));        
     t.remove(0, i +1);
      
     i = t.indexOf("|");
     lineEditQslMgr->setText(t.left(i));     
}
    
