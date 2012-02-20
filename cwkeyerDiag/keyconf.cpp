/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.08.2009
********************************************************************** */
#include <QtSql>
#include <QMessageBox>
#include "keyconf.h"
#define ESC 0x1b

keyconf::keyconf(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
   int n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                             
   setFont(font);
   
   connect(ButtonReturn, SIGNAL(pressed()), this, SLOT(goReturn()));
   connect(lineKeyPort, SIGNAL(returnPressed()), this, SLOT(lineKeyPortCb()));
   
   connect(lineSoundOnOff, SIGNAL(returnPressed()), this, SLOT(lineSoundOnOffCb()));
   connect(lineSpeed, SIGNAL(returnPressed()), this, SLOT(lineSpeedCb()));
   connect(lineSideton, SIGNAL(returnPressed()), this, SLOT(lineSidetonCb()));
   connect(lineSoundVolume, SIGNAL(returnPressed()), this, SLOT(lineSoundVolumeCb()));
   connect(lineSoundDev, SIGNAL(returnPressed()), this, SLOT(lineSoundDevCb()));
   connect(lineWeight, SIGNAL(returnPressed()), this, SLOT(lineWeightCb()));
   connect(linePttKeying, SIGNAL(returnPressed()), this, SLOT(linePttKeyingCb()));
   connect(linePttDelay, SIGNAL(returnPressed()), this, SLOT(linePttDelayCb()));
   connect(lineSSBsig, SIGNAL(returnPressed()), this, SLOT(lineSSBsigCb()));
   connect(lineWordmode, SIGNAL(returnPressed()), this, SLOT(lineWordmodeCb()));
   connect(lineUdpPort, SIGNAL(returnPressed()), this, SLOT(lineUdpPortCb()));
   connect(lineTuneX, SIGNAL(returnPressed()), this, SLOT(lineTuneXCb()));
   connect(lineBandIndex, SIGNAL(returnPressed()), this, SLOT(lineBandIndexCb()));
   connect(lineReset, SIGNAL(returnPressed()), this, SLOT(lineResetCb()));
   udpSocket = new QUdpSocket(this);  
   udpPort = 6789;
   QString h = QDir::homePath();      
   s = h+"/log/iniFiles/cwdaemon.ini";
   qDebug() << "conf_1:" << s;
   QFile file(s);
   if(!file.open(QIODevice::ReadOnly)) {
          qDebug() << "FEHLER" << s;
   }
    
   QTextStream istream( &file);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineKeyPort->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineSoundOnOff->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineSpeed->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineSideton->setText(t);         
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineSoundVolume->setText(t);    
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineSoundDev->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineWeight->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   linePttKeying->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   linePttDelay->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineSSBsig->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineWordmode->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineUdpPort->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineTuneX->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineBandIndex->setText(t);
   s = istream.readLine(0);       
   i = s.indexOf("=");
   t = s.mid(i +1);
   lineReset->setText(t);
   file.close();
}

keyconf::~keyconf()
{
}

void keyconf::goReturn()
{
   saveConfigValue();
   accept();
}

void keyconf::lineKeyPortCb()
{
  processWriteDatagram("8", lineKeyPort->text()); 
  
  qDebug() << "PORT";
  s[0] = '0x1B';
  s += "8"+lineKeyPort->text();
  qDebug() << s;
  
}



void keyconf::lineResetCb()
{
   //processWriteDatagram(ESC, '0', lineSpeed->text(), udpPort); 
}



void keyconf::lineSpeedCb()  
{
   //processWriteDatagram(ESC, '2', lineSpeed->text(), udpPort);
}


void keyconf::lineSoundOnOffCb()
{
  
}


void keyconf::lineSidetonCb() 
{
   //processWriteDatagram(ESC, '3', lineSideton->text(), udpPort);
}






void keyconf::lineWordmodeCb()
{
   //qDebug() << "lineWordmode" << lineWordmode->text();
   //processWriteDatagram(ESC, '6', lineSpeed->text(), udpPort);
}


void keyconf::lineWeightCb()
{
   //processWriteDatagram(ESC, '7', lineWeight->text(), udpPort);
}



void keyconf::lineUdpPortCb()             
{
   //processWriteDatagram(ESC, '9', lineUdpPort->text(), udpPort);
}


void keyconf::linePttKeyingCb()
{
   //processWriteDatagram(ESC, 'a', linePttKeying->text(), udpPort);
}


void keyconf::lineSSBsigCb() 
{
   //processWriteDatagram(ESC, 'b', lineSSBsig->text(), udpPort);
}



void keyconf::lineTuneXCb() 
{
   //processWriteDatagram(ESC, 'c', lineTuneX->text(), udpPort);
}


void keyconf::linePttDelayCb()          
{
  
}



void keyconf::lineBandIndexCb()
{
   //qDebug() << "lineBandIndex" << lineBandIndex->text();
}


void keyconf::lineSoundDevCb()
{
   //qDebug() << "lineSoundDev" << lineSoundDev->text();
   //processWriteDatagram(ESC, 'f', lineSoundDev->text(), udpPort);
}



void keyconf::lineSoundVolumeCb()
{
   //qDebug() << "lineSoundVolume" << lineSoundVolume->text();
   //processWriteDatagram(ESC, 'g', lineSoundVolume->text(), udpPort);
}




void keyconf::processWriteDatagram(QString cmd, QString dev)
{
  char esc;
  
         esc = 27;
         //qDebug() << cmd << dev;
  
         datagram.clear();
         /*
         datagram[0] = esc;                      
         datagram[1] = com;                      
         if((com != '0') || (com != '6')) {
           datagram.append(" ");
           datagram.append(val);                 
         }
         */
 
         datagram.append(esc);
         datagram.append(cmd);
         datagram.append(dev);
   
    
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, udpPort); 
}

void keyconf::saveConfigValue()
{
    QString h = QDir::homePath();
    s = h+"/log/iniFiles/cwdaemon.ini";
    qDebug() << "conf:" << s;
    QFile file(s);
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "FEHLER" << s;
    }
    QTextStream ostream( &file);
    s = "Device="+lineKeyPort->text();         
    ostream << s << "\n";

    s = "SoundOnOff="+lineSoundOnOff->text();  
    ostream << s << "\n";
    s = "Speed="+lineSpeed->text();            
    ostream << s << "\n";
    s = "Sideton="+lineSideton->text();        
    ostream << s << "\n";
    s = "SoundVolume="+lineSoundVolume->text();
    ostream << s << "\n";
    s = "SoudDev="+lineSoundDev->text();       
    ostream << s << "\n";
    s = "Weight="+lineWeight->text();          
    ostream << s << "\n";
    s = "PttKeying="+linePttKeying->text();    
    ostream << s << "\n";
    s = "PttDelay="+linePttDelay->text();      
    ostream << s << "\n";
    s = "SSBsig="+lineSSBsig->text();          
    ostream << s << "\n";
    s = "Wordmode="+lineWordmode->text();      
    ostream << s << "\n";
    s = "UdpPort="+lineUdpPort->text();        
    ostream << s << "\n";
    s = "TuneX="+lineTuneX->text();            
    ostream << s << "\n";
    s = "BandIndex="+lineBandIndex->text();    
    ostream << s << "\n";
    s = "Reset="+lineReset->text();             
    ostream << s << "\n";
    file.close();
}
