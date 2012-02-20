/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 20.08.2008
********************************************************************** */

#ifndef SETUP_H
#define SETUP_H
#include <QtGui>
#include <QSettings>
#include <QtSql>
#include "ui_setup.h"
class QLabel;
class setup : public QMainWindow,
	       private Ui::setup
{
  Q_OBJECT
  public:
    setup(QWidget *parent = 0);
    ~setup();
    QSettings settings;
    void writeSettings();
    QTimer *timer;
    QSqlError err;                       
    QString s,b,t, release;
    QString qy;
    int dbFlg;
    int reinstall;
    int n, dbstat;
    QList <QString> loglist;
  protected:
    //void showText(QString);
    void goinstall();
    void dbRestore();         
    void createlogdb();       
    void checkUpGrade();      
    int openDatabase();
    void copyInifile();
    
    void upgradeto58();
    void upgradeto56();
    void upgradeto55();
    void upgradeto54();
    void update52();
    void update51();
  protected slots:
    void goExit();
    void hilfeAc();
    void checkBoxParamCb();
    void checkBoxUpdateCb();
    void checkBoxReInstallCb();
    void updateParam();
    void installQtlog();
    void installOldCb();
    void closeParamUpdate(); 
    void closeUpdate();      
    void closeReInstall();   
    void updatework();
    void loadTable(QString);
  private:
    
    QSqlDatabase db;          
};
#endif 
