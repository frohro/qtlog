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
#include "reportLayout.h"

reportLayout::reportLayout(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                    
   setFont(font);
   connect(pushButtonOK, SIGNAL(clicked()), this, SLOT(goExit()));
   connect(dbList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(dbListClicked(QTreeWidgetItem*,int)));
   connect(layoutList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(layoutListClicked(QTreeWidgetItem*,int)));
   connect(BoxLayout, SIGNAL(currentIndexChanged(QString)), this, SLOT( getReportListItemsCb(QString)));
   dbList->setColumnWidth(0,25);
   dbList->setColumnWidth(1,80);
   transl.installHashTable();                      
   QSqlQuery query; 
   qy = "SELECT sel,refnam FROM refnamen WHERE sel='*' ORDER BY idn DESC";
   query.exec(qy);
   while(query.next()) {                           
      i = 0;
      n = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(dbList);
      s = query.value(n++).toString();                 
      item->setText(i++,s);                            
      item->setText(i++,query.value(n).toString());    
   }
       
   qy = "SELECT sel,refnam FROM refnamen WHERE type='2' AND sel='' OR type='3' AND refnam != '' ORDER BY idn";
   query.exec(qy);
   while(query.next()) {                             
      i = 0;
      n = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(dbList);
      s = query.value(n++).toString();                 
      item->setText(i++,s);                            
      item->setText(i++,query.value(n).toString());    
   }
   
   
   s = settings.value("Val").toString();
   i = BoxLayout->findText(s, Qt::MatchCaseSensitive );
   BoxLayout->setCurrentIndex(i);                     
   layoutList->clear();
   qy = "SELECT qstring FROM wlayout WHERE qhname='"+BoxLayout->currentText()+"'";
   qDebug() << qy;
   query.exec(qy);
   query.next();
   s = query.value(0).toString();                     
   i = s.indexOf("day",0);                            
   t = s.mid(i);
   QTreeWidgetItem *dbItem;                           
   d = dbList->topLevelItemCount();                   
   while( 1 ) {                                       
      i = t.indexOf(",",0);                           
      if( i == -1 ) {
         QTreeWidgetItem *litem = new QTreeWidgetItem(layoutList);  
         s = transl.getUsrField(t);
         litem->setText(0,s);                                        
         dbItem = dbList->currentItem();                             
         n = 0;
         while( n != d ) {
           dbItem = dbList->topLevelItem (n);
           x = s.compare(dbItem->text(1));                           
           if( !x ) {
             dbItem->setText(0,"x");                                 
            break;
           }
           n++;
         }
         break;
      }
      s = transl.getUsrField(t.left(i));
      QTreeWidgetItem *litem = new QTreeWidgetItem(layoutList);     
      litem->setText(0,s);                                           
      
      dbItem = dbList->currentItem();                                
      n = 0;
      while( n != d ) {
         if((s.compare("Datum") == 0) || (s.compare("Cept") == 0) || (s.compare("Call")== 0)) 
            break;
         dbItem = dbList->topLevelItem (n);
         x = s.compare(dbItem->text(1));                             
         if( !x ) {
            dbItem->setText(0,"x");                                  
           break;
         }
         n++;
      }
      t.remove(0, i +1);                                             
   }
   editflg = 1;
   changeflg = 0;
}

reportLayout::~reportLayout()
{
}

void reportLayout::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
                goExit();
             break;
    default : break;
   }
}

void reportLayout::getReportListItemsCb(QString name)
{
     getReportListItems( name );
}



void reportLayout::getReportListItems(QString layout)
{
   layoutList->clear();                               
   QTreeWidgetItem *dbItem;                           
   d = dbList->topLevelItemCount();                   
   dbItem = dbList->currentItem();
   n = 0;
   n = 3;
   while( n != d ) {                                 
     dbItem = dbList->topLevelItem (n++);
     dbItem->setText(0,"");
   }
   QSqlQuery query; 
   qy = "SELECT qstring FROM wlayout WHERE qhname='"+layout+"'";
   qDebug() << qy;
   query.exec(qy);
   qDebug() << query.lastError();
   query.next();
   s = query.value(0).toString();                     
   i = s.indexOf("day",0);                            
   t = s.mid(i);
   while( 1 ) {                                      
      i = t.indexOf(",",0);
      if( i == -1 ) {
         QTreeWidgetItem *litem = new QTreeWidgetItem(layoutList);  
         s = transl.getUsrField(t);
         litem->setText(0,s);                          
         dbItem = dbList->currentItem();               
         n = 0;
         while( n != d ) {
           dbItem = dbList->topLevelItem (n);
           x = s.compare(dbItem->text(1));             
           if( !x ) {
             dbItem->setText(0,"x");                   
             break;
           }
           n++;
         }
         break;
      }
      s = transl.getUsrField(t.left(i));
      QTreeWidgetItem *litem = new QTreeWidgetItem(layoutList);     
      litem->setText(0,s);                                           
      
      dbItem = dbList->currentItem();                                
      n = 0;
      while( n != d +1 ) {
         if((s.compare("Datum") == 0) || (s.compare("Cept") == 0) || (s.compare("Call")== 0)) 
            break;
         dbItem = dbList->topLevelItem (n);
         x = s.compare(dbItem->text(1));                       
         if( !x ) {
            dbItem->setText(0,"x");                                  
           break;
         }
         n++;
      }
      t.remove(0, i +1);                                             
   }
   editflg = 1;
}

