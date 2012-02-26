/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */


#ifndef MNOTE_H
#define MNOTE_H
#include <QDialog>
#include <QSettings>
#include "ui_mnote.h"

class QLabel;
class mNote : public QDialog,
                    private Ui::mNote
{
  Q_OBJECT
  public:
    mNote(QWidget *parent = 0);
    ~mNote();
    QSettings settings;
    void keyPressEvent( QKeyEvent * event );
    QString qy, s;           
    int row, col, i, r, z;
  protected:
  protected slots:
    void updateNoteItem(QTableWidgetItem *);
    void deleteNoteItem();
    void goExit();
  private:
};
#endif 
