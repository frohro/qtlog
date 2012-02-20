/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef QUERY_H
#define QUERY_H
#include <QDialog>
#include <QSettings>
#include "ui_query.h"
#include "translate.h"
class QLabel;
class query : public QDialog,
	            private Ui::query
{
  Q_OBJECT
  public:
    query(QWidget *parent = 0);
    ~query();
    QSettings settings;
   
    int i, n, cnt, idx;
    int awdPos;
    int showTable;
    int cntdpos;                
    int err;
    QString qy, f, s, logFile, path;
    QString t, whv, whb, AWD;
  protected:
    QTreeWidgetItem * upItem;
    QTreeWidgetItem * lItem;
    void createQuery();
    void mkText();
    void mkAdif();
    void insertTok(QString, int);
  protected slots:
    void goExit();
    void queryListDoubleClicked(QTreeWidgetItem*,int);
    void queryListClicked(QTreeWidgetItem*,int);
    void textEditedQnameCb(QString);
    void buttonSaveCb();
    void buttonMkTextCb();
    void buttonMkAdifCb();
    void logListClickedCb(QTreeWidgetItem*,int);
    void dataListClickedCb(QTreeWidgetItem*,int);
    void buttonBackCb();
    void buttonClearCb();
    void buttonCopyDataCb();
    void showHilfeCb();
    void tabCurrentChangedCb(int);
  private:
    translate transl;
};
#endif 