void reportLayout::goExit()                       
{
  if(editflg)
      putQstring();
    if( i == -1 ) {
        QString text = QObject::tr("<qt>Das Feld <b>AWD</b> wurde nicht eingebunden. "
        "Mit AWD werden Award-Kenner z.B DOK usw. angezeigt oder abgefragt. "
        "Normal stehen AWD-Kenner in Spalte 10, "
        "es koennen auch andere Spalten sein.</qt>"
        "Ohne <b>Award-Feld</b> weitermachen ?");
        QMessageBox msg(QObject::tr("Vermisse Award-Einbindung"), text,
        QMessageBox::Warning, QMessageBox::Yes|QMessageBox::Default,
        QMessageBox::No|QMessageBox::Escape,QMessageBox::NoButton);
        if(msg.exec() == QMessageBox::Yes)
           ;
        else 
         return;
    }
    accept();                                       
}


void reportLayout::dbListClicked(QTreeWidgetItem *item, int col)
{
  int x = col;
      if(dbList->indexOfTopLevelItem(item) <= 2)     
           return;
      if(item->text(0).count() == 0) {
          item->setText(0,"x");
          QTreeWidgetItem *itemL = new QTreeWidgetItem(layoutList);
          itemL->setText(0,item->text(1));
      }
      else {
       item->setText(0,"");
       i = layoutList->topLevelItemCount();          
       lItem = layoutList->currentItem();            
       n =  layoutList->indexOfTopLevelItem(lItem);  
       n = 0;
       while( n != i ) {
         lItem = layoutList->topLevelItem (n);
         s = lItem->text(0);
         x = s.compare(item->text(1));               
         if( !x ) {
           layoutList->takeTopLevelItem(n);          
           break;
         }
         n++;
       }
       if( x ) {
         qDebug() << "Grober FEHLER";                
        return;
       }
    }
    changeflg = 1;
}


void reportLayout::layoutListClicked(QTreeWidgetItem *item, int col)
{
     int x;
     x = col;
     i = dbList->topLevelItemCount();          
     lItem = dbList->currentItem();
     n = 0;
     while( n != i ) {
       lItem = dbList->topLevelItem (n);
       s = lItem->text(1);
       x = s.compare(item->text(0));            
         if( !x ) {
           lItem->setText(0,"");
           break;
         }
         n++;
       }
    n =  layoutList->indexOfTopLevelItem(item);  
    layoutList->takeTopLevelItem(n);             
    changeflg = 1;
}


void reportLayout::putQstring()
{
     i = layoutList->topLevelItemCount();
  
     n = layoutList->indexOfTopLevelItem(lItem);
     x = 0;
     awdPos = 0;
     n = 0;
     s = "SELECT id,";
     while( n != i ) {
        lItem = layoutList->topLevelItem (n++);
        s += transl.getDbField(lItem->text(0))+",";  
        t = lItem->text(0);
        if( !x ) {                                  
           if(t.compare("AWD") != 0 )
               awdPos++;
           else 
              x = 1;                                 
        }
     }
     n = s.count();
     s = s.rightJustified(n -1, '.', true);         
     i = s.indexOf("AWD");                           
     awdPos++;
     editflg = i;
     if(i == -1)
       awdPos = i;
     QSqlQuery query;
     qy = "UPDATE wlayout SET awdpos="+a.setNum(awdPos)+",qstring='"+s+"' WHERE qhname='"+BoxLayout->currentText()+"'";
     query.exec(qy);
     a = BoxLayout->currentText();
}
