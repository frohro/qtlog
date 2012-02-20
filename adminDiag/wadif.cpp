/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007, 20.11.08, 08.07.09
********************************************************************** */

#include <QtGui>
#include <QtSql>
#include <QString>
#include "wadif.h"
#include "../qtlogDiag/version.h"
wadif::wadif() {
  state = 0;
}
wadif::~wadif() {
}

void wadif::openRefListQso(int rows)
{
   qsoFields = rows;
   adh = new ADIFHEAD[rows];         
   state = 0;
   count = 0;
}

void wadif::openRefListQsl(int rows)
{
   qslFields = rows;
   adhq = new ADIFHEAD[rows];         
}

void wadif::openRefListAwd(int rows)
{
   awdFields = rows;
   adhw = new ADIFHEAD[rows];         
}

void wadif::closeRefLists()
{
   delete adh;                      
   delete adhq;
   delete adhw;
}


int wadif::getState()
{
    return state;                   
}


QString wadif::getErrorInfo()
{
  return Error;
}


int wadif::getCount()
{
   return count;
}




void wadif::loadRefList()
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
    
    
    qy = "SELECT dbfield,refnam,adifnam FROM refnamen WHERE type='3' OR type ='4' ORDER BY idn";
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
}
void wadif::loadRefListAwd()
{
    
    QSqlQuery query;
    qy = "SELECT atype,adiftype FROM wawdlist WHERE aset != '0' ORDER BY aset";
    query.exec(qy);
    openRefListAwd(query.size());                                                
    i = 0;
    while(query.next()) {                                                        
       n = 0;                                                                    
       adhw[i].pos = i;                                                          
       strcpy(adhw[i].field, query.value(n++).toString().toAscii().toUpper());   
       strcpy(adhw[i].refnam,"");                                                
       strcpy(adhw[i].adifnam, query.value(n++).toString().toAscii().toUpper()); 
       strcpy(adhq[i].value,"");                                                 
       i++;
    }
}


char * wadif::getAdifName(char * nam)
{
    int idx = 0;
    while( idx != qsoFields ) {
      if(strcmp(adh[idx].field,nam) == 0) break;
         idx++;
    }
    return adh[idx].adifnam;
}


char * wadif::getdbName(char * nam)
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


char * wadif::getdbNameQsl(char * nam)
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


char * wadif::getdbNameAwd(char *nam)
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


void wadif::setValue(char *nam, QString v)
{
     int idx = 0;
     while( idx != qsoFields ) {
       if(strcmp(adh[idx].field,nam) == 0) break;
          idx++;
     }
     strcpy(adh[idx].value,v.toAscii());
}




void wadif::addQslFlag(char *nam, QString v)
{
  char sbuf[12];
  int i;
  int idx = 0;
  
     while( idx != qsoFields ) {
       if(strcmp(adh[idx].field,nam) == 0) break;
          idx++;
     }
     strcpy(sbuf,adh[idx].value);
     i = 0;
     while(i != 10) {
       if(sbuf[i] == (QChar)v[0]) break;
       if(i++ == 10) break;
     }
     if(i != 10) return;                    
     else
       strcat(adh[idx].value,v.toAscii());  
}


void wadif::setValueQsl(char *nam, QString v)
{
     int idx = 0;
     while( idx != qsoFields ) {
       if(strcmp(adhq[idx].field,nam) == 0) break;
          idx++;
     }
     strcpy(adhq[idx].value,v.toAscii());
}


void wadif::setValueAwd(char *nam, QString v)
{
     int idx = 0;
     while( idx != awdFields ) {
       if(strcmp(adhw[idx].field,nam) == 0) break;
          idx++;
     }
     strcpy(adhw[idx].value,v.toAscii());
}


QString wadif::getValue(char * fname)
{
     int idx = 0;
     while( idx != qsoFields ) {
       if(strcmp(adh[idx].field,fname) == 0) break;
          idx++;
     }
     return adh[idx].value;
}


