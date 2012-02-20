/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.07 - 20.01.09
********************************************************************** */

#include <QtGui>
#ifndef QUADI_H
#define QUADI_H
class quadi {
 public:
   quadi();
  ~quadi();
     QString qy, s, f, Error;
     QString tmp, etmp;                    
     int getState();                       
     int getCount();                       
     QString getErrorInfo();
     int i, n, count;
     QHash <QString, QString> toAdif;       
     QHash <QString, QString> toAdifBand;   
     QHash <QString, QString> toSysBand;    
     QHash <QString, QString> toAdifCustom; 
     QString Lcn, Cqz, Ituz, Dxcc;
     int  tform, dform;

     void createAdifTableHeader();           
     void loadAdifAwdTable(QString,QString); 
     void doExport(QString,QString,QString,QString,int,int); 
     QString getAdifName(QString);           
     QString getAdifCustom(QString);         
     QString getAdifqsl(QString);            
     void clearExpHash();                    
     void setBandTable();                    
     void setDayForm(int);
     void setTimeForm(int);
     QString getAdifBand(QString);           
     void clearBandHash();
 protected:
 private:
   int qsoFields;         
   int qslFields;
   int awdFields;
   int state;
};
#endif
