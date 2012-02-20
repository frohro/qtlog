/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 20.11.2008 / 02.06.09
********************************************************************** */

#include <QtGui>
#include <QtSql>
#include <QString>
#include "adif.h"
adif::adif() {
    state = 0;
}
adif::~adif() {
}

void adif::createTranslatTable()
{
   QString df, an;
   QSqlQuery query;
   qy = "SELECT dbfield,adifnam FROM refnamen WHERE adifnam != '' ORDER BY idn";
   query.exec(qy);
   while(query.next()) {                      
       n = 0;
       df = query.value(n++).toString();      
       an = query.value(n++).toString();      
       toAdif.insert(df,an);                  
   }
}

void adif::clearTranslateTable()
{
     toAdif.clear();
}

QString adif::getAdifName(QString dbfield)
{
    QHash<QString, QString>::const_iterator i = toAdif.find(dbfield);
    if(i == toAdif.end())
       return "?";                           
    return i.value();                        
}


void adif::setModeTabel()
{
  QString sm, mym, am;
    QSqlQuery query;
    qy = "SELECT dbfield,refnam,adifnam FROM refnamen WHERE type='6'";
    query.exec(qy);
    while(query.next()) {
       n = 0;
       sm = query.value(n++).toString();         
       mym = query.value(n++).toString();        
       am = query.value(n++).toString();         
       toAdifMode.insert(sm,am);                 
       toAdiMode.insert(mym,am);                 
    }
}


QString adif::getAdifMode(QString sysmode)
{
    QHash<QString, QString>::const_iterator i = toAdifMode.find(sysmode);
    if(i == toAdifMode.end())
       return "";                            
    return i.value();                        
}


QString adif::getAdiMode(QString mymode)
{
    QHash<QString, QString>::const_iterator i = toAdifMode.find(mymode);
    if(i == toAdiMode.end())
       return "";                            
    return i.value();                        
}

void adif::clearModeTable()
{
     toAdifMode.clear();
     toAdiMode.clear();
}



void adif::setBandTable()
{
  QString sb, mb, ab;
    QSqlQuery query;
    qy = "SELECT band,mband,aband FROM wband WHERE work != 0";
    query.exec(qy);
    while(query.next()) {
       n = 0;
       sb = query.value(n++).toString();         
       mb = query.value(n++).toString();         
       ab = query.value(n++).toString();         
       toAdifBand.insert(sb,ab);                 
       toAdiBand.insert(mb,ab);                  
       toSysBand.insert(ab,sb);                  
       tosysBand.insert(mb,sb);                  
    }
}


void adif::clearBandTable()
{
     toAdifBand.clear();
     toAdiBand.clear();
     toSysBand.clear();
     tosysBand.clear();
}


QString adif::getAdifBand(QString sysband)
{
    QHash<QString, QString>::const_iterator i = toAdifBand.find(sysband);
    if(i == toAdifBand.end())
       return "?";                           
    return i.value();                        
}


QString adif::getAdiBand(QString myband)
{
    QHash<QString, QString>::const_iterator i = toAdiBand.find(myband);
    if(i == toAdiBand.end())
       return "?";                           
    return i.value();                        
}


QString adif::getsysBand(QString myband)
{
    QHash<QString, QString>::const_iterator i = tosysBand.find(myband);
    if(i == tosysBand.end())
       return "?";                           
    return i.value();                        
}


void adif::openRefListQso(int rows)
{
   qsoFields = rows;
   adh = new ADIFHEAD[rows];         
   state = 0;
   rowcnt = 0;                       
}

void adif::openRefListQsl(int rows)
{
   qslFields = rows;
   adhq = new ADIFHEAD[rows];         
}

void adif::openRefListAwd(int rows)
{
   awdFields = rows;
   adhw = new ADIFHEAD[rows];         
}

void adif::closeRefList()
{
   delete adh;                        
   delete adhq;
   delete adhw;
}