QString wadif::getValueQsl(char *nam)
{
    int idx = 0;
    while( idx != qslFields ) {
      if(strcmp(adhq[idx].field,nam) == 0) break;
         idx++;
    }
    return adhq[idx].value;
}




void wadif::setBandTable()
{
  QString sb, ab;
    QSqlQuery query;
    qy = "SELECT band,aband FROM wband WHERE work != 0";
    query.exec(qy);
    while(query.next()) {                       //
       n = 0;
       sb = query.value(n++).toString();         
       ab = query.value(n++).toString();         
       toAdifBand.insert(sb,ab);                 
       toSysBand.insert(ab,sb);                  
    }
}

void wadif::clearBandHash()
{
     toAdifBand.clear();
     toSysBand.clear();
}


QString wadif::getAdifBand(QString sysband)
{
    QHash<QString, QString>::const_iterator i = toAdifBand.find(sysband);
    if(i == toAdifBand.end())
       return "?";                           
    return i.value();                        
}


QString wadif::getSysBand(QString aband)
{
    QHash<QString, QString>::const_iterator i = toSysBand.find(aband);
    if(i == toSysBand.end())
       return "?";                           
    return i.value();                        
}




void wadif::createAdifTableHeader()
{
 QString df, an, q;
 int n;
   QSqlQuery query; 
   qy = "SELECT dbfield,adifnam FROM refnamen WHERE adifnam != '' AND type='2' ORDER BY idn";
   query.exec(qy);
   query.next();                                
   n = 0;
   tmp = "";                                    
   df = query.value(n++).toString();            
   an = query.value(n++).toString();            
   tmp += df;                                   
   toAdif.insert(df,an);                        
   while(query.next()) {                        
       n = 0;
       tmp += ",";
       df = query.value(n++).toString();        
       an = query.value(n++).toString();        
       tmp += df; 
       toAdif.insert(df,an);                    
   }
   n = 0;                                       
   qy = "SELECT dbfield,adifnam FROM refnamen WHERE adifnam != '' AND type='3' ORDER BY idn";
   query.exec(qy);
   while(query.next()) {
      n = 0;
      etmp += ",";
      df = query.value(n++).toString();         
      an = query.value(n++).toString();         
      toAdif.insert(df,an);                     
      etmp += df;
   }
}


void wadif::loadAdifAwdTable(QString awd,QString addadif)
{
    toAdifCustom.insert(awd,addadif);
}


QString wadif::getAdifName(QString dbfield)
{
    QHash<QString, QString>::const_iterator i = toAdif.find(dbfield);
    if(i == toAdif.end())
       return "?";                           
    return i.value();                        
}


QString wadif::getAdifCustom(QString custom)
{
    QHash<QString, QString>::const_iterator i = toAdifCustom.find(custom);
    if(i == toAdifCustom.end())
       return "?";                           
    return i.value();                        
}


void wadif::clearExpHash()
{
    toAdif.clear();
    toAdifCustom.clear();
}




