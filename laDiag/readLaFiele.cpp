/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */
/* readlaTextFiele.cpp    01.05.2007
   Es wird die ARRL 'DXCC country list' (04/2003) gelesen und in die mySQL.db geladen.
   Als Datenquelle wird die überarbeite Version von k2di verwndet.
   -http://www.arrl.org/files/infoserv/tech/dxcck2di.txt
   Die county_liste wird in 2 Tabellen aufgespalten.
   Tabelle 'tla' enthält den dxcc_Landes_Prefix
   Tabelle 'tpref' hält die sub_prefixses des Landes und verweist auf den ARRL_dxcc_landes_prefix.
   - arrl_dxcc.dat muß im ("HOME") stehen !!
   - LEER_zeilen muessen mit '#' auskommentart werden !!
   - Zeilen die nicht übernommen werden sollen - zB die mit ~ am Anfang, mit # ausklammern.
*******************************************************************************************/

#include <QMessageBox>
#include "laDiag.h"
#include "../qtlogDiag/dirmngr.h"
extern DirMngr dirMngr;


void laDiag::readlaTextFile()
{
  int i, n, p, l;
  QString zeile, z, a, lpref;
  char buf[50];
  int Status = QMessageBox::question(this,
               tr("Datei ueberschreiben?"),
               tr("Die Tabelle la.DXCC existiert bereits.\n"
                  "Wollen Sie diese Tabelle ueberschreiben?\n"
                  "Alle bisher eingetragenen Prefixe gehen hierbei verloren !"),
               QMessageBox::Yes,QMessageBox::No);
   if(Status != QMessageBox::Yes)
     return;
#ifdef Q_WS_WIN
   QFile datei(dirMngr.CreatePath("qtlog_arrl_dxcc.dat",DIR_PROGRAM));
#else
   sprintf(buf,"%s/log/qtlog_arrl_dxcc.dat",(char*)getenv("HOME"));
   QFile datei(buf);
#endif
   
   if(! datei.open(QFile::ReadOnly)) {
     QMessageBox::information( this,
       tr("ARRL_DXCC.dat"),
       tr("Datei 'qtlog_arrl_dxcc.dat' kann nicht geoeffnet werden !"));
     return;
   }
   dbflg = 1;
   QSqlQuery query;
   qy = "DELETE FROM tla";                      
   query.exec(qy);
   qy = "DELETE FROM tpref";
   query.exec(qy);
  QTextStream inStream(&datei);                 
  l = 0;
  QString qy;
  while(1) {                                    
     while( !inStream.atEnd()) {                
        zeile = inStream.readLine(0);
        if(zeile[0] != QChar('#'))              
           break;
     }
    
    
    
    
    
    QSqlQuery query;
    while( 1 ) {
       i = 0, n = 0, p = 0;
       qy = "INSERT INTO tla VALUES (";
       while( n != 13) {                         
          while(zeile[i] != QChar(':')) i++;     
          z = "";
          a = "";
          while(i != p) a += zeile[p++];         
          z += a.simplified();                   
          if ( n == 0 )                          
             lpref = z;                          
          if( n == 2 ) {                         
             a = z.toUpper();                    
             z = a;
          }
          if(n == 6) {                          
             if(z.endsWith("N") == true) {      
                a = z.left(z.length() -1);
                z = a;
             }
              else {                            
                a = "-"+z.left(z.length() -1);
                z = a;
              }
          }
          if( n == 7 ) {                        
             if(z.endsWith("E") == true) {      
                a = z.left(z.length() -1);
                z = a;
             }
             else {                             
               a = "-"+z.left(z.length() -1);
               z = a;
             }
          }
          qy += "'"+z;
          if( n == 12 ) {                       
             qy += "'";                         
             break;
          }
          qy += "',";
          i++;                                 
          p++;                                 
          n++;                                 
      }                                        
      qy += ")";                               
      query.exec(qy);                          

      i = 0, n = 0, p = 0;
      zeile = inStream.readLine(0);
      while(1) {                               
        while(1) {                             
             if(zeile[i] == QChar(','))
                   break;
             else                              
               if(zeile[i] == QChar(':'))
                break;
             else                              
               if(zeile[i] == QChar(';'))
                break;
          i++;                                  
        }
        z = "";
        while(i != p) z += zeile[p++];          
        qy = "INSERT INTO tpref VALUES('";
        qy += z.simplified();                   
        qy += "','"+lpref+"')";                 
        query.exec(qy);                         
        if(zeile[p] == QChar(';'))              
            break;
        if(zeile[p] == QChar(':')) {            
          zeile = inStream.readLine(0);         
          i = 0, n = 0, p = 0;
        }
        else {
          i++;                                  
          p++;
        }
       }
       //if(++l == 400) {                        
         
       
       //}
       zeile = inStream.readLine();               
       if(zeile[0] == QChar('#'))
           break;                                 
       z = "";                                    
     }
      break;                                      
  }
  datei.close();
  qy = "SELECT * FROM tla";                       
  getLaTable(qy);
  dbflg = 0;
}



