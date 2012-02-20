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
#include <QtSql>
#include "dxlist.h"
dxlist::dxlist()
{
    ErrorStr = "";
    errCnt = 0; 
    laCnt = 0;                           
}
dxlist::~dxlist()
{
}


int dxlist::getErrCnt()
{
   return errCnt;
}
void dxlist::clrErrStr()
{
   ErrorStr = "";
}
void dxlist::clrErrCnt()
{
   errCnt = 0;;
}


QString dxlist::getErorrList()
{
  return ErrorStr;
}


void dxlist::setLogname(QString name)   
{
   logname = name;
}


void dxlist::setDxlistClear()           
{
   qy = "DELETE FROM "+logname;
   query.exec(qy);
   laCnt = 0;
}


void dxlist::insertDxItem(int id, QString mode, QString band, QString qslr, QString adxcc) 
{
 int cntx, cntb, n, z;
 QString dxcc, xband;
 bool ok;
    z = 0;
    if(mode.count() == 0 ) {
       ErrorStr += "QSO "+s.setNum(id)+" Das Feld 'mode' ist leer\n"; 
       errCnt++;
      return;
    }
    if(band.count() == 0 ) {
       ErrorStr += "QSO "+s.setNum(id)+" Das Feld 'band' ist leer\n";  
       errCnt++;
      return;
    }
    if(qslr.count() == 0 ) {
       ErrorStr += "QSO "+s.setNum(id)+"Das Feld 'QSLe' ist leer\n";  
       errCnt++;
      return;
    }
    if(adxcc.count() == 0 ) {
       ErrorStr += "QSO "+s.setNum(id)+" Das Feld 'dxcc' ist leer\n";
       errCnt++;
      return;
    }

    dxcc = adxcc;
    n = adxcc.toInt(&ok,10);                  
    if(ok == true ) {                         
       qy = "SELECT ldxcc FROM tla WHERE adif="+s.setNum(n);
       query.exec(qy);
       if( !query.size() ) {
         ErrorStr += "QSO "+s.setNum(id)+" Adif_Nr "+adxcc+" ist unbekannt\n";  
         errCnt++;
        return;
       }
       query.next();
       dxcc = query.value(0).toString();
   }
 
   
   qy = "SELECT ldxcc FROM tla WHERE la = '"+dxcc+"'";
   query.exec(qy);
   if(query.size() == 0 ) {           
      ErrorStr += "QSO "+s.setNum(id)+" Im dxcc ist ein SubPrefix '"+dxcc+"' eingetragen\n"; 
      errCnt++;
     return;
   }
   
   
   qy = "SELECT cntx, cntb, b"+band+" FROM "+logname;
   qy += " WHERE dxpref='"+dxcc+"' AND mode='"+mode+"'";
   query.exec(qy);
   z = query.size();
   if(z != 0) {
       query.next();
       cntx = query.value(0).toInt();
       cntb = query.value(1).toInt();
      xband = query.value(2).toInt();
       if(qslr.compare("R") == 0 ) {            
          cntx++;                               
          qy = "UPDATE "+logname+" SET cntx=";  
          qy += s.setNum(cntx);
          if(xband.compare("y") != 0) {         
             qy += ", b"+band;                  
             qy += "='x' WHERE dxpref='"+dxcc+"' AND mode='"+mode+"'";
          }
          else {                                
            qy += " WHERE dxpref='"+dxcc+"' AND mode='"+mode+"'";
          }
       }
       else {
         cntb++;                                
         cntx++;                                
         qy = "UPDATE "+logname+" SET cntx=";
         qy += s.setNum(cntx);
         qy += ",cntb="+s.setNum(cntb);
         qy += ", b"+band;
         qy += " ='y' WHERE dxpref='"+dxcc+"' AND mode='"+mode+"'";      
       }
       query.exec(qy);
   }
   else {                                       
     qy = "INSERT INTO "+logname+" VALUE ('";
     qy += dxcc;                                
     qy += "','"+mode;                          
     qy += "',0,0,";                            
     qy += "'','','','','','','','','','','','','','','','','','','')";  
     query.exec(qy);
     if(qslr.compare("R") == 0 ) {                                       
          cntx = 1;                             
          qy = "UPDATE "+logname+" SET cntx=";
          qy += s.setNum(cntx);
          qy += ", b"+band;
          qy += " ='x' WHERE dxpref='"+dxcc+"' AND mode='"+mode+"'";
     }
     else {
         cntx = 1;                              
         cntb = 1;                              
         qy = "UPDATE "+logname+" SET cntx="+s.setNum(cntx);
         qy += ",cntb="+s.setNum(cntb);
         qy += ", b"+band;
         qy += " ='y' WHERE dxpref='"+dxcc+"' AND mode='"+mode+"'";      
     }
     query.exec(qy);
     laCnt++;
   }
}
