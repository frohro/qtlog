/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef AUTH_H
#define AUTH_H
#include <QDialog>
#include <QSettings>
#include "ui_auth.h"
class QLabel;
class auth : public QDialog,
                    private Ui::auth
{
  Q_OBJECT
  public:
    auth(QWidget *parent = 0);
    ~auth();
    QSettings settings;
  protected:
  protected slots:
    void goExit();
  private:
};
#endif 
