/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n 12.06.09
********************************************************************** */

#include <QCoreApplication>
#include <QtGui>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTimer>
#include <stdio.h>
#include <QtSql>
#include "uploadMgr.h"
#include "../qtlogDiag/dirmngr.h"
extern DirMngr dirMngr;

UploadMgr::UploadMgr()
{
     connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(uploadFinished(QNetworkReply*)));
     timer = new QTimer(this);                   
     connect(timer,SIGNAL(timeout()), this, SLOT(timerCheckforUpload()));
     timer->start(2000);                         
     QStringList args = QCoreApplication::instance()->arguments();
     int n;
     n = args.size();                            
     for (int i = 1; i < args.size(); ++i) {
          if(i != 0)
             s += args.at(i).toLocal8Bit().constData();
          if(i == n -1) break;
     }
     bPort = s.toInt();                           
     udpSocket = new QUdpSocket(this);            
     udpSocket->bind(QHostAddress::LocalHost,bPort);
     connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
     openDatabase();
     setBandTable();
     uploadAc = 0;
    
}

UploadMgr::~UploadMgr()
{
  db.close();
}



void UploadMgr::doUpload(const QUrl url)
{
    QNetworkRequest request;
    QString n;
    n = 2;
    request.setAttribute(QNetworkRequest::RedirectionTargetAttribute, n.toInt()); 
    request.setUrl(url);
 
    QNetworkReply *reply = manager.get(request);
    currentUploads.append(reply);
}

QString UploadMgr::saveFileName(const QUrl &url)
{
    QString s, p, r;
    QString path = url.path();
    QString basename = QFileInfo(path).fileName(); 
    p =  QDir::homePath(); 
    p += "/eQsl/";
    s = p+basename;                               
    return s;                                     
}

bool UploadMgr::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
            qPrintable(filename),
            qPrintable(file.errorString()));
        return false;
    }
    file.write(data->readAll());
    file.close();
    return true;
}

void UploadMgr::execute()
{
    
}

void UploadMgr::uploadFinished(QNetworkReply *reply)
{
    QString t, eqslr;
    QUrl url = reply->url();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    
    if( statusCode == 302 ) {                             
        QUrl nUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        doUpload(nUrl);                                   
        
    }
    else {
     if (reply->error()) {
         
         
        return;
     } 
     else {
      QString filename = saveFileName(url);             
      if (saveToDisk(filename, reply)) {                
       
       
       //: <BR>
       
        QFile datei(filename);
        datei.open(QIODevice::ReadOnly);
        QTextStream istream( &datei);
        n = 0;
        while( istream.atEnd() != true ) {              
          t = istream.readLine(0);
          if(t.count() != 0) {
            if(t.indexOf("Result: 1 out of 1 records added") != -1) { 
               n = 1;
               break;
            }
          }
        }
        if( n ) {                                           
          QDate heute = QDate::currentDate();               
          datum = heute.toString("yyyy-MM-dd");
          
          QSqlQuery query;                                  
          qy = "SELECT qslSvia,eqslr";
          qy += " FROM "+logFile+" LEFT JOIN ("+logFile+"qsl) ON (qsoid=id)";
          qy += " WHERE id="+s.setNum(id);
          query.exec(qy);
          query.next();
          t = query.value(0).toString();
          eqslr = query.value(1).toString();
    
          if(t.indexOf("E") == -1)
            t += "E";
          qy = "UPDATE "+logFile+" SET qslSvia='"+t+"' WHERE id="+s.setNum(id);
          query.exec(qy);
          if(eqslr.compare("Y") == 0)
            t = "Y";
          else
            t = "U";
          qy = "UPDATE "+logFile+"qsl SET eqsls='"+t+"', eqslsd='"+datum+"' WHERE qsoid="+s.setNum(id);
          query.exec(qy);
          //qDebug() << "info an addQso oder qsldb " << val << Call << rport;
          processWriteDatagram(val, Call, rport);            
          QHash<int, int>::iterator i = uploadHash.find(id); 
          while (i != uploadHash.end() && i.key() == id) {
             if(i.key() == id) break;
              ++i;
          } 
          uploadHash.erase(i);                               
          uploadAc = 0;                                      
        }
        else {               
          processWriteDatagram(val,"ERROR", rport);          
          QCoreApplication::instance()->quit(); 
        }
       }
     }
     reply->deleteLater();                                   
     if (currentUploads.isEmpty()) {                         
         
         return;
     }
    }
}

void UploadMgr::timerCheckforUpload()
{
    if( !uploadAc ) {                     
      if(uploadHash.isEmpty() != true) {  
        uploadAc++;                       
        mkuploadEqslData();               
      }
    }
    else {
     if(uploadAc++ == 15) {                     
        processWriteDatagram(9, Call, rport);   
        
        QHash<int, int>::iterator i = uploadHash.find(id);
        while (i != uploadHash.end() && i.key() == id) {
          if(i.key() == id) break;
          ++i;
        }
        uploadHash.erase(i);             
        uploadAc = 0;
     }
   }
}

