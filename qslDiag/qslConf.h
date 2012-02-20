/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 16.11.2008
********************************************************************** */

#ifndef QSLCONF_H
#define CSLCONF_H
#include <QDialog>
#include <QSettings>
#include "ui_qslConf.h"
class QLabel;
class qslConf : public QDialog,
	            private Ui::qslConf
{
  Q_OBJECT
  public:
    qslConf(QWidget *parent = 0);
    ~qslConf();
    QSettings settings;
    int n;
    QString s;
  protected:
  protected slots:
    void goExit();
    void checkBoxLotwShortChanged();
    void checkBoxLotwLongChanged();
  private:
};
#endif 
