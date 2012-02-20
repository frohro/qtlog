/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 16-03-2010
********************************************************************** */
#include <QtSql>
#include <qmessagebox.h>
#include "rigctl.h"
#include "../addClog/defs.h"


rigctl::rigctl(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    connect(ButtonESC, SIGNAL(pressed()), this, SLOT(goExit()));
    connect(ButtonStart, SIGNAL(pressed()), this, SLOT(goStart()));
    connect(ButtonStop, SIGNAL(pressed()), this, SLOT(goStop()));
    connect(buttonReturn, SIGNAL(pressed()), this, SLOT(showRigPage()));
	
    tcpSocket = new QTcpSocket(this);                   
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(sentCommand()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readRigReply()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                                            this, SLOT(displayError(QAbstractSocket::SocketError)));                        
    Retry = 0;
    Runing = 0;
    r = 0;
    
    home = getenv("HOME");                             
    s = "ls /dev/ttyS* > "+home+"/.qtlog/rigstatus";
    i = system(s.toAscii());                           
    home += "/.qtlog/rigstatus";                       
    QFile iniFile(home);
    iniFile.open(QIODevice::ReadOnly);
    QTextStream istream( &iniFile);                    
       
    RigDevBox->clear();
    PttBitDevBox->clear();
    i = 0;
    while(istream.atEnd() != true) {
       s = istream.readLine(0);
       RigDevBox->insertItem(i,s);
       PttBitDevBox->insertItem(i++,s);
    }
    iniFile.close();
         
    home = getenv("HOME"); 
    s = "ls /dev/ttyU* > "+home+"/.qtlog/rigstatus";
    n = system(s.toAscii());                           
    home += "/.qtlog/rigstatus";
    QFile inuFile(home);
    inuFile.open(QIODevice::ReadOnly);
    QTextStream iustream( &inuFile);                   
    while(iustream.atEnd() != true) {
      s = iustream.readLine(0);
      RigDevBox->insertItem(i,s);                      
      PttBitDevBox->insertItem(i++,s);
    }
    inuFile.close();
         
    QSqlQuery query;                                    
    qy = "SELECT * FROM wproz WHERE proz='HamLib'";
    query.exec(qy);
    query.next(); 
    proz = query.value(r++).toString();                 
    owner = query.value(r++).toString();                
    if(owner.count() != 0) {                            
      editRig->setText(query.value(r++).toString());    
      editPid->setText(query.value(r++).toString());    
      editRetry->setText(query.value(r++).toString());  
      Retry = editRetry->text().toInt();
      i = RigDevBox->findText(query.value(r++).toString());
      RigDevBox->setCurrentIndex(i);                    
         
      i = BaudBox->findText(query.value(r++).toString());
      BaudBox->setCurrentIndex(i);                      
      
      i = InterfaceBox->findText(query.value(r++).toString()); 
      InterfaceBox->setCurrentIndex(i);
      i = PttBitDevBox->findText(query.value(r++).toString());
      PttBitDevBox->setCurrentIndex(i);                 
      editRts->setText(query.value(r++).toString());    
      editDtr->setText(query.value(r++).toString());    
      editCts->setText(query.value(r++).toString());    
      
      if(InterfaceBox->currentText().compare("NO") == 0) {
         PttBitDevBox->setEnabled(FALSE);
         editRts->setEnabled(FALSE);
         editDtr->setEnabled(FALSE);
         editCts->setEnabled(FALSE);
      }
      editHost->setText(query.value(r++).toString());   
      editPort->setText(query.value(r++).toString());   
      Runing = query.value(r).toInt();                  
    }
        
    if(Runing) {
       ButtonStop->setPalette( QPalette(QColor(180, 210, 200)));  
       ButtonStart->setPalette( QPalette(QColor(180, 210, 200))); 
    }
   
    if(owner.compare("hamlib") == 0) {                 
      RigDevBox->setEnabled(FALSE);
      BaudBox->setEnabled(FALSE);
      InterfaceBox->setEnabled(FALSE);
      PttBitDevBox->setEnabled(FALSE);
      ButtonRigList->setEnabled(FALSE);
      
      editRts->setEnabled(FALSE);
      editDtr->setEnabled(FALSE);
      editCts->setEnabled(FALSE);
      
      ButtonStop->setPalette( QPalette(QColor(180, 210, 200)));  
      ButtonStart->setPalette( QPalette(QColor(180, 210, 200))); 
    }
    
    editHost->setEnabled(FALSE);                          
    editPort->setEnabled(FALSE);                          
    updateFlg = 0;
                         
    if((owner.compare("hamlib") == 0) || (owner.count() == 0)) 
      checkhamlib();                                      
      
}

rigctl::~rigctl()
{
}

void rigctl::goExit()
{
   accept();
}

void rigctl::showRigPage()
{
  stackedWidget->setCurrentIndex(0);
}


