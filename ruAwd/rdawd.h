/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef RDAWD_H
#define RDAWD_H
#include <QtSql>
#include "ui_rdawd.h"
class QLabel;
class Rda : public QMainWindow,
	       private Ui::Rda
{
  Q_OBJECT
  public:
    Rda(QWidget *parent = 0);
    ~Rda();
     QSettings settings;
     QString qy;
     QString y, s;
     QSqlDatabase db;             
     int row;
     int col;
  protected:
    void writeSettings();
    void readSettings();
  protected slots:
   void goExit();
   void rdAwdTextFileAc();
  private:
};
#endif 
