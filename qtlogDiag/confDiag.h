/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef CONFDIALOG_H
#define CONFDIALOG_H
#include <QDialog>
#include <QSettings>
#include <QTcpSocket>
#include "ui_configdialog.h"
class QLabel;
class QTcpSocket;
class confDialog : public QDialog,
	            private Ui::confDialog
{
  Q_OBJECT
  public:
    confDialog(QWidget *parent = 0);
    ~confDialog();
    void keyPressEvent( QKeyEvent * event );
    int work;
    int state;               
    int i, n;
    int row, col;
    QString Rig, Rigpid,RigDev, Baud, PttDev, PttBit, Interface;
    int newRig;
    
    QString qy, s, colval;
    void writeSettings();
    QSettings settings;
  protected:
    
    void setupAction();
  protected slots:
    
    void textMoved(const QString &);
    void buttonClicked();
    void makeLogFile();
    void deleteLogFile();
    void changeLogCb(QTreeWidgetItem *, int);
    void wCallCb(QString);
    void wLocatorCb(QString);
    void goExit();
    void comboBoxHomeCb(int);
    //void internetBoxChangedCb();
    void internetParamSaveCb();
    //void initializeHamlibCb();
    void updateLocationItemCb( QTableWidgetItem *);
    void saveColValCb(QTableWidgetItem *);
    void getHilfeCb();
    //-
    //void setPtt0();
    //void requestTestConnect();
    //void readhamlib();
    //void sentCommand();
    //void displayError(QAbstractSocket::SocketError socketError);
    
  private:
    
    //QTcpSocket * tcpSocket;
};
#endif //- CONFDIALOG_H
