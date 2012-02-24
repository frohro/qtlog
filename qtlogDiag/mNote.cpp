/* **********************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
************************************************************************** */

#include <QSettings>
#include <QString>
#include <QtGui>
#include <QtSql>
#include <qmessagebox.h>
#include "mNote.h"

mNote::mNote(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
   setupUi(this);
   int n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n); 			
   setFont(font);
   
   connect (ButtonDelete, SIGNAL(clicked()), this,SLOT(deleteNoteItem()));
   connect (ButtonEnde, SIGNAL(clicked()), this,SLOT(goExit()));
   
  NoteTable->setColumnWidth(0,140);          // Name
  NoteTable->setColumnWidth(1,900);          // Notiz
  QSqlQuery query;
  qy = "SELECT * FROM mynote ORDER BY name";
  query.exec(qy);
  row = query.size();                                       //  rows 
  // NoteTable->setRowCount(row +1);                        //  TabellenGroeße row + 1_leer setzen 
  NoteTable->setRowCount(row);
  row = 0;
  while(query.next()) {
    col = 0;
    i = 0;
    s = query.value(i++).toString();                        // Id für vertikal_header holen
    QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((r++)*(z++))); //Id_item erzeugen
    rowItem->setText(s);                                    // text eintragen
    NoteTable->setVerticalHeaderItem(row,rowItem);          // Id ( ist QSO_satz_nr ) setzen
    
    rowItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));    // name
    rowItem->setText(query.value(i++).toString());
    NoteTable->setItem(row,col++,rowItem);
    
    rowItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));    // notiz
    rowItem->setText(query.value(i++).toString());
    NoteTable->setItem(row,col++,rowItem);
    row++;
   }
   // erzeuge eine LeerZeile mit einer fortlaufenden ID_nummer
    qy = "SELECT nd FROM mynote ORDER BY nd DESC LIMIT 1";
    query.exec(qy);
    query.next();
    i = query.value(0).toInt();                                     // letzte Id_Nr
    i++;
    NoteTable->setRowCount(row +1);
    QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((row)*(z++))); //Id_item erzeugen
    rowItem->setText(s.setNum(i));                             // text eintragen
    NoteTable->setVerticalHeaderItem(row,rowItem);             // Id ( ist QSO_satz_nr ) setzen
    
    connect (NoteTable, SIGNAL(itemChanged(QTableWidgetItem *)), this,
                        SLOT(updateNoteItem(QTableWidgetItem *)));     
}
// -----------
mNote::~mNote()
{
}

// ------------------------------------------
void mNote::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                           
             goExit();
           break;
   }
}

// *******************************************************************
// Daten in der Notiz_Tabelle editiert - jetzt in der DB updaten
// SIGNAL ( itemChanged(QTableWidgetItem *) );
//-------------------------------------------------------------------
void mNote::updateNoteItem( QTableWidgetItem *item )
{
    if (item != NoteTable->currentItem())                     
        return;                                      // fremde items nicht bearbeiten
    if (item) {                                               
       r = 0;
       QSqlQuery query; 
       r = NoteTable->row( item );                             
       col = NoteTable->column ( item );                       
       QTableWidgetItem * h = NoteTable->horizontalHeaderItem( col ); 
       QTableWidgetItem * v = NoteTable->verticalHeaderItem( r );     
       if( row == r ) {
          QTableWidgetItem * nI = NoteTable->item(r,col);
           if(col != 0) {
              QMessageBox::information( this,
              tr("INFO"),
              tr("\nErst den Namen eingeben"),
              QMessageBox::Ok);
              return;
           }
           qy = "INSERT INTO mynote VALUES (0,'";
           nI = NoteTable->item(r,0);
           if( h != 0)
              qy += nI->text();
           else
              qy += "";
           qy += "','";
           nI = NoteTable->item(r,1);
           if( nI != 0)
              qy += nI->text();
           else
              qy += "";
           qy += "')";
           query.exec(qy);
           
	    // Tabelle neu laden
           NoteTable->setRowCount(0);                    
           qy = "SELECT * FROM mynote";
           query.exec(qy);
           row = query.size();                           
           NoteTable->setRowCount(row +1);                   //  TabellenGroeße row + 1_leer setzen      
           row = 0;
           while(query.next()) {
             col = 0;
             i = 0;
             s = query.value(i++).toString();                        // Id für vertikal_header holen
             QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((r++)*(z++))); //Id_item 
             rowItem->setText(s);                                    // text eintragen
             NoteTable->setVerticalHeaderItem(row,rowItem);          // Id ( ist satz_nr ) setzen
             rowItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));   // name
             rowItem->setText(query.value(i++).toString());
             NoteTable->setItem(row,col++,rowItem);
             rowItem = new QTableWidgetItem((tr("%1").arg((row)*(col))));   //  notiz
             rowItem->setText(query.value(i++).toString());
             NoteTable->setItem(row,col++,rowItem);
             row++;
           }
            // erzeuge eine LeerZeile mit einer fortlaufenden ID_nummer
            qy = "SELECT nd FROM mynote ORDER BY nd DESC LIMIT 1";  // letzte Id_nr
            query.exec(qy);                                         // holen
            query.next();
            i = query.value(0).toInt();                             // letzte Id_Nr
            i++;
            NoteTable->setRowCount(row +1);                         // row +1
            QTableWidgetItem *rowItem = new QTableWidgetItem(tr("%1").arg((row)*(z++)));//Id_item erzeugen
            rowItem->setText(s.setNum(i));                          // text eintragen
            NoteTable->setVerticalHeaderItem(row,rowItem);          // Id ( ist QSO_satz_nr ) setzen
        }
        else {                                                      // update cell
           h = NoteTable->item(r,col);
           if( col == 1 ) 
              qy = "UPDATE mynote SET note='"+h->text()+"' WHERE nd="+v->text();
           else 
             qy = "UPDATE mynote SET name='"+h->text()+"' WHERE nd="+v->text();
           query.exec(qy);
        }
    }
}

// -------------------------------------
void mNote::deleteNoteItem()
{
   QTableWidgetItem * item;
   item = NoteTable->currentItem();
   r = NoteTable->row( item );
   if( r == -1 )
     return;
   QTableWidgetItem * v = NoteTable->verticalHeaderItem( r );  
   s = v->text();
   int status = QMessageBox::question( this,
                tr("Loeschen ?"),
                tr("\nNotiz-Eintrag Nr:  %1   loeschen?").arg(s),
       QMessageBox::Yes|QMessageBox::Default,
       QMessageBox::No|QMessageBox::Escape,
       QMessageBox::NoButton);
    if(status != QMessageBox::Yes)
        return;
   qy = "DELETE FROM mynote WHERE nd="+v->text();
   QSqlQuery query;
   query.exec(qy);
   NoteTable->removeRow(r);
}

// ------------------------
void mNote::goExit()
{
   accept();                                                   
}
