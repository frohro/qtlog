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
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "dxspot.h"


void dxspot::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QString n;
    n = 2;                                          
    request.setAttribute(QNetworkRequest::RedirectionTargetAttribute, n.toInt());
    manager.get(request);
}

QString dxspot::saveFileName(const QUrl &url)
{
    QString p;
    QString path = url.path();
    QString basename = QFileInfo(path).fileName(); 
    p = QDir::homePath(); 
    p += "/.qtlog/spots.htm";
    return p;                                      //return basename;
}

bool dxspot::saveToDisk(const QString &filename, QIODevice *data)
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



void dxspot::getSpots()
{
      if(timerflg)                                         
         ntimer->stop(); 
 
      QUrl ur;
      ur = QUrl::fromEncoded(urlSpot.toLocal8Bit());
      doDownload(ur);  
}

void dxspot::downloadFinished(QNetworkReply *reply)
{
    QString t, z, u, m;
    int i, n, x, len;
    
    QUrl url = reply->url();
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();  
    if( statusCode == 302 ) {                             
        QUrl nUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        doDownload(nUrl);                                 
    }
    else 
     if (reply->error()) {
         fprintf(stderr, "Download of %s FAILED: %s\n",
                url.toEncoded().constData(),qPrintable(reply->errorString()));
        ntimer->stop();
        return;
     } 
     else {                                               
       QString filename = saveFileName(url);              
       if (saveToDisk(filename, reply)) {                 
            toDxCall.clear();                             
            
            QFile datei(filename);
            datei.open(QIODevice::ReadOnly);
            QTextStream instream( &datei);
            n = 0;
            while( instream.atEnd() != true ) {           
              t = instream.readLine(0);
              if(t.indexOf("<pre>") != -1) break;         
            }
          
            spotList->clear();
            while(instream.atEnd() != true ) {            
              m = instream.readLine(0);                   
              t = m.toAscii();                            
 
              if(t.count() == 4) break;                   
              if(t.indexOf("</pre>") != -1) break;        
              x = 0;
              QTreeWidgetItem *item = new QTreeWidgetItem(spotList);
              i = t.indexOf("=");
              t.remove(0,i+1);
              
              i = t.indexOf(">");      
              u = t.left(i);
              t.remove(0,i+1);
              i = t.indexOf(" "); 
              z = t.left(i);
              item->setText(x++,z);            
              t.remove(0,i+1);
              
              i = t.indexOf("</A>");
              t.remove(0,i+4);
              i = t.indexOf("<A");
              z = t.left(i);
              z = z.simplified();
              item->setTextAlignment(x,Qt::AlignRight);
              z += "   ";
              item->setText(x++,z);            
              
              i = t.indexOf("=");
              t.remove(0,i+1);
              i = t.indexOf(">");
              u = t.left(i);                  
              t.remove(0,i+1);
              i = t.indexOf(" ");
              z = t.left(i);
              z = z.simplified();
              item->setText(x++,z);            
              toDxCall.insert(z,u);            
              
              i = t.indexOf("</A>");
              t.remove(0,i+4);                 
              len = t.size();                  
              n = 0;
              for( i = len; i > 0; --i ) {     
                if (t.at(i) >= QChar('0') && t.at(i) <= QChar('9')) {
                   ++n;                        
                   if(n == 6) break;           
                }
              }
              item->setText(x++,t.left(i));    
              item->setText(x++,t.mid(i));     
            }
          }
       }
       urlFlg = 0;
       if(timerflg) {                                       
           n = BoxRetrieve->currentText().toInt() * 60000;  
           ntimer->start(n);                                
           timerflg = 0;
       }
}
