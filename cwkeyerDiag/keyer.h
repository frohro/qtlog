/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 14.09.2009
********************************************************************** */
#ifndef KEYER_H
#define KEYER_H
#include <QtGui>
#include <QtSql>
#include <QtNetwork>
#include "ui_keyer.h"
class QLabel;
class QUdpSocket;
class keyer : public QMainWindow,
             private Ui::keyer
{
  Q_OBJECT
  public:
    keyer(QWidget *parent = 0);
    ~keyer();
     QByteArray datagram;
     QSettings settings;
     QString s, t;
     int i, n;
     int cwdPort;            
     void keyPressEvent( QKeyEvent * event );
     //QString checkMacro(QString);
 
  protected:
    void processWriteDatagram(QString, int);
    //void processWriteDatagram(int, char, QString, int);
    void processWriteDatagram(int, QString, int);
    //QTimer * timerR;                      
  protected slots:
     void goExit();
     void cwkeyerConfAc();
     void saveCWtext();
     //void repeaterTimerCb();
  private slots:
     void processPendingDatagram();
  private:
     QUdpSocket *udpSocket;
};
#endif 
