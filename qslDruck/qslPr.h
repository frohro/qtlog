/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef QSLPR_H
#define QSLPR_H
#include <QtSql>
#include "ui_qslpr.h"
#include "translate.h"
class QLabel;
class QUdpSocket;
class qslPr : public QMainWindow, 
               private Ui::qslPr
{
  Q_OBJECT
  public:
    qslPr(QWidget *parent = 0);
    ~qslPr();
    QByteArray datagram;
    QList <QString> sList;
    QString logFile;
    QString qy, s, pf;
    QPushButton *labelOld;
    int labelPos;
    int i, n;
    //translate tr;                
    void keyPressEvent( QKeyEvent * event );
  protected:
    int cnt;
    void setupAction();
    QSettings settings;
    void writeSettings();
    void readSettings();
    QSqlDatabase db;             
    void loadQsls(QString, int);
    QString zuf;
    char style[12];
    
    QString lprtype;
    int prtype;
    void loadPrtMedium();
    void printQSL();
    void mediumTextEditorCb();
  protected slots:
     void goExit();
     void printMediumAc();
     void LabelAc();
     void LabelSammelAc();
     void LabelSwlAc();
     void KarteAc();
     void KarteSammelAc();
     void KarteSwlAc();
     void qslPrinter();
     void checkBoxFormCb();
     void callEditedCb();
     void LabPos1();
     void LabPos2();
     void LabPos3();
     void LabPos4();
     void LabPos5();
     void LabPos6();
     void LabPos7();
     void LabPos8();
     void itemPressedCb(QTreeWidgetItem *, int);
     void checkBoxSqslCb();
     void checkBoxPrCb();
     void checkBoxOrderCb();
     void getHilfeAc();
 private slots:
   void processWriteDatagram(int, QString);
  private:
   QUdpSocket *udpSocket;
};
#endif 
