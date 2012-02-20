/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef CONFMODE_H
#define CONFMODE_H
#include <QtGui>
#include <QDialog>
#include <QSettings>
#include <QtSql>
#include "ui_confMode.h"
class QLabel;
class confMode : public QDialog,
	            private Ui::confMode
{
  Q_OBJECT
  public:
    confMode(QWidget *parent = 0);
    ~confMode();
    QSettings settings;
    QSqlDatabase db;             
    void keyPressEvent( QKeyEvent * event );
    int rIndex;                  
    int customId1,customId2;
    QString val, val2;
    QString custom1, custom2;
  protected:
   int n, i;
   QString qy;
  
  
   QTreeWidgetItem * rItem;
   QTreeWidget * rtreeWidget;
   void clearLineEdit(int);
  protected slots:
   void goExit();
   void bandListCb(QTreeWidgetItem*,int);
   void modeListCb(QTreeWidgetItem*,int);
   void pwrListCb(QTreeWidgetItem*,int);
   void lineListCb(QTreeWidgetItem*,int);
   void awdListCb(QTreeWidgetItem*,int);
   void customsListCb(QTreeWidgetItem *,int);
   void rigListCb(QTreeWidgetItem*,int);
   void RigSaveButton();
   void RigClearButton();
   void SaveConfValues();
   void NewAwdValues();
   void getHilfeModeCb();
   void currentIndexchangedCb(int);
  private:
};
#endif 
