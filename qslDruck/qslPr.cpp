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
#include <QtSql>
#include <QtNetwork>
#include "qslPr.h"
#include "medium.h"
#include "mediumText.h"
#include "translate.h"
#include "../qtlogDiag/dirmngr.h"

qslPr::qslPr(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    setupAction();
}
void qslPr::setupAction()
{
   n = settings.value("FontSize").toString().toInt();
   QFont font;
   font.setPointSize(n);                               
   setFont(font);
  //-- File ---
   connect(actionENDE, SIGNAL(triggered(bool)), this, SLOT(goExit()));
   connect(ButtonEnde, SIGNAL(pressed()), this, SLOT(goExit()));
   connect(actionPrMedium, SIGNAL(triggered(bool)), this, SLOT(printMediumAc()));

   connect(actionLabel, SIGNAL(triggered(bool)), this, SLOT(LabelAc()));
   connect(actionLabelSammel, SIGNAL(triggered(bool)), this, SLOT(LabelSammelAc()));
   connect(actionLabelSwl, SIGNAL(triggered(bool)), this, SLOT(LabelSwlAc()));
   connect(actionKarte, SIGNAL(triggered(bool)), this, SLOT(KarteAc()));
   connect(actionKarteSammel, SIGNAL(triggered(bool)), this, SLOT(KarteSammelAc()));
   connect(actionKarteSwl, SIGNAL(triggered(bool)), this, SLOT(KarteSwlAc()));
   connect(actionHilfe, SIGNAL(triggered(bool)), this, SLOT(getHilfeAc()));

   connect(Lab_1, SIGNAL(pressed()), this, SLOT(LabPos1()));
   connect(Lab_2, SIGNAL(pressed()), this, SLOT(LabPos2()));
   connect(Lab_3, SIGNAL(pressed()), this, SLOT(LabPos3()));
   connect(Lab_4, SIGNAL(pressed()), this, SLOT(LabPos4()));
   connect(Lab_5, SIGNAL(pressed()), this, SLOT(LabPos5()));
   connect(Lab_6, SIGNAL(pressed()), this, SLOT(LabPos6()));
   connect(Lab_7, SIGNAL(pressed()), this, SLOT(LabPos7()));
   connect(Lab_8, SIGNAL(pressed()), this, SLOT(LabPos8()));
   connect(ButtonPrinter, SIGNAL(pressed()), this, SLOT(qslPrinter()));
   connect(CallEdit, SIGNAL(returnPressed()), this, SLOT(callEditedCb()));
   connect(prList, SIGNAL(itemPressed(QTreeWidgetItem *,int)), this,
      SLOT(itemPressedCb(QTreeWidgetItem *, int))); //QSO aus der pr_VorschlagsListe entfernen
   connect(checkBoxSqsl, SIGNAL(pressed()), this, SLOT(checkBoxSqslCb()));
   connect(checkBoxLpr, SIGNAL(pressed()), this, SLOT(checkBoxPrCb()));
   connect(checkBoxForm, SIGNAL(pressed()), this, SLOT(checkBoxFormCb()));
   connect(checkBoxOrder, SIGNAL(pressed()), this, SLOT(checkBoxOrderCb()));
   readSettings();

   //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
   db = QSqlDatabase::addDatabase("QMYSQL");
   db.setHostName(settings.value("host").toString()); 
   db.setDatabaseName(settings.value("dbname").toString());
   db.setUserName(settings.value("dbuser").toString());
   db.setPassword(settings.value("dbpasswd").toString());
   if(!db.open()) {
       qDebug() << db.lastError();                      
   }
   udpSocket = new QUdpSocket(this);                    
   udpSocket->bind(QHostAddress::LocalHost,45454);
   prList->setColumnWidth(0,100); 
   prList->setColumnWidth(1,80);  
   prList->setColumnWidth(2,80);  
   prList->setColumnWidth(3,50);  
   prList->setColumnWidth(4,50);  
   prList->setColumnWidth(5,50);  
   prList->setColumnWidth(6,100); 
   prList->setColumnWidth(7,70);  
   prList->setColumnWidth(8,50);  
   prList->setColumnWidth(9,50);  
   prList->setColumnWidth(10,70); 
   prList->setColumnWidth(11,50); 
   logFile = settings.value("Logfile").toString();            
   labelLogbook->setText(logFile);
   labelOperator->setText(settings.value("Call").toString()); 
   if(settings.value("PrtForm").toString().toInt() == 0)
      checkBoxForm->setChecked(FALSE);
   else
     checkBoxForm->setChecked(TRUE);  
   Lab_1->setPalette( QPalette( QColor(180, 210, 200) ) );    
   labelOld = Lab_1;
   labelPos = 1;                                              
   ButtonPrinter->setPalette( QPalette(QColor(180, 210, 200)));
   loadPrtMedium();               
   translate tr;
   tr.installHashTableBand();     
   qy = "SELECT cept,rufz,name,qslmgr,qsls,qslr,band,day,btime,mode,txpwr,rig,id ";
   qy += "FROM "+logFile+"om LEFT JOIN "+logFile+" ON (omid=oid) WHERE prflg='B' ORDER BY day";
   loadQsls(qy, 0);               
   cnt = 0;
   CallEdit->setFocus();
}