void laDiag::readFileCtyWtDat()
{  
  int i, n, n1, n2, p, zlen;
  QString zeile, z, a;
  QString Country, pCQz, pITUz, eCQz, eITUz, Cont, Br, Lg, TimeOfset, Pref, Dxcc, subPref, sPref;
  QString subCountry, info;
  char buf[50];
 
     int Status = QMessageBox::question(this,
               tr("Datei ueberschreiben?"),
               tr("Die Tabelle cty_tw.dat existiert bereits.\n"
                  "Wollen Sie diese Tabelle ueberschreiben?\n"
                  "Alle bisher eingetragenen Prefixe gehen hierbei verloren !"),
               QMessageBox::Yes,QMessageBox::No);
     if(Status != QMessageBox::Yes)
       return;
  
  
     sprintf(buf,"%s/log/iniFiles/cty_wt.dat",(char*)getenv("HOME"));
     QFile datei(buf);
   
     if(! datei.open(QFile::ReadOnly)) {
        QMessageBox::information( this,
        tr("cty.dat"),
        tr("Datei 'cty_wt.dat' kann nicht geoeffnet werden !"));
      return;
     }
     
     n1 = 0;
     n2 = 0;
     QSqlQuery query;
     qy = "DELETE FROM tla";                       
     query.exec(qy);
     qy = "DELETE FROM tpref";
     query.exec(qy);
  
     QTextStream inStream(&datei);                 
     QString qy;
     while(1) {                                    
       while(1) {
	 if(inStream.atEnd()) {
	   datei.close();                          
	   //qDebug() << "ENDE";
	   return;
         }
         else {
          zeile = inStream.readLine(0);
          if(zeile[0] != QChar('#')) {            
             break;
          }
	 }
        }
       
       
       
       
       
       
       
       info ="";
       QSqlQuery query;
       QSqlField field("feld",QVariant::String);  
       i = 0, n = 0, p = 0;                       
       qy = "INSERT INTO tla VALUES (";           
       while( n != 8) {                           
           while(zeile[i] != QChar(':')) i++;     
           z = "";
           a = "";
           while(i != p) a += zeile[p++];        
           z = a.simplified();                   
           if ( n == 0 )                         
              Country = z;                       
           else 
            if( n == 1 )                         
               pCQz = z;                         
           else 
            if( n == 2 )                         
              pITUz = z;                         
           else 
            if( n == 3 )                         
               Cont = z;                         
           else 
            if( n == 4 )                         
               Br = z;                           
           else
            if( n == 5 ) {                       
              if(z.indexOf("-") != -1) {         
	         z.remove(QChar('-'), Qt::CaseInsensitive);
                 Lg = z;                         
              }
              else                               
	        Lg = "-"+z;                      
	    }
           else 
            if( n == 6 ) {                       
	       if(z.indexOf("-") != -1) {
	         z.remove(QChar('-'), Qt::CaseInsensitive);
                 TimeOfset = "+"+z;
	       }
	       else
		 TimeOfset = "-"+z;
            }
           else
            if( n == 7 ) {                       
               Pref = z;
	       Dxcc = Pref;                      
	     break;
	    }
          i++;                                   
          p++;                                   
          n++;                                   
       }                                         
       if(Pref.indexOf("*") != -1) {  
          Pref.remove("*");           
          info = "* nur DARC WAEdc & CQWW, nicht ARRL Contest"; 
       }
       qy = "INSERT INTO tla VALUES ('"+Pref;    
       qy += "','"+Country;                      
       qy += "','"+Cont;
       qy += "','"+pITUz;
       qy += "','"+pCQz;
       qy += "','"+TimeOfset;
       qy += "','"+Br;
       qy += "','"+Lg;
       qy += "','"+Dxcc;
       qy += "','','','','"+info+"')";           
       query.exec(qy);                           
       info ="";
       
       
       
       subCountry = "";
       while( 1 ) {                              
         zeile = inStream.readLine(0);
         zeile = zeile.simplified();
         if(zeile.indexOf("#") != -1) {          //check #; ja - ist sub_La_name
	    subCountry = zeile.replace(0,1,"-");
	    zeile = inStream.readLine(0);        
	    zeile = zeile.simplified();
	 }
	  
	 p = 0;
	 zlen = zeile.count();
	 while( 1 ) {                            
	    z = "";                              
	    while(1) {
	      if((zeile[p] == QChar(',')) | (zeile[p] == QChar(';')))
	        break;                           
	      z += zeile[p++]; 
	    }
	 
	  z.remove("=");
	  eCQz = pCQz;                           
	  eITUz = pITUz;                         
          if((z.indexOf("(") == -1) & (z.indexOf("[") == -1)) {  
	     qy = "INSERT INTO tpref VALUES('";  
             qy += z.simplified();               
             qy += "','"+Pref+"')";              
             
             query.exec(qy);                     
	  }
	  else {                                 
	    n1 = z.indexOf("(");                 
	    if(n1 != -1) {                       
	      n2 = z.indexOf(")");               
	      sPref = z.left(n1);                
	      eCQz = z.mid(n1+1,n2 -(n1+1));     
	      
	      n1 = z.indexOf("[");               
	      if(n1 != -1) {                     
	        n2 = z.indexOf("]");
	        eITUz = z.mid(n1+1,n2 -(n1+1));  
	      }
	    }
	    else {                               
	      n1 = z.indexOf("[");               
	      if(n1 != -1) {
		n2 = z.indexOf("]");
		sPref = z.left(n1);
		eITUz = z.mid(n1+1,n2 -(n1+1));  
	      }
	    }
	    
	    if(eCQz.count() == 1) {              
	      s = "0"+eCQz;
	      eCQz = s;
	    }
	    
	    if(eITUz.count() == 1) {
	      s = "0"+eITUz;
	      eITUz = s;
	    }
	   
	                                        
	    if(Dxcc.compare("UA") == 0) {                   
	      subPref = Dxcc+"-"+eCQz;                      
	      i = subCountry.count();
	      subPref += subCountry.mid(i -3,2);            
	    }
	    else 
	    if(Dxcc.compare("UA9") == 0) {                  
	      subPref = Dxcc+"-"+eCQz;                      
	      i = subCountry.count();
	      subPref += subCountry.mid(i -3,2);            
	    }
	    else 
	    if(Dxcc.compare("K") == 0) {                    
	       subCountry.remove("USA");
	       subPref = Dxcc+"-"+eCQz+subCountry.mid(2,2); 
	    }
	    else 
	    if(Dxcc.compare("VE") == 0) {                   
	       subPref = Dxcc+"-"+eCQz;                     
	       i = subCountry.count();
	       subPref += subCountry.mid(i -2,2);           
            }
	    else 
	    if(Dxcc.compare("VK") == 0) {                   
	       subPref = Dxcc+"-"+eCQz;                     
	       subPref += subCountry.mid(2,3);              
	       subPref = subPref.simplified();              
            }
	    else
	    if(Dxcc.compare("LU") == 0) {                   
	       subPref = Dxcc+"-"+eCQz+subCountry.mid(2,3); 
	    }
	    else
	    if(Dxcc.compare("BY") == 0) {                   
	       subPref = Dxcc+"-"+eCQz+subCountry.mid(2,3); 
	       subPref = subPref.simplified();              
	    }
	    else
	    if(Dxcc.compare("YB") == 0) {                   
	       subPref = Dxcc+"-"+eCQz+subCountry.mid(2,4); 
	    }
	    else 
	    if(Dxcc.compare("PY") == 0) {                   
	        subPref = Dxcc+"-"+eCQz;                    
	        i = subCountry.count();
	        subPref += subCountry.mid(i -3,2);          
	    }
	    else {                                          
	       subPref = Dxcc+"-"+eCQz+subCountry.mid(2,3); 
	       subPref = subPref.simplified();              
	    }
	    
	    a = Country + subCountry;                  
	    qy = "SELECT ldxcc FROM tla WHERE la='"+subPref+"'";
	    query.exec(qy);                            
	    if( !query.size() ) {                      
               qy = "INSERT INTO tla VALUES ('"+subPref; 
               qy += "',";
               field.setValue(a);                           
               qy += db.driver()->formatValue(field,false); 
               qy += ",'"+Cont;                             
               qy += "','"+eITUz;                           
               qy += "','"+eCQz;                            
               qy += "','"+TimeOfset;                       
               qy += "','"+Br;                              
               qy += "','"+Lg;                              
               qy += "','"+Pref;                            
               qy += "','','','','')";                      
               query.exec(qy);  
	    }
	    qy = "INSERT INTO tpref VALUES('";             
            qy += sPref;                                   
            qy += "','"+subPref+"')";                      
            query.exec(qy);                                
         }
         if((zeile[p] == QChar(';')) || (++p == zlen))
	   break;                                          
	}                                                  
	//if(Pref.compare("CM") == 0) return;              
	if(zeile[p] == QChar(';'))                         
	  break;                                           
        }                                           //--- while subPrefixes_str != ';', - nächste Zeile lesen
    }
}
