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
#include "laDiag.h"
#include "convert.h"
conVert::conVert()
{
  i = 0;
  n = 0;
}
conVert::~conVert()
{
  delete RefNamen;
}


//---------------------------------------------------------
void conVert::loadRefnamen()
{
  QString t;
  qy = "SELECT dbfield,refnam FROM refnamen WHERE type = 4";
  QSqlQuery query(qy);
  RefEntries = query.numRowsAffected();
  RefNamen = new REFNAMEN[RefEntries +1];
  i = 0;
  while(query.next()) {
    n = 0;
    //qDebug() << query.value(n).toString();
    strcpy(RefNamen[i].Field,query.value(n++).toString().toAscii());  
    t = RefNamen[i].Field;
    strcpy(RefNamen[i].Refnam,query.value(n).toString().toAscii());   
    
    //qDebug() << query.value(n).toString();
    i++;
   }
}


QString conVert::getdbField(QString name)      
{
  for( i = 0; i != RefEntries; i++ ) {
     if(strcmp(RefNamen[i].Refnam,name.toAscii()) == 0) {
       return RefNamen[i].Field;              
     }
  }
  return "";                                  
}
