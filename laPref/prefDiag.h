/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef PREFDIAG_H
#define PREFDIAG_H
#include <QtSql>
#include "ui_prefix.h"
class QLabel;
class prefDiag : public QWidget,
	       private Ui::prefDiag
{
  Q_OBJECT
  public:
    prefDiag(QWidget *parent = 0);
    ~prefDiag();
    QSettings settings;
    void keyPressEvent( QKeyEvent * event );
    QString a;
    QString qy;
    QSqlDatabase db;             
    int row;
    int col;
    int i;
    void showPrefs( QString );
  protected:
  protected slots:
    void setupAction();
    void goExit();
    void addPref();
    void delPref();
    void getSubPrefs(QString);
    void getPrefs(QString);
    void getHilfeDxPrefCb();
    
    void updatePrefItem(QTableWidgetItem *);
  private:
};
#endif 
