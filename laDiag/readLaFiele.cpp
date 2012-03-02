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

#include <QtGui>
#include <QtSql>
#include <QMessageBox>
#include <QTranslator>

#include "laDiag.h"


// ------------------------------------------------------
// wird zZ. nicht verwendet ->   siehe readFileCtyWtDat()
void laDiag::readlaTextFile()
{
  int i, n, p;
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
   
   // -
   sprintf(buf,"%s/log/qtlog_arrl_dxcc.dat",(char*)getenv("HOME"));
   QFile datei(buf);
   if(! datei.open(QFile::ReadOnly)) {
     QMessageBox::information( this,
       tr("ARRL_DXCC.dat"),
       tr("Datei 'qtlog_arrl_dxcc.dat' kann nicht geoeffnet werden !"));
     return;
   }
   
   dbflg = 1;
   QSqlQuery query;
   
   qy = "DELETE FROM tla";                       // alte Tabellen löschen       besser DROP ?
   query.exec(qy);
   qy = "DELETE FROM tpref";
   query.exec(qy);
   
  QTextStream inStream(&datei);                 // neue Tabellen aufbauen
  QString qy;
  while(1) {                                    
     while( !inStream.atEnd()) {                
        zeile = inStream.readLine(0);
        if(zeile[0] != QChar('#'))              
           break;
     }
    
    // ab hier die 1. und alle folgenden Text_zeilen bearbeiten
    // -------------------------------------------------------------------------
    // pref: name: cont: ITUz: QQz: tz: Br: Lg: pref: WAE: WPX: Adif: Notiz:
    //    0:    1:    2:    3:   4:  5:  6:  7:    8:   9:  10:   11:    12:
    // -------------------------------------------------------------------------
    QSqlQuery query;
    while( 1 ) {
       i = 0, n = 0, p = 0;
       qy = "INSERT INTO tla VALUES (";
       while( n != 13) {                         // 0 - 13 Felder bearbeiten
          while(zeile[i] != QChar(':')) i++;     // index i vor bis ':'
          z = "";
          a = "";
          while(i != p) a += zeile[p++];         // 1.item holen: kopiere item nach -> a
          z += a.simplified();                   // space entfernen
          if ( n == 0 )                          // Feld 0 ?
             lpref = z;                          // ja: pref sichern
          if( n == 2 ) {                         // Feld 2 Countinent ?
             a = z.toUpper();                    // ja: wandle in großen Buchtaben
             z = a;
          }
          if(n == 6) {                           // check geograph.Koordin.
             if(z.endsWith("N") == true) {       // ja: Breite NORD ? '+'
                a = z.left(z.length() -1);
                z = a;
             }
              else {                             // ist Breite SUED '-'
                a = "-"+z.left(z.length() -1);
                z = a;
              }
          }
          if( n == 7 ) {                         // falls Feld 7 - check geograph.Länge
             if(z.endsWith("E") == true) {       // ja: Länge E (OST)'+'
                a = z.left(z.length() -1);
                z = a;
             }
             else {                              // ist Laenge WEST '-'
               a = "-"+z.left(z.length() -1);
               z = a;
             }
          }
          qy += "'"+z;
          if( n == 12 ) {                        // letztes Feld ?
             qy += "'";                          // ja:
             break;
          }
          qy += "',";
          i++;                                  // text[i] position ':' im textfld
          p++;                                  // text[p] schreibposition
          n++;                                  // Feld_zähler
      }                                         // 12 Spalten durchlaufen
      qy += ")";                                // query fertig
      query.exec(qy);                           // la.prefix in db eintragen

      i = 0, n = 0, p = 0;
      zeile = inStream.readLine(0);
      while(1) {                                // alle sub_prefix_zeilen bearbeiten
        while(1) {                              // suche nach ',' ':' ';'
             if(zeile[i] == QChar(','))
                   break;
             else                               // oder
               if(zeile[i] == QChar(':'))
                break;
             else                               // oder
               if(zeile[i] == QChar(';'))
                break;
          i++;                                  // nicht gefunden: index vorrücken
        }
        z = "";
        while(i != p) z += zeile[p++];          // item_sub.prefix uebernehmen
        qy = "INSERT INTO tpref VALUES('";
        qy += z.simplified();                   // space entfernen
        qy += "','"+lpref+"')";                 // + ref_landes_prefix
        query.exec(qy);                         // SUb_prefix in db eintragen
	
        if(zeile[p] == QChar(';'))              // Zeilen_ENDE erreicht ?
            break;
        if(zeile[p] == QChar(':')) {            // falls zeilen_umbruch gefunden
          zeile = inStream.readLine(0);         
          i = 0, n = 0, p = 0;
        }
        else {
          i++;                                   // sonst weitermachen
          p++;
        }
       }
                    
       //if(++l == 400) {                        //  TEST
         // qDebug() << l << "jetzt return";     // -.-
       //   return;
       //}
       
       zeile = inStream.readLine();              // naechste laender_prefix_Zeile lesen
       if(zeile[0] == QChar('#'))
           break;                                // Datei_ENDE ?
       z = "";                                   // sonst neue Zeile
     }
      break;                                     // ENDE
  }
  datei.close();
  qy = "SELECT * FROM tla";                      // neue Tabelle von db lesen und anzeigen
  getLaTable(qy);
  dbflg = 0;
}




