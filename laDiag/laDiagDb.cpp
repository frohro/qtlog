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

void laDiag::getLaTable(QString q)
{
  int i, r,z;
  QSqlQuery query;
  query.exec(q);
  row = query.size();                            
  col = query.record().count();                  
  laTabelle->setRowCount(row);                   
  laTabelle->setColumnWidth(0,200);
  laTabelle->setColumnWidth(1,50);
  laTabelle->setColumnWidth(2,50);
  laTabelle->setColumnWidth(3,50);
  laTabelle->setColumnWidth(4,50);
  laTabelle->setColumnWidth(5,50);
  laTabelle->setColumnWidth(6,60);
  laTabelle->setColumnWidth(7,60);
  laTabelle->setColumnWidth(8,60);
  laTabelle->setColumnWidth(9,60);
  laTabelle->setColumnWidth(10,60);
  laTabelle->setColumnWidth(11,200);
  row = 0;
  while(query.next()) {
    z = 0;
    r = 0;
    col = 0;
    i = 0;
    y = query.value(i++).toString();
    y += "";
    QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((r++)*(z++)));  
    rowItem->setText(y);
    laTabelle->setVerticalHeaderItem(row,rowItem);
//--
    QTableWidgetItem *newItem = new QTableWidgetItem(tr("%1").arg((row)*(col)));  
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                  
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                  
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                   
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                   
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                   
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                   
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                   
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                    
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                   
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                   
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col++,newItem);
    newItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));                   
    newItem->setText(query.value(i++).toString());
    laTabelle->setItem(row,col,newItem);
    row++;
  }
  s = s.setNum(row) + tr(" Eintraege");
  labelcnt->setText(s);
  WlineEditPref->setFocus();
}