void UploadMgr::mkuploadEqslData()
{
       QString cept, call, t;
       int len;
       QHash<int,int>::const_iterator i = uploadHash.begin(); 
       id  = i.key();                                         
       val = i.value();                                       
       if(val == 1)                   
           rport = 45450;             
        if(val == 2)
           rport = 45504;             
       if( val != -1 ) {              
        QString h="http://www.eqsl.cc/qslcard/ImportADIF.cfm?ADIFData=upload%20%3CADIF%5FVER%3A1%3E2%20%3CPROGRAMID%3A5%3EQtLog%20%3CEOH%3E%20";
        QSqlQuery query;                            
        qy = "SELECT cept,rufz,day,btime,band,mode,rsts,rstr,eqslr,ktext";
        qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
        qy += " WHERE id="+s.setNum(id);
        query.exec(qy);
        n = 0;
        while(query.next()) {                       
          cept = query.value(n++).toString();
          call = query.value(n++).toString();       
          t = cept+call;
          Call = t;
          h += "%3CCALL%3A";
          len = t.count();
          h += s.setNum(len)+"%3E"+t+"%20";
          t = query.value(n++).toString();          
          t.remove(QChar('-'), Qt::CaseInsensitive); 
          len = t.count();
          h += "%3CQSO%5FDATE%3A";
          h += s.setNum(len)+"%3E"+t+"%20";         
          t = query.value(n++).toString();          
          t.remove(QChar(':'), Qt::CaseInsensitive); 
          t = t.left(4);                            
          len = t.count();
          h += "%3CTIME%5FON%3A";
          h += s.setNum(len)+"%3E"+t+"%20";         
          t = query.value(n++).toString();          
          t = getAdifBand(t);                       
          h += "%3CBAND%3A";
          h += s.setNum(t.count())+"%3E"+t+"%20";   
 
          t = query.value(n++).toString();          
          h += "%3CMODE%3A";
          h += s.setNum(t.count())+"%3E"+t+"%20";   
          t = query.value(n++).toString();          
          h += "%3CRST%5FSENT%3A";
          h += s.setNum(t.count())+"%3E"+t+"%20";   
          t = query.value(n++).toString();          
          h += "%3CRST%5FRCVD%3A";
          h += s.setNum(t.count())+"%3E"+t+"%20";   
          t = query.value(n++).toString();          
          len = t.count();
          h += "%3CQSL%5FRCVD%3A";
          if(t.compare("Y") != 0)
            h += "1%3EN%20";                        
          else 
            h += s.setNum(len)+"%3E"+t+"%20";       
          h += "%3CQSL%5FSENT%5FVIA%3A1%3EE%20";    
          t = query.value(n++).toString();          
          len = t.count();
          if(len != 0) {                            
            h += "%3CQSLMSG%3A";
            h += s.setNum(len)+"%3E"+t+"%20";       
          }
          h += "%3CEOR%3E";                         
          h += "&EQSL%5FUSER="+eqslUser;            
          h += "&EQSL%5FPSWD="+eqslPasswd;          
        }
        QUrl url = QUrl::fromEncoded(h.toLocal8Bit());
        doUpload(url);
      }
      else {
        
        QCoreApplication::instance()->quit(); 
      }
}




void UploadMgr::processPendingDatagram()
{
   while(udpSocket->hasPendingDatagrams()) {
     datagram.resize(udpSocket->pendingDatagramSize());
     QHostAddress sender;
     quint16 senderPort;
     udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
     int i = datagram.indexOf (",",0);
     QByteArray Left = datagram.left(i);           
     QByteArray Mid = datagram.mid(i+1);           
     int com = Left.toInt();                       
     int id = Mid.toInt();                         
     uploadHash.insert(id,com);                    
   }
}



void UploadMgr::processWriteDatagram(int dNr, QString mss, int rPort)
{
    datagram = QByteArray::number(dNr);            
    datagram.append(",");
    datagram.append(mss);
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
               QHostAddress::LocalHost, rPort);
}



QString UploadMgr::getAdifBand(QString sysband)
{
    QHash<QString, QString>::const_iterator i = toAdifBand.find(sysband);
    if(i == toAdifBand.end())
       return "?";                           
    return i.value();                        
}


void UploadMgr::setBandTable()
{
    QString sb, ab;
    QSqlQuery query;
    qy = "SELECT band,aband FROM wband WHERE work != 0";
    query.exec(qy);
    while(query.next()) {                        //
       n = 0;
       sb = query.value(n++).toString();         
       ab = query.value(n++).toString();         
       toAdifBand.insert(sb,ab);                 
    }
}

void UploadMgr::openDatabase()
{
      QString driver, host, dbname, user, passwd;
      QString b =  QDir::homePath();                
      QString f = b+"/.config/QtLog/qtlog.ini";
      QFile iniFile(f);
      iniFile.open(QIODevice::ReadOnly);
      QTextStream istream( &iniFile);       
      while( istream.atEnd() != true ) {   
          s = istream.readLine(0);
          if(s.count() == 0) break;
          i = s.indexOf("=");
          b = s.left(i);
          if(b.compare("qsqlDatabase") == 0)
             driver = s.mid(i +1);
          else
          if(b.compare("host") == 0)
            host = s.mid(i +1);
          else
          if(b.compare("dbname") == 0)
             dbname = s.mid(i +1);
          else
          if(b.compare("dbuser") == 0)
             user = s.mid(i +1);
          else
          if(b.compare("dbpasswd") == 0)
             passwd = s.mid(i +1);
          else
          if(b.compare("Logfile") == 0)
             logFile = s.mid(i +1);
          else
          if(b.compare("EqslUser") == 0)
             eqslUser  = s.mid(i +1);
          else
          if(b.compare("EqslPasswd") == 0)
             eqslPasswd = s.mid(i +1);
      }
      iniFile.close();
 
      //db = QSqlDatabase::addDatabase(driver);
      db = QSqlDatabase::addDatabase("QMYSQL");
      db.setHostName(host);                            
      db.setDatabaseName(dbname);
      db.setUserName(user);
      db.setPassword(passwd);
      if(! db.open()) {
        QSqlError err = db.lastError ();               
        s = err.databaseText();
        s += tr("\n\nFehlermeldung vom MySQL-Server !\nDie Zugangsberechtigung ist beschaedigt\nBetrieb ist nicht moeglich");
        
        
        exit(0);                                       
      }
}
