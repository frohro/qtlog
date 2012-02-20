/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n : 14.09.2009
********************************************************************** */

#include <QtGui>
#include <QtSql>
#include <QTimer>
#include "keyer.h"
#include "keyconf.h"
#define ESC 0x1b
keyer::keyer(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    
    connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
    connect(actionCwkeyer, SIGNAL(triggered(bool)), this, SLOT(cwkeyerConfAc())); 
    connect(ButtonSave, SIGNAL(clicked()), this, SLOT(saveCWtext()));
    udpSocket = new QUdpSocket(this);              
    udpSocket->bind(QHostAddress::LocalHost,6809); 
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
    QString h = QDir::homePath();      
    s = h+"/log/iniFiles/cwtext.ini";
    qDebug() << s;
    QFile file(s);
    if(!file.open(QIODevice::ReadOnly)) {
          qDebug() << "FEHLER cwtext.ini fehlt" << s;
    }
    
    QTextStream istream( &file);       
    s = istream.readLine(0);       
    i = s.indexOf("=");
    t = s.mid(i +1);
    lineEditF1->setText(t);           
    s = istream.readLine(0);
    i = s.indexOf("=");
    t = s.mid(i +1);
    lineEditF2->setText(t);           
    s = istream.readLine(0);  
    i = s.indexOf("=");
    t = s.mid(i +1);
    lineEditF3->setText(t);           
    s = istream.readLine(0); 
    i = s.indexOf("=");
    t = s.mid(i +1);
    lineEditF4->setText(t);           
    s = istream.readLine(0);  
    i = s.indexOf("=");
    t = s.mid(i +1);
    lineEditF5->setText(t);           
    s = istream.readLine(0);  
    i = s.indexOf("=");
    t = s.mid(i +1);
    lineEditF6->setText(t);           
    s = istream.readLine(0);  
    i = s.indexOf("=");
    t = s.mid(i +1);
    lineEditF7->setText(t);           
    s = istream.readLine(0);  
    i = s.indexOf("=");
    t = s.mid(i +1);
    lineEditF8->setText(t);           
    s = istream.readLine(0);  
    i = s.indexOf("=");
    t = s.mid(i +1);
    lineEditF9->setText(t);           
    s = istream.readLine(0);  
    i = s.indexOf("=");
    t = s.mid(i +1);
    lineEditF10->setText(t);          
    s = istream.readLine(0); 
    i = s.indexOf("=");
    t = s.mid(i +1); 
    BoxSpeed->setValue(t.toInt());   
    s = istream.readLine(0); 
    i = s.indexOf("=");
    t = s.mid(i +1); 
    repeater->setValue(t.toInt());   
    file.close();
    cwdPort = 6789;                  
}

keyer::~keyer()
{
    goExit();
}

void keyer::goExit()
{
   qApp->quit();
}

void keyer::cwkeyerConfAc() 
{
     keyconf * kconf = new keyconf();
     kconf->exec();
     
}

void keyer::saveCWtext()
{
    QString h = QDir::homePath();      
    s = h+"/log/iniFiles/cwtext.ini";
    QFile file(s);
    if(!file.open(QIODevice::WriteOnly)) {
          qDebug() << "FEHLER cwtext.ini fehlt" << s;
    }
    QTextStream ostream( &file);       
    s = "F1="+lineEditF1->text();
    ostream << s << "\n"; 
    s = "F2="+lineEditF2->text();
    ostream << s << "\n"; 
    s = "F3="+lineEditF3->text();
    ostream << s << "\n"; 
    s = "F4="+lineEditF4->text();
    ostream << s << "\n"; 
    s = "F5="+lineEditF5->text();
    ostream << s << "\n"; 
    s = "F6="+lineEditF6->text();
    ostream << s << "\n"; 
    s = "F7="+lineEditF7->text();
    ostream << s << "\n"; 
    s = "F8="+lineEditF8->text();
    ostream << s << "\n"; 
    s = "F9="+lineEditF9->text();
    ostream << s << "\n"; 
    s = "F10="+lineEditF10->text();
    ostream << s << "\n"; 
    i = BoxSpeed->value();            
    s = "wpm="+t.setNum(i);  ;
    ostream << s << "\n";    
    
    i = repeater->value();           
    s ="repeat="+t.setNum(i); 
    ostream << s << "\n";  
    file.close();
    processWriteDatagram(15, "readmacros", 6799);  
}


//--------------------------------------------------------------
void keyer::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_PageUp : {                       
               i = BoxSpeed->value();
               i++;
               //i++;
               BoxSpeed->setValue(i);
            }
            break;
     case Qt::Key_PageDown : {                     
               i = BoxSpeed->value();
               i--;
               //i--;
               BoxSpeed->setValue(i);
            }
            break;
   }
}



void keyer::processWriteDatagram(QString str, int port)
{
    datagram.clear();
    datagram.append(str);
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, port);
}



void keyer::processWriteDatagram(int comNr, QString macroStr, int port)
{
    datagram.clear();
    datagram = QByteArray::number(comNr);
    datagram.append(",");
    datagram.append(macroStr);
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, port);
}



void keyer::processPendingDatagram()
{
   while(udpSocket->hasPendingDatagrams()) {
     datagram.resize(udpSocket->pendingDatagramSize());
     QHostAddress sender;
     quint16 senderPort;
     udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
     if(datagram[1] == '0')
        goExit();
   }
}
