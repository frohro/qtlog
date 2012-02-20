/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007, 10-03-20010
********************************************************************** */

#ifndef ADDQSO_H
#define ADDQSO_H
#include <QtSql>
#include <QHash>
#include <QHashIterator>
#include <QTcpSocket>
#include "ui_addqso.h"
#include "serve.h"
#include "destination.h"
class QLabel;
class QUdpSocket;
class QTcpSocket;
class addQso : public QMainWindow,
	         private Ui::addQso
{
  Q_OBJECT
   public:
     addQso(QWidget *parent = 0);
     ~addQso();
    QSettings settings;
    QSqlDatabase db;             
    QByteArray datagram;
    
    QSqlError err;               
    QString s;
    QString t;
    QString f;
    QTreeWidgetItem * qsoItem;
    QTreeWidgetItem * omItem;
    QTreeWidgetItem * qslItem;
    int operatorId;
    int i;
    int n;
    int len;
    int x;
    int id;
    int strlen;
    int awCnt;
    int tflg;                     
    int hisflg;                   
    int editflg;                  
    int updateflg;                
    int uaflg;                    
    int qslflg;                   
    int flgf6;                    
    int BandIdx, ModeIdx;
    int upMgr;
    int rmess;
    
    
    int ctl, runing, rigpolling;  
    int retry;
    int cmdFlg, done, poll, slotFlg;
    QString owner,cmdStr, cmd;    
    QString lastBand, lastMode, lastFreq;
    
    QTime rtime;                  
    QTimer * timer;               
    QTimer * ptimer;              
    QTimer * dxtimer;             
    
    
    QString logFile;              
    QString qy;
    int Id;                       
    int omId;                     
    QString Rufz;                 
    QString dxCall;               
    QString Cept;                 
    
    QString Qsls;                 
    QString Qslr;                 
    QString La;                   
    QString Last;
    QString toDay;
    
    void keyPressEvent( QKeyEvent * event );
    void startQso();
   
    void setQslTabel();           
    void getQslStatus();          
    void toggelQslsent();
    void setQslDefault();
    void qslMsg(int);
    void qslViaEqslSetOff();
 
    void prepareVia(QString,QString);
    void setAwdOmList();
    void getAdwKennerList();
  protected:
    QHash <QString, QString> awdhash;
    QHash <QString, QString> toMyband;
    QHash <QString, QString> toSysband;
    Serve serv;
    void writeSettings();
    void readSettings();
    void getHistoryLast();
    void getHistoryPref();
    int getla( QString );
    int startUploadMgrR(int);
    
    void getQrzComData();
    void getQrzCallsign();
    
    void setColor(int);
    void clearQrzData();
       
    
    void getQrzDbCall(QString);
    
    void setText(QString, QString);
    
    
    QString getRigModelName(QString);
    QString tcphost, tcpport;
    
    QHash <QString, QLineEdit *> wLineEdit;   
    QLineEdit * getEditLine(QString wLineName) {
    QHash<QString, QLineEdit*>::iterator i = wLineEdit.find(wLineName);
        if(i == wLineEdit.end())
           return NULL;
        return i.value();
    }
    
    QHash<QString, QString> FreqToBand;       
    QString getBand(QString freq) {
        QHash<QString, QString>::const_iterator i=FreqToBand.find(freq);
        if(i == FreqToBand.end())
           return "";
        return i.value();
    }
    
    
     QHash<QString, QString> ModeToMod;      
     QString getMode(QString mode) {
     QHash<QString, QString>::const_iterator i=ModeToMod.find(mode);
     if(i == ModeToMod.end())
           return 0;
        return i.value();
     }
  protected slots:
    
    void goExit();
    void qsoVerwaltungAc();
    void qslBearbeitungAc();
    void dxSpotAc();
    void rigctlAc();
    void realtimeUpAc();
    void dumpLogbookAc();
    
    void hilfeQsoEingabeAc();
    void hilfeRigCtlAc();
    void hilfeDxSpotAc();
    void showTime();
    void uhrStop(int);
    void timerService();
    void putDxCall();
     
    void callEdited(QString );
    void checkRSTeChanged(int);
    void checkRSTgChanged(int);
    
    //void checkQSLsChanged();
    //void qslViaBueroChanged();
    //void qslViaEqslChanged();
    void qslviaChanged();
    
    void checkKomChanged();
    void checkQsoEndeChanged();
    void checkLocatorChanged();
    void comboBoxRSTeChanged(QString);
    void comboBoxRSTgChanged(QString);
    void comboBoxMaxCb(QString);
    //void checkBoxStopPolling();
    void HomeBoxCb(int);
    void BandBoxChangedCb(QString);
    void ModeBoxChangedCb(QString);
    void comboBoxSucheCb(int);
    void resetAddMask();
    void addNewQso();
    void locatorCb(QString );
    void locatorZielCb(QString );
    void awdCb( QString );
    void iotaCb( QString );
    void qslMgrCb(QString);
    void awdBoxChanged(int);
    void tabChanged(int);
    void wOpCallCb(QString);
    void qsoListClicked(QTreeWidgetItem *, int);
    void buttonBackCb();
    void buttonUpdateCb();
    void changeHistoryCb();
    void buttonDeleteCb();
    void notizCheckCR();
    void whileCRgoTab0();
  private slots:
    void processPendingDatagram();
    void processWriteDatagram(int, QString, int);
    
    
    void starthamlibServer();
    void requestRigNew();
    void sentCommand();
    void readRigReply();
    void displayError(QAbstractSocket::SocketError socketError);
    
  private:
    
   QString b;
   QString qso, last;
   Dest distanz;
   QUdpSocket * udpSocket;
   QTcpSocket * tcpSocket;
};
#endif 
