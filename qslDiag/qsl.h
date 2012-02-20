/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 16.11.2008
********************************************************************** */
#ifndef QSL_H
#define QSL_H
#include <QtGui>
#include <QtSql>
#include <QtNetwork>
#include "ui_qsl.h"
#include "../qtlogDiag/translate.h"
#include "adif.h"
class QLabel;
class QUdpSocket;
class qsl : public QMainWindow,
             private Ui::qsl
{
  Q_OBJECT
  public:
    qsl(QWidget *parent = 0);
    ~qsl();
     QSettings settings;
     QByteArray datagram;
     int splitqsl;
     int Err;
     QString qy, url;
     QString logFile;
     QString celltext;
     QString s, p, l, path, strlen;
     QString header, Error, last, mess;
     QString callFrom, qid, call, band, mode, datum, ontime, sentDay, qslr,qsls;
     QHash <QString, QString> toError;    
     QString getErrId(QString id) {
        QHash<QString, QString>::const_iterator i = toError.find(id);
        if(i == toError.end())
          return "";                      
        return id;                        
     }
     QTableWidgetItem * bItem; 
     int qslvia;
     int forUp;
     int i, r, n, c, row, col, dirsel;
     int awdqId;                         
     int awdPos, blfg, sflg;             
     int newQsl;                         
     int lotwfl;
     int flen;
     int upMgr;
     int cardview;
     int selectedbut, rcount, tabRow, tabLen, yes, no, err, ignor;
     int urlmode;
     int work;
     void keyPressEvent( QKeyEvent * event );
     //QProgressBar* bar;
     
    QCheckBox *lastcheckBox;
    void writeColumnWidth();
    void setpalettgruen(QCheckBox *);
    void setpalettgrau(QCheckBox *);
    void buildHeaderUnreceived();
    void buildHeaderNotConfirmed();
    void buildHeaderNotSend();
    void buildHeaderReceived(); 
    void notSend();
    void mkLotwUploadAdi(int); 
    void mkEqslUploadAdi(); 
    void readLastDownLoad(int, QString);
    void readrecordFile(QString, QString);
    void checkQslCardstatus();
    void checkQsoVerification();
    void changeButtonColor(int);
    void takeSelectedButton();
    QString DirSelector();
    QString getUrlForCard();
    QString getUrlForVerifyQSO();
  protected:
    QSqlDatabase db;                   
    QTimer * timer;
    QTimer * mtimer;
    int state;
    void readData(QString,int,int);
  protected slots:
    void qslConfAc();
    void goExit();
    void qslDruckerAc();
    void qtlogAc();
    void checkBoxZeitraumChanged();
    void uploadLotwAc();                
    void uploadEqslAc();                
    void adifLotwQslRecordsAc();
    void adifLotwImportAc();
    void adifEqslImportAc();
    void adifEqslRecordsAc();
    void qslEingangAc();
    void checkAwdItem(QTableWidgetItem * );
    void updateAwdItem(QTableWidgetItem * );
    void updateQslItem(QTableWidgetItem * );
    void sucheCb();   
    void wLineEditedTextCb(QString);
    void awdBoxchangedCb(int);
    void qslviaBoxchangedCb(int);
    void UnreceivedCb();
    void NotSentCb();
    void UnConfirmedCb();
    void ReceivedCb();
    void showCardCb();
    void timerServiceUpload();
    void timertransferUrl();
  private slots:
    void processPendingDatagram();
    void processWriteDatagram(int, QString, int);
  private:
    adif adi;
    translate transl;
    QUdpSocket *udpSocket;
};
#endif 
