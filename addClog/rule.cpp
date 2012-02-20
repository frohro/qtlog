/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   QtLog - V 1.5 : 01.12.2007
********************************************************************** */

#include <QtGui>
#include "rule.h"

extern int Qpts;             
extern int Mul1p;            
extern int Mul2p;            
extern QString Multi1;       //
extern QString Multi2;       



cBase::cBase() 
{
    state = 0;
}
cBase::~cBase() {
}

void cBase::setQsoBand(QString b) {           
     band = b;
     state = 0;
}

void cBase::setBandPoint(int n)              
{
     qsopoint = n;
}

void cBase::setqsoPoints(int A, int B, int C, int D, int E, int F) 
{
     qsoPointA = A;
     qsoPointB = B;
     qsoPointC = C;
     qsoPointD = D;
     qsoPointE = E;
     qsoPointF = F;
}

void cBase::setmul1p(int n)                  
{
     mul1p = n;
}

void cBase::setmul2p(int n)                 
{    
     mul2p = n;
}

void cBase::setHomeDxcc(QString hdx)        
{
     homedxcc = hdx;
}

void cBase::setHomeContinent(QString hcn)   
{
     homecont = hcn;
}

void cBase::setDxContinent(QString str)     
{
     dxcont = str;
}

                                                     
void cBase::insertMulti1(QString key, QString val)   
{
     hashMulti1.insert(key, val); 
}     

void cBase::insertMulti2(QString key, QString val)  
{
     hashMulti2.insert(key, val);
}

void cBase::insertRange(QString key, QString val)   
{
     hashRange.insert(key, val);
}
 

void cBase::insertRange2(QString key, QString val)   
{
     hashRange2.insert(key, val);
}

void cBase::removeMulti1(QString key)
{
     hashMulti1.remove(key); 
}

void cBase::removeMulti2(QString key)
{
     hashMulti2.remove(key); 
}

void cBase::clearAllTable()                          
{
     hashMulti1.clear();
     hashMulti2.clear();
     hashRange.clear();
     hashRange2.clear();
}

QString cBase::getContName() {                     
    return name;
}

int cBase::getStatus() {                           
    return state;
}

QString cBase::getInfo() {                         
    return info;
}

QString cBase::getHomeDxcc() {                     
   return homedxcc;
}

QString cBase::getHomeContinent() {                
   return homecont;
}



cCheck::cCheck() 
{
    name = "CHECK";
    contId = CHECK;
}
cCheck::~cCheck() {
}




int cCheck::getqsoPoints(int n,QString p) 
{
    if( !n ) {
      Multi1 = "-";
      Multi2 = "-";
      Qpts = qsopoint;                              
      if(gethashMulti1(p+band).count() == 0) {      
	Multi1 = p;                                 
	Mul1p = mul1p;
      }
      else {
        Multi1 = "-";                               
        Mul1p = 0;
      }
    }
    return 0;
}

int cCheck::getqsoPoints(int,QString,QString) {
    return 0;
}

int cCheck::getMulti2Points(QString) {
    return 0;
}



cXmas::cXmas() 
{
      name = "XMAS";
      contId = XMAS;
}

cXmas::~cXmas() {
}


int cXmas::getqsoPoints(int n, QString str )       
{
      QString p;
      if( !n ) {
        p = str;
        if((band.compare("b40") != 0) && (band.compare("b80") != 0)) {  
            info = "  WRONG - only 80 or 40m band "; 
           return -1;
        }
        
        Multi1 = "-";
        Multi2 = "-";
        Qpts = qsopoint;                           
        if(gethashMulti1(p+band).count() == 0) {   
	  Multi1 = p;                              
	  Mul1p = mul1p;                           
        }
        else {
          Multi1 = "-";                            
          Mul1p = 0;
        }
      }
     return 0;
}

int cXmas::getqsoPoints(int, QString, QString) {
      return 0;
}


int cXmas::getMulti2Points(QString str) 
{
      if(str.count() > 2) {                         
         if(gethashMulti2(str+band).count() == 0) {
            Multi2 = str;                           
	    Mul2p = mul2p;
         }
         else {                                     
           Mul2p = 0;                               
           Multi2 = "-";
         }
      }
      return 0;
}



