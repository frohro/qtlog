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

// -------------------------------
DownloadMgr::DownloadMgr()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinished(QNetworkReply*)));
    udpSocket = new QUdpSocket(this);                    
    udpSocket->bind(QHostAddress::LocalHost,45500);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
}

// ----------------------
DownloadMgr::~DownloadMgr()
{
}

// *******************************************************************
// -------------------------------------------------------------------
void DownloadMgr::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QString n;
    n = 2;
    request.setAttribute(QNetworkRequest::RedirectionTargetAttribute, n.toInt()); // allowed for redirection
    QNetworkReply *reply = manager.get(request);
    currentDownloads.append(reply);
}

// ------------------------------------------------------------------
QString DownloadMgr::saveFileName(const QUrl &url)
{
    QString s, p, r;
    QString path = url.path();
    QString basename = QFileInfo(path).fileName(); 
    p = QDir::homePath(); 

    if(mode == 10) {
      p += "/lotw/";
      fpath = p;
      s = p + basename;                            // base_path
      if (QFile::exists(s)) {                      // already exists, don't overwrite, mv
        r = "mv "+s+" "+p+"tmp";                   // last upload_file for mv
        int i = system(r.toAscii());
      }
    }
    else
     if(mode == 15) {
      p += "/eQsl/";
      s = p + basename;
     }
    else
     if(mode == 18) {                              // QSL_Karte holen
      //p += "/eQsl/";
      //s = p + basename;
      s = "/tmp/"+basename;
     }
    else
     if(mode == 20) {
       p += "/eQsl/";
       s = p + basename;
     }
    return s;                                     // return basename;
}

// --------------------------------------------------------------------
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



