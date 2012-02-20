/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QSettings>
#include <QString>
#include <QTabWidget>
#include <QtGui>
#include <QtSql>
#include "medium.h"

medium::medium(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
  setupUi(this);
  n = settings.value("FontSize").toString().toInt();
  QFont font;
  font.setPointSize(n); 			
  setFont(font);
 
  connect(ButtonEnde, SIGNAL(clicked()), this, SLOT(goExit()));
  connect(mediumList, SIGNAL(itemClicked(QTreeWidgetItem *,int)), this,SLOT(mListClickedSlot(QTreeWidgetItem *, int)));
  connect(ButtonUpdate, SIGNAL(clicked()), this, SLOT(mUpdateCb()));
  
  mediumList->setColumnWidth(0,110);  
  mediumList->setColumnWidth(1,40);   
  mediumList->setColumnWidth(2,40);   
  mediumList->setColumnWidth(3,50);   
  mediumList->setColumnWidth(4,50);   
  mediumList->setColumnWidth(5,60);   
  mediumList->setColumnWidth(6,50);   
  mediumList->setColumnWidth(7,60);   
  mediumList->setColumnWidth(8,60);   
  mediumList->setColumnWidth(9,70);   
  mediumList->setColumnWidth(10,70);  
  mediumList->setColumnWidth(11,90);  
  mediumList->setColumnWidth(12,60);  
  mediumList->setColumnWidth(13,30);  
  mediumList->setColumnWidth(14,60);  
  mediumList->setColumnWidth(15,60);  

  showData();
  pItem = 0;
  prflg = 0;                          
}

medium::~medium() 
{
}

void medium::goExit()
{
   accept();                              
}

void medium::showData()
{
    int i, n;
    QSqlQuery query;
    mediumList->clear();
    query.exec("SELECT * FROM prmedium");
    while(query.next()) {                                
        n = 0;
        i = 0;
        QTreeWidgetItem *item = new QTreeWidgetItem(mediumList);
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
        item->setText(i++,query.value(n++).toString());    
    }
    i = 0;
    QTreeWidgetItem *item = mediumList->topLevelItem(i);
    while( 1 ) {                               
       n = item->text(2).toInt();
       if( n == 1 ) {
           TraegerW->setText(item->text(0));    
           TraegerTypeW->setText(item->text(1));
          break;
       }
       item = mediumList->topLevelItem(++i);
    }
}


void medium::mListClickedSlot(QTreeWidgetItem * item, int col)
{
    if( col != -1 ) {
      pItem = item;
      n = mediumList->topLevelItemCount();
      i = 0;
      QTreeWidgetItem *item = mediumList->topLevelItem(i);
      while( n != 0 ) {                     
         item->setText(2,"0");
         if(--n == 0) break;
         item = mediumList->topLevelItem(++i);
      }
      TraegerW->setText(pItem->text(0));
      TraegerTypeW->setText(pItem->text(1));
      pItem->setText(2,"1");
      BreiteW->setText(pItem->text(3));
      HoeheW->setText(pItem->text(4));
      ColsW->setText(pItem->text(5));
      RowsW->setText(pItem->text(6));
      RimxW->setText(pItem->text(7));
      RimyW->setText(pItem->text(8));
      DifxW->setText(pItem->text(9));
      DifyW->setText(pItem->text(10));
      PrTextW->setText(pItem->text(11));
      ZufW->setText(pItem->text(13));
      FrameW->setText(pItem->text(14));
      JustW->setText(pItem->text(15));
    }
}

void medium::mUpdateCb()
{
  if( pItem == 0 ) {
     QMessageBox::about(this,"Print Medium",
       tr("Kein Printmedium selctiert"));
       return;
  }
     QSqlQuery query;
     qy = "UPDATE prmedium SET sel=0";   
     query.exec(qy);
     qy = "UPDATE prmedium SET style='"+TraegerTypeW->text(); 
     qy += "',sel="+pItem->text(2);                           
     qy += ",lbreite='"+BreiteW->text();
     qy += "',lhoehe='"+HoeheW->text();
     qy += "',spalten='"+ColsW->text();
     qy += "',reihen='"+RowsW->text();
     qy += "',rimx='"+RimxW->text();
     qy += "',rimy='"+RimyW->text();
     qy += "',difx='"+DifxW->text();
     qy += "',dify='"+DifyW->text();
     qy += "',tpos='"+PrTextW->text();
     qy += "',zuf='"+ZufW->text();
     qy += "',frame='"+FrameW->text();
     qy += "',just='"+JustW->text();
     qy += "' WHERE traeger='"+TraegerW->text()+"'";
     query.exec(qy);
     i = 0;
     pItem->setText(1,TraegerTypeW->text());
     pItem->setText(3,BreiteW->text());
     pItem->setText(4,HoeheW->text());
     pItem->setText(5,ColsW->text());
     pItem->setText(6,RowsW->text());
     pItem->setText(7,RimxW->text());
     pItem->setText(8,RimyW->text());
     pItem->setText(9,DifxW->text());
     pItem->setText(10,DifyW->text());
     pItem->setText(11,PrTextW->text());
     pItem->setText(13,ZufW->text());
     pItem->setText(14,FrameW->text());
     pItem->setText(15,JustW->text());
     prflg = 1;                          
    goExit();
}