void adif::loadRefList()
{
    QSqlQuery query;
    qy = "SELECT dbfield,refnam,adifnam FROM refnamen WHERE type = '2' ORDER BY idn";
    query.exec(qy);
    openRefListQso(query.size());                           
    i = 0;
    while(query.next()) {                                   
       n = 0;
       adh[i].pos = i;                                               
       strcpy(adh[i].field,query.value(n++).toString().toAscii());   
       strcpy(adh[i].refnam,query.value(n++).toString().toAscii());  
       strcpy(adh[i].adifnam,query.value(n++).toString().toAscii()); 
       strcpy(adh[i].value,"");                            
       i++;
    }
    
    
    qy = "SELECT dbfield,refnam,adifnam FROM refnamen WHERE type = '3' OR type = '4' ORDER BY idn";
    query.exec(qy);
    openRefListQsl(query.size());                           
    i = 0;
    while(query.next()) {                                   
       n = 0; 
       adhq[i].pos = i;                                               
       strcpy(adhq[i].field,query.value(n++).toString().toAscii());   
       strcpy(adhq[i].refnam,query.value(n++).toString().toAscii());  
       strcpy(adhq[i].adifnam,query.value(n++).toString().toAscii()); 
       strcpy(adhq[i].value,"");                            
       i++;
    }
    
    qy = "SELECT atype,adiftype FROM wawdlist WHERE aset != '0' ORDER BY aset";
    query.exec(qy);
    openRefListAwd(query.size());                           
    i = 0;
    while(query.next()) {                                   
       n = 0;
       adhw[i].pos = i;                                     
       s = query.value(n++).toString();
       strcpy(adhw[i].field,s.toAscii());                   
       strcpy(adhw[i].refnam,"");                           
       s = query.value(n++).toString();
       strcpy(adhw[i].adifnam,s.toAscii());                 
       strcpy(adhw[i].value,"");                            
       i++;
    }
}


char * adif::getdbName(char * nam)
{
    int idx = 0;
    while( idx != qsoFields ) {
      if(strcmp(adh[idx].adifnam,nam) == 0) break;
         idx++;
    }
    if(idx != qsoFields)                
      return adh[idx].field;
    else 
      return (char *)"";                
}


char * adif::getdbNameAwd(char *nam)
{
   int idx = 0;
    while( idx != awdFields ) {
      if(strcmp(adhw[idx].adifnam,nam) == 0) break;
         idx++;
    }
    if(idx != awdFields)                
      return adhw[idx].field;
    else 
      return (char *)"";                
}


QString adif::getValueAwd(char *nam)
{
   int idx = 0;
    while( idx != awdFields ) {
      if(strcmp(adhw[idx].adifnam,nam) == 0) break;
         idx++;
    }
    if(idx != awdFields)                
      return adhw[idx].value;
    else 
      return (char *)"";                
}


char * adif::getdbNameQsl(char * nam)
{
    int idx = 0;
    while( idx != qslFields ) {
      if(strcmp(adhq[idx].adifnam,nam) == 0) break;
         idx++;
    }
    if(idx != qslFields)                
      return adhq[idx].field;
    else 
      return (char *)"";                
}

int adif::getla(QString str)
{
  int i,n;
    n = 0;
    i = 0;
    QSqlQuery query;                                       
    qy = "SELECT pref FROM tpref WHERE spref='"+str+"'";   
    query.exec(qy);
    i = query.size();                                      
    if( i ) {                                              
       query.next();
       s = query.value(0).toString();
       qy = "SELECT lcn,cqz,ituz,ldxcc FROM tla WHERE ldxcc='"+s+"'";
       query.exec(qy);
       query.next();                                    
       Lcn  = query.value(0).toString();                
       Cqz  = query.value(1).toString();                
       Ituz = query.value(2).toString();                
       Dxcc = query.value(3).toString();                
      }
      return i;
}


QString adif::getValue(char * fname)
{
     int idx = 0;
     while( idx != qsoFields ) {
       if(strcmp(adh[idx].field,fname) == 0) break;
          idx++;
     }
     return adh[idx].value;
}


