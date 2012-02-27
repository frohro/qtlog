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
    QtLog(QWidget *parent = 0, int Lang=-1);
    ~QtLog();
    
    QSettings settings;
    QSqlDatabase db;                  // QSqlDatabase_Instanz
    QSqlError err;
    
    QByteArray datagram;
    
    void writedefaultSettings();      // settings nach NeuInstallation anleg 
    void keyPressEvent( QKeyEvent * event );
    
    QTableWidgetItem * rItem;         // report_item
    int Lang;
    int awdqId;                       // QSO_datensatz_Nr für AWD
    
    QHash <QString, QString> awdHash; // struktur der übersetzungstabelle -AWD_type_name
    
    QString logFile; 	              // Arbeits_LOG
    int operatorId;
    int dflg;                         // debug_flg
    int dbstat;
    int checkCount;
    int bflg;                         // band_positions_flg
    QString celltext;                 // Text in der Daten_celle
    
    QTimer *timer;
    
    QString qystr;                    // query Layout.str: EU_type
    QString qy;                       // querys allerart
    QString s;
    QString p;
    int i, n, c, r, z, row, col;
    int awdPos;                       // position der AWD_spalte im Header
    
    int awdBoxCnt;                    // Anzahl AwdBoxEinträge
    int eflg, cflg;                   // edit_Flag, call_flg
    
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
    void awdDxStatusSsbAc();      // awd_DxStatus_SSB
    void awdDxStatusCwAc();       // awd_DxStatus_CW
    void awdJahresModeAc();       // awd_Jahres_Aktivität_MODE
    void awdJahresBandAc();       // awd_Jahres_Aktivität_Band
    void awdDLDstatusAc();        // awd DLD-Status
    void awdIOTAstatusAc();       // awd IOTA-Status
    
    void logconfAc();             // Betriebsarten
    void standortAc();            // --------------
    void clubAc();                // -.-
    void InternetOnAc();          // -.-
    //void hamLibAc();            
    void rigCtlAc();
    
    void confmodeAc();            // Betriebsarten
    void verbindungenAc();        // --------------
    void rigAc();                 // -.-
    
    void reportLayoutAc();        // logbook Verwaltung
    void dataSaveAc();            // ------------------
    void adifExportAc();         
    void adifImportAc();          // -.-
    void queryTextoutAc();        // Tabellen_Layout als Text_datei ausgeben
    
    void qslEingangAc();          // QSL_Eingang 
    void qslDruckAc();            // QSL_drucken
    
    void admindbAc();             // Tabellen
    void dxTableAc();             // --------
    void prefTableAc();           // -.-
    void ruDistrictAwdListAc();   // -.-
    void qrzTableAc();            // -.-
    void dokVerwaltungAc();       // -.-
    void iotaVerwaltungAc();      // -.-
    
    void notizAc();              // Notiz
    
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