qslPr::~qslPr()
{
    writeSettings();
}
void qslPr::goExit()
{
    db.close();
    qApp->quit();
}
void qslPr::keyPressEvent( QKeyEvent * event )
{
   switch ( event->key() ) {
     case Qt::Key_F10 :                        
              goExit();
            break;
    default :
            break;
   }
}

void qslPr::writeSettings()
{
  settings.setValue("qslPr/Size",size());
  settings.setValue("qslPr/Properties",saveState());
}

void qslPr::readSettings()
{
  resize(settings.value("qslPr/Size",sizeHint()).toSize());
  restoreState(settings.value("qslPr/Properties").toByteArray());
}


void qslPr::qslPrinter()
{
    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() != QDialog::Accepted)
         return;
    QString t = lprtype;
    t += " : ";
    s = printer.printerName();
    t = t + s;
    labelPrinter->setText(t);
    printQSL();
}



void qslPr::LabPos1()
{
   labelOld->setPalette( QPalette(QColor(238, 238, 238))); 
   Lab_1->setPalette( QPalette(QColor(180, 210, 200)));    
   labelOld = Lab_1;
   labelPos = 1;
}
void qslPr::LabPos2()
{
   labelOld->setPalette( QPalette(QColor(238, 238, 238)));
   Lab_2->setPalette( QPalette(QColor(180, 210, 200)));
   labelOld = Lab_2;
   labelPos = 2;
}
void qslPr::LabPos3()
{
   labelOld->setPalette( QPalette(QColor(238, 238, 238)));
   Lab_3->setPalette( QPalette(QColor(180, 210, 200)));
   labelOld = Lab_3;
   labelPos = 3;
}
void qslPr::LabPos4()
{
   labelOld->setPalette( QPalette(QColor(238, 238, 238)));
   Lab_4->setPalette( QPalette(QColor(180, 210, 200)));
   labelOld = Lab_4;
   labelPos = 4;
}
void qslPr::LabPos5()
{
   labelOld->setPalette( QPalette(QColor(238, 238, 238)));
   Lab_5->setPalette( QPalette(QColor(180, 210, 200)));
   labelOld = Lab_5;
   labelPos = 5;
}
void qslPr::LabPos6()
{
   labelOld->setPalette( QPalette(QColor(238, 238, 238)));
   Lab_6->setPalette( QPalette(QColor(180, 210, 200)));
   labelOld = Lab_6;
   labelPos = 6;
}
void qslPr::LabPos7()
{
   labelOld->setPalette( QPalette(QColor(238, 238, 238)));
   Lab_7->setPalette( QPalette(QColor(180, 210, 200)));
   labelOld = Lab_7;
   labelPos = 7;
}
void qslPr::LabPos8()
{
   labelOld->setPalette( QPalette(QColor(238, 238, 238)));
   Lab_8->setPalette( QPalette(QColor(180, 210, 200)));
   labelOld = Lab_8;
   labelPos = 8;
}


