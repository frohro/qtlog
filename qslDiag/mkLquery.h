/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 16.11.2008
********************************************************************** */

#ifndef MKLQUERY_H
#define MKLQUERY_H
#include <QDialog>
#include <QSettings>
#include "../qtlogDiag/translate.h"
#include "adif.h"
#include "ui_mkLquery.h"
class QLabel;
class mkLquery : public QDialog,
	            private Ui::mkLquery
{
  Q_OBJECT
  public:
    mkLquery(QWidget *parent = 0);
    ~mkLquery();
    QSettings settings;
    QCheckBox *BoxLast;
    int n;
    int getRec;
    QString s, f, qyrec, qy;
  protected:
    
    
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
  protected slots:
    void goExit();
    void mkLqueryRec();
    void checkQslQueryType_1();
    void checkQslQueryType_2();
    void checkQslQueryType_3();
    void checkQslQueryType_4();
    void checkQslQueryType_5();
    void checkQslQueryType_6();
    void checkQslQueryType_7();
    //void checkQslQueryType_8();
  private:
    QHash <QString, QString> toAdifMode;   
    QHash <QString, QString> toAdifBand;   
};
#endif 
