/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef EDITLOG_H
#define EDITLOG_H
#include <QDialog>
#include <QSettings>
#include "ui_editLog.h"
class QLabel;
class editLog : public QDialog,
                    private Ui::editLog
{
  Q_OBJECT
  public:
    editLog(QWidget *parent = 0);
    ~editLog();
    QSettings settings;
    void keyPressEvent( QKeyEvent * event );
    QString qy, s, t, a, band, qsoId;
    int i, n, col, row, z, r, c;
    int eflg, CNr;
    QString ecall, emult;
    QHash<QString, QString> toSysName;     
    QString getSysName(QString name) {
        QHash<QString, QString>::const_iterator i=toSysName.find(name);
        if(i == toSysName.end())
           return "";
        return i.value();
    }
    void readLogs(int);
  protected slots:
    void goExit();
    void updateQsoItem(QTableWidgetItem * );
    void deleteQsoCb();
    void convertToQtlog();
  private:
};
#endif 
