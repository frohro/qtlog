/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QString>
#include <QTabWidget>
#include <QtGui>
#include <QtSql>
#include "mediumText.h"


void mediumTxt::showLabelData(QListWidgetItem *item)
{
   QSqlQuery query;
   qy = "SELECT name,dear,well,text1,text2,text3 ";
   qy += "FROM qslk WHERE name='"+item->text()+"' AND ktype='L'";
   query.exec(qy);
   n = 0;
   while(query.next()) {
       dxccW->setText( query.value(n++).toString());
       Ldxcc = dxccW->text();
       LdearW->setText(query.value(n++).toString());
       LwellW->setText(query.value(n++).toString());
       Ltext1W->setText( query.value(n++).toString());
       Ltext2W->setText( query.value(n++).toString());
       Ltext3W->setText( query.value(n++).toString());
   }
}


void mediumTxt::showSLabelData(QListWidgetItem *item)
{
   QSqlQuery query;
   qy = "SELECT name,text1,text2 ";
   qy += "FROM qslk WHERE name='"+item->text()+"' AND ktype='LS'";
   query.exec(qy);
   n = 0;
   while(query.next()) {
       dxccW->setText( query.value(n++).toString());
       SLtext1W->setText(query.value(n++).toString());
       SLtext2W->setText(query.value(n++).toString());
   }
}


void mediumTxt::showKartData(QListWidgetItem *item)
{
   QSqlQuery query;
   qy = "SELECT name,dear,well,cqz,text1,text2,text3 ";
   qy += "FROM qslk WHERE name='"+item->text()+"' AND ktype='K'";
   query.exec(qy);
   n = 0;
   while(query.next()) {
       dxccW->setText( query.value(n++).toString());
       Kdxcc = dxccW->text();
       KdearW->setText(query.value(n++).toString());
       KwellW->setText(query.value(n++).toString());
       KcqzW->setText(query.value(n++).toString());
       Ktext1W->setText( query.value(n++).toString());
       Ktext2W->setText( query.value(n++).toString());
       Ktext3W->setText( query.value(n++).toString());
   }
}


void mediumTxt::showSKartData(QListWidgetItem *item)
{
   QSqlQuery query;
   qy = "SELECT name,text1,text2,text3 ";
   qy += "FROM qslk WHERE name='"+item->text()+"' AND ktype='KS'";
   query.exec(qy);
   n = 0;
   while(query.next()) {
       dxccW->setText( query.value(n++).toString());
       SKtext1W->setText(query.value(n++).toString());
       SKtext2W->setText(query.value(n++).toString());
       SKtext3W->setText(query.value(n++).toString());
   }
}


void mediumTxt::showSWL_LabelData(QListWidgetItem *item)
{
   QSqlQuery query;
   qy = "SELECT name,dear,well,cqz,text3 ";
   qy += "FROM qslk WHERE name='"+item->text()+"' AND ktype='LSWL'";
   query.exec(qy);
   n = 0;
   while(query.next()) {
       dxccW->setText( query.value(n++).toString());
       LSdxcc = dxccW->text();
       LSdearW->setText(query.value(n++).toString());
       LSwellW->setText(query.value(n++).toString());
       LScqzW->setText(query.value(n++).toString());
       LStext3W->setText(query.value(n++).toString());
   }
}


void mediumTxt::showSWL_KartData(QListWidgetItem *item)
{
   QSqlQuery query;
   qy = "SELECT name,dear,well,cqz,text3 ";
   qy += "FROM qslk WHERE name='"+item->text()+"' AND ktype='KSWL'";
   query.exec(qy);
   n = 0;
   while(query.next()) {
       dxccW->setText( query.value(n++).toString());
       KSdxcc = dxccW->text();
       KSdearW->setText(query.value(n++).toString());
       KSwellW->setText(query.value(n++).toString());
       KScqzW->setText(query.value(n++).toString());
       KStext3W->setText(query.value(n++).toString());
   }
}



void mediumTxt::updateLabelData()
{
    qy = "UPDATE qslk SET dear='"+LdearW->text();
    qy += "',well='"+LwellW->text();
    qy += "',text1='"+Ltext1W->text();
    qy += "',text2='"+Ltext2W->text();
    qy += "',text3='"+Ltext3W->text();
    qy += "' WHERE name='"+dxccW->text()+"' AND ktype='L'";
    QSqlQuery query(qy);
}


void mediumTxt::updateSLabelData()
{
    qy = "UPDATE qslk SET ";
    qy += "text1='"+SLtext1W->text();
    qy += "',text2='"+SLtext2W->text();
    qy += "' WHERE name='"+dxccW->text()+"' AND ktype='LS'";
    QSqlQuery query(qy);
}


void mediumTxt::updateKartData()
{
     qy = "UPDATE qslk SET dear='"+KdearW->text();
     qy += "',well='"+KwellW->text();
     qy += "',cqz='"+KcqzW->text();
     qy += "',text1='"+Ktext1W->text();
     qy += "',text2='"+Ktext2W->text();
     qy += "',text3='"+Ktext3W->text();
     qy += "' WHERE name='"+dxccW->text()+"' AND ktype='K'";
     QSqlQuery query(qy);
}