void adif::setValue(char *nam, QString v)
{
     int idx = 0;
     while( idx != qsoFields ) {
       if(strcmp(adh[idx].field,nam) == 0) break;
          idx++;
     }
     strcpy(adh[idx].value,v.toAscii());
}


void adif::setValueQsl(char *nam, QString v)
{
     int idx = 0;
     while( idx != qsoFields ) {
       if(strcmp(adhq[idx].field,nam) == 0) break;
          idx++;
     }
     strcpy(adhq[idx].value,v.toAscii());
}


void adif::setValueAwd(char *nam, QString v)
{
     int idx = 0;
     while( idx != awdFields ) {
       if(strcmp(adhw[idx].field,nam) == 0) break;
          idx++;
     }
     strcpy(adhw[idx].value,v.toAscii());
}


QString adif::getSysBand(QString aband)
{
    QHash<QString, QString>::const_iterator i = toSysBand.find(aband);
    if(i == toSysBand.end())
       return "?";                           
    return i.value();                        
}


QString adif:: getDayLastDown()
{
  return dayLastDown;                        
}


void adif::setdateHeute(QString sDay)
{
   dheute = sDay;
}

QString adif::getdheute()
{
  return dheute;
}


void adif::setLastDownNr(QString s)
{
     nLastDown = s;                   
}


int adif::getState()
{
    return state;                    
}


QString adif::getErrorInfo()
{
   return Error;                     
}

int adif::getNewqslCnt()
{
   return nqsl;                       
}




