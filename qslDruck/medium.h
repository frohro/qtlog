/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef MEDIUM_H
#define MEDIUM_H
#include <QtGui>
#include <QDialog>
#include <QtSql>
#include <QSettings>
//#include "translate.h"
#include "ui_medium.h"
class QLabel;
class medium : public QDialog,
	          private Ui::medium
{
  Q_OBJECT
  public:
    medium(QWidget *parent = 0);
    ~medium();
    QTreeWidgetItem * pItem;
    QString qy;
    QSettings settings;
    int i, n;
    int prflg;                 
  protected:
    void showData();
  protected slots:
    void goExit();
    void mListClickedSlot(QTreeWidgetItem *, int);
    void mUpdateCb();
    //void mAddMediumCb();
  private:
};
#endif 