cHHcont::cHHcont() 
{
      name = "HHCont";
      contId = HHCont;
}

cHHcont::~cHHcont() {
}


int cHHcont::getqsoPoints(int n,QString str)         
{
     if( !n ) {
       if((band.compare("b40") != 0) && (band.compare("b80") != 0)) {  
          info = "  WRONG - only 80 or 40m band "; 
          return -1;
       }
       
       Multi1 = "-";
       Multi2 = "-";
       Qpts = qsopoint;                           
       qDebug() << "HHcont:" << str;
       if(gethashMulti1(str+band).count() == 0) {
	  Multi1 = str;                           
	  Mul1p = mul1p;
        }
        else {
             Multi1 = "-";                        
             Mul1p = 0;
        }
       }
     return 1;
}

int cHHcont::getqsoPoints(int,QString,QString) {
      return 0;
}


int cHHcont::getMulti2Points(QString str) 
{
      Mul2p = 0;
      Multi2 = "-";
      if(str.at(0) == QChar('E') || str.at(0) == QChar('Z')) { 
	 if(str.count() == 3) {
            if(gethashMulti2(str+band).count() == 0) {
               Multi2 = str;                                    
	       Mul2p = mul2p;
            }
	 }
	 else {
	   Multi2 = "-";                                        
	   Mul2p = 0;
	 }
      }
      else {
	if(str.at(0) >= QChar('0') && str.at(0) <= QChar('9')) { 
	  if(str.count() >= 3) {                                 
            if(gethashMulti2(str+band).count() == 0) {
             Multi2 = str;                                       
	     Mul2p = mul2p;
            }
            else {
              Multi2 = "-";                                       
	      Mul2p = 0;  
	    }
          }
	} 
      } 
      return 1;
}




cDARC10M::cDARC10M() 
{
      name = "DARC10M";
      contId = DARC10M;
}

cDARC10M::~cDARC10M() {
}

int cDARC10M::getqsoPoints(int n, QString dxc)    
{
       if(band.compare("b10") != 0) {  
          info = "  WRONG - only 10m band "; 
          return -1;
       }
       
       if( !n ) {
         Multi1 = "-";
         Multi2 = "-";
         Qpts = qsopoint;                           
         if(gethashMulti1(dxc+band).count() == 0) {
	    Multi1 = dxc;                           
	    Mul1p = mul1p;
         }
         else {
          Multi1 = "-";                             
          Mul1p = 0;
         }
       }
       return 1;
}

int cDARC10M::getqsoPoints(int,QString,QString) {
      return 0;
}

int cDARC10M::getMulti2Points(QString str)        
{
      i = str.indexOf("/");                       
      if(i != -1)                                 
         s = str.mid(i +1);                       
      else                                        
        s = str.mid(3);
      if(s.count() != 0) {                        //
         if(gethashMulti2(s+band).count() == 0) {
            Multi2 = s;                           
	    Mul2p = mul2p;
         }
         else {                                   
           Mul2p = 0;
           Multi2 = "-";
         }
      }
      
      return 1;
}



cCQPTY::cCQPTY() 
{
      name = "CQPTY";
      contId = CQPTY;
}

cCQPTY::~cCQPTY() {
}

int cCQPTY::getqsoPoints(int n, QString str)       
{
       Qpts = 0;
       if(!n) {                                    
          if(gethashMulti1(str).count() == 0) {
	     Multi1 = str;                         
	     Mul1p = mul1p;
	     Qpts = 0;
          }
          else {
            Multi1 = "-";                          
            Mul1p = 0;
	  }
	  qsopoint = 0;
	  if(str.left(2).compare("DN") == 0) {
	    qsopoint = 20;                         
	    Qpts = 20;                             
	  }
	}
	
	else {                                     
         if(str.compare("00") == 0)                
           Qpts = 100;                             
	 else
	   if(str.compare("XX") == 0)
	     Qpts = 20;
	 Qpts += str.toInt();
	 Qpts += qsopoint;                        
       } 
       return 1;
}

int cCQPTY::getqsoPoints(int,QString,QString ) {
      return 0;
}

