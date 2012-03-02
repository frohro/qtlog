/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.05.2007
********************************************************************** */

#include <QtGui>
#include "addQso.h"



// -----------------------------------
void addQso::setAwdOmList()
{
   awardOmList->setColumnWidth(0,100);  // Call
   awardOmList->setColumnWidth(1,120);  // AwdType
   awardOmList->setColumnWidth(2,100);  // Kenner
}

// -----------------------------------
void addQso::getAdwKennerList()
{
     awardOmList->clear();
     qy = "SELECT acall,awtype,awkenner FROM "+logFile+"awd WHERE acall='"+wCall->text()+"' GROUP BY acall,awtype";
     QSqlQuery query;
     query.exec(qy);
     if(query.size() != 0) { 
        while(query.next()) {                                 // QSO_Liste aufbauen
         n = 0;
         i = 0;
         QTreeWidgetItem * item = new QTreeWidgetItem(awardOmList);
         item->setText(i++,query.value(n++).toString());      // Call
         item->setText(i++,query.value(n++).toString());      // awtype
         
         item->setText(i++,query.value(n++).toString());      // kenner
       }
      }
}

// QSL-Status_Liste
// -------------------------------------------------
void addQso::setQslTabel()
{
    qslStatusList->setColumnWidth(0,110);   // Call
    qslStatusList->setColumnWidth(1,110);   // day
    qslStatusList->setColumnWidth(2,50);    // band
    qslStatusList->setColumnWidth(3,10);    // | 
    
    qslStatusList->setColumnWidth(4,70);    // qslRvia 
    qslStatusList->setColumnWidth(5,10);    // |
    qslStatusList->setColumnWidth(6,60);    // qsls 
    qslStatusList->setColumnWidth(7,10);    // |
    qslStatusList->setColumnWidth(8,60);    // eqsls
    qslStatusList->setColumnWidth(9,10);    // |
    qslStatusList->setColumnWidth(10,60);   // lotqs
}

// zeige QSL_status
//--------------------------------------
void addQso::getQslStatus()
{
  QString cept, rufz, band;
 
     QBrush brush(QColor(178,181,229));                     //
     qslStatusList->clear();
     QSqlQuery query;
     qy = "SELECT id,cept,rufz,day,band,qslRvia,qsls,eqsls,lotqsls";
     qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
     qy += " WHERE rufz LIKE '"+Rufz+"%' ORDER by day DESC LIMIT "+Last;
     query.exec(qy);
     QString id;
     if(query.size() != 0) {                                // falls QSO's gefunden
       while(query.next()) {                                // QSO_Liste aufbauen
         n = 0;
         i = 0;
         QTreeWidgetItem * item = new QTreeWidgetItem(qslStatusList);
         id = query.value(n++).toString();                  //  QSO_Id
         cept = query.value(n++).toString();                //  cept
         rufz = query.value(n++).toString();                //  Rufzeichen
         if(cept.count() != 0)
            s = cept+rufz;
	 else
           s = rufz;
         item->setText(i++,s);                              // i=0 cep/Call
	 
         item->setText(i++,query.value(n++).toString());    // i=1 day
         item->setText(i++,serv.getMyband(query.value(n++).toString())); 
	 
         item->setText(i++,"|");                            // i=3
         item->setText(i,query.value(n++).toString());      // i=4 qslRvia
         item->setTextAlignment(i++,Qt::AlignCenter);
         item->setText(i++,"|");                            // i=5
         item->setText(i,query.value(n++).toString());      // i=6 eqsls
         item->setTextAlignment(i,Qt::AlignCenter);
	 item->setBackground(i++,brush);                    // fablich markieren
         item->setText(i++,"|");                            // i=7
         item->setText(i,query.value(n++).toString());      // i=8 eqsls
	 item->setTextAlignment(i++,Qt::AlignCenter);
         item->setText(i++,"|");                            // i=9
	 s = "   "+query.value(n).toString();
	 item->setText(i,s);                                 // i=10 lotqs
       }
     }
}