void wadif::doExport(QString log, QString fpath, int lpref)
{
 QString p, dfeld;
 int col, idx;
 QString Call, Name, Cept, val, v, id;
    state = 0;
    p = fpath+log+".adi";
    QFile file(p);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
         state = 2;
        return;
    }
    QTextStream out(&file);
    n = lpref;                                       
    QSqlQuery query;
    qy = "SELECT id,cept,"+tmp+etmp;             
    qy += " FROM "+log+"om LEFT JOIN ("+log+"qsl,"+log+") ON (omid=oid AND qsoid=id)";
    qy += " ORDER BY day";
    qDebug() << qy;
    
    query.exec(qy);
    n = query.size();                                
    col = query.record().count();                    
    
    p = "** ADIF V2.0 file exportet by QtLog V1.5.07 ** LOG:"+log+" - logs:"+s.setNum(n)+"  date:";
    QDate d = QDate::currentDate();                  
    p += s.setNum(d.year());
    p += "-";
    p += s.setNum(d.month());
    p += "-";
    p += s.setNum(d.day());
    p += "\n";
    p += "<PROGRAMID:5>QtLog";
    p += "\n";
    p += "<eoh>\n";
    out << p;
    count = 0;
    while(query.next() ) {                           
      idx = 0;
      p = "";
      while( idx != col ) {                          
       n = 0;                                        
       dfeld = query.record().fieldName(idx);        
       val = query.value(idx).toString();            
       if(dfeld.compare("id") == 0) {                
          id = val;
       }
       else 
        if(dfeld.compare("cept") == 0) { 
           Cept = val;                                
        }
       else
        if(dfeld.compare("rufz") == 0) {              
          Call = val;
          if(Cept.count() != 0) {
              val = Cept+Call;                        
              p += "<CALL:"+v.setNum(val.count())+">"+val;
           }
           else 
             p += "<CALL:"+v.setNum(Call.count())+">"+Call;
        }
       else 
        if(dfeld.compare("day") == 0) {              
            val.remove(QChar('-'), Qt::CaseInsensitive);
            p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+":d>"+val;
        }
       else 
        if((dfeld.compare("qslrd") == 0) || (dfeld.compare("qslsd") == 0)) {  
           val.remove(QChar('-'), Qt::CaseInsensitive);
           if(val.count() != 0) 
             p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+":d>"+val;
        }
       else 
        if(dfeld.compare("btime") == 0) {            
           val.remove(QChar(':'), Qt::CaseInsensitive);
           p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val;
        }
       else
        if(dfeld.compare("etime") == 0) {            
           val.remove(QChar(':'), Qt::CaseInsensitive);
           p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val;
        }
       else
        if(dfeld.compare("band") == 0) {
           val = getAdifBand(val);                  
           p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val;
        }
        else
         if(dfeld.compare("ntext") == 0) {
           if(val.count() != 0) {
             val = val.simplified();                
             p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val; 
           }
        }
       else
         if(dfeld.compare("ktext") == 0) {
           if(val.count() != 0) {
             val = val.simplified();                
             p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val; 
           }
        }
        else
         if(dfeld.compare("dxcc") == 0) {
            if(lpref) {                             
               if(val.count() != 0) {               
                 QSqlQuery query;
                 qy = "SELECT adif FROM tla WHERE ldxcc='"+val+"'";
                 query.exec(qy);
                 query.next(); 
                 val = query.value(0).toString(); 
               }
            }
            p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val;
        }
       else
        if((dfeld.compare("lotqslrd") == 0) || (dfeld.compare("lotqslsd") == 0)) {
            val.remove(QChar('-'), Qt::CaseInsensitive);
            if(val.count() != 0)
              p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+":d>"+val;
        }
       else {
         if(val.count() != 0) {                     
           p += " <"+getAdifName(dfeld)+":"+v.setNum(val.count())+">"+val; 
         }
       }
       idx++;                                      
     }
//--
    
    qy = "SELECT awtype,awkenner FROM "+log+"awd WHERE qid="+id;
    QSqlQuery query(qy);
    n = query.size();
    while(query.next()) {
       n = 0;
       dfeld = query.value(n++).toString();         
       val = query.value(n++).toString();           
       p += " <"+getAdifCustom(dfeld)+":"+v.setNum(val.count())+">"+val; 
    }                                               
    p += " <eor>\n";                                
    out << p;                                       
    count++;
  }                                                 
  out << "<EOF>\n";
  file.close();
}