int cCQPTY::getMulti2Points(QString) {
      return 0;
}



cHNY::cHNY() 
{
      name = "HNY";
      contId = HNY;
}

cHNY::~cHNY() {
}

int cHNY::getqsoPoints(int n, QString str)             
{
       if( !n ) {
         if( !qsopoint ) {
            info = "  WRONG - only 80m, 40m, 20m band "; 
            return -1;
         }
       
         Multi1 = "-";
         Multi2 = "-";
         Qpts = qsopoint;                             
         return 1;
       }
       else {
         if(str.count() != 0) {                       
            if(gethashMulti1(str+band).count() == 0) {
              Multi1 = str;                           
	      Mul1p = mul1p;
            }
            else {                                    
             Mul1p = 0;
             Multi1 = "-";
           }
        }
        return 1;
      }
}

int cHNY::getqsoPoints(int, QString, QString) { 
       return 0;
}

int cHNY::getMulti2Points( QString ) { 
      return 0;
}



cAGCWHTP::cAGCWHTP() 
{
      name = "AGCWHTP";
      contId = AGCWHTP;
}

cAGCWHTP::~cAGCWHTP() {
}


int cAGCWHTP::getqsoPoints(int n, QString st) 
{ 
  QString a, b;
 
      if( !n ) {                              
	 i = st.indexOf("/");
	 sentLine = st.left(i);
      }
      
      else {                                  
	a = sentLine;                         
	i = st.indexOf("/");                  
	b = st.left(i);                       
	a += b;
	Qpts = getRangeCheck(a).toInt();
      }
      return 1;
}

int cAGCWHTP::getqsoPoints(int, QString, QString) {
       return 0;
}

int cAGCWHTP::getMulti2Points( QString ) { 
      return 0;
}



cWAEDC::cWAEDC() 
{
      name = "CQWAEDC";
      contId = CQWAEDC;
}

cWAEDC::~cWAEDC() {
}


int cWAEDC::getqsoPoints(int n, QString cpref, QString dxcc)  
{
      if(dxcont.compare("EU") == 0) {                         
	 info = "  WRONG - Continent EU";                     
        return -1;
      }
 
      if( !n ) {
        QString d, s;
        Multi1 = "-";
        Multi2 = "-";
        Mul1p = 0;
      
        Qpts = qsopoint;
        d = getRangeCheck(dxcc);                      
        if(d.count() != 0) {                          
           m = checkDistrict(d,cpref);                
	   if(m.count() != 0) {
	     if(gethashMulti1(m+band).count() == 0) { 
                Multi1 = m;                           
	        Mul1p = mul1p;
	     }
	   }
	   else {                                     
             Multi1 = "-";                            
	     Mul1p = 0;
	   }
        } 
        else {                                        
         if(gethashMulti1(dxcc+band).count() == 0) {  
           Multi1 = dxcc;                             
           Mul1p = mul1p;
         }
         else {
	    Multi1 = "-";                             
	    Mul1p = 0;
         }
       }
      }
      return 1;
}

int cWAEDC::getqsoPoints(int,QString) {
      return 0;
}

int cWAEDC::getMulti2Points(QString) {
      return 0;
}

QString cWAEDC::checkDistrict(QString d, QString cstr)    
{
     int i = cstr.count() -1; 
     QString s = cstr.mid(i);                   
     
     bool ok;
     i = s.toInt(&ok,10);                       
     if( ok ) {                                 
       d += s.setNum(i);                        
       if(gethashMulti1(d+band).count() == 0)   
          return d;                             
     }
     return "";
}



cWAG::cWAG() 
{
      name = "WAG";
      contId = WAG;
}

cWAG::~cWAG() {
}


int cWAG::getqsoPoints(int n, QString ldxcc) 
{
      Multi1 = "-";
      Mul1p = 0;
      Multi2 = "-";
      Mul2p = 0;
     
      if( !n ) {
        if(ldxcc.compare(homedxcc) == 0) {                
          Qpts = qsoPointC;                               
        }
        else {
	  if(dxcont.compare("EU") == 0) 
            Qpts = qsoPointB;                             
            else
	      Qpts = qsoPointA;                           
	}
            if(gethashMulti1(ldxcc+band).count() == 0) {  
              Multi1 = ldxcc;
              Mul1p = mul1p;
           //}
        }
      }
      return 1;
}