// ------------------------------------------------------------------------------------------------------
// download: url_sample for default
// https://www.arrl.org/lotwuser/lotwreport.adi?login=mycall&password=****&qso_query=1&qso_callsign=3B7C
// ------------------------------------------------------------------------------------------------------
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
      if(u.indexOf("GeteQSL.cfm") != -1) {  // QSL_Karte
         mode = 18;  
         pUrl = u;                          // warte auf datagram
      }
      else
      if(u.indexOf("VerifyQSO.cfm") != -1) {
         mode = 20;
         pUrl = u;                          // warte auf datagram
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

// ------------------------------------------------------------
void DownloadMgr::downloadFinished(QNetworkReply *reply)
{
    QString t, z, u;
    int idx, n;
    
    QUrl url = reply->url();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();  // STATUS 302
    // qDebug() << "STATUS:" << statusCode;
    
    if( statusCode == 302 ) {                             // found for redirection
        QUrl nUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
       
        doDownload(nUrl);                                 // download from url.redirection
    }
    else {
     if (reply->error()) {
        // qDebug() << "STATUS replay->error_No:" << statusCode;
        fprintf(stderr, "Download of %s FAILED: %s\n",
                url.toEncoded().constData(),qPrintable(reply->errorString()));
        if(mode == 10)                                     // prüfe die Fehlerquell
           processWriteDatagram(10,"FAILED");              // Datagramm for qsldb, break
        else
         if(mode == 15)
          processWriteDatagram(15,"FAILED eqsl");          // error ?
        else
         if(mode == 18)
           processWriteDatagram(18,"FAILED eqslCard");     // error ?
        QCoreApplication::instance()->quit();              // download finished
        return;
     } 
     else {                                                // STATUS 200 - Datenfile erhalten
       // qDebug() << "STATUS:data incoming" << statusCode;
       QString filename = saveFileName(url);               // hole filename
       if (saveToDisk(filename, reply)) {                  // falls datei gespeichert
           // qDebug() << "mode" << mode;
           if(mode == 10) {                                // LOTW_mode
              processWriteDatagram(10,"lotwreport.adi");   // Datagramm for qsldb , lotw_report incoming
              // qDebug() << "currentDownLoads.isEmpty";
              QCoreApplication::instance()->quit();        // downloads finished
           }
           else 
            if(mode == 15) {                               // EQSL_mode
              // qDebug() << "replyflg:" << replyflg;
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
                    idx = t.indexOf("=");            // suche downloadedfiles/WL1HBWxxxx.adi holen
                    if(idx != -1) break;                        // gefunden     
                  }
                  z = "";
                  idx++;
                  idx++;
                  while(t[idx] != QChar('"')) z += t[idx++];   // downloadedfiles/WL1HBWxxxx.adi
                  n = z.indexOf("/");
                  n++;
                  fname = z.mid(n);                            // WL1HBW40314.adi
                  replyflg = 1;
                  QString u = "http://www.eqsl.cc/qslcard/"+z; // das geht
                  
                  QUrl url = QUrl::fromEncoded(u.toLocal8Bit());
                  doDownload(url);                              // hole WL1HBWxxxx.adi
                }
                else {                             // wenn Ergebnis: "You have no log entries"
                  processWriteDatagram(15,fname);  // fname ist leer
                  currentDownloads.removeAll(reply);
                  reply->deleteLater();
                  // qDebug() << "currentDownLoads.isEmpty";
                  QCoreApplication::instance()->quit();       // downloads finished
                  return;
                }
             } 
             else {                                // file xxxxx.adi eingegangen
                 // qDebug() << statusCode << "replyflg:" << replyflg << "mgr:" << 15 << fname;
                processWriteDatagram(15,fname);    // Datagramm for qsldb , eqsl_report.adi incoming
                replyflg = 0;
                currentDownloads.removeAll(reply);
                reply->deleteLater();
                // qDebug() << "currentDownLoads.isEmpty ->QCoreApp::quit()";
                QCoreApplication::instance()->quit();    // downloads finished
                return;
             }
          }
          else
           if(mode == 18) {                             // EQSL_Card_image mode
             if(replyflg == 0) {                        // --------------------
               QFile datei(filename);
               datei.open(QIODevice::ReadOnly);
               QTextStream istream( &datei);
               fname ="";
               n = 0;
               while( istream.atEnd() != true ) {       // search card_url
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
               replyflg = 1;                 // url gefunden:
               QUrl url = QUrl::fromEncoded(u.toLocal8Bit());
               doDownload(url);              // "http://www.eqsl.cc/qslcard/TempeQSLs/TWL1HBW43411.jpg";  
            }
            else {    
              processWriteDatagram(18,filename);  // Datagramm for qsldb , qslcard_image incoming
	      // qDebug() << "18" << filename;
              replyflg = 0;
            }
          }
          else
           if(mode == 20) {                       // Eqsl Verifcatoin mode for "Authenticity
             processWriteDatagram(20,filename);   // Datagramm for qsldb , datei incoming
           }
       }
     }   
   }
}


// ------------------------------------------------------------
// Datagram : WRITE datagram_message
// ------------------------------------------------------------
void DownloadMgr::processWriteDatagram(int dNr, QString mss)
{
    datagram = QByteArray::number(dNr);
    datagram.append(",");
    datagram.append(mss);
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
               QHostAddress::LocalHost, 45504);
}


// ------------------------------------------------------------
// Datagram : messages von qsldb lesen und agieren
// ------------------------------------------------------------
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
     QByteArray Left = datagram.left(i);           // Funktions_Nr
     QByteArray Mid = datagram.mid(i+1);           // Parameter_Liste
     
     int com = Left.toInt();                       // funktion
     s = Mid;                                      // parameter liste
     // qDebug() << "pending for eQslCard:";
     if(com == 18) {                               // 18, url : message from eQsl - hole QslCard
        if(s.count() == 0) {                       // keinen leeren urlstr
          // qDebug() << "currentDownLoads.isEmpty";
          QCoreApplication::instance()->quit();   // downloads finished
         return;
        }
        QUrl url = QUrl::fromEncoded(s.toLocal8Bit());
        doDownload(url);
     }
     else
      if(com == 20) {                              // do verification
         // qDebug() << s;
        QUrl url = QUrl::fromEncoded(s.toLocal8Bit());
        doDownload(url);
      }
     else
      if(com == -1) {                              // mgr abschalten
        // qDebug() << "currentDownLoads.isEmpty";
        QCoreApplication::instance()->quit();      // downloads finished
        return;
      }
   }
}
