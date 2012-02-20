/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef LOGEXP_H
#define LOGEXP_H
#include <QDialog>
#include <QSettings>
#include <QtSql>
#include "ui_logexp.h"
class QLabel;
class logexp : public QDialog,
                    private Ui::logexp
{
  Q_OBJECT
  public:
    logexp(QWidget *parent = 0);
    ~logexp();
     QSettings settings;
     QDateTime h;
     QString qy, s, path, contest,p;
     int i,n, dirsel, CNr;
     QString mode;
     QString qsoOrder, qtcOrder, subJect, phMail, cwMail;
     QString DirSelector();
     QString getTemplateForm(int, int);
     int getClaimed(QString, QString);
     int getMulti1poitsBand(QString,QString);
     int workExportCQWW(QString);
     void creatSysName();
     int workExportWAG(QString);
     int workExportWAE(QString);
     int workExportDTC(QString);
     int workExportAGCWHTP(QString);
     int workExportXMAS(QString);
     int workExportCQPTY(QString);
     int workExportDARC10M(QString);
     int workExportHNY(QString);
     //int workExportCHECK(QString);
     int workExportRSGB160M(QString); 
     int workExportOCEANIADX(QString);
     int workExportPACC(QString);
     int workExportRUSSDX(QString);
     int workExportUBALB(QString);
     int workExportUBADX(QString);
     int workExportNASPRINT(QString);
     int  workExportREFC(QString);
     int workExportSPDX(QString);
     
     
     int workExportCSV(QString);       
     
     int readQsoData(QString, QString);
     int readQsoData_2(QString, QString);
     int rButton;
     
     QHash<QString, QString> toSysName;         
     QString getSysName(QString name) {
        QHash<QString, QString>::const_iterator i=toSysName.find(name);
        if(i == toSysName.end())
           return "";
        return i.value();
     }
  protected slots:
     void goExit();
     void showExport();
     void rButtonStfCb();
     void rButtonCbr3Cb();
     void rButtonCbr2Cb();
     void rButtonCsvCb();
     void checkBoxBandCb();
     void getDirCb();
     void exportCb();
  private:
};
#endif 