void mediumTxt::updateSKartData()
{
     qy = "UPDATE qslk SET ";
     qy += "text1='"+SKtext1W->text();
     qy += "',text2='"+SKtext2W->text();
     qy += "',text3='"+SKtext3W->text();
     qy += "' WHERE name='"+dxccW->text()+"' AND ktype='KS'";
     QSqlQuery query(qy);
}


void mediumTxt::updateSWL_LabelData()
{
     qy = "UPDATE qslk SET dear='"+LSdearW->text();
     qy += "',well='"+LSwellW->text();
     qy += "',cqz='"+LScqzW->text();
     qy += "',text3='"+LStext3W->text();
     qy += "' WHERE name='"+dxccW->text()+"' AND ktype='LSWL'";
     QSqlQuery query(qy);
}


void mediumTxt::updateSWL_KartData()
{
    qy = "UPDATE qslk SET dear='"+KSdearW->text();
    qy += "',well='"+KSwellW->text();
    qy += "',cqz='"+KScqzW->text();
    qy += "',text3='"+KStext3W->text();
    qy += "' WHERE name='"+dxccW->text()+"' AND ktype='KSWL'";
    QSqlQuery query(qy);
}



void mediumTxt::addLabelData()
{
    qy = "SELECT name FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='L'";
    QSqlQuery query(qy);
    if(query.size() != 0) {                          
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist schon vorhanden   "));
       return;
    }
    qy = "INSERT INTO qslk VALUES('"+dxccW->text();  
    qy+= "','L','"+LdearW->text();                   
    qy += "','"+LwellW->text();                      
    qy += "','";                                     
    qy += "','"+Ltext1W->text();                     
    qy += "','"+Ltext2W->text();                     
    qy += "','"+Ltext3W->text()+"')";                
    query.exec(qy);
    QListWidgetItem * nItem = new QListWidgetItem;
    nItem->setText( dxccW->text());
    dxccWidget->insertItem(-1,nItem);
    LdearW->setText("");
    LwellW->setText("");
    Ltext1W->setText("");
    Ltext2W->setText("");
    Ltext3W->setText("");
    dxccW->setText("");
    Ldxcc = dxccW->text();
}


void mediumTxt::addSLabelData()
{
    qy = "SELECT name FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='LS'";
    QSqlQuery query(qy);
    if(query.size() != 0) {                       
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist schon vorhanden   "));
       return;
    }
    qy = "INSERT INTO qslk VALUES('"+dxccW->text();  
    qy += "','LS','";                                
    qy += "','";                                     
    qy += "','";                                     
    qy += "','"+SLtext1W->text();                    
    qy += "','"+SLtext2W->text();                    
    qy += "','')";                                   
    query.exec(qy);
    QListWidgetItem * nItem = new QListWidgetItem;
    nItem->setText( dxccW->text());
    dxccWidget->insertItem(-1,nItem);
    SLtext1W->setText("");
    SLtext2W->setText("");
    dxccW->setText("");
    Ldxcc = dxccW->text();
}


void mediumTxt::addKartData()
{
    qy = "SELECT name FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='K'";
    QSqlQuery query(qy);
    if(query.size() != 0) {                       
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist schon vorhanden   "));
       return;
    }
    qy = "INSERT INTO qslk VALUES('"+dxccW->text(); 
    qy += "','K','"+KdearW->text();                 
    qy += "','"+KwellW->text();                     
    qy += "','"+KcqzW->text();                      
    qy += "','"+Ktext1W->text();                    
    qy += "','"+Ktext2W->text();                    
    qy += "','"+Ktext3W->text()+"')";               
    query.exec(qy);
    QListWidgetItem * nItem = new QListWidgetItem;
    nItem->setText( dxccW->text());
    dxccWidget->insertItem(-1,nItem);
    KdearW->setText("");
    KwellW->setText("");
    KcqzW->setText("");
    Ktext1W->setText("");
    Ktext2W->setText("");
    Ktext3W->setText("");
    dxccW->setText("");
    Kdxcc = dxccW->text();
}


void mediumTxt::addSKartData()
{
    qy = "SELECT name FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='KS'";
    QSqlQuery query(qy);
    if(query.size() != 0) {                       
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist schon vorhanden   "));
       return;
    }
    qy = "INSERT INTO qslk VALUES('"+dxccW->text(); 
    qy += "','KS','";                               
    qy += "','";                                    
    qy += "','";                                    
    qy += "','"+SKtext1W->text();                   
    qy += "','"+SKtext2W->text();                   
    qy += "','"+SKtext3W->text()+"')";              
    query.exec(qy);
    QListWidgetItem * nItem = new QListWidgetItem;
    nItem->setText( dxccW->text());
    dxccWidget->insertItem(-1,nItem);
    SKtext1W->setText("");
    SKtext2W->setText("");
    SKtext3W->setText("");
    dxccW->setText("");
}


