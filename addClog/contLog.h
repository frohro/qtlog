/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n : 01.11.2010
********************************************************************** */

#ifndef CONLOG_H
#define CONLOG_H
#include <QtGui>
#include <QtSql>
#include <QtNetwork>
#include "ui_contLog.h"
#include "../addQso/destination.h"
#include "contRules.h"
#include "rule.h"
#include "defs.h"
class QLabel;
class contLog : public QMainWindow,
                  private Ui::contLog
{
  Q_OBJECT
  public:
    contLog(QWidget *parent = 0);
    ~contLog();
     QSettings settings;
     QString appString;
     
     int qsoRow;                                  
     int qsoid;                                   
     int bandruning;
     int i, n, r, found;
     int rows;
     
     
     
     QString qy;                                  
     QString s, t,lg ,br,st;                      
     void keyPressEvent( QKeyEvent * event );
     
     QTreeWidgetItem * Item;
     QTreeWidgetItem * qsoItem;
     QTreeWidgetItem * headItem;
     
     void processWriteDatagram(QString, int);
     void processWriteDatagram(int, char, QString, int);
     QByteArray datagram;
     int udpPort;
     QString checkMacro(QString);
     float getPause(QString, float, int);
     
     
     int repeat;                
     float pause;               
     int escflg;                
     int eflg;                  
     int bflg;                  
     int bQso;                  
     int timeflg;               
     int sflg;                  
     int qtcflg;
     int clFlg;                 
     int idx;                   
     int numflg;                
     
     QDateTime h;  
     QString u;                 
     QString Contestday;        
     QString ContestStart;      
     QString Runtime;           
     int min, mm;
     int lastHour;              
     int lastMin;
     int viewtiming;            

     QString cName;             
     QString NamId;             
     QString ContestID;         
     
     int methode, exchp, cPrefmode; 
     int offband;               
     
     int laPref;                
     int cPref;                 
     int scPref;                
     int nlen, olen;            
     int qNrlen;                
     int prefMode;              
 
     int CNr;                   
     int incNr;                 

     QString Callsign, Call, sentVal;  
     QString band;              
     QString MHzFreq;           
     int freq;                  
     int stxNr;                 
     //QString stxStr;            
     QString sentSave;          

     QString dxcc, wae, lname;

     QString cpref;             
     QString cnt;               
     QString cqz;               
     QString ituz;

     int setPointA;             
     int setPointB;             //
     int setPointC;             //
     int setPointD;             //
     int setPointE;
     int setPointF;
     int setMultiA;             
     int setMultiB;             

     int bqsos;            
     int bpoints;          
     int bmulti1p;         
     int bmulti2p;         
     int bqsosh;           
     int bscore;           

     int tqsos;            
     int tpoints;          
     int tmulti1p;         
     int tmulti2p;         
     int tqsosh;           
     int tscore;           
     int tbandmin;         

     int bMin, mMin;       
     
     
     QString d_Id, d_Date, d_Time, d_Band, d_Mode, d_Call;
     QString d_Srst, d_Sent, d_Rrst, d_Rcvd, d_Rcvd2, d_Dxcc, d_Pref, d_Multi1, d_Multi2; 
     QString lcompl;
     int d_Pts, d_Mul1p, d_Mul2p, d_Qtcflg;
     
     
     
     QHash<QString, QString> Macros;            
     QString getMacro(QString key) {
     QHash<QString, QString>::const_iterator i=Macros.find(key);
        if(i == Macros.end())
           return "";
        return i.value();
     } 
     
     
     
     QHash<int, int> FreqUp;                     
     int setfreqUp(int freq) {  
        QHash<int, int>::const_iterator i=FreqUp.find(freq);
        if(i == FreqUp.end())
           return 0;
       return i.value();                         
     }
     QHash<int, int> FreqDown;                  
     int setfreqDown(int freq) { 
        QHash<int, int>::const_iterator i=FreqDown.find(freq);
        if(i == FreqDown.end())
           return 0;
       return i.value();                        
     }
     
     QHash<QString, QString> FreqToBand;       
     QString getBand(QString freq) {
        QHash<QString, QString>::const_iterator i=FreqToBand.find(freq);
        if(i == FreqToBand.end())
           return "";
        return i.value();
     }
     
     
     QHash<QString, int> BandToFreq;          
     int getFreq(QString band) {
     QHash<QString, int>::const_iterator i=BandToFreq.find(band);
     if(i == BandToFreq.end())
           return 0;
        return i.value();
     }
     
     
     QHash<QString, QString> ModeToMod;       
     QString getMode(QString mode) {
     QHash<QString, QString>::const_iterator i=ModeToMod.find(mode);
     if(i == ModeToMod.end())
           return 0;
        return i.value();
     }
     
     
     QHash<QString, int>bandIdx;              
     int getBindex(QString band) {
       QHash<QString,int>::const_iterator i=bandIdx.find(band);
       if(i == bandIdx.end())
         return -1;
       return i.value();
     }
     
     /*
     
     QHash<QString, int>bandCheck;              
     int getBandCheck(int wband) {
       QHash<QString,int>::const_iterator i=bandChck.find(wband);
       if(i == bandCheck.end())
         return -1;                             
       return i.value();                        
     }
     */
     
     
     QHash<QString, int> hqsos;   
     QHash<QString, int> hpoints; 
     QHash<QString, int> hmult1;  
     QHash<QString, int> hmult2;  
     QHash<QString, int> hqsosh;  
     QHash<QString, int> hscore;  
     QHash<QString, int> hbtime;  
     
     
     int set, get, cmdFlg, retry, rigpolling, runflg;
     QString cmdStr;
     QString lastFreq, lastMode;
     int lastF;
     QTimer *stimer;             
     QString tcphost, tcpport;
     
     
     QHash<QString, int>hashBandPoint;     
     int getBandPoint(QString band) {
     QHash<QString, int>::const_iterator i=hashBandPoint.find(band);
         if(i == hashBandPoint.end()) { 
           return 0;                       
         }
         return i.value();                 
     }
 
  protected:
    
    QSqlDatabase db;                     
    QTimer * timer;                      
    QTimer * timerR;                     
    void setTime();
     
    void setTimeDate();
	
    void setBandValues(QString);
    void setContinueValues();
    void makePoinsValueList();
    void countPoints(int,QString);
    
    void switchExchangePolicy(QString);
    
     int getLaValues(QString);
    void setContestValues();
    void checkMultPrefix(QString);
    
    void workCHECK(QString);
    
    void setColorMult(int);
    void setColorMult2(int);
    void setColorQsopts(int);
    void setColorQsoPoints(int);
    void setColorBandMin(QString);
    void setColorUhr(QString);
    void setColorButtonUhr(QString);
    
    void clearAll();
    void createContestClass(int, int);
    void contestInfo(int);
    
    void insertQso();
    void showAllQso();
     int showMasterCallsign(QString);
    void setNewMasterTable();
    void refreshMasterTable();
    void setWorkdeBand(QSqlQuery);
    
    
    void deleteContest();
    void removeQso(QString);
    
    void saveAll();
    
    void showPoints(QString);
    
    void prepCwdaemon();
  
  protected slots:
     void newContestLogAc();
     void continueAc();
     void sentLogAc();
     void deleteContestAc();
     void convertToQtLogAc();
     void hlibAc();
     void cwkeyerAc();
      
     void hilfeContestLogAc();
     void hilfeRigCtlAc();
     void hilfeCwDaemonAc();
     
     void selectContestList();
     void contestContinue();
     void stackButtonOkCb();
     
     void lineCallsignCb(QString);
     void lineCallsignCrCb();
     void lineExchangeBackCb(QString);
     void lineEditedSentCb(QString);
     
     void lineExchangeCb();
     void insertNewQso();
     
     void contWorkedListCb(QTreeWidgetItem *,int);
     void worckedListClickedCb(QTreeWidgetItem*,int);
     
     void BandChangedCb(int);
     void ModeChangedCb(QString);
    
     void ButtonInfoCb();
     void buttonEditLogCb();
     
     void removeQsoCb();
     
     void infoSetExchange();
     
     void readMasterAc();
     void goExit();
     
     void uhrStopCb();
 
 
  private slots:
    void processPendingDatagram();
    void showTime();
    void repeaterTimerCb();
    
    void starthamlibServer();
    void requestRigNew();
    void sentCommand();
    void readRigReply();
    void displayError(QAbstractSocket::SocketError socketError);
    
  private:
    
    QTcpSocket * tcpSocket;
    QUdpSocket * udpSocket;
    Dest distanz;
   
};
#endif 