//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void adif::readQslReport(QString log, QString fpath, QString qslmode)
{
 QString val, token;
 QString Call, qsoDate;
 QString t, z, band;
 int idx, n, oid, sekcnt, vl, len;
 bool ok;
 char buf[255];
    nqsl = 0;
    sekcnt = 10;                                      
    oid = 0;
    QSqlQuery query;
    QFile datei( fpath );
    datei.open(QIODevice::ReadOnly);
    rowcnt = 0;
    
    QTextStream istream( &datei);
    while( istream.atEnd() != true ) {                 
        len = 0;
        while(len == 0) {                              
          t = istream.readLine(0);
          len = t.count();
          if(len != 0) break;                          
        }
        idx = 0;
        while(len != idx)                              
          if(t[idx++] == QChar('<')) break;            
          if(len == idx) {                             
            if(t.indexOf("Fatal error:") != -1) {
              Error = "\nLOTW : General database error.\nToo many users (task limit)";
              state = 9;
              return;
            }
         }
        z = "";
        if(len != idx) {                               
          while(t[idx] != QChar(':') && t[idx] != QChar('>')) z += t[idx++]; 
          if(t[idx] == QChar('>')) {                   
            z = z.toUpper();                           
            if(z.compare("EOH") == 0) break;           
            if(z.compare("HTML") == 0) {               
               Error = "\nError message from LoTW  !\nNo QSLreport.\nBecause Password or Username mistake.";
               state = 9;
               return;
            }
          }
          else {                                           
            val = "";
            idx++;                                         
            if(z.compare("APP_LoTW_LASTQSL") == 0) {       
              while(t[idx] != QChar('>')) val += t[idx++]; 
              dayLastDown = val;                           
              vl = val.toInt(&ok,10);
              idx++;
              val = "";
              n = vl;                                     
              
              
            }
            if(z.compare("PROGRAMID") == 0) {             
              while(t[idx] != QChar('>')) val += t[idx++]; 
              vl = val.toInt(&ok,10);
              idx++;
              val = "";
              n = vl;                                    
              while(n-- != 0) val += t[idx++];           
              if(qslmode.compare("eqsl") == 0) {
                 if(val.compare("eQSL.cc DownloadInBox") != 0) {
                    Error += "\nFEHLER !\nDie Datei ist nicht:  'eQSL.cc DownloadInBox'";
                    state = 1;       
                    return;
                 }
              }
             
             
             
            }
          }
          
       }
    }                                                    
    
    
    Error = "";
    rowcnt = 0;                                           
    while(istream.atEnd() != true) {                      
        i = 0;
        while(i != qsoFields) strcpy(adh[i++].value,"");  
        i = 0;
        while(i != qslFields) strcpy(adhq[i++].value,""); 
        i = 0;
        while(i != awdFields) strcpy(adhw[i++].value,""); 
        z = istream.readLine(0);
                                
        while(z.count() == 0) {                 
           if(istream.atEnd() == TRUE) break;   
           z = istream.readLine(0);             
        }
        if(z.compare("<EOF>") == 0) break;      
        if(z.compare("<APP_LoTW_EOF>") == 0) break; 
     
     
        len = z.count();                        
        vl = 0;
        Lcn  = "";
        Cqz  = "";
        Ituz = "";
        Dxcc = "";
        t = "";
        n = 0;
        idx = 0;                                    
        while( 1 ) {                                
            while(len != idx) {
               if(z[idx++] == QChar('<')) break;    
            }
            if(len == idx) {                        
                z = istream.readLine(0);  
                len = z.count();
                if(len == 0)                        
                   z = istream.readLine(0);         
                len = z.count();
                idx = 0;
                while(z[idx] != QChar('<')) idx++;  
                idx++; 
            } 
            t = "";
            while(z[idx] != QChar(':') && z[idx] != QChar('>')) t += z[idx++]; 
            if(z[idx] == QChar('>')) {               
                t = t.toUpper();                     
                if((t.compare("EOR") == 0) || (t.compare("EOF") == 0)) 
                  break;
            }
            if(t.compare("EOF") == 0) break;          
            if(t.compare("APP_LoTW_EOF") == 0) break; 
            token = t;                                
            t = "";
            idx++;
            while(z[idx] != QChar('>') && z[idx] != QChar(':')) t += z[idx++]; 
            vl = t.toInt(&ok,10);                    
            
            if(z[idx] == QChar(':')) {               
              idx++;                                 
              idx++;                                 
            }
            idx++;                                   
            t = "";
            n = vl;                                  
            while(n-- != 0) {                        
              if(z[idx] == QChar('<')) break;        
               t += z[idx++];                        
            }
            if(z[idx] == QChar('<')) {               
               vl = t.count(); 
            }
            val = t;
                                                     
            token = token.toUpper();                 
            strcpy(buf,token.toAscii());             
            strcpy(buf,getdbName(buf));              
            
            if(strlen(buf) != 0) {                   
                if(strcmp(buf,"rufz") == 0) {        
                  n = val.lastIndexOf("/",-1);       
                  if(n != -1) {                      
                   if( n <= 3 ) {  
                      s = val.left(n);               
                      s += "/";                      
                      val.remove(0,n +1);            
                      setValue((char *)"cept",s);    
                    }
                   }
                   setValue((char *)"rufz",val);     
                   Call = val;                       
                }
                else
                 if(strcmp(buf,"dxcc") == 0) {       
                   n = val.toInt(&ok,10);            
                   if(ok == true )                   
                     qy = "SELECT lcn,cqz,ituz,ldxcc FROM tla WHERE adif="+s.setNum(n);
                   else
                     qy = "SELECT lcn,cqz,ituz,ldxcc FROM tla WHERE ldxcc='"+val+"'";
                   query.exec(qy);
                   if(query.size() != 0 ) {             
                      query.next();
                      Lcn  = query.value(0).toString(); 
                      Cqz  = query.value(1).toString(); 
                      Ituz = query.value(2).toString(); 
                      Dxcc = query.value(3).toString(); 
                      setValue(buf,Dxcc);               
                   }
                   else {                               
                     Error += " Call "+getValue((char *)"rufz");
                     Error += " dxcc_prefix < "+val;
                     Error += " > nicht gefunden\n";    
                     state = 1;                         
                   }
                 }
                else
                 if(strcmp(buf,"band") == 0) {        
                   val = getSysBand(val);             
                   setValue(buf,val);                 
                   band = val;                        
                 }
                else
                 if(strcmp(buf,"day") == 0) {         
                    setValue(buf,val);                
                    qsoDate = val;                    
                  }
                 else 
                  if(strcmp(buf,"btime") == 0) {      
                    if(vl != 6) {                     
                      sekcnt++;                       
                      if(sekcnt == 60)                
                        sekcnt = 10;
                      val.append(s.setNum(sekcnt));   
                    }
                    setValue(buf,val);                
                  }
                else
                  if(strcmp(buf,"etime") == 0) {       
                     if(vl != 6) {
                       sekcnt++;                       
                       if(sekcnt == 60)
                         sekcnt = 10;
                       val.append(s.setNum(sekcnt));   
                     }
                     setValue(buf,val);                
                  }
                else {
                 setValue(buf,val);                    
               }
             }
             else {                                    
              strcpy(buf,token.toAscii());             
              strcpy(buf,getdbNameQsl(buf));           
              if(strlen(buf) != 0) {      
                 setValueQsl(buf,val);                 
              }
              else {                                   
                if(token.compare("US-CA") == 0) {      
                   token = "STATE";
                   //token = s;                        
                }
                strcpy(buf,token.toAscii());           
                strcpy(buf,getdbNameAwd(buf));         
                if(strlen(buf) != 0) {
                  setValueAwd(buf,val);                
                }
                else {
                  if(token.compare("STATION_CALLSIGN") != 0)
                    ;
                  else
                  if(token.compare("APP_LOTW_OWNCALL") != 0)
                    ;
                  else {
                    Error += " Call "+getValue((char *)"rufz");
                    Error += " "+token;                  
                    Error += " text < "+val; 
                    Error += " > nicht gefunden\n";      
                    state = 1;                           
                  }
                }
              }
           }
       }   
//--   END_adif_row_loop    - ein Datensatz bis < eor > übernommen ---------------------

/*
        
         n = 0;   
         i = 0;
         qDebug() << "\n";
         while(i != qsoFields) {
            qDebug() << adh[i].pos <<  adh[i].field << adh[i].adifnam << adh[i].value;
            i++;
         }
         qDebug() << "\n";
         n = 0;  
         i = 0;
         while(i != qslFields) {
           qDebug() << adhq[i].pos <<  adhq[i].field << adhq[i].adifnam << adhq[i].value;
            i++;
         }
         qDebug() << "\n";
         n = 0;  
         i = 0;
         while(i != awdFields) {
           qDebug() << adhw[i].pos <<  adhw[i].field << adhw[i].adifnam << adhw[i].value;
           i++;
         }

*/
        setQslNewQsoStatus( log, Call, qslmode );
        rowcnt++;                      
   }  
   datei.close();
}