void rigctl::goStart()
{
    if(owner.compare("hamlib") == 0) {                    
      Retry = editRetry->text().toInt();
      tcphost = editHost->text();
      tcpport = editPort->text();
  
      if(!checkServerProz())                              
        startServer();                                    
      QTimer::singleShot(400, this, SLOT(requestRig()));  
    }
}


bool rigctl::showSuccess(QString freq)
{
    int status = QMessageBox::question( this,   
                 tr("Erfolg ?"),
                 tr("\nRx - %1 Herz\n\n"
                    "Wird die Frequenz richtig angezeigt ?")
                 .arg(freq),
                 QMessageBox::Yes|QMessageBox::Default,
                 QMessageBox::No|QMessageBox::Escape,
                 QMessageBox::NoButton);
        if(status != QMessageBox::Yes) {
          return false;
        }
        
        Retry = editRetry->text().toInt();    
        Runing = 1;                           
        settings.setValue("RigPid",editPid->text());
        qy = "UPDATE wproz set runing=1 WHERE proz='HamLib'"; 
        QSqlQuery query(qy); 
        updateFlg = 1;
        
        goExit();
        return true;
}

void rigctl::goStop()
{
     updateFlg = 2;            
     Retry = 0;
     qy = "UPDATE wproz SET runing=0 WHERE proz='HamLib'";
     QSqlQuery query(qy); 
     goExit();
}



void rigctl::checkhamlib()
{
  QString s, home, t;
  int i;
     
     home = getenv("HOME");                         
     s = "rigctl -l > ";
     s += home;
     s +="/.qtlog/rigstatus";
     i = system(s.toAscii());
     
     home += "/.qtlog/rigstatus";                   
     QFile inFile(home);
     inFile.open(QIODevice::ReadOnly);
     
     if(!inFile.size()) {                           
        QMessageBox::information(this,tr("Hamlib"), 
        tr("\nHamlib ist nicht installiert"));
       return;
     }
    
     wrigList->clear();
     inFile.close();
}

int rigctl::checkServerProz()
{
      QString home, p;                       
      home = getenv("HOME"); 
      p = "ps -e | grep rigctld > "+home+"/.qtlog/rigstatus";
      i = system(p.toAscii());               
      home += "/.qtlog/rigstatus";           
      QFile iniFile(home);
      iniFile.open(QIODevice::ReadOnly);
      QTextStream istream( &iniFile);        
      p = istream.readLine(0);               
      iniFile.close();
      
      if(p.indexOf("rigctld") == -1)         
       return 0;                             
      else
       return 1;                             
}

void rigctl::startServer()
{
       QString p;
       int n;
       QSqlQuery query;                     
       qy = "SELECT rigpid,rigdev,baud,pttdev,rts,dtr FROM wproz WHERE proz='HamLib'";
       query.exec(qy);
       query.next();
       n = 0;
       p = "rigctld -m "+query.value(n++).toString();       
       p += " -r "+query.value(n++).toString();             
       p += " -s "+query.value(n++).toString();             
       if(InterfaceBox->currentText().compare("Funkamateur ONE") == 0) {
         p += " -p "+query.value(n++).toString();           
         p += " -C rts_state="+query.value(n++).toString(); 
         p += " -C dtr_state="+query.value(n++).toString(); 
       }
       p += " &";
       i = system(p.toAscii());  
}


void rigctl::requestRig()
{
     tcpSocket->abort();            
     tcpSocket->connectToHost(tcphost,tcpport.toInt());
}



void rigctl::sentCommand()
{
    QString message;
    message = "f";                   
    tcpSocket->write(message.toAscii());
    tcpSocket->flush();
}



void rigctl::readRigReply()
{
    QString st;
    QString message(tcpSocket->readAll());            
    
    if(message.indexOf("RPRT") != -1) {
      QMessageBox::information(this,tr("Hamlib Client"),
                   tr("\nError ocuured timedout.\n\n"
                   "Check the transceiver connection.\n"
                   "Hardware unavailable !\n\n"
                   "* Server stopped. *"));
      return;
    }
    
    message = message.simplified();
    showSuccess(message);
}


void rigctl::displayError(QAbstractSocket::SocketError socketError)
{ 
    switch(socketError) {
      case QAbstractSocket::RemoteHostClosedError:
        break;
      case QAbstractSocket::HostNotFoundError:
           QMessageBox::information(this,tr("Hamlog Client"),
                                    tr("\nThe host was not found.\n"
                                    "Please check the host name and port setting."));
        break;
      case QAbstractSocket::ConnectionRefusedError:
           QMessageBox::information(this,tr("Hamlog Client"),
                                    tr("\nThe connection was refused by the peer.\n\n"
                                    "Make sure the hamlib server is running.\n"
                                    "and check that the host name and port settings\n"
                                    "are correct."));
        break;
      default: 
           QMessageBox::information(this,tr("Hamlog Client"),
                                    tr("\nThe folloing error ocuured: %1.")
                                    .arg(tcpSocket->errorString()));
    }
}
