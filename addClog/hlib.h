/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 16-03-2010
********************************************************************** */

#ifndef HLIB_H
#define HLIB_H
#include <QtGui>
#include <QDialog>
#include <QSettings>
#include "ui_hlib.h"
class QLabel;
class hlib : public QDialog,
                    private Ui::hlib
{
  Q_OBJECT
  public:
    hlib(QWidget *parent = 0);
    ~hlib();
    QSettings settings;
    QString qy, s, nRetry;
    
    int halt, run;
    
  protected slots:
    
    void checkBoxRChanged();
    void goExit();
    void hamlibError();
  private:
};
#endif 