void mediumTxt::addSWL_LabelData()
{
    qy = "SELECT name FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='LSWL'";
    QSqlQuery query(qy);
    if(query.size() != 0) {                       
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist schon vorhanden   "));
       return;
    }
    qy = "INSERT INTO qslk VALUES('"+dxccW->text(); 
    qy += "','LSWL','"+LSdearW->text();             
    qy += "','"+LSwellW->text();                    
    qy += "','"+LScqzW->text();                     
    qy += "','";                                    
    qy += "','";                                    
    qy += "','"+LStext3W->text()+"')";              
    query.exec(qy);
    QListWidgetItem * nItem = new QListWidgetItem;
    nItem->setText( dxccW->text());
    dxccWidget->insertItem(-1,nItem);
    LSdearW->setText("");
    LSwellW->setText("");
    LScqzW->setText("");
    LStext3W->setText("");
    dxccW->setText("");
    LSdxcc = dxccW->text();
}


void mediumTxt::addSWL_KartData()
{
    qy = "SELECT name FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='KSWL'";
    QSqlQuery query(qy);
    if(query.size() != 0) {                          
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist schon vorhanden   "));
       return;
    }
    qy = "INSERT INTO qslk VALUES('"+dxccW->text();  
    qy += "','KSWL','"+KSdearW->text();              
    qy += "','"+KSwellW->text();                     
    qy += "','"+KScqzW->text();                      
    qy += "','";                                     
    qy += "','";                                     
    qy += "','"+KStext3W->text()+"')";               
    query.exec(qy);
    QListWidgetItem * nItem = new QListWidgetItem;
    nItem->setText( dxccW->text());
    dxccWidget->insertItem(-1,nItem);
    KSdearW->setText("");
    KSwellW->setText("");
    KScqzW->setText("");
    KStext3W->setText("");
    dxccW->setText("");
    KSdxcc = dxccW->text();
}



void mediumTxt::delLabelData()
{
  if(dxccW->text().count() == 0) {
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist nicht selectiert   "));
      return;
  }
  qy = "DELETE FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='L'";
  QSqlQuery query(qy);
  LdearW->setText("");
  LwellW->setText("");
  Ltext1W->setText("");
  Ltext2W->setText("");
  Ltext3W->setText("");
  int r = dxccWidget->currentRow();
  dxccWidget->takeItem (r);
  dxccW->setText("");
  Ldxcc = dxccW->text();
}


void mediumTxt::delSLabelData()
{
  if(dxccW->text().count() == 0) {
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist nicht selectiert   "));
      return;
  }
  qy = "DELETE FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='LS'";
  QSqlQuery query(qy);
  int r = dxccWidget->currentRow();
  dxccWidget->takeItem (r);
  SLtext1W->setText("");
  SLtext2W->setText("");
  dxccW->setText("");
  LSdxcc = dxccW->text();
}


void mediumTxt::delKartData()
{
  if(dxccW->text().count() == 0) {
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist nicht selectiert   "));
      return;
  }
  qy = "DELETE FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='K'";
  QSqlQuery query(qy);
  int r = dxccWidget->currentRow();
  dxccWidget->takeItem (r);
  KdearW->setText("");
  KwellW->setText("");
  KcqzW->setText("");
  Ktext1W->setText("");
  Ktext2W->setText("");
  Ktext3W->setText("");
  dxccW->setText("");
  Kdxcc = dxccW->text();
}


void mediumTxt::delSKartData()
{
  if(dxccW->text().count() == 0) {
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist nicht selectiert   "));
      return;
  }
  qy = "DELETE FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='KS'";
  QSqlQuery query(qy);
  int r = dxccWidget->currentRow();
  dxccWidget->takeItem (r);
  SKtext1W->setText("");
  SKtext2W->setText("");
  SKtext3W->setText("");
  dxccW->setText("");
  KSdxcc = dxccW->text();
}


void mediumTxt::delSWL_LabelData()
{
  if(dxccW->text().count() == 0) {
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist nicht selectiert   "));
      return;
  }
  qy = "DELETE FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='LSWL'";
  QSqlQuery query(qy);
  int r = dxccWidget->currentRow();
  dxccWidget->takeItem (r);
  LSdearW->setText("");
  LSwellW->setText("");
  LScqzW->setText("");
  LStext3W->setText("");
  dxccW->setText("");
}


void mediumTxt::delSWL_KartData()
{
  if(dxccW->text().count() == 0) {
      QMessageBox::about(this,"Print Medium",
         tr("   DXCC ist nicht selectiert   "));
      return;
  }
  qy = "DELETE FROM qslk WHERE name='"+dxccW->text()+"' AND ktype='KSWL'";
  QSqlQuery query(qy);
  int r = dxccWidget->currentRow();
  dxccWidget->takeItem (r);
  KSdearW->setText("");
  KSwellW->setText("");
  KScqzW->setText("");
  KStext3W->setText("");
  dxccW->setText("");
}
