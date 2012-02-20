/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   QtLog V 1.5.n
********************************************************************** */
  
#ifndef QSLDB_H
#define QSLDB_H
#include <QtGui>
#include <QtNetwork>
#include <mysql.h>
#include "ui_qsldb.h"
#include "dbconnection.h"
#include "dbresult.h"
#include "dbrow.h"
class Connection;
class Result;
class Row;
class qsldb : public QMainWindow,
              private Ui::qsldb
{
  Q_OBJECT
  public:
    qsldb(QWidget *parent = 0);
    ~qsldb();
    QSettings settings;
    QByteArray datagram;
    QString s, p, path;
  protected:
    int state;
    int r, i, n, pcount;
    QString logFile;
    QString qy, strqy;
    char host[12];
    char dbname[12];
    char dbuser[16];
    char dbpasswd[12];
    Connection * db;
    Result * res;
    Row * rows;

    void insertQslCard();
    void viewCard(QString, int);
    void showQslCards(QString);
  protected slots:
    void goExit();
    void cardListClicked(QTreeWidgetItem *, int );
    void processPendingDatagram();
    void processWriteDatagram(int, QString, int);
    void searchQslCardCb(QString);
    void searchQslCardCB();
  private slots:
 
  private:
    QUdpSocket *udpSocket;
};
#endif 
