/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef AWDST_H
#define AWDST_H
#include <QtGui>
#include <QSettings>
#include <QtSql>
#include "ui_awdst.h"
class QLabel;
class awdst : public QMainWindow,
	       private Ui::awdst
{
  Q_OBJECT
  public:
    awdst(QWidget *parent = 0);
    ~awdst();
    QSettings settings;
    QSqlDatabase db;                   
    void keyPressEvent( QKeyEvent * event );
    QHash <QString, QString> toMband;  
    QHash <QString, QString> toSband;  
    QList<QString> bandlist;           
    QList<QString> logbook;            
    QString logFile;                   
    QString qy, s, ms;
    int i, n, h;
    int hcount;                        
  protected:
    void writeSettings();
    void readSettings();
    void getStatus();
    QString getSysband(QString);       
    QString getMyband(QString);        
    int getBandIndex(QString);         
    void showAwdData(QString);
    void awdStatus(QString,int);
    void awStatusDLD(QString);
    void awStatusIOTA(QString);
    void awStatusDIV(QString);        
    void awStatusWAS(QString);        
    
    void getAWDband(QString);
  protected slots:
    void goExit();
    void setDxModeCb(QString);
    void getModeAktivitaet();
    void getBandAktivitaet();
    void getDxStatus();
    void makedxccStatusListAc();
    void BoxAwdCb(QString);
    void awStatusDldAc();
    void awStatusIotaAc();
    void checkBoxBandCb();
    void getHilfeAc();
  private:
};
#endif 