int cWAG::getqsoPoints(int,QString,QString) {
      return 0;
}


int cWAG::getMulti2Points(QString str) 
{
     Mul2p = 0;
     Multi2 = "-";
  
     if(!str.toInt()) {
      if(str.count() > 2) {                         
         if(gethashMulti2(str+band).count() == 0) {
            Multi2 = str;                           
	    Mul2p = mul2p;
         }
         else {                                     
           Mul2p = 0;
           Multi2 = "-";
         }
      }
     }
     return 0;
}
      



cCQWPX::cCQWPX() 
{
      name = "CQWPX";
      contId = CQWPX;
}

cCQWPX::~cCQWPX() {
}


int cCQWPX::getqsoPoints(int n, QString cpref, QString dxc)  
{
      if( !n ) {
        Multi1 = "-";
	Mul1p = 0;
        if(dxcont.compare(homecont) != 0) {       
          Qpts = getRangeCheck(band).toInt();     
	}
	else                                      
	 if(dxc.compare(homedxcc) != 0) {         
            Qpts = getRangeCheck2(band).toInt();  
	 }
         else                                     
	  Qpts = 1;                               
	  
         if(cpref.count() != 0) {                  
	   if(gethashMulti1(cpref).count() == 0) { 
              Multi1 = cpref;                      
	      Mul1p = mul1p;                       
           }
	   else {                                  
             Multi1 = "-";                         
	     Mul1p = 0;
	   }
        }
      }
      return 1;
}

int cCQWPX::getqsoPoints(int,QString) {
      return 0;
}

int cCQWPX::getMulti2Points(QString) {
      return 0;
}



cARRLDX::cARRLDX() 
{
      name = "ARRLDX";
      contId = ARRLDX;
}

cARRLDX::~cARRLDX() {
}


int cARRLDX::getqsoPoints(int mp, QString cpref, QString dxc) 
{
      if((dxc.compare("K") != 0) && (dxc.compare("VE") != 0)) {
        info = "    WRONG - not W / VE";
        return -1;
      }
      /*
      if(getRangeCheck2(dxc) != 0) {                  
	 info = "    Wrong - not ARRLDX  prefix";
         return -1;
      }
      */
      
      if( !mp ) { 
	 Multi1 = "-";
         Mul1p = 0;
	 Qpts = qsoPointA;                            
	 
         if(dxc.compare("VE") == 0) {                 
           s = getRangeCheck(cpref);                  
	   Multi1 = s;
           if(s.count() != 0) {                       
             if(gethashMulti1(s+band).count() == 0) { 
		Mul1p = mul1p;                        
	     }
	     return 3;                                
	   }
	}
      }
      return 1;                                       
}


int cARRLDX::getqsoPoints(int n,QString sta)          
{
       if( n ) {
          Multi1 = "-";
          Mul1p = 0;
	  if(gethashMulti1(sta+band).count() == 0) {  
	     Multi1 = sta;                            
             Mul1p = mul1p;
	  }
	  return 1;
       }
       return 0;
}

int cARRLDX::getMulti2Points(QString) {
      return 0;
}



cUBADX::cUBADX() 
{
      name = "UBADX";
      contId = UBADX;
}

cUBADX::~cUBADX() {
}


int cUBADX::getqsoPoints(int n, QString cpref, QString dxcc) 
{
      if( !n ) {
        Multi1 = "-";
        Mul1p = 0;
        Multi2 = "-";
        Mul2p = 0;
	
        if(dxcc.compare("ON") == 0) {                   
	   Qpts = qsoPointA;                            
           if(gethashMulti1(cpref+band).count() == 0) { 
              Multi1 = cpref;
              Mul1p = mul1p;                            
           }
           else {                                       
             Multi1 = "-";
             Mul1p = 0;
           }
        }
        else {                                          
         if(getRangeCheck(dxcc).count() != 0) {         
           Qpts = qsoPointB;                            
           if(gethashMulti1(dxcc+band).count() == 0) {  
              Multi1 = dxcc;                            
              Mul1p = mul1p;                            
           }
           else {                                       
             Multi1 = "-";
             Mul1p = 0;
           }
         }
         else {                                         
	  Qpts = qsoPointC;                             
         }
        } 
      }
      return 0;
}

