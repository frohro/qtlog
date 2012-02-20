/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007, 20.11.08
********************************************************************** */

#include <QtGui>
#ifndef ADIF_H
#define ADIF_H
class adif {
 public:
   adif();
  ~adif();
     QString qy, s, f, Error;
     QString tmp;
     int getState();                        
     int getCount();                        
     QString getErrorInfo();
     int i, n, rowcnt;
     QString dayLastDown;                   
     QString getDayLastDown();              
     QString dheute;
     void setdateHeute(QString);
     QString getdheute();
     QString nLastDown;                     
     void setLastDownNr(QString);           
     QHash <QString, QString> toAdif;       
     QHash <QString, QString> toAdifBand;   
     QHash <QString, QString> toAdiBand;    
     QHash <QString, QString> toSysBand;    
     QHash <QString, QString> tosysBand;    
     QHash <QString, QString> toAdifMode;   
     QHash <QString, QString> toAdiMode;    
     QString Lcn, Cqz, Ituz, Dxcc;
     char * getdbName(char * );             
     char * getdbNameQsl(char * );          
     char * getdbNameAwd(char *);           
     void setValue(char *, QString);
     void setValueQsl(char *, QString);
     void setValueAwd(char *, QString);
  
     QString getValue(char *);                 
     QString getValueAwd(char *);
     QString getAdifName(QString);             
     void createTranslatTable();
     void clearTranslateTable();
     void readQslReport(QString,QString,QString); 
     void setBandTable();                      
     void clearModeTable();
     void setModeTabel();
     void clearBandTable();                    
     QString getAdifMode(QString);             
     QString getAdiMode(QString);              
     QString getAdifBand(QString);             
     QString getAdiBand(QString);              
     QString getSysBand(QString);              
     QString getsysBand(QString);              
     
     
     void openRefListQso(int);
     void openRefListQsl(int);
     void openRefListAwd(int);
     void closeRefList();
     void loadRefList();
     int getla(QString);                       
     int nqsl;                                 
     int getNewqslCnt();                       
     void setQslNewQsoStatus(QString, QString, QString);
 protected:

  typedef class {
  public:
     int pos;             
    char field[28];       
    char refnam[28];      
    char adifnam[28];     
    char value[255];      
  }ADIFHEAD;
  ADIFHEAD * adh;         
  ADIFHEAD * adhq;        
  ADIFHEAD * adhw;        
 private:
   int qsoFields;         
   int qslFields;
   int awdFields;
   int state;
};
#endif
