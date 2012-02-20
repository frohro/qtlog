/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.05.2007, 01.12.2007
********************************************************************** */

#ifndef QTLOG_H
#define QTLOG_H
#include <QtGui>
#include <QSettings>
#include <QtSql>
#include "ui_qtlog.h"
#include "translate.h"
class QLabel;
class QUdpSocket;
class QTcpServer;
class QtLog : public QMainWindow,
	               private Ui::QtLog
{
  Q_OBJECT
  public:
    QtLog(QWidget *parent = 0);
    ~QtLog();
    QSettings settings;
    QSqlDatabase db;                  
    QSqlError err;                    
    QByteArray datagram;
    void writedefaultSettings();      
    void keyPressEvent( QKeyEvent * event );
    QTableWidgetItem * rItem;         
    int awdqId;                       
    QHash <QString, QString> awdHash; 
    QString logFile; 	              
    int operatorId;
    int dflg;                         
    int dbstat;
    int checkCount;
    int bflg;                         
    QString celltext;                 
    QTimer *timer;
    QString qystr;                    
    QString qy;                       
    QString s;
    QString p;
    int i, n, c, r, z, row, col;
    int awdPos;                       
    int awdBoxCnt;                    
    int eflg, cflg;                   
    void buildQuery();
    void awdQuery();
    void awdQuerykenner();
    void textExportData(QString,int);
    void readData(QString,int);
    void deleteReportRow(int);
    void addReportRow(int);
  protected:
    void setupAction();
    void CheckDBconnection();
    void writeSettings();
    void readSettings();
    void writeColumnWidth();
    void configCb();
  protected slots:
    void goExit();
    void addLog();
    void addClog();
    void addDlog();
    void awdDxStatusSsbAc();     
    void awdDxStatusCwAc();      
    void awdJahresModeAc();      
    void awdJahresBandAc();      
    void awdDLDstatusAc();       
    void awdIOTAstatusAc();      
    void logconfAc();            
    void standortAc();           
    void clubAc();               
    void InternetOnAc();         
    //void hamLibAc();             
    void rigCtlAc();
    void confmodeAc();           
    void verbindungenAc();       
    void rigAc();                
    void reportLayoutAc();       
    void dataSaveAc();           
    void adifExportAc();         
    void adifImportAc();         
    void queryTextoutAc();       
    void qslEingangAc();         
    void qslDruckAc();           
    void admindbAc();            
    void dxTableAc();            
    void prefTableAc();          
    void ruDistrictAwdListAc();  
    void qrzTableAc();           
    void dokVerwaltungAc();      
    void iotaVerwaltungAc();     
    void notizAc();              
    void VersionAc();            
    void RigCtlHamlibAc();
    void InfoAc();
    void checkReport();
    void checkBoxSortCb();
    void boxLimitCb(QString);
    
    void groupEditLineCb();        //
    void groupEditLineCb(QString); //
    
    void boxAwdCb(int);
    void boxSortCb(int);
    void awdEditLineCb();
    void boxLayoutCb(QString);
    void updateReportItem(QTableWidgetItem*);
    void updateAwdItem(QTableWidgetItem *);
    
    //void test(QTableWidgetItem *);
    void removeQsoCb();
    
    void checkBoxDateCb();
    
    void checkAwdItem(QTableWidgetItem*);
  private slots:
    void processPendingDatagram();
    void processWriteDatagram(int, QString);
  private:
    translate transl;
    QLabel *mStatLabel;
    QUdpSocket *udpSocket;
   
};
#endif 
