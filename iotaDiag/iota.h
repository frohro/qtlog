/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef IOTA_H
#define IOTA_H
#include <QtGui>
#include <QSettings>
#include <QtSql>
#include "ui_iota.h"
class QLabel;
class iota : public QMainWindow,
	       private Ui::iota
{
  Q_OBJECT
  public:
    iota(QWidget *parent = 0);
    ~iota();
    QSettings settings;
    QSqlDatabase db;                   
    void readSettings();
    void writeSettings();
    QTreeWidgetItem * uItem;           
    QString qy, t;
    int n, i, r;
  protected:
    void showIota(QString);
  protected slots:
    void goExit();
    void keyPressEvent( QKeyEvent * event );
    void wLineEditCb(QString);
    void iotaListClickedCb(QTreeWidgetItem *, int);
    void iotaBoxChangedCb(int);
    void backCb();
    void updateCb();
    void neuCb();
  private:
};
#endif 
