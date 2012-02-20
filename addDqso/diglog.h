/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 02-10-2010
********************************************************************** */

#ifndef DIG_H
#define DIG_H
#include <QtGui>
#include <QtSql>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "ui_diglog.h"
#include "../addQso/destination.h"
class QLabel;
class diglog : public QMainWindow,
                private Ui::diglog
{
  Q_OBJECT
  public:
     diglog(QWidget *parent = 0);
     ~diglog();
      QSettings settings;
      QSqlDatabase db;             
      
      QString logFile;
      QString qy, s, t;
      QString toDay, La, qDay, qTime, qFreq;
      void keyPressEvent( QKeyEvent * event );
      
      int i, n, row, col, sekCnt, qslflg;
      int id, omId;
      
      QTimer * iptimer;
      int msgid;

      typedef struct {
	  long mtype;
	  char mtext[1024];
      } msgtype;
      msgtype msgbuf;
       
      void add_ipc_data (int, char *);
      void parse_ipc_data(char *);

       int getla(QString);
      void setAwdKenner();
      //void updateQso();
       
      
      
      
      
      
      QHash<QString, QString> Freqto_Band;    
      QString getBand(QString freq) {
        QHash<QString, QString>::const_iterator i=Freqto_Band.find(freq);
        if(i == Freqto_Band.end())
           return "";
        return i.value();
      }
    
      QHash<QString, QString> Freqto_mBand;     
      QString getmBand(QString freq) {
        QHash<QString, QString>::const_iterator i=Freqto_mBand.find(freq);
        if(i == Freqto_mBand.end())
           return "";
        return i.value();
      }
    
      QHash<QString, QString> BandTo_mBand;   
      QString getmyBand(QString band) {
        QHash<QString, QString>::const_iterator i=BandTo_mBand.find(band);
        if(i == BandTo_mBand.end())
           return "";
        return i.value();
      }
      
      QHash<QString, QString> mBandTo_Band;    
      QString getsBand(QString band) {
        QHash<QString, QString>::const_iterator i=mBandTo_Band.find(band);
        if(i == mBandTo_Band.end())
           return "";
        return i.value();
      }
      
      
      QHash<QString, QString> recFields;         
      QString getRecValue(QString fldname) {
        QHash<QString, QString>::const_iterator i=recFields.find(fldname);
        if(i == recFields.end())
           return "";
        return i.value();
      }
  protected slots:
      void goExit();
      void setup_ipcData();
      void getipcData();
      void searchQso(QString);
      void getHistory();
      void insertQso();
      void qsoListClicked(QTreeWidgetItem*,int);
      void updateAwdItem(QTableWidgetItem *);
      void updateQso();
      void tabwidgetChanged(int);
      
      void InfoCb();
      
  private:
       
       Dest distanz;
       
};
#endif 
