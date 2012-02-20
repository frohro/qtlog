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
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <qdebug.h>
#include <QtGui>
#include <stdio.h>
#include "downloadMgr.h"
#include "../qtlogDiag/dirmngr.h"
extern DirMngr dirMngr;

DownloadMgr::DownloadMgr()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
    udpSocket = new QUdpSocket(this);                    
    udpSocket->bind(QHostAddress::LocalHost,45500);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
}

DownloadMgr::~DownloadMgr()
{
}


void DownloadMgr::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QString n;
    n = 2;
    request.setAttribute(QNetworkRequest::RedirectionTargetAttribute, n.toInt()); 
    QNetworkReply *reply = manager.get(request);
    currentDownloads.append(reply);
}

QString DownloadMgr::saveFileName(const QUrl &url)
{
    QString s, p, r;
    QString path = url.path();
    QString basename = QFileInfo(path).fileName(); 
    p = QDir::homePath(); 
    if(mode == 10) {
      p += "/lotw/";
      fpath = p;
      s = p + basename;                            
      if (QFile::exists(s)) {                      
        r = "mv "+s+" "+p+"tmp";                   
        StartProcess(r.toAscii());
      }
    }
    else
     if(mode == 15) {
      p += "/eQsl/";
      s = p + basename;
     }
    else
     if(mode == 18) {                             
      //p += "/eQsl/";
      //s = p + basename;
      s = "/tmp/"+basename;
     }
    else
     if(mode == 20) {
       p += "/eQsl/";
       s = p + basename;
     }
    return s;                                     
}

bool DownloadMgr::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }
    file.write(data->readAll());
    file.close();
    return true;
}




void DownloadMgr::execute()
{
      QStringList args = QCoreApplication::instance()->arguments();
      QString u;
      int n;
      n = args.size();
      for (int i = 1; i < args.size(); ++i) {
          u += args.at(i).toLocal8Bit().constData();
          if(i == n -1) break;
          u += "&";
      }
      
      replyflg = 0;
      if(u.indexOf("GeteQSL.cfm") != -1) {  
         mode = 18;  
         pUrl = u;                          
      }
      else
      if(u.indexOf("VerifyQSO.cfm") != -1) {
         mode = 20;
         pUrl = u;                          
      }
      else {
        if(u.indexOf("lotwreport") != -1) {
           mode = 10;
        }
        else
        if(u.indexOf("DownloadInBox.cfm") != -1) {
           mode = 15;
        }
        //qDebug() << u;
        QUrl url = QUrl::fromEncoded(u.toLocal8Bit());
        doDownload(url);
      }
}

void DownloadMgr::downloadFinished(QNetworkReply *reply)
{
    QString t, z, u;
    int idx, n;
    QUrl url = reply->url();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();  
    
    if( statusCode == 302 ) {                             
        QUrl nUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
       
        doDownload(nUrl);                                 
    }
    else {
     if (reply->error()) {
        
        fprintf(stderr, "Download of %s FAILED: %s\n",
                url.toEncoded().constData(),qPrintable(reply->errorString()));
        if(mode == 10)                                    
           processWriteDatagram(10,"FAILED");             
        else
         if(mode == 15)
          processWriteDatagram(15,"FAILED eqsl");         
        else
         if(mode == 18)
           processWriteDatagram(18,"FAILED eqslCard");    
        QCoreApplication::instance()->quit();             
        return;
     } 
     else {                                                
       
       QString filename = saveFileName(url);               
       if (saveToDisk(filename, reply)) {                  
           
           if(mode == 10) {                                
              processWriteDatagram(10,"lotwreport.adi");   
              
              QCoreApplication::instance()->quit();        
           }
           else 
            if(mode == 15) {                               
              
              if(replyflg == 0) {
                QFile datei(filename);
                datei.open(QIODevice::ReadOnly);
                QTextStream istream( &datei);
                fname ="";
                n = 0;
                while( istream.atEnd() != true ) { 
                  t = istream.readLine(0);
                  if(t.count()!= 0) {
                    if(t.indexOf("Your ADIF log file has been built") != -1) {
                      n = 1;
                      break;
                    }
                  }
                }
                if( n ) {
                  
                  n = 10;
                  while(n-- != 0) {
                    t = istream.readLine(0);
                    idx = t.indexOf("=");            
                    if(idx != -1) break;                       
                  }
                  z = "";
                  idx++;
                  idx++;
                  while(t[idx] != QChar('"')) z += t[idx++];   
                  n = z.indexOf("/");
                  n++;
                  fname = z.mid(n);                            
                  replyflg = 1;
                  QString u = "http://www.eqsl.cc/qslcard/"+z; 
                  
                  QUrl url = QUrl::fromEncoded(u.toLocal8Bit());
                  doDownload(url);                             
                }
                else {                             
                  processWriteDatagram(15,fname);  
                  currentDownloads.removeAll(reply);
                  reply->deleteLater();
                  
                  QCoreApplication::instance()->quit();       
                  return;
                }
             } 
             else {                                
                
                processWriteDatagram(15,fname);    
                replyflg = 0;
                currentDownloads.removeAll(reply);
                reply->deleteLater();
                
                QCoreApplication::instance()->quit();    
                return;
             }
          }
          else
           if(mode == 18) {                             
             if(replyflg == 0) {                        
               QFile datei(filename);
               datei.open(QIODevice::ReadOnly);
               QTextStream istream( &datei);
               fname ="";
               n = 0;
               while( istream.atEnd() != true ) {       
                  t = istream.readLine(0);
                  if(t.count()!= 0) {
                    if(t.indexOf("<IMG SRC=\"..") != -1) {
                      n = 1;
                      break;
                    }
                  }
               }
               if( n ) {
                 idx = 0;
                 while(t[idx] != QChar('/')) idx++;
                 while(t[idx] != QChar('"')) z += t[idx++]; 
                 u = "http://www.eqsl.cc"+z;
               }
               replyflg = 1;                     
               QUrl url = QUrl::fromEncoded(u.toLocal8Bit());
               doDownload(url);                  
            }
            else {    
              processWriteDatagram(18,filename); 
	      
              replyflg = 0;
            }
          }
          else
           if(mode == 20) {                      
             processWriteDatagram(20,filename);  
           }
       }
     }   
   }
}



void DownloadMgr::processWriteDatagram(int dNr, QString mss)
{
    datagram = QByteArray::number(dNr);
    datagram.append(",");
    datagram.append(mss);
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
               QHostAddress::LocalHost, 45504);
}



void DownloadMgr::processPendingDatagram()
{
   int i;
   QString s;
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
     
     if(com == 18) {                               
        if(s.count() == 0) {                       
          
          QCoreApplication::instance()->quit();   
         return;
        }
        QUrl url = QUrl::fromEncoded(s.toLocal8Bit());
        doDownload(url);
     }
     else
      if(com == 20) {                              
        
        QUrl url = QUrl::fromEncoded(s.toLocal8Bit());
        doDownload(url);
      }
     else
      if(com == -1) {                              
        
        QCoreApplication::instance()->quit();      
        return;
      }
   }
}