// ========================================================================================================
// cty_tw.dat
// -------------------------------------------
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
     qy = "DELETE FROM tla";                        // alte Tabellen löschen  - besser DROP ?
     query.exec(qy);
     qy = "DELETE FROM tpref";
     query.exec(qy);
  
     QTextStream inStream(&datei);                  // neue Tabellen aufbauen
     QString qy;
     while(1) {                                     // bis atEnd() 
       while(1) {
	 if(inStream.atEnd()) {
	   datei.close();                          // ENDE
	   //qDebug() << "ENDE";
	   return;
         }
         else {
          zeile = inStream.readLine(0);
          if(zeile[0] != QChar('#')) {             // List_Anfang, 4 Info_Zeilen überspringen
             break;
          }
	 }
        }
       // --
       // ab hier die 1. und alle folgenden Text_zeilen bearbeiten
       // ---------------------------------------------------------------------------
       // country: CQz: ITUz: Cont: Br: Lg: GMT: pref: | Dxcc: WAE: WPX: Adif: Notiz:
       //       0:   1:    2:    3:  4:  5:   6:    7:      8:   9:  10:   11:    12:
       // ---------------------------------------------------------------------------
       // Primary Prefix
       info ="";
       QSqlQuery query;
       QSqlField field("feld",QVariant::String);  // arbeits_var. 'field'
       i = 0, n = 0, p = 0;                       // zeile enthält den String
       qy = "INSERT INTO tla VALUES (";           // Primary DXCC Prefix bilden
       while( n != 8) {                           // 0 - 7 Felder bearbeiten
           while(zeile[i] != QChar(':')) i++;     // index i vor bis ':'
           z = "";
           a = "";
           while(i != p) a += zeile[p++];        // 1.item holen: kopiere item nach -> a
           z = a.simplified();                   // space entfernen
           if ( n == 0 )                         // Feld 0 ?
              Country = z;                       // ja: Country
           else 
            if( n == 1 )                         // Feld 1 CQzone - primary_CQz
               pCQz = z;                         // 
           else 
            if( n == 2 )                         // Feld 2 ITUzone - primary_ITUz
              pITUz = z;                         // 
           else 
            if( n == 3 )                         // Feld 3 Continent
               Cont = z;                         // space entfernen
           else 
            if( n == 4 )                         // Feld 4 - check geograph.Breite
               Br = z;                           // space entfernen
           else
            if( n == 5 ) {                       // falls Feld 5 - check geograph.Länge
              if(z.indexOf("-") != -1) {         // ist '-'Zeichen vorhanden 'Ost'
	         z.remove(QChar('-'), Qt::CaseInsensitive);
                 Lg = z;                         // übernehme ohne Vorzeichen
              }
              else                               // nein; 'ist West' mit '-' Vorzeichen übernehmen
	        Lg = "-"+z;                      // 
	    }
           else 
            if( n == 6 ) {                       // Feld 6 TimeZone
	       if(z.indexOf("-") != -1) {
	         z.remove(QChar('-'), Qt::CaseInsensitive);
                 TimeOfset = "+"+z;
	       }
	       else
		 TimeOfset = "-"+z;
            }
           else
            if( n == 7 ) {                       // Feld 7 Prefix
               Pref = z;
	       Dxcc = Pref;                      // Primary DXCC-Prefix
	     break;
	    }
          i++;                                   // text[i] position ':' im textfld
          p++;                                   // text[p] schreibposition
          n++;                                   // Feld_zähler
       }                                         // 0 - 7 Spalten durchlaufen
       
       if(Pref.indexOf("*") != -1) {  // Primary DXCC: A * preceding this prefix indicates that the contry 
          Pref.remove("*");           // is on the DARC WAEDC list, and counts in CQ-sponsored
          info = "* nur DARC WAEdc & CQWW, nicht ARRL Contest";  // but not ARRL-sponsord contests
       }
       qy = "INSERT INTO tla VALUES ('"+Pref;    // Primary Prefix
       qy += "','"+Country;                      // --------------
       qy += "','"+Cont;
       qy += "','"+pITUz;
       qy += "','"+pCQz;
       qy += "','"+TimeOfset;
       qy += "','"+Br;
       qy += "','"+Lg;
       qy += "','"+Dxcc;
       qy += "','','','','"+info+"')";           // lawd,lwpx,adif,notiz
       query.exec(qy);                           // la.prefix in db eintragen 
       info ="";
       
       // jetzt Sub_prefixe übernehmen - die Parameter vom main_Prefix stehen in den Variablen
       // subPrefixe sind durch Komma getrennet.
       subCountry = "";
       while( 1 ) {                              // min. einen subPrefix gibt es immer
         zeile = inStream.readLine(0);
         zeile = zeile.simplified();
         if(zeile.indexOf("#") != -1) {          //check #; ja - ist sub_La_name
	    subCountry = zeile.replace(0,1,"-");
	    zeile = inStream.readLine(0);        // nächste zeile jetzt mit subprefixes
	    zeile = zeile.simplified();
	 }
	  
	 p = 0;
	 zlen = zeile.count();
	 while( 1 ) {                             // while zeile.count() != 0
	    z = "";                               // ein sPref_feld holen
	    while(1) {
	      if((zeile[p] == QChar(',')) | (zeile[p] == QChar(';')))
	        break;                            // p zeigt auf ','
	      z += zeile[p++]; 
	    }
	 
	  z.remove("=");
	  eCQz = pCQz;                           // mit primary laden
	  eITUz = pITUz;                         // falls keine abweichende CQz oder ITUz vorhanden ?  
          if((z.indexOf("(") == -1) & (z.indexOf("[") == -1)) {  
	     qy = "INSERT INTO tpref VALUES('";   // INSERT sub.prefix
             qy += z.simplified();                // subPrefix
             qy += "','"+Pref+"')";               // Primary_landes_prefix 
              // qDebug() << qy;
             query.exec(qy);                     
	  }
	  else {                                  // ABWEICHENDE zonen sind vorhanden
	    n1 = z.indexOf("(");                  // check auf CQz
	    if(n1 != -1) {                        // ja; 3H0(23)[42]
	      n2 = z.indexOf(")");                // separiere CQz, hole Prefix
	      sPref = z.left(n1);                 // subPrefix
	      eCQz = z.mid(n1+1,n2 -(n1+1));      // mit neuer CQz überschreiben
	      
	      n1 = z.indexOf("[");               // check auf ITUz
	      if(n1 != -1) {                     // ist auch vorhanden - 3H0(23)[42]
	        n2 = z.indexOf("]");
	        eITUz = z.mid(n1+1,n2 -(n1+1));  // mit neuer ITUz überschreiben
	      }
	    }
	    else {                               // keine CQz Änderung vorhanden
	      n1 = z.indexOf("[");               // check nur auf ITUz
	      if(n1 != -1) {
		n2 = z.indexOf("]");
		sPref = z.left(n1);
		eITUz = z.mid(n1+1,n2 -(n1+1));  // ITUz überschreiben
	      }
	    }
	     // --
	    if(eCQz.count() == 1) {              // prüfe zonen_str_len
	      s = "0"+eCQz;
	      eCQz = s;
	    }
	    
	    if(eITUz.count() == 1) {
	      s = "0"+eITUz;
	      eITUz = s;
	    }
	    
	    // - bilde subPrefix für abweichende CQ- und ITU_zonen                                    
	    if(Dxcc.compare("UA") == 0) {                    // European Russia
	      subPref = Dxcc+"-"+eCQz;                      // UA-16
	      i = subCountry.count();
	      subPref += subCountry.mid(i -3,2);            // UA-16KL
	    }
	    else 
	    if(Dxcc.compare("UA9") == 0) {                  // Asiatic Russia
	      subPref = Dxcc+"-"+eCQz;                      // UA9-18
	      i = subCountry.count();
	      subPref += subCountry.mid(i -3,2);            // UA-16TO
	    }
	    else 
	    if(Dxcc.compare("K") == 0) {                    // USA
	       subCountry.remove("USA");
	       subPref = Dxcc+"-"+eCQz+subCountry.mid(2,2); // K-04W0
	    }
	    else 
	    if(Dxcc.compare("VE") == 0) {                   // Canada
	       subPref = Dxcc+"-"+eCQz;                     // VE-04
	       i = subCountry.count();
	       subPref += subCountry.mid(i -2,2);           // VE-04
            }
	    else 
	    if(Dxcc.compare("VK") == 0) {                   // Australia
	       subPref = Dxcc+"-"+eCQz;                     // VK-30
	       subPref += subCountry.mid(2,3);              // VK-30QLD
	       subPref = subPref.simplified();              // (für VK-29WA 2-stellig )
            }
	    else
	    if(Dxcc.compare("LU") == 0) {                   // Argentinia
	       subPref = Dxcc+"-"+eCQz+subCountry.mid(2,3); // LU-13Rio
	    }
	    else
	    if(Dxcc.compare("BY") == 0) {                   // China
	       subPref = Dxcc+"-"+eCQz+subCountry.mid(2,3); // BY-24Hei
	       subPref = subPref.simplified();              // (für BY-24Ji 2-stellig )
	    }
	    else
	    if(Dxcc.compare("YB") == 0) {                   // Indonesia
	       subPref = Dxcc+"-"+eCQz+subCountry.mid(2,4); // YB-28Jaka  
	    }
	    else 
	    if(Dxcc.compare("PY") == 0) {                   // Brazil
	        subPref = Dxcc+"-"+eCQz;                    // PY-04
	        i = subCountry.count();
	        subPref += subCountry.mid(i -3,2);          // PY-04
	    }
	    else {                                          // default
	       subPref = Dxcc+"-"+eCQz+subCountry.mid(2,3); // wie BY-24Hei
	       subPref = subPref.simplified();              // (für BY-24Ji 2-stellig ) 
	    }
	    // -  
	    a = Country + subCountry;                    // falls subPref noch nicht vorhanden INSERT 
	    qy = "SELECT ldxcc FROM tla WHERE la='"+subPref+"'";
	    query.exec(qy);                              // falls nicht vorhanden anlegen
	    if( !query.size() ) {                        // Primary für subPref mit abweichenden CQz
               qy = "INSERT INTO tla VALUES ('"+subPref; // K-04W0 usw
               qy += "',";
               field.setValue(a);                           // la_name  zB United States K-04W0
               qy += db.driver()->formatValue(field,false); // Name - escape_field  - Dumont D'Urville Base
	       
               qy += ",'"+Cont;                             // NA
               qy += "','"+eITUz;                           //  abweichende UTIz
               qy += "','"+eCQz;                            // abweichende CQz
               qy += "','"+TimeOfset;                       // time_index
               qy += "','"+Br;                              // Br
               qy += "','"+Lg;                              // Lg
               qy += "','"+Pref;                            // K dxcc
               qy += "','','','','')";                      // lawd,lwpx,adif,notiz
               query.exec(qy);  
	    }
	    qy = "INSERT INTO tpref VALUES('";              //
            qy += sPref;                                    // space entfernen      ??
            qy += "','"+subPref+"')";                       // + ref_landes_prefix  ??
            query.exec(qy);                                 // SUb_prefix in db eintragen 
         }
         if((zeile[p] == QChar(';')) || (++p == zlen))
	   break;                                           // Zeien_Ende erreicht
	}                                                   // while zeile != 0
	//if(Pref.compare("CM") == 0) return;               // TEST       
	if(zeile[p] == QChar(';'))                          // falls subPrefixes_string_Ende
	  break;                                            // break für nächsten string
        }                                           //--- while subPrefixes_str != ';', - nächste Zeile lesen
    }
}
