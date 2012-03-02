/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include "laDiag.h"


// ----------------------------------
void laDiag::getLaTable(QString q)
{
  int i, r,z;
  
  QSqlQuery query;
  query.exec(q);
  
  row = query.size();                // 369 rows      
  col = query.record().count();      //  	12 Spalten
   
  laTabelle->setRowCount(row);       // TabellenLänge setzen - col ist schon gesetzt
  // la
  laTabelle->setColumnWidth(0,220);  // name
  laTabelle->setColumnWidth(1,60);   // lcn
  laTabelle->setColumnWidth(2,55);   // ituz
  laTabelle->setColumnWidth(3,55);   // cqz
  laTabelle->setColumnWidth(4,55);   // timeZ
  laTabelle->setColumnWidth(5,60);   // br
  laTabelle->setColumnWidth(6,60);   // lg
  laTabelle->setColumnWidth(7,60);   // dxcc
  laTabelle->setColumnWidth(8,60);   // wae
  laTabelle->setColumnWidth(9,60);   // wxp
  laTabelle->setColumnWidth(10,60);  // adif_nr
  laTabelle->setColumnWidth(11,200); // Bemerkungen
  
  row = 0;
  while(query.next()) {
    z = 0;
    r = 0;
    i = 0;
    QTableWidgetItem *rItem = new QTableWidgetItem(tr("%1").arg((r++)*(z++)));  // la
    rItem->setText(query.value(i++).toString());
    laTabelle->setVerticalHeaderItem(row,rItem);
    r = 0;
    while( r != col -1) {
       QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(r))); 
       newItem->setText(query.value(i++).toString());       // text eintragen
       laTabelle->setItem(row,r++,newItem);                 // an row,col übergeben
     }
     row++;
    }
  
  s = s.setNum(row) + tr(" Eintraege");
  labelcnt->setText(s);
  WlineEditPref->setFocus();
  
}
