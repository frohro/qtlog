/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef DOK_H
#define DOK_H
#include <QtGui>
#include <QSettings>
#include <QtSql>
#include "ui_dok.h"
class QLabel;
class dok : public QMainWindow,
	       private Ui::dok
{
  Q_OBJECT
  public:
    dok(QWidget *parent = 0);
    ~dok();
    QSettings settings;
    QSqlDatabase db;                   
    QTreeWidgetItem * uItem;
    QString qy, s, t;
    int i, r, n;
    int tabIdx;
    void readSettings();
    void writeSettings();
    void getSdokList();
    void showDok(QString);
    void getSdokData(QString);
  protected:
  protected slots:
    void goExit();
    void keyPressEvent( QKeyEvent * event );
    void dokListeClickedCb(QTreeWidgetItem *, int);
    void dokUpdateCb();
    void dokNeuCb();
    void dokDeleteCb();
    void getDokCb(QString);
    void boxDokChangedCb(int);
    void wEditDokCb(QString);
    void wEditDxccCb(QString);
    void sdokListeClickedCb(QTreeWidgetItem *, int);
    void sdokUpdateCb();
    void sdokNeuCb();
    void sdokDeleteCb();
    void getSdokCb(QString);
    void boxSdokChangedCb(int);
    void wEditSdokCb(QString);
    void wEditCallCb(QString);
    void wEditHdokCb(QString);
    void tabwidgetCurrentChangedCb(int);
    void getHilfeCb();
  private:
};
#endif 
