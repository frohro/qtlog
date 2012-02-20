/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef ADMIN_H
#define ADMIN_H
#include <QtGui>
#include <QSettings>
#include <QtSql>
#include <QtNetwork>
#include "ui_admin.h"
class QLabel;
class QUdpSocket;
class admin : public QMainWindow,
	       private Ui::admin
{
  Q_OBJECT
  public:
    admin(QWidget *parent = 0);
    ~admin();
    void keyPressEvent( QKeyEvent * event );
    QSqlDatabase db;              
    QSettings settings;
    QTreeWidget * ltreeWidget;    
    QByteArray datagram;
    QString qy, b, s, f,logbook;
    QString val;
    int i, n, x, dirsel;
    int row,col,r,z;
    QTreeWidgetItem * lItem;
    QTreeWidgetItem * sItem;
  protected:
    void openDatabase();
    void showTables(int);
    void setupAction();
    void writeSettings();
    void readSettings();
    QString DirSelector();
    void message();
    void restoreOldTable();

    void getRefnamen();
    void getCustomsfields();
  protected slots:
    void goExit();
    void getHilfeCb();
    void adifExpCb();
    void adifImpCb();
    void dumpCb();
    void refNamenCb();
    void stackedWidgetCb(int);
    void toggelLogBoxCb();
    void toggelDbBoxCb();
    void toggelQslModeCb();
    void sysListCb(QTreeWidgetItem*,int);
    void logListCb(QTreeWidgetItem*,int);
    void updateRefItemCb( QTableWidgetItem *);    
    void saveValCb(QTableWidgetItem*);            
    void updateCustomItemCb( QTableWidgetItem *); 
    void getSaveDirCb();

    void dbDump();
    void dbRestore();

    void getAdifDirCb();
    void adifExport();
    void adifImport();
  private:
    QUdpSocket *udpSocket;
};
#endif 
