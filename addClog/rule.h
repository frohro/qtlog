/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   QtLog 1.5 - 01.12.2007
********************************************************************** */

#include <QtGui>
#include "contLog.h"
#ifndef RULE_H
#define RULE_H

class cBase {
 public:
   cBase();
   ~cBase();
     int state;                  
     int contId;                 
     QString name;               
     QString band;               
     QString info;
     QString s, qy;
     
     int i;
     int qsopoint;                 
     int qsoPointA, qsoPointB, qsoPointC, qsoPointD, qsoPointE, qsoPointF;
     int mul1p;                    
     int mul2p;                    
     QString homedxcc, homecont, dxcont, m, dum, dxla;  
     QString sentLine;
     
     void setQsoBand(QString);           
     void setBandPoint(int);             
     void setqsoPoints(int,int,int,int,int,int); 
     void setmul1p(int);                 
     void setmul2p(int);                 
     void setHomeDxcc(QString);          
     void setHomeContinent(QString);     
     void setDxContinent(QString);       
     
     QString getContName();              
     QString getInfo();                  
     QString getHomeDxcc();              //
     QString getHomeContinent();         //
     
     int getStatus();            
     
     void insertMulti1(QString,QString);   
     void insertMulti2(QString,QString);   
     void insertRange(QString,QString);    
     void insertRange2(QString,QString);   
     
     void removeMulti1(QString);
     void removeMulti2(QString);
     
     void clearAllTable();                 
     
     
     QHash<QString, QString> hashMulti1;      
     QString gethashMulti1(QString bkey) {
     QHash<QString, QString>::const_iterator i=hashMulti1.find(bkey);
         if(i == hashMulti1.end()) { 
           return "";                         
         }
         return i.value();                    
     }
     
     
     QHash<QString, QString> hashMulti2;      
     QString gethashMulti2(QString bkey) {
     QHash<QString, QString>::const_iterator i=hashMulti2.find(bkey);
         if(i == hashMulti2.end()) {
           return "";                         
         }
         return i.value();                    
     }
     
     
     QHash<QString, QString> hashRange;       
     QString getRangeCheck(QString nkey) {
     QHash<QString, QString>::const_iterator i=hashRange.find(nkey);
         if(i == hashRange.end()) {
           return "";                         
         }
         return i.value();                    
     }
     
     
     QHash<QString, QString> hashRange2;      
     QString getRangeCheck2(QString nkey) {
     QHash<QString, QString>::const_iterator i=hashRange2.find(nkey);
         if(i == hashRange2.end()) {
           return ""; 
         }
         return i.value(); 
     }
     
     virtual int getqsoPoints(int,QString) = 0;         
     virtual int getqsoPoints(int,QString,QString) = 0;  
     virtual int getMulti2Points(QString) = 0;          
     
   protected:
   private:
};



class cCheck : cBase  {
 public:
   cCheck();
   ~cCheck();
   
     int getqsoPoints(int,QString);             
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);              
     
   protected:
   private:
};


class cXmas : cBase {
 public:
   cXmas();
   ~cXmas();
     int getqsoPoints(int,QString);          
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);           
     
   protected:
   private:
};


class cHHcont : cBase {
 public:
   cHHcont();
   ~cHHcont();
     int getqsoPoints(int,QString);           
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);            
   
   protected:
   private:
};


class cDARC10M : cBase {
 public:
   cDARC10M();
   ~cDARC10M();
     int getqsoPoints(int,QString);           
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);            
     
   protected:
   private:
};


class cCQPTY : cBase {
 public:
   cCQPTY();
   ~cCQPTY();
     int getqsoPoints(int,QString);           
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);            
     
   protected:
   private:
};


class cHNY : cBase {
 public:
   cHNY();
   ~cHNY();
     int getqsoPoints(int,QString);           
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);            
     
   protected:
   private:
};


class cAGCWHTP : cBase {
 public:
   cAGCWHTP();
   ~cAGCWHTP();
     int getqsoPoints(int,QString);           
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);            
     
   protected:
   private:
};


class cWAEDC : cBase {
 public:
   cWAEDC();
   ~cWAEDC();
   
     QString checkDistrict(QString, QString); 
   
     int getqsoPoints(int,QString);          
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);           
     
   protected:
   private:
};


class cWAG : cBase {
 public:
   cWAG();
   ~cWAG();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cARRLDX : cBase {
 public:
   cARRLDX();
   ~cARRLDX();
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cCQWPX : cBase {
 public:
   cCQWPX();
   ~cCQWPX();
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cCQWWDX : cBase {
 public:
   cCQWWDX();
   ~cCQWWDX();
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cUBADX : cBase {
 public:
   cUBADX();
   ~cUBADX();
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cREFC : cBase {
 public:
   cREFC();
   ~cREFC();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cRSGB160M : cBase {
 public:
   cRSGB160M();
   ~cRSGB160M();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cCQWW160M : cBase {
 public:
   cCQWW160M();
   ~cCQWW160M();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cNASPRINT : cBase {
 public:
   cNASPRINT();
   ~cNASPRINT();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cPACC : cBase {
 public:
   cPACC();
   ~cPACC();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cRUSSDX : cBase {
 public:
   cRUSSDX();
   ~cRUSSDX();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cOCEANIADX : cBase {
 public:
   cOCEANIADX();
   ~cOCEANIADX();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cUBASP : cBase {
 public:
   cUBASP();
   ~cUBASP();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cUBALB : cBase {
 public:
   cUBALB();
   ~cUBALB();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cONCONT : cBase {
 public:
   cONCONT();
   ~cONCONT();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cHADX : cBase {
 public:
   cHADX();
   ~cHADX();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};


class cSPDX : cBase {
 public:
   cSPDX();
   ~cSPDX();
   
     int getqsoPoints(int,QString);            
     int getqsoPoints(int,QString,QString);
     int getMulti2Points(QString);             
     
   protected:
   private:
};
#endif 