int cUBADX::getqsoPoints(int,QString) {
      return 0;
}


int cUBADX::getMulti2Points(QString str) 
{
      Multi2 = "-";
      Mul2p = 0;
 
      if(str.count() == 2) {                           
        if(getRangeCheck2(str).count() != 0) {         
  	   if(gethashMulti2(str+band).count() == 0) {  
               Multi2 = str;
               Mul2p  = mul2p;                         
           }
           else { 
             Multi2 = "-";
             Mul2p = 0;
           }
         }
         else {                                        
           info = "Wrong province: Not  - ON"; 
	   return -1;
        }
      }
      return 0;
}



cRSGB160M::cRSGB160M() 
{
      name = "RSGB160M";
      contId = RSGB160M;
}

cRSGB160M::~cRSGB160M() {
}


int cRSGB160M::getqsoPoints(int n ,QString st)       
{
       Multi1 = "-";
       Mul1p = 0;
       
       if( !n ) {
	 if(getRangeCheck(st).count() == 0) {
	    info = "Wrong  - country ( not UK )"; 
	    return -1;
	 }
	 Qpts = qsoPointA;                           
       }
       else  {
	 Qpts = qsoPointA;
	 if(st.count() == 2 ) {
	    if(gethashMulti1(st+band).count() == 0) { 
              Multi1 = st;
	      Mul1p = qsoPointB;                      
	    }
	    else {
	      Multi1 = st;
	      Mul1p = 0;
	    }
	 }
       }
       return 1;
}

int cRSGB160M::getqsoPoints(int, QString,QString)  {
      return 0;
}

int cRSGB160M::getMulti2Points(QString) {  
      return 0;
}




cREFC::cREFC() 
{
      name = "REFC";
      contId = REFC;
}

cREFC::~cREFC() {
}

int cREFC::getqsoPoints(int n, QString dx)        
{
     if( !n ) {
        Multi1 = "-";
        Multi2 = "-";
        
        if((dx.compare("F") == 0) || (dx.compare("TM") == 0) || (dx.compare("TK") == 0)) {  
	  Qpts = qsopoint;                         
	}
	else                                       
        if(getRangeCheck(dx).count() != 0) {  
            Qpts = qsoPointB;                      
	}                                          
        if(dx.compare("F") != 0) {
	  if(gethashMulti1(dx+band).count() == 0) { 
             Multi1 = dx;                           
	     Mul1p = mul1p;
          }
          else {                                    
	    Multi1 = "-";                           
            Mul1p = 0;
          }
        }
      }
      return 1;
}

int cREFC::getqsoPoints(int,QString,QString) {
      return 0;
}

int cREFC::getMulti2Points(QString str)             
{
      if(str.count() != 2) {                        
          Mul2p = 0;
          Multi2 = "-";
	 return 0;
      }
      if(str.count() != 0) {                        
         if(gethashMulti2(str+band).count() == 0) {
            Multi2 = str;                           
	    Mul2p = mul2p;
         }
         else {                                     
           Mul2p = 0;
           Multi2 = "-";
         }
      }
     return 1;
}



cCQWW160M::cCQWW160M() 
{
      name = "CQWW160M";
      contId = CQWW160M;
}

cCQWW160M::~cCQWW160M() {
}


int cCQWW160M::getqsoPoints(int n ,QString st)        
{
       Multi1 = "-";
       Mul1p = 0;
       
       if(n) {                                                         
         if((dxla.compare("K") == 0) || (dxla.compare("VE") == 0) ) {  
            if(gethashMulti1(st+band).count() == 0) {                  
               Mul1p = mul1p;                                          
               Multi1 = st;                                            
	    }
	 }
       }
       return 1;
}


