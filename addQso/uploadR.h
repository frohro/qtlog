/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 16-03-2010
********************************************************************** */

#ifndef UPLOADR_H
#define UPLOADR_H
#include <QDialog>
#include <QSettings>
#include "ui_uploadR.h"
class QLabel;
class uploadR : public QDialog,
                private Ui::uploadR
{
  Q_OBJECT
  public:
    uploadR(QWidget *parent = 0);
    ~uploadR();
    QSettings settings;
    int upLd;
     
  protected slots:
    
    void qslviaChanged();
    void goExit();
  private:
};
#endif 
