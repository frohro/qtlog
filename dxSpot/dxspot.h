/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n 
********************************************************************** */

#ifndef DXSPOT_H
#define DXSPOT_H
#include <QtGui>
#include <QLineEdit>
#include <QtSql>
#include <QtNetwork>
#include "../addQso/destination.h"
#include "ui_dxspot.h"
class QLabel;
class QUdpSocket;
class dxspot : public QMainWindow,
             private Ui::dxspot
{
  Q_OBJECT
  public:
    dxspot(QWidget *parent = 0);
    ~dxspot();
     //QString urlStr;
     QString urlSpot;
     QString urlQrz;
     int urlFlg;
     
     QByteArray datagram;
     QString qy, s, t;
     QString dxCall;
     int xcnt;                   
     QTreeWidgetItem * bItem;
     
     int n, x, i;
     
     void keyPressEvent( QKeyEvent * event );
         
     QSettings settings;
     QNetworkAccessManager manager;
     void doDownload(const QUrl &url);
     QString saveFileName(const QUrl &url);
     bool saveToDisk(const QString &filename, QIODevice *data);
     
     void getLaPrefix(QString);
     
     
     
     QHash<QString, QString> RangeDes; 
     QString getRangeVal(QString rangedes) {
     QHash<QString, QString>::const_iterator i=RangeDes.find(rangedes);
     if(i == RangeDes.end())
           return 0;
        return i.value();                     
     }
     
     
     QHash<QString, QString> CustomDes;       
     QString getCustomVal(QString custdes) {
     QHash<QString, QString>::const_iterator i=CustomDes.find(custdes);
     if(i == CustomDes.end())
           return 0;
        return i.value();                     
     }
     
     
    
     QHash<QString, int> Line;                
     int getLine(QString tok) {
     QHash<QString, int>::const_iterator i=Line.find(tok);
     if(i == Line.end())
           return 0;
        return i.value();
     }
    
     QHash<QString, QString> toDxCall;      
     QString getDxCallUrl(QString dxcall) {
     QHash<QString, QString>::const_iterator i=toDxCall.find(dxcall);
     if(i == toDxCall.end())
           return 0;
        return i.value();
     }
     
  protected:
    
    QSqlDatabase db;                     
    QTimer * ntimer;
    int timerflg;
    QTimer * dxtimer;
    
  protected slots:
    
    void getNodes();
    void buttonReturnCb();
    void goExit();
    void sendSpotCb();
    void callUpperCb(QString);
    void sendSpotTextCb();
    void sendAnnounceTextCb();
    
    void spotsCb(QString);
    void customCb(QString);
    void spotListClicked(QTreeWidgetItem*,int);
    void writeForAddQso();
    void scanDxCall();
    
  private slots:
    
    void processPendingDatagram();
    void getSpots();
    void downloadFinished(QNetworkReply *reply);
  private:
    QUdpSocket * udpSocket;
    Dest distanz;
};
#endif 
