/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef REPORTLAYOUT_H
#define REPORTLAYOUT_H
#include <QDialog>
#include <QSettings>
#include <QSettings>
#include "ui_reportlayout.h"
#include "translate.h"
class QLabel;
class reportLayout : public QDialog,
                    private Ui::reportLayout
{
  Q_OBJECT
  public:
    reportLayout(QWidget *parent = 0);
    ~reportLayout();
    QSettings settings;
    void keyPressEvent( QKeyEvent * event );
    QString qy, s, t, a;  
    int i, n, d, x, awdPos;
    int editflg;
    int changeflg;
    QTreeWidgetItem * lItem;
  protected:
    void putQstring();
    translate transl;
    QHash <QString, QString> toUsr;  
    QHash <QString, QString> toDb;   
    void getReportListItems(QString);
  protected slots:
    void goExit();
    void dbListClicked(QTreeWidgetItem *, int);
    void layoutListClicked(QTreeWidgetItem *, int);
    void getReportListItemsCb(QString);
  private:
};
#endif 