void qslPr::itemPressedCb(QTreeWidgetItem *item, int col)
{
   col = 0;
   int index = prList->indexOfTopLevelItem(item);
   prList->takeTopLevelItem(index);               
   n = prList->topLevelItemCount();
   s = s.setNum(n)+QString::fromUtf8("  QSO.s nicht bestätigt");
   labelQsos->setText(s);
}

void qslPr::checkBoxOrderCb()
{
    if(checkBoxOrder->isChecked() == false) 
      checkBoxOrder->setText("Sortiere n. Call");
    else 
      checkBoxOrder->setText("Sortiere n. Datum");
}


void qslPr::checkBoxSqslCb()
{
  QString rufz;
  int cnti;
  if(checkBoxSqsl->isChecked() == false) {
    checkBoxSqsl->setText(QString::fromUtf8("Sammel-Bestätigung"));
    
    QString sCall,lastCall;
    int idx, row;
    QSqlQuery query;
    QTreeWidgetItem *Item;                       
    row = prList->topLevelItemCount();           
    cnti = 0;
    idx = 0;
    sList.clear();
    while( row != 0) {                           
      Item = prList->topLevelItem(idx);          
      rufz = Item->text(0);                      
      qy = "SELECT rufz,qsls,qslr ";
      qy += "FROM "+logFile+"om LEFT JOIN "+logFile+" ON (omid=oid) ";
      qy += "WHERE rufz LIKE '"+rufz+"%' AND prflg = 'B'";
      query.exec(qy);
      n = query.size();                          
      if(n > 1) {
         if(rufz.compare(lastCall) != 0) {
            sList.append(rufz);
            cnti += n;
         }
         lastCall = rufz;
      }
      row--;
        if(row == 0) break;
      idx++;                                    
    }
    prList->clear();
    for (int i = 0; i < sList.size(); ++i) {
       qy = "SELECT cept,rufz,name,qslmgr,qsls,qslr,band,day,btime,mode,txpwr,rig,id ";
       qy += "FROM "+logFile+"om LEFT JOIN "+logFile+" ON (omid=oid) ";
       qy += "WHERE rufz='"+sList[i]+"' AND prflg='B' ORDER BY rufz";
       loadQsls(qy,1);                  
    }
    s = s.setNum(cnti)+QString::fromUtf8("  QSO.s nicht bestätigt");
    labelQsos->setText(s);
  }
  else                                 
    checkBoxSqsl->setText(QString::fromUtf8("Einzel-Bestätigung"));
}


void qslPr::checkBoxFormCb()
{
   if(checkBoxForm->isChecked() == TRUE) {
       checkBoxForm->setText("Erzeuge PS-format");
       settings.setValue("PrtForm","0");
   }
   else {
     checkBoxForm->setText("Erzeuge PDF-format");
     settings.setValue("PrtForm","1");
   }
}

void qslPr::checkBoxPrCb()
{
  if(checkBoxLpr->isChecked() == true)
    checkBoxLpr->setText("Drucke (HOME)/tmp/qslFile.ps");
  else 
    checkBoxLpr->setText("Drucke auf Lineprinter");
}


void qslPr::printMediumAc()
{
     medium * medi = new medium();
     medi->exec();
     if( medi->prflg )         
       loadPrtMedium();         
}



void qslPr::LabelAc()                
{
     settings.setValue("Val","0");
     mediumTextEditorCb();
}
void qslPr::LabelSammelAc()          
{
     settings.setValue("Val","1");
     mediumTextEditorCb();
}
void qslPr::LabelSwlAc()             
{
     settings.setValue("Val","4");
     mediumTextEditorCb();
}
void qslPr::KarteAc()                 
{
     settings.setValue("Val","2");
     mediumTextEditorCb();
}
void qslPr::KarteSammelAc()           
{
     settings.setValue("Val","3");
     mediumTextEditorCb();
}
void qslPr::KarteSwlAc()             
{
     settings.setValue("Val","5");
     mediumTextEditorCb();
}


void qslPr::mediumTextEditorCb()
{
     mediumTxt * medT = new mediumTxt();
     medT->exec();
}


void qslPr::getHilfeAc()
{
    settings.setValue("Val","Qsl Druck");
    settings.sync();
    StartProcess("hilfedb &");
}
