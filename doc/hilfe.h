/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef HILFE_H
#define HILFE_H
#include <QtGui>
#include "ui_hilfe.h"
class QLabel;
class hilfe : public QMainWindow,
	       private Ui::hilfe
{
  Q_OBJECT
  public:
    hilfe(QWidget *parent = 0);
    ~hilfe();
     QSettings settings;
     QTimer *timer;
     void keyPressEvent( QKeyEvent * event );
     QString s, b;
     void showText(QString);
  protected:
  protected slots:
    void goExit();
    void itemChangedCb(QListWidgetItem *);
    void getHilfe();
  private:
};
#endif 