int cCQWW160M::getqsoPoints(int mp, QString cpref, QString dxc)      
{
      Multi2 = "-";
      Mul2p = 0;
      
        if(!qsopoint) {
	   info = "Wrong band ( not 160m )";
           return -1;
        }
      
      	Multi1 = "-";
        Mul1p = 0;                            
        dxla = dxc;                           
        
        if( !mp ) {                           
	  if(dxcont.compare(homecont) != 0)   
	     Qpts = qsoPointA;                
	  else {                              
	    if(dxc.compare(homedxcc) == 0)    
               Qpts = qsoPointC;              
	    else
	      Qpts = qsoPointB;               
	 }
	                                      
         if(dxc.compare("VE") == 0) {         
            s = getRangeCheck(cpref);         
	    if(s.count() != 0) {              
               Multi1 = s;
               if(gethashMulti1(s+band).count() == 0) {   
		  Mul1p = mul1p;                          
	       }
	       else {
                  Mul1p = 0;
	       }
	    }	    
	    return 3;                         
	 }
	 else {   
	    if(dxc.compare("K") != 0) {                   
               if(gethashMulti1(dxc+band).count() == 0) { 
		  Mul1p = mul1p;                          
		  Multi1 = dxc;
	       }
	       else {
		  Multi1 = "-";
                  Mul1p = 0;
	       }
	       return 2;                       
	    }
	   }
	   return 1;                           
	}
	return 1;
}

int cCQWW160M::getMulti2Points(QString) {  
      return 0;
}



cNASPRINT::cNASPRINT() 
{
      name = "NASPRINT";
      contId = NASPRINT;
}

cNASPRINT::~cNASPRINT() {
}


int cNASPRINT::getqsoPoints(int mp, QString cpref, QString dxc) 
{
      Multi2 = "-";
      Mul2p = 0;
      
     if( ! mp ) {
      if(dxcont.compare("NA") != 0) {
	 info = "Wrong Continent ( #  not NA  # )";
	 return -1;
      }
      else {
	if(dxc.compare("VE") == 0) {                    
           Qpts = 1;
	   s = getRangeCheck(cpref);
	   if(s.count() != 0) {
	     Multi1 = s;
	     if(gethashMulti1(s+band).count() == 0) {
	       Mul1p = 1;
	       return 1;                                
	     }
           }
	}
	else {                                          
          Qpts = 1;
	}
        return 1;                                       
      }
     }
}


int cNASPRINT::getqsoPoints(int n ,QString st)       
{
       Multi1 = "-";
       Mul1p = 0;
       
       if( n ) {
	 if(st.count() == 1) {                       
	   Multi1 = "-";                             
           Mul1p = 0;
	 }
	 else
	 if(gethashMulti1(st+band).count() == 0) {   
	     Multi1 = st;                            
             Mul1p = 1;
	  }
	  else {
	     Multi1 = "-";                           
             Mul1p = 0;
	  }
       }
       return 1;
}

int cNASPRINT::getMulti2Points(QString) {  
      return 0;
}



cPACC::cPACC() 
{
      name = "PACC";
      contId = PACC;
}

cPACC::~cPACC() {
}


int cPACC::getqsoPoints(int n, QString str)         
{
      if( !n ) {                                    
         if(str.compare("PA") != 0) {
           info = "Wrong country ( not PA )";
           return -1;
         }
         Multi1 = "-";
         Multi2 = "-";
        Qpts = qsoPointA;                           
      }
      else {                                        
	if(gethashMulti1(str+band).count() == 0) {  
            Multi1 = str;                           
	    Mul1p = mul1p;
         }
         else {                                     
	    Multi1 = "-";                           
            Mul1p = 0;
         }
      }
      return 1;
}

int cPACC::getqsoPoints(int,QString,QString) {
      return 0;
}

int cPACC::getMulti2Points(QString) {
     return 0;
}



cCQWWDX::cCQWWDX() 
{
      name = "CQWWDX";
      contId = CQWWDX;
}

cCQWWDX::~cCQWWDX() {
}

int cCQWWDX::getqsoPoints(int,QString) {
      return 0;
}


