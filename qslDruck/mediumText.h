/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef MEDIUMTXT_H
#define MEDIUMTXT_H
#include <QDialog>
#include <QtSql>
#include <QSettings>
#include "ui_mediumText.h"
class QLabel;
class mediumTxt : public QDialog,
	          private Ui::mediumTxt
{
  Q_OBJECT
  public:
    mediumTxt(QWidget *parent = 0);
    ~mediumTxt();
    QSettings settings;
    QString qy;
    int pageIndex;
    int n, row;
    QString Ldxcc;
    QString Kdxcc;
    QString LSdxcc;
    QString KSdxcc;
    QListWidgetItem * selectItem;
  protected:
    void showLabelData(QListWidgetItem *);
    void showSLabelData(QListWidgetItem *);
    void showKartData(QListWidgetItem *);
    void showSKartData(QListWidgetItem *item);
    void showSWL_LabelData(QListWidgetItem *);
    void showSWL_KartData(QListWidgetItem *);
    void updateLabelData();
    void updateSLabelData();
    void updateKartData();
    void updateSKartData();
    void updateSWL_LabelData();
    void updateSWL_KartData();
    void addLabelData();
    void addSLabelData();
    void addKartData();
    void addSKartData();
    void addSWL_LabelData();
    void addSWL_KartData();
    void delLabelData();
    void delSLabelData();
    void delKartData();
    void delSKartData();
    void delSWL_LabelData();
    void delSWL_KartData();
  protected slots:
    void goExit();
    void textPageChanged(int);
    void dxccTextChanged(QString);
    void showLabelDataCb(QListWidgetItem *);
    void updateMediumData();
    void addMediumData();
    void delMediumData();
    void getHilfeAc();
  private:
};
#endif 
