/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 16-03-2010
********************************************************************** */

#ifndef RIGCTL_H
#define RIGCTL_H
#include <QDialog>
#include <QSettings>
#include <QtNetwork>
#include "ui_rigctl.h"
class QTcpSocket;
class QLabel;
class rigctl : public QDialog,
                    private Ui::rigctl
{
  Q_OBJECT
  public:
    rigctl(QWidget *parent = 0);
    ~rigctl();
    QSettings settings;
    QString qy, s, PttBit, proz, owner, home;
    QString tcphost, tcpport;        
    
    int Runing, Retry, r, i, updateFlg;
    
    int checkServerProz();           
    void startServer();
    bool showSuccess(QString);
     
  protected slots:
   
    void goExit();
    
    void showRigPage();
    
    //void itemClickedCb(QTreeWidgetItem *, int);
    
    
    void goStart();
    void goStop();
    void checkhamlib();
    
    void requestRig();
    void sentCommand();
    void readRigReply();
    void displayError(QAbstractSocket::SocketError socketError);
  private:
    QTcpSocket * tcpSocket;
};
#endif 
