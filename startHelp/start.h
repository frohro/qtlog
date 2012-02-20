/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef START_H
#define START_H
#include <QDialog>
#include "ui_start.h"
class QLabel;
class start : public QDialog,
	          private Ui::start
{
  Q_OBJECT
  public:
    start(QWidget *parent = 0);
    ~start();
  protected:
  protected slots:
  void goExit();
  private:
};
#endif 