void wadif::doImport(QString log,QString fpath, int qslmode, QString rig, int hId)
{
 QString val, token, prog;
 QString Call, qsoDate;
 QString t, z;
 int idx, n, oid, id, sekcnt, vl, len;
 bool ok;
 char buf[255];
    sekcnt = 10;                                      
    oid = 0;
    QSqlQuery query;
    QFile datei( fpath );
    if(! datei.open(QIODevice::ReadOnly)) {
        state = 1;
        return;                                       
    }
    count = 0;
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
        z = "";
        if(len != idx) {                              
           while(t[idx] != QChar(':') && t[idx] != QChar('>')) z += t[idx++]; 
           if(t[idx] == QChar('>')) {                 
              z = z.toUpper();                        
              if(z.compare("EOH") == 0) break;        
           }
           else {                                     
              val = "";                               
              idx++;
              if(z.compare("PROGRAMID") == 0)         
                while(t[idx] != QChar('>')) val += t[idx++]; 
              vl = val.toInt(&ok,10);
              idx++;                                  
              val = "";
              n = vl;                                 
              while(n-- != 0) val += t[idx++];        
              prog = val;
           }
        }
    }
    
    
    
    Error = "";
    count = 0;                                            
    while(istream.atEnd() == FALSE) {                     
        i = 0;
        while(i != qsoFields) strcpy(adh[i++].value,"");  
        i = 0;
        while(i != qslFields) strcpy(adhq[i++].value,""); 
        i = 0;
        while(i != awdFields) strcpy(adhw[i++].value,""); 
        z = istream.readLine(0); 
                                 
        if(z.compare("<EOF>") == 0) break;      
        while(z.count() == 0) {                 
           if(istream.atEnd() == TRUE) break;   
           z = istream.readLine(0);             
        }
        if(istream.atEnd() == TRUE) break;      
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
            while(n-- != 0) t += z[idx++];           
            val = t;
                                                     
            token = token.toUpper();                 
            strcpy(buf,token.toAscii());             
            strcpy(buf,getdbName(buf));              
            
            if(strlen(buf) != 0) {                
                if(strcmp(buf,"rufz") == 0) {     
                  n = val.indexOf("/");           
                  if(n != -1) {                   
                    if( n <= 4 ) {                
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
                   val = val.toUpper();
                   val = getSysBand(val);              
                   setValue(buf,val);                  
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
                else 
                  
                  if(strcmp(buf,"qslr") == 0) {              
		     if(val.compare("N") == 0)
		       val = "R";
		     else {
			if(val.compare("Y") == 0)            
			  addQslFlag((char *)"qslRvia","B"); 
		     }
                     setValue(buf,val);                      
	          }
                else
                  if(strcmp(buf,"qsls") == 0) {              
		     if( !qslmode )                          
		       val = "";
		     else {                                  
		       if(val.compare("N") == 0) {
		         val = "R";
			 setValue((char *)"prflg","B");      
		       }
		       else
			 if(val.compare("Y") == 0)           
		          addQslFlag((char *)"qslSvia","B"); 
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
                 if(strcmp(buf,"eqsls") == 0) {              
		   if(val.compare("N") == 0)                 
                      val = "R";                             
                    else {
                      if(val.compare("Y") == 0) {
                        addQslFlag((char *)"qslSvia","E");   
                        setValueQsl(buf,val);                
		      }
		    }
		 }
	         else                                        
                  if(strcmp(buf,"eqslr") == 0) {
                    if(val.compare("N") == 0)                
                      val = "R";                             
                    else {
                      if(val.compare("Y") == 0) {            
                        addQslFlag((char *)"qslRvia","E");   
                        setValueQsl(buf,val);                
		      }
		    }
	        }
	        
	        else                                         
		if(strcmp(buf,"lotqslr") == 0) {             
                   if(val.compare("N") == 0)                 
                      val = "R";                             
                   else {
                     if(val.compare("Y") == 0) {             
                       addQslFlag((char *)"qslRvia","L");    
                       setValueQsl(buf,val);                 
		     }
		   }
	        }
		else                                         
		if(strcmp(buf,"lotqsls") == 0) {
                   if(val.compare("N") == 0)                 
                      val = "R";                             
                   else {
                     if(val.compare("Y") == 0) {             
                       addQslFlag((char *)"qslSvia","L");    
                       setValueQsl(buf,val);                 
		     }
		   }
	        }
	        else
	         setValueQsl(buf,val);                       
	      }
              else {  
                strcpy(buf,token.toAscii());
                strcpy(buf,getdbNameAwd(buf));         
                if(strlen(buf) != 0) {                 
                  setValueAwd(buf,val);                
                }
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
         
//--     ENDE_Adif_data_zeile -- <EOR> gefunden
         
         if(getValue((char *)"dxcc").count() == 0) {   
           s = getValue((char *)"rufz");               
           n = 1;
           z = s.left(n);
           while(getla( z ) == 0) {
              if(n >= 4) {                             
                Error += "dxcc fuer Rufzeichen "+s+" nicht gefunden\n";
                state = 1;                             
               break;
              }
              z = s.left(++n);
           }
           setValue((char *)"cn",Lcn);
           setValue((char *)"cqz",Cqz);
           setValue((char *)"ituz",Ituz);
           setValue((char *)"dxcc",Dxcc);
         }
         if(getValue((char *)"cn").count() == 0) {    
            setValue((char *)"cn",Lcn);               
         }
         if(getValue((char *)"cqz").count() == 0) {   
            setValue((char *)"cqz",Cqz);              
         }
         if(getValue((char *)"ituz").count() == 0) {  
            setValue((char *)"ituz",Ituz);            
         }
         if(getValue((char *)"etime").count() == 0) {             
            setValue((char *)"etime", getValue((char *)"btime")); 
         }
         
         s = s.setNum(hId);                          
         strcpy(buf,s.toAscii());
         setValue((char *)"rig",rig);                
         setValue((char *)"qline",(char *)"KW");     
         setValue((char *)"quarter",buf);            
         setValue((char *)"op",(char *)"0");         

/*
         n = 0;   
         i = 0;
         while(i != qsoFields) {
            qDebug() << adh[i].pos <<  adh[i].field << adh[i].adifnam << adh[i].value;
            i++;
         }
         n = 0;
         i = 0;
         while(i != qslFields) {
           qDebug() << adhq[i].pos <<  adhq[i].field << adhq[i].adifnam << adhq[i].value;
           i++;
         }
         n = 0;
         i = 0;
         while(i != awdFields) {
           qDebug() << adhw[i].pos <<  adhw[i].field << adhw[i].adifnam << adhw[i].value;
           i++;
         }
         //datei.close(); 
         //return;        
*/

         QSqlQuery query;
	 

         qy = "SELECT omid FROM "+log+"om WHERE rufz='";
         qy += Call;
         qy += "'";
         query.exec(qy);
         n = query.size();
         if(n == 0 ) {                                    
            qy = "INSERT INTO "+log+"om VALUES (0,'";     
            qy += Call;
            qy += "','";
            qy += adh[2].value;
            qy += "')";
            QSqlQuery query;
            query.exec(qy);
            oid = query.lastInsertId().toInt();           
         }
         else {
          query.next();
          oid = query.value(0).toInt();
         } 

         n = 3;                        
         qy = "INSERT INTO "+log+" VALUES (0,"+s.setNum(oid)+",'";
         qy += adh[0].value;           
         while(n != qsoFields -3 ) {   
           qy += "','";
           qy += adh[n++].value;
         }
         qy += "',";
         qy += adh[n++].value;         
         qy += ",";
         qy += adh[n++].value;         
         qy += ")";
         query.exec(qy);               
         id = query.lastInsertId().toInt();

         n = 0;                         
         qy = "INSERT INTO "+log+"qsl VALUES (0,"+s.setNum(id)+",'";
         qy += Call;
         qy += "','"+qsoDate;
         while(n != qslFields) {        
           qy += "','";
           qy += adhq[n++].value;
         }
         qy += "')";
         query.exec(qy);                
         

         n = 0;
         while(n != awdFields) {            
          if(strlen(adhw[n].value) != 0) {  
             qy = "INSERT INTO "+log+"awd VALUES (0,"+s.setNum(id)+",'";
             qy += Call;
             qy += "','";
             qy += adhw[n].field;
             qy += "','";
             qy += adhw[n].value;
             qy += "')";
             query.exec(qy);               
           }
           n++;                            
         }
         count++;                          
/*
         if(count == 1) {                  
          datei.close();
          return;
         }
*/
     }
     datei.close();
     return;
}

int wadif::getla(QString str)
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
