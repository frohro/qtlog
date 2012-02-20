/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QtGui>
#ifndef WADIF_H
#define WADIF_H
class wadif {
 public:
   wadif();
  ~wadif();
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

     void openRefListQso(int);             
     void openRefListQsl(int);             
     void openRefListAwd(int);             
     void closeRefLists();                 
     void loadRefList();                   
     void loadRefListAwd();
     char * getAdifName(char * );          
     char * getdbName(char * );            
     char * getdbNameQsl(char * );         
     char * getdbNameAwd(char *);          
     void setValue(char *, QString);
     void setValueQsl(char *, QString);
     void setValueAwd(char *, QString);
     void addQslFlag(char *, QString);     
     
     QString getValue(char *);               
     QString getValueQsl(char *);

     void createAdifTableHeader();           
     void loadAdifAwdTable(QString,QString); 
     void doExport(QString,QString,int);     
     QString getAdifName(QString);           
     QString getAdifCustom(QString);         
     QString getAdifqsl(QString);            
     void clearExpHash();                    
     //void doImport(QString,QString,int,QString); 
    
     void doImport(QString, QString, int, QString, int); 
     
     void setBandTable();                  
     QString getAdifBand(QString);         
     QString getSysBand(QString);          
     void clearBandHash();
     int getla(QString);                   
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
