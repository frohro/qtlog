/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 02-10-2010
********************************************************************** */

/* intercepts messages that are in the same format as those
 * sent to xlog and should consist of fields, separated by the '\1' character.
 * Fields are started with a descriptor, followed by a colon (':'). Possible fields are:
 *
 ****************************************************************************
 * program: <name of program which sends the QSO information>
 * version: <version string for the message queue, must be '1' for now>
 * date   : <date of QSO, preferable 'dd mmm yyyy'>
 * time   : <start time of QSO, preferably in GMT >
 * endtime: <end time of QSO, preferably GMT (mandatory in some countries)>
 * call   : <callsign of your contact (will be converted to uppercase)>
 * mhz    : <frequency in MHz>
 * mode   : <any one of CW,SSB,RTTY,PSK31,etc (will be converted to uppercase)>
 * tx     : <report (RST) which you have sent>
 * rx     : <report (RST) which you have received>
 * name   : <name of the operator you have contacted>
 * qth    : <town of the operator you have contacted>
 * notes  : <additional notes>
 * power  : <power you have used (mandatory in some countries)>
 * locator: <QRA locator, as used in VHF QSO's>
 * free1  : <information to put in freefield1>
 * free2  : <information to put in freefield2>
 ****************************************************************************/
 //dl1hbd - 02.10.2010
 
#include <QtGui>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "diglog.h"


void diglog::setup_ipcData()
{
  	msgid = msgget ((key_t) 1238, 0666 | IPC_CREAT);
	if (msgid != -1) {                                        
	    iptimer->start(1000);
	    QTimer::singleShot(100, this, SLOT(getipcData()));    
	}
	else
         qDebug() << "* ERROR *" << msgid;
}


void diglog::getipcData()
{
    ssize_t status = -1;
    long msgtyp = 0;
    status = msgrcv(msgid, (void *) &msgbuf, 1024, msgtyp,
                            MSG_NOERROR | IPC_NOWAIT);
    qDebug() << "ipc_status" << status;
  
    if(status != -1) 
      add_ipc_data (msgbuf.mtype, msgbuf.mtext);
}



void diglog::add_ipc_data (int type, char * entry)
{
     if (type == 88 && entry && (strlen (entry) > 0)) {
         recFields.clear();                               
	 char *p = entry, *p1, *p2 = entry + strlen(entry);
	 while( 1 ) {
	    p1 = strchr(p, 1);
	    if(p1 && p < p2) {
	       *p1 = 0;
		parse_ipc_data( p );
		p = p1 + 1;
	    } 
            else {
	      if(p < p2)
	        parse_ipc_data( p );
	      break;
	   }
	}
	
	QHash<QString,QString>::const_iterator n = recFields.constBegin();
        while (n != recFields.constEnd()) {
          qDebug() << "recFields:" << n.key() << ": " << n.value();
          ++n;
	}
	
	insertQso();    
     }
}

char * months[12] = {
	(char *)"jan",(char *)"feb",(char *)"mar",(char *)"apr",(char *)"may",(char *)"jun",
	(char *)"jul",(char *)"aug",(char *)"sep",(char *)"Okt",(char *)"nov",(char *)"dec"};

void diglog::parse_ipc_data (char *s)
{
	char *p = strchr(s,':');
	if (!p) return;
	
	
	p++;
	if(strstr(s, "date:") == s) {
	    int d, y, i;
	    char mon[8], date[12];
	    sscanf(p,"%d %s %d", &d, mon, &y);
	    for( i = 0; i < 12; i++ )
	      if (strncasecmp (mon, months[i],3) == 0)
		 break;
	    sprintf(date,"%04d%02d%02d", y, i+1, d);
	    recFields.insert("date",date);       
	   return;
	}
	
	if(strstr(s, "time:") == s) {            
	    recFields.insert("time",p);
	   return;
	}
	
	if(strstr(s, "endtime:") == s) {         
	    recFields.insert("endtime",p);
	   return;
	}
	
	if(strstr(s, "call:") == s) {            
	    recFields.insert("call",p);   
	   return;
	}
	if(strstr(s, "mhz:") == s) {             
	    recFields.insert("mhz",p);   
	   return;
	}
	if(strstr(s, "mode:") == s) {            
	    recFields.insert("mode",p);   
	   return;
	}
	
	if(strstr(s, "tx:") == s) {              
	    recFields.insert("tx",p);   
	   return;
	}
	
	if(strstr(s, "rx:") == s) {              
	    recFields.insert("rx",p);   
	   return;
	}
	
	if(strstr(s, "name:") == s) {            
	    recFields.insert("name",p);
	   return;
	}
	
	if((strstr(s, "qth:") == s) && *p) {     
	     recFields.insert("qth",p);   
	   return;
	}
	
        if((strstr(s, "locator:") == s) && *p) { 
	     recFields.insert("locator",p);   
	   return;
	}
	
	if(strstr(s, "notes:") == s) {           
	    recFields.insert("notes",p);   
	   return;
        }
        
	if(strstr(s, "power:") == s) {               
	    char *power = new char[strlen(p) + 10];  
	    sprintf(power," Pwr: %s", p);            
	    recFields.insert("pwr",p);   
	   return;
	}
	
	
        if(strstr(s, "state:") == s) {           
	     recFields.insert("state",p);   
	   return;
	}
	
	if(strstr(s, "province:") == s) {        
	     recFields.insert("province",p);   
	   return;
	}
	
	if(strstr(s, "country:") == s) {         
	     recFields.insert("country",p);   
	   return;
	}
	
	
	if(strstr(s, "serialout:") == s) {       
	     recFields.insert("serialout",p);   
	   return;
        } 
	
	if(strstr(s, "serialin:") == s) {        
	     recFields.insert("serialin",p);   
	   return;
	}
	   
	
	if(strstr(s, "free1:") == s) {           
	     recFields.insert("free1",p);   
	   return;
	}
	
	if(strstr(s, "free2:") == s) {           
	     recFields.insert("free2",p);        
	   return;
	}
	return;
}