int cCQWWDX::getqsoPoints(int n, QString dxc,QString cqz)         
{
      if( !n ) {                                    
        if(dxcont.compare(homecont) != 0) 
	  Qpts =  qsoPointA;
	else {
	   if(dxc.compare(homedxcc) != 0)
	     Qpts = qsoPointB;
	   else 
	     Qpts = qsoPointC;
	}
	
	if(gethashMulti1(dxc+band).count() == 0) {  
            Multi1 = dxc;                           
	    Mul1p = mul1p;
         }
         else {                                     
	    Multi1 = "-";                           
            Mul1p = 0;
         }
         if(cqz.count() != 0)
	   getMulti2Points(cqz);
      }
      return 1;
}

int cCQWWDX::getMulti2Points(QString cqz) 
{
       if(gethashMulti2(cqz+band).count() == 0) {  
            Multi2 = cqz;                          
	    Mul2p = mul2p;
       }
       else {                                      
	    Multi2 = "-";                          
            Mul2p = 0;
       }
       
     return 1;
}



cRUSSDX::cRUSSDX() 
{
      name = "RUSSDX";
      contId = RUSSDX;
}

cRUSSDX::~cRUSSDX() {
}


int cRUSSDX::getqsoPoints(int n, QString dxc) 
{
     if( !n ) {
       if((dxc.compare("UA") == 0) || (dxc.compare("UA2") == 0) || (dxc.compare("UA9") == 0))  {  
          Qpts = qsoPointA;                        
       }
       else 
       if(dxcont.compare(homecont) != 0 )          
          Qpts = qsoPointD;                        
       else {
        if(dxc.compare(homedxcc) != 0)             
          Qpts = qsoPointC;                        
        else
	 Qpts = qsoPointB;                         
       }
       
       if(dxc.compare(homedxcc) != 0) {             
         if(gethashMulti1(dxc+band).count() == 0) { 
            Multi1 = dxc;                           
	    Mul1p = mul1p;
         }
         else {                                     
            Mul1p = 0;
            Multi1 = "-";
         }
       }
    }
    return 1;
}

int cRUSSDX::getqsoPoints(int,QString,QString) {
      return 0;
}


int cRUSSDX::getMulti2Points(QString obl) 
{
      if(obl.count() != 2) {                       
          Mul2p = 0;
          Multi2 = "-";
	 return 0;
      }
      
      if(gethashMulti2(obl+band).count() == 0) {  
          Multi2 = obl;                           
	  Mul2p = mul2p;                          
      }
      else {                                      
         Mul2p = 0;
         Multi2 = "-";
     }
     return 1;
}



cOCEANIADX::cOCEANIADX() 
{
      name = "OCEANIADX";
      contId = OCEANIADX;
}

cOCEANIADX::~cOCEANIADX() {
}

int cOCEANIADX::getqsoPoints(int n, QString cpref) 
{
        Multi1 = "-";
	Mul1p = 0;
	Qpts = 0;
	
	qDebug() << n << cpref;
        if( !n ) {
           if((dxcont.compare("OC") == 0) && (cpref.count() != 0)) {
	     Qpts = qsopoint;                                
	     if(cpref.count() >= 3) {                        
	       if(gethashMulti1(cpref+band).count() == 0) {  
                  Multi1 = cpref;                            
	          Mul1p = mul1p;
               }
               else {                                        
                Mul1p = 0;
                Multi1 = "-";
               }
	     }
	   }
	   else {                                            
	      Mul1p = 0;
              Multi1 = "-";
	      info = "Info : not Oceania continent 'OC'";
	   }
	}
        return 1;
}

int cOCEANIADX::getqsoPoints(int,QString,QString) {
      return 0;
}

int cOCEANIADX::getMulti2Points(QString) {
     return 0;
}



cUBASP::cUBASP() 
{
      name = "UBASP";
      contId = UBASP;
}

cUBASP::~cUBASP() {
}

int cUBASP::getqsoPoints(int n, QString st)       
{
       Multi1 = "-";
       Mul1p = 0;
	
       if( !n ) {
	 if(st.compare("ON") != 0) {
	   info = "Wrong country ( not ON )";
           return -1;
         }
         Qpts = qsoPointA;
       }
       else {
	 if(st.count() != 3)
	   return 0;
	  if(gethashMulti1(st+band).count() == 0) { 
             Multi1 = st;                           
	     Mul1p = mul1p;
          }
          else {                                    
            Mul1p = 0;
            Multi1 = "-";
          }
       }
       return 1;
}

