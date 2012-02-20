/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef CONTCONF_H
#define CONTCONF_H
#include <QDialog>
#include <QSettings>
#include <QtNetwork>
#include "ui_keyconf.h"
//#include "translate.h"
class QLabel;
class QUdpSocket;
class keyconf : public QDialog,
                private Ui::keyconf
{
  Q_OBJECT
  public:
    keyconf(QWidget *parent = 0);
    ~keyconf();
    QSettings settings;
    QByteArray datagram;
    QString s, t;
    int udpPort; 
    int i;
    //void processWriteDatagram(char, char, QString, int);
    void processWriteDatagram(QString,QString);
    void saveConfigValue();
  protected slots:
    void goReturn();
    //void lineSportCb();
    //void lineParportCb();
    
    void lineKeyPortCb();
    
    void lineSoundOnOffCb();
    void lineSpeedCb();
    void lineSidetonCb();
    void lineSoundVolumeCb();
    void lineSoundDevCb();
    void lineWeightCb();
    void linePttKeyingCb();
    void linePttDelayCb();
    void lineSSBsigCb();
    void lineWordmodeCb();
    void lineUdpPortCb();
    void lineTuneXCb();
    void lineBandIndexCb();
    void lineResetCb();
  private:
    QUdpSocket *udpSocket;
};
#endif 
