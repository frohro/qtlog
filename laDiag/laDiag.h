/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef LADIAG_H
#define LADIAG_H
#include <QtSql>
#include "ui_ladialog.h"
#include "convert.h"
class QLabel;
class laDiag : public QMainWindow,
	       private Ui::laDiag
{
  Q_OBJECT
  public:
    laDiag(QWidget *parent = 0);
    ~laDiag();
     void keyPressEvent( QKeyEvent * event );
     QSettings settings;
     QString qy;
     QString y, s;
     QSqlDatabase db;             
     conVert conv;
     int row;
     int col;
  protected:
    
    void writeSettings();
    void readSettings();
    void getLaTable(QString); 
  protected slots:
    void updateLaItem( QTableWidgetItem *item );
    void getPraefixList(QString);
    void subPrefixdb();
    void getHilfeDxListCb();
    
    void readFileCtyWtDat();
    
    //void saveAdifDataAc();
    //void saveGeoDataAc();
    //void saveWaeDataAc();
    
    //void setAdifDataAc();
    //void setGeoDataAc();
    //void setWaeDataAc();
    
    
    void setupAction();
    void goExit();
    void readlaTextFile();
    void combiBoxChangedCb(int);
  private:
    int dbflg;
};
#endif 