int cUBASP::getqsoPoints(int,QString,QString) {
      return 0;
}

int cUBASP::getMulti2Points(QString) {
     return 0;
}



cUBALB::cUBALB() 
{
      name = "UBALB";
      contId = UBALB;
}

cUBALB::~cUBALB() {
}

int cUBALB::getqsoPoints(int n, QString st)       
{
       Multi1 = "-";
       Mul1p = 0;
	
       Qpts = qsoPointA;
  
       if( n ) {
	 if(st.count() != 3)
	   return 0;
	  if(gethashMulti1(st+band).count() == 0) { 
             Multi1 = st;                           
	     Mul1p = mul1p;
          }
          else {                                    
            Mul1p = 0;
            Multi1 = "-";
          }
       }
       return 1;
}

int cUBALB::getqsoPoints(int,QString,QString) {
      return 0;
}

int cUBALB::getMulti2Points(QString) {
     return 0;
}



cONCONT::cONCONT() 
{
      name = "ONCONT";
      contId = ONCONT;
}

cONCONT::~cONCONT() {
}

int cONCONT::getqsoPoints(int n, QString st)       
{
       Multi1 = "-";
       Mul1p = 0;
	
       if( !n ) {
	 if(st.compare("ON") != 0) {
	   info = "Wrong country ( not ON )";
           return -1;
         }
         Qpts = qsoPointA;
       }
       else {
	 if(st.count() != 3)
	   return 0;
	  if(gethashMulti1(st+band).count() == 0) { 
             Multi1 = st;                           
	     Mul1p = mul1p;
          }
          else {                                    
            Mul1p = 0;
            Multi1 = "-";
          }
       }
       return 1;
}

int cONCONT::getqsoPoints(int,QString,QString) {
      return 0;
}

int cONCONT::getMulti2Points(QString) {
     return 0;
}



cHADX::cHADX() 
{
      name = "HADX";
      contId = HADX;
}

cHADX::~cHADX() {
}

int cHADX::getqsoPoints(int n, QString st)  
{
       Multi1 = "-";
       Mul1p = 0;
	
       if( !n ) {                          
	 if(st.compare("HA") == 0)         
           Qpts = qsoPointA;               
       else 
       if(dxcont.compare("EU") != 0 )
	   Qpts = qsoPointB;               
       else
         Qpts = qsoPointC;                 
       
       }
       else {                              
          if(getRangeCheck(st).count() == 0) {         
	      info = "Not HA country !!";
	      return -1;
	  }
	   
          if(gethashMulti1(st+band).count() == 0) { 
             Multi1 = st;                  
	     Mul1p = mul1p;
          }
          else {                           
            Mul1p = 0;
            Multi1 = "-";
          }
       }
       return 1;
}

int cHADX::getqsoPoints(int,QString,QString) {
      return 0;
}

int cHADX::getMulti2Points(QString) {
     return 0;
}



cSPDX::cSPDX() 
{
      name = "SPDX";
      contId = SPDX;
}

cSPDX::~cSPDX() {
}

int cSPDX::getqsoPoints(int n, QString st)  
{
       Multi1 = "-";
       Mul1p = 0;
	
       if( !n ) {  
	 if(st.compare("SP") != 0) {            
           info = "Info : not SP";
           return -1;
	 }
         Qpts = qsoPointA;                      
       }
       else {                                   
         if(st.count()) {
           if(getRangeCheck(st).count() == 0) {  
	      info = "#   Unknown voivodeship !!";
	      return -1;
	  }
          if(gethashMulti1(st+band).count() == 0) { 
             Multi1 = st;                      
	     Mul1p = mul1p;
          }
          else {                               
            Mul1p = 0;
            Multi1 = "-";
          }
       }
       else {
	 info = "";
	 return -1;
       }
     }
     return 1;
}

int cSPDX::getqsoPoints(int,QString,QString) {
      return 0;
}

int cSPDX::getMulti2Points(QString) {
     return 0;
}



