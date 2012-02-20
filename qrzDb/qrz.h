/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef QRZ_H
#define QRZ_H
#include <QtSql>
#include "ui_qrz.h"
class QLabel;
class Qrz : public QMainWindow,
                    private Ui::Qrz
{
  Q_OBJECT
  public:
    Qrz(QWidget *parent = 0);
    ~Qrz();
    QSettings settings;
    QSqlDatabase db;             
    void keyPressEvent( QKeyEvent * event );
    int dirsel;
    QString qy, s;
    QString tname;
    int n, i;
  protected:
    QString DirSelector();
    void loadCallBkcdat();
    void loadCallBkcdatDb();
    void loadImageDB();
    void insertData();
  protected slots:
    void goExit();
    void getHilfeQrzCb();
    void cdRead();
    void imageView();
    void rdCall();
    void comboBoxChangedCb(int);
    void getQrzFilePathCb();
    void loadDataCb();
    void checkCallChangedCb();
    void checkImageChangedCb();
    void getCallData();
    void qrzListClickedCb(QTreeWidgetItem *,int);
    void backToPage0Cb();
  private:
};
#endif 