void adif::setQslNewQsoStatus(QString log, QString Call, QString qslmode)
{
   QString t, val, vflg, qslr, qsls, qyerr;
   int id, n;
       if(qslmode.compare("lotw") == 0)
          qy = "SELECT id,dxcc,lotqslr,lotqsls";
       else
         qy = "SELECT id,dxcc,eqslr,eqsls";
       qy += " FROM "+log+"om LEFT JOIN ("+log+"qsl,"+log+") ON (omid=oid AND qsoid=id)";
       qy += " WHERE band='"+getValue((char *)"band");        
       qy += "' AND mode='"+getValue((char *)"mode");
       qy += "' AND rufz='"+getValue((char *)"rufz");
       qy += "' AND day='"+getValue((char *)"day");
       qy += "' AND btime BETWEEN '";
       t = getValue((char *)"btime");
       QTime th = th.fromString(t,"hhmmss");
       QTime nh;
       nh = th.addSecs(-300);            
       val = nh.toString("hh:mm:ss");
       qy += val;
       qy += "' AND '";
       nh = th.addSecs(300);             
       val = nh.toString("hh:mm:ss");
       qy += val+"'";
       n = 0;
       qyerr = qy;        
       QSqlQuery query;
       query.exec(qy);
       if( query.size() == 1 ) {                            
         query.next();
         id = query.value(n++).toString().toInt();
         Dxcc = query.value(n++).toString();
         qslr = query.value(n++).toString();                
         qsls = query.value(n++).toString();                
         if(qslr.compare("Y") == 0) {                       
            qy = "UPDATE "+log+"qsl SET sel='"+nLastDown;   
            qy += "' WHERE qsoid="+t.setNum(id);
            query.exec(qy);   
         }
         else {                                          
          if(qslmode.compare("lotw") == 0) {             
            vflg = "L";                                  
            qy = "UPDATE "+log+"qsl SET lotqslr='";
            qy += getValue((char *)"qslr");
            qy += "',lotqsls='Y";
            qy += "',lotqslrd='";
            qy += getValue((char *)"qslrd");
          }
          else {                                         
            vflg = "E";                                  
            qy = "UPDATE "+log+"qsl SET eqslr='Y";
            if((qsls.compare("U") == 0) || (qsls.compare("") == 0))  
              qy += "',eqsls='Y";                        
            qy += "',eqslrd='"+getdheute();              
          }
          qy += "',sel='"+nLastDown;
          qy += "' WHERE qsoid="+t.setNum(id);
          query.exec(qy);                   
          
          qy = "SELECT qslRvia FROM "+log+" WHERE id="+t.setNum(id);
          query.exec(qy);
          query.next();
          s = query.value(0).toString();   
          if(s.indexOf(vflg) == -1)        
            s += vflg;                     
          qy = "UPDATE "+log+" SET qslRvia='"+s;
          t = getValue((char *)"loc");     
          if(t.count()) {                  
            qy += "',loc='";
            qy += t;                       
          }
          t = getValue((char *)"qslmsg");
          qy += "',qslmsg='"+t;
          qy += "' WHERE id="+t.setNum(id);
          query.exec(qy);                  
         
          s = getValueAwd((char *)"IOTA");
          if(s.count()) {                  
            qy = "SELECT awtype FROM "+log+"awd WHERE qid="+t.setNum(id)+" AND awtype='";
            qy += s+"'"; 
            query.exec(qy);
            if(!query.size()) {            
               qy = "INSERT INTO "+log+"awd VALUES (0,"+t.setNum(id);
               qy += ",rufz='"+Call+"',IOTA='";
               qy += s;
               qy += "')";
               query.exec(qy);             
            }
          }
          s = getValueAwd((char *)"STATE");
          if(s.count()) {                   
            qy = "SELECT awtype FROM "+log+"awd WHERE qid="+t.setNum(id)+" AND awtype='";
            qy += s+"'"; 
            query.exec(qy);
            if(!query.size()) {             
               qy = "INSERT INTO "+log+"awd VALUES (0,"+t.setNum(id);
               qy += ",rufz='"+Call+"',STATE='";
               qy += s;
               qy += "')";
               query.exec(qy);              
             }
          }
         
          t = getValue((char *)"mode");                      
          qy = "SELECT cntb FROM "+log+"dx ";
          qy += "WHERE dxpref='"+Dxcc+"' AND mode='"+t+"'";  
          query.exec(qy);
          n = query.size();                                  
          if( n ) {
            query.next();
            i = query.value(0).toInt();                      
            qy = "UPDATE "+log+"dx SET cntb=";               
            qy += s.setNum(++i);
            qy += ", b"+getValue((char*)"band");     
            qy += "='x' WHERE dxpref='"+Dxcc+"' AND mode='"+t+"'";
            query.exec(qy);                          
          }
          nqsl++;                                    
       }
     }
     else {                                          
       Error += " qso: Call "+getValue((char *)"rufz");
       Error += " "+getValue((char *)"day");
       Error += " not found\n";
       state = 2;                          
     }
}
