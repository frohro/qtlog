/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */


#ifndef CONTRULES_H
#define CONTRULES_H
#include <QDialog>
#include <QSettings>
#include "ui_contRules.h"
class QLabel;
class contRules : public QDialog,
                    private Ui::contRules
{
  Q_OBJECT
  public:
    contRules(QWidget *parent = 0);
    ~contRules();
    QSettings settings;
    QDateTime h; 
    
   
    QString qy, s, t, a;
    QString confBand;
    QString confMode;
    QString namId;            
    QString StartDay;
    QString StartTime;
    QString RunTime;
       
    QString confcontestId;
    QString confcontestName;
    QString method;          
    QString prefMode;        
    
    QString cType;           
    QString view;
    QString qsoOrder, qtcOrder;
    int stat;
    QString PointsA;
    QString PointsB;
    QString PointsC;
    QString PointsD;
    QString PointsE;
    QString PointsF;
    
    QString MultiA;
    QString MultiB;
    QString Pts160, Pts80, Pts40, Pts20, Pts15, Pts10;
    int Inc;                           
    
    QString srst,sent,rrst,band;
    int i, n, r;
  protected slots:
    
    void confListclicked(QTreeWidgetItem*,int);
    void radioButtonCW();
    void radioButtonSSB();
    void radioButtonRTTY();
    void radioButtonMIXD();
    void radioButtonBandCb_160();
    void radioButtonBandCb_80();
    void radioButtonBandCb_40();
    void radioButtonBandCb_20();
    void radioButtonBandCb_15();
    void radioButtonBandCb_10();
    void radioButtonBandCb_All();
    void radioButtonRealtimeCb();
    void radioButtonFastCb();
    void lineContestCb(QString);
    void buttonCreateContestCb();
    void deleteContest();
   
    void goExit();
  private:
};
#endif 
