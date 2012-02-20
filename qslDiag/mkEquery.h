/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 16.11.2008
********************************************************************** */

#ifndef MKLEQUERY_H
#define MKEQUERY_H
#include <QDialog>
#include <QSettings>
#include "../qtlogDiag/translate.h"
#include "adif.h"
#include "ui_mkEquery.h"
class QLabel;
class mkEquery : public QDialog,
                    private Ui::mkEquery
{
  Q_OBJECT
  public:
    mkEquery(QWidget *parent = 0);
    ~mkEquery();
    QSettings settings;
    QCheckBox *BoxLast;
    int n, box;
    int getRec;
    QString s, f, qyrec, qy;
    QDate heute;
  protected:
/*
    
    
    QString getAdifBand(QString mband)
    {
      QHash<QString, QString>::const_iterator i = toAdifBand.find(mband);
      if(i == toAdifBand.end())
       return "";                           
      return i.value();                      
    }
    
    
    QString getAdifMode(QString refnam)
    {
      QHash<QString, QString>::const_iterator i = toAdifMode.find(refnam);
      if(i == toAdifMode.end())
       return "";                            
      return i.value();                      
    }
*/
  protected slots:
    void goExit();
    void mkEqueryRec();
    void checkQslQueryType_1();
    void checkQslQueryType_2();
    void checkQslQueryType_3();
    void checkQslQueryType_4();
    void checkQslQueryType_5();
    void checkQslQueryType_6();
    void checkQslQueryType_7();
  private:
    QHash <QString, QString> toAdifMode;   
    QHash <QString, QString> toAdifBand;   
};
#endif 
