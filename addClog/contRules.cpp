/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.08.2009
********************************************************************** */

#include <QtSql>
#include <QMessageBox>
#include "defs.h"
#include "contRules.h"

contRules::contRules(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    confList->setColumnWidth(0,80);
    confList->setColumnWidth(1,260);
    QSqlQuery query; 
    qy = "SELECT sel,namid,name FROM wCruls WHERE sel = 1 ORDER BY namid";
    query.exec(qy);
    while(query.next()) { 
      i = 0;
      r = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(confList);
      
      s = query.value(r).toString();
      n = query.value(r++).toInt();
      if(n != 1) {
	s += " "+query.value(r++).toString();            
        item->setText(i++,s);
      }
      else
        item->setText(i++,query.value(r++).toString());  
      item->setText(i++,query.value(r++).toString());    
    }
    h = QDateTime::currentDateTime().toUTC();            
    contestDate->setDate(h.date());
    labelRuleInfo->hide();
 
   connect (confList, SIGNAL(itemClicked(QTreeWidgetItem *,int)), this, SLOT(confListclicked(QTreeWidgetItem*,int)));
   connect (radioButtonCw, SIGNAL (pressed()), this, SLOT (radioButtonCW()));
   connect (radioButtonSsb, SIGNAL (pressed()), this, SLOT (radioButtonSSB()));
   connect (radioButtonRtty, SIGNAL (pressed()), this, SLOT (radioButtonRTTY()));
   connect (radioButtonMixd, SIGNAL (pressed()), this, SLOT (radioButtonMIXD()));
   connect (radioButtonBand_160, SIGNAL (pressed()), this, SLOT (radioButtonBandCb_160()));
   connect (radioButtonBand_80, SIGNAL (pressed()), this, SLOT (radioButtonBandCb_80()));
   connect (radioButtonBand_40, SIGNAL (pressed()), this, SLOT (radioButtonBandCb_40()));
   connect (radioButtonBand_20, SIGNAL (pressed()), this, SLOT (radioButtonBandCb_20()));
   connect (radioButtonBand_15, SIGNAL (pressed()), this, SLOT (radioButtonBandCb_15()));
   connect (radioButtonBand_10, SIGNAL (pressed()), this, SLOT (radioButtonBandCb_10()));
   connect (radioButtonBand_All, SIGNAL (pressed()), this, SLOT (radioButtonBandCb_All()));
   connect (lineContest, SIGNAL (textEdited(QString)), this, SLOT (lineContestCb(QString)));
   connect (buttonCreate, SIGNAL (pressed()), this, SLOT (buttonCreateContestCb()));
   connect(buttonExit, SIGNAL(pressed()), this, SLOT(goExit()));
/*
   connect (radioButtonRealtime, SIGNAL (pressed()), this, SLOT (radioButtonRealtimeCb()));
   connect (radioButtonFast, SIGNAL (pressed()), this, SLOT (radioButtonFastCb()));
   contestDate->setEnabled(FALSE);
   */
   tabWidgetRule->setCurrentIndex(0);
   stat = 0;
   //method = "";
}

contRules::~contRules() {
}


void contRules::radioButtonCW()
{
     confMode = radioButtonCw->text();
     radioButtonCw->setPalette( QPalette(QColor(180, 210, 200)));   
     radioButtonSsb->setPalette( QPalette(QColor(238, 238, 238)));  //
     radioButtonRtty->setPalette( QPalette(QColor(238, 238, 238))); //
     radioButtonMixd->setPalette( QPalette(QColor(238, 238, 238))); //
}
void contRules::radioButtonSSB()
{
     confMode = radioButtonSsb->text();
     radioButtonSsb->setPalette( QPalette(QColor(180, 210, 200)));  
     radioButtonCw->setPalette( QPalette(QColor(238, 238, 238)));   //
     radioButtonRtty->setPalette( QPalette(QColor(238, 238, 238))); //
     radioButtonMixd->setPalette( QPalette(QColor(238, 238, 238))); //
}
void contRules::radioButtonRTTY()
{
     confMode = radioButtonRtty->text();
     radioButtonRtty->setPalette( QPalette(QColor(180, 210, 200))); 
     radioButtonCw->setPalette( QPalette(QColor(238, 238, 238)));   //
     radioButtonSsb->setPalette( QPalette(QColor(238, 238, 238)));  //
     radioButtonMixd->setPalette( QPalette(QColor(238, 238, 238))); //
}
void contRules::radioButtonMIXD()
{
     confMode = radioButtonMixd->text();
     radioButtonMixd->setPalette( QPalette(QColor(180, 210, 200))); 
     radioButtonCw->setPalette( QPalette(QColor(238, 238, 238)));   //
     radioButtonRtty->setPalette( QPalette(QColor(238, 238, 238))); //
     radioButtonSsb->setPalette( QPalette(QColor(238, 238, 238)));  //
}


void contRules::radioButtonBandCb_160()
{
     confBand = "1820";
}
void contRules::radioButtonBandCb_80()
{
     confBand = "3510";
}
void contRules::radioButtonBandCb_40()
{
     confBand = "7010";
}
void contRules::radioButtonBandCb_20()
{
     confBand = "14010";
}
void contRules::radioButtonBandCb_15()
{
     confBand = "21010";
}
void contRules::radioButtonBandCb_10()
{
     confBand = "28010";
}
void contRules::radioButtonBandCb_All()
{
     confBand = "All";
}

void contRules::radioButtonRealtimeCb()
{
   contestDate->setEnabled(false);
}

void contRules::radioButtonFastCb()
{
   contestDate->setEnabled(true);
}



void contRules::confListclicked(QTreeWidgetItem* item,int col)
{
   int x;
   x = col;
   
     i = item->text(0).indexOf(" ");
     if(i != -1)
       namId = item->text(0).mid(i +1);
     else
        namId = item->text(0);
     
     labelContest->setText(item->text(1));
     
     QDate h = QDate::currentDate();            
     t = h.toString("dd-MM-yy");
 
     confcontestId = namId;
     lineContestID->setText(confcontestId);
     confcontestName = item->text(1);           
      
     QSqlQuery query;
     qy = "SELECT method,cprefm,contnr,pointsA,pointsB,pointsC,pointsD,pointsE,pointsF,multiA,multiB,bpts160,bpts80,bpts40,bpts20,bpts15,bpts10,increm,startday,starttime,runtime,srst,sent,rrst,qsoOrder,qtcOrder FROM wCruls WHERE namid='"+namId+"'";
     query.exec(qy);
     query.next();
     n = 0;
     method = query.value(n++).toString();             
     prefMode = query.value(n++).toString();
     cType = query.value(n++).toString();              
     
     PointsA = query.value(n++).toString();            
     PointsB = query.value(n++).toString();            
     PointsC = query.value(n++).toString();            
     PointsD = query.value(n++).toString();            
     PointsE = query.value(n++).toString();            
     PointsF = query.value(n++).toString();            
     
     MultiA = query.value(n++).toString();             
     MultiB = query.value(n++).toString();             
     
     Pts160 = query.value(n++).toString();             
     Pts80 = query.value(n++).toString();              
     Pts40 = query.value(n++).toString();              
     Pts20 = query.value(n++).toString();              
     Pts15 = query.value(n++).toString();              
     Pts10 = query.value(n++).toString();              
     
     Inc = query.value(n++).toInt();                   
     if(Inc) {
       checkBoxInc->setEnabled(TRUE);                  
       checkBoxInc->setChecked(TRUE);                  
     }
     else {
      checkBoxInc->setEnabled(FALSE);                  
      checkBoxInc->setChecked(FALSE);                  
     }

     StartDay = query.value(n++).toString();           
     StartTime = query.value(n++).toString();          
     wContestStart->setText(StartTime);       
      
     RunTime = query.value(n++).toString();            
     lineRuntime->setText(RunTime);
     
     srst = query.value(n++).toString();
     sent = query.value(n++).toString();
     rrst = query.value(n++).toString();
     
     qsoOrder = query.value(n++).toString();           
     qtcOrder = query.value(n++).toString();           
     lineContest->setText("");
     labelRuleInfo->show();
     QDate d;
     int year, month, day;
     i = StartDay.indexOf("-");      
     year = StartDay.left(i).toInt();                   
     month = StartDay.mid(i +1,2).toInt();              
     day = StartDay.mid(i +4).toInt();                  
     d.setDate(year,month,day);
     contestDate->setDate(d);
     
     wPrefMode->setText(prefMode);
     wMethod->setText(method);
     
     switch(cType.toInt()) {
     case XMAS : {                                     
                 tabWidgetRule->setCurrentIndex(0);    
                 labelPoints_A->setText("QSO");
                 wPoints_A->setText(PointsA);          
                 labelPoints_B->setText("");
                 wPoints_B->setText("");
                 labelPoints_C->setText("");
                 wPoints_C->setText("");
                 
                 labelMulti_A->setText("Pref");        
                 wMulti_A->setText(MultiA);            
		 labelMultiAinfo->setText("");         
                 labelMulti_B->setText("DOK");         
                 wMulti_B->setText(MultiB);            
                 
                 radioButtonBand_160->setEnabled(false); 
                 radioButtonBand_160->setPalette( QPalette(QColor(238, 238, 238)));
                 radioButtonBand_160->setText(""); 
                 radioButtonBand_80->setEnabled(true);
                 radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));  
                 radioButtonBand_80->setText("80 m"); 
                 radioButtonBand_80->setChecked(TRUE);                              
                 confBand = "3510";
                 radioButtonBand_40->setEnabled(true);
                 radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));  
                 radioButtonBand_40->setText("40 m"); 
                 radioButtonBand_20->setEnabled(false);
                 radioButtonBand_20->setPalette( QPalette(QColor(238, 238, 238)));
                 radioButtonBand_20->setText(""); 
                 radioButtonBand_15->setEnabled(false);
                 radioButtonBand_15->setPalette( QPalette(QColor(238, 238, 238)));
                 radioButtonBand_15->setText(""); 
                 radioButtonBand_10->setEnabled(false);
                 radioButtonBand_10->setPalette( QPalette(QColor(238, 238, 238)));
                 radioButtonBand_10->setText(""); 
                 radioButtonBand_All->setEnabled(false);
		 radioButtonBand_All->setPalette( QPalette(QColor(238, 238, 238)));
                 radioButtonBand_All->setText(""); 
                 radioButtonSsb->setChecked(TRUE);                                  
		 radioButtonSSB();
		 confMode = radioButtonSsb->text();
		    
		 wBand160->setText(Pts160);                                         
		 wBand80->setText(Pts80);
		 wBand40->setText(Pts40);
		 wBand20->setText(Pts20);
		 wBand15->setText(Pts15);
		 wBand10->setText(Pts10);
               }
          break;
    case HHCont : {                                     
                 tabWidgetRule->setCurrentIndex(0);     
                 labelPoints_A->setText("Qso");
                 wPoints_A->setText(PointsA);           
                 labelPoints_B->setText("");
                 wPoints_B->setText("");
                 labelPoints_C->setText("");
                 wPoints_C->setText("");
                 
                 labelMulti_A->setText("DXCC");         
                 wMulti_A->setText(MultiA);             
		 labelMultiAinfo->setText("");          
                 labelMulti_B->setText("DOK");          
                 wMulti_B->setText(MultiB);             
                 
                 radioButtonBand_160->setEnabled(false); 
                 radioButtonBand_160->setPalette( QPalette(QColor(238, 238, 238)));
                 radioButtonBand_160->setText(""); 
                 radioButtonBand_80->setEnabled(true);
                 radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));
                 radioButtonBand_80->setText("80 m"); 
                 radioButtonBand_80->setChecked(TRUE);   
                 confBand = "3510";
                 radioButtonBand_40->setEnabled(true);
                 radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));
                 radioButtonBand_40->setText("40 m"); 
                 radioButtonBand_20->setEnabled(false);
                 radioButtonBand_20->setPalette( QPalette(QColor(238, 238, 238)));
                 radioButtonBand_20->setText(""); 
                 radioButtonBand_15->setEnabled(false);
                 radioButtonBand_15->setPalette( QPalette(QColor(238, 238, 238)));
                 radioButtonBand_15->setText(""); 
                 radioButtonBand_10->setEnabled(false);
                 radioButtonBand_10->setPalette( QPalette(QColor(238, 238, 238)));
                 radioButtonBand_10->setText(""); 
		 
                 radioButtonBand_All->setEnabled(false);
		 radioButtonBand_All->setPalette( QPalette(QColor(238, 238, 238)));
                 radioButtonBand_All->setText(""); 
                 radioButtonSsb->setChecked(TRUE);       
		 radioButtonCW();
                 confMode = radioButtonSsb->text();
		 
		 wBand160->setText(Pts160);
		 wBand80->setText(Pts80);
		 wBand40->setText(Pts40);
		 wBand20->setText(Pts20);
		 wBand15->setText(Pts15);
		 wBand10->setText(Pts10);
               }
          break;
    case HNY : {                                         
                tabWidgetRule->setCurrentIndex(0);       
                labelPoints_A->setText("QSO");           
                wPoints_A->setText(PointsA);             
                labelPoints_B->setText("");
                wPoints_B->setText("");
                labelPoints_C->setText("");
                wPoints_C->setText("");
                
                labelMulti_A->setText("MitgldNr");       
                wMulti_A->setText(MultiA);               
		labelMultiAinfo->setText("");            
                labelMulti_B->setText("");
                wMulti_B->setText(""); 
                
                radioButtonBand_160->setEnabled(false);
                radioButtonBand_160->setPalette( QPalette(QColor(238, 238, 238)));
                radioButtonBand_160->setText(""); 
                radioButtonBand_80->setEnabled(true);
                radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));
                radioButtonBand_80->setText("80 m"); 
                radioButtonBand_80->setChecked(true);    
                radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));
                confBand = "3510";
                radioButtonBand_40->setEnabled(TRUE);
                radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));
                radioButtonBand_40->setText("40 m"); 
                radioButtonBand_20->setEnabled(true);
                radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));
                radioButtonBand_20->setText("20 m"); 
                radioButtonBand_15->setEnabled(false);
                radioButtonBand_15->setPalette( QPalette(QColor(238, 238, 238)));
                radioButtonBand_15->setText(""); 
                radioButtonBand_10->setEnabled(false);
                radioButtonBand_10->setPalette( QPalette(QColor(238, 238, 238)));
                radioButtonBand_10->setText(""); 
                radioButtonBand_All->setEnabled(false);
                radioButtonBand_All->setText(""); 
                radioButtonCw->setChecked(TRUE);         
		radioButtonCW();                         
                confMode = radioButtonCw->text();
		
		wBand160->setText(Pts160);
	        wBand80->setText(Pts80);
                wBand40->setText(Pts40);
                wBand20->setText(Pts20);
                wBand15->setText(Pts15);
                wBand10->setText(Pts10);
              }
         break;
    
    case DARC10M :{                                     
                   tabWidgetRule->setCurrentIndex(0);   
                   labelPoints_A->setText("");          
                   wPoints_A->setText(PointsA);
                   labelPoints_B->setText("");
                   wPoints_B->setText("");
                   labelPoints_C->setText("");
                   wPoints_C->setText("");
                   
                   labelMulti_A->setText("Dxcc/WAE");   
                   wMulti_A->setText(MultiA);           
		   labelMultiAinfo->setText("");        
                   labelMulti_B->setText("DOK");        
                   wMulti_B->setText(MultiB);           
                   
                   radioButtonBand_160->setEnabled(false); 
                   radioButtonBand_160->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_160->setText(""); 
                   radioButtonBand_80->setEnabled(false);
                   radioButtonBand_80->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_80->setText("");
                   radioButtonBand_40->setEnabled(false);
                   radioButtonBand_40->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_40->setText(""); 
                   radioButtonBand_20->setEnabled(false);
                   radioButtonBand_20->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_20->setText(""); 
                   radioButtonBand_15->setEnabled(false);
                   radioButtonBand_15->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_15->setText(""); 
                   radioButtonBand_10->setEnabled(true);
                   radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));
                   radioButtonBand_10->setText("10 m"); 
                   radioButtonBand_10->setChecked(TRUE);      
                   confBand = "28010";
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   radioButtonSsb->setChecked(TRUE);          
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                 }
            break;
	    
      case CQPTY :{                                           
                   tabWidgetRule->setCurrentIndex(0);
                   labelPoints_A->setText("je Qso");          
                   wPoints_A->setText(PointsA);
                   labelPoints_B->setText("");
                   wPoints_B->setText("");
                   labelPoints_C->setText("");
                   wPoints_C->setText("");
                   
                   labelMulti_A->setText("je Praefix"); 
                   wMulti_A->setText(MultiA);           
		   labelMultiAinfo->setText("");        
                   labelMulti_B->setText("");           
                   wMulti_B->setText(MultiB);           
                   
                   radioButtonBand_160->setEnabled(false); 
                   radioButtonBand_160->setPalette( QPalette(QColor(238, 238, 238))); 
                   radioButtonBand_160->setText(""); 
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80 m");
		   radioButtonBand_80->setChecked(TRUE);                             
                   confBand = "3510";
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(true);
                   radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_20->setText("20 m"); 
                   radioButtonBand_15->setEnabled(true);
                   radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_15->setText("15 m"); 
                   radioButtonBand_10->setEnabled(true);
                   radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_10->setText("10 m"); 
		   
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   radioButtonSsb->setChecked(TRUE);                                 
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                 }
            break;
	    
      case WAG : {                                      
                   tabWidgetRule->setCurrentIndex(0);   
                   labelPoints_A->setText("Dxcc");      
                   wPoints_A->setText(PointsA);
                   labelPoints_B->setText("EU");        
                   wPoints_B->setText(PointsB);         
                   labelPoints_C->setText("HomeDX");    
                   wPoints_C->setText(PointsC);         
                   
                   labelMulti_A->setText("Dxcc");       
                   wMulti_A->setText(MultiA);           
		   labelMultiAinfo->setText("");        
                   labelMulti_B->setText("");           
                   wMulti_B->setText("");               
                   
                   radioButtonBand_160->setEnabled(false); 
                   radioButtonBand_160->setPalette( QPalette(QColor(238, 238, 238))); 
                   radioButtonBand_160->setText(""); 
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80 m");
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(true);
                   radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));
                   radioButtonBand_20->setText("20 m"); 
                   radioButtonBand_20->setChecked(TRUE);
                   confBand = "14010";
                   radioButtonBand_15->setEnabled(true);
                   radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));
                   radioButtonBand_15->setText("15 m"); 
                   radioButtonBand_10->setEnabled(true);
                   radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));
                   radioButtonBand_10->setText("10 m"); 
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   radioButtonSsb->setChecked(TRUE);          
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                }
            break;    
            
      case ARRLDX : {                                        
                     tabWidgetRule->setCurrentIndex(0);      
                     labelPoints_A->setText("QSO");          
                     wPoints_A->setText(PointsA);            
                     labelPoints_B->setText(""); 
                     wPoints_B->setText(PointsB);
                     labelPoints_C->setText("");
                     wPoints_C->setText(PointsC);
                     
                     labelMulti_A->setText("US states");     
                     wMulti_A->setText(MultiA);              
                     labelMulti_B->setText("VE province");   
                     wMulti_B->setText(MultiB);              
                     
                     radioButtonBand_160->setEnabled(true); 
                     radioButtonBand_160->setPalette( QPalette(QColor(180, 210, 200)));  
                     radioButtonBand_160->setText("160"); 
                     radioButtonBand_80->setEnabled(true);
                     radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));   
                     radioButtonBand_80->setText("80 m");
                     radioButtonBand_40->setEnabled(true);
                     radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));   
                     radioButtonBand_40->setText("40 m"); 
                     radioButtonBand_20->setEnabled(true);
                     radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));   
                     radioButtonBand_20->setText("20 m"); 
                     radioButtonBand_20->setChecked(TRUE);
                     confBand = "14010";
                     radioButtonBand_15->setEnabled(true);
                     radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));   
                     radioButtonBand_15->setText("15 m"); 
                     radioButtonBand_10->setEnabled(true);
                     radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));   
                     radioButtonBand_10->setText("10 m"); 
                     radioButtonBand_All->setEnabled(true);
                     radioButtonBand_All->setPalette( QPalette(QColor(180, 210, 200)));  
                     radioButtonBand_All->setText("All"); 
                     radioButtonSsb->setChecked(TRUE);                                   
		     radioButtonSSB();
                     confMode = radioButtonSsb->text();
		     
                     wBand160->setText(Pts160);
                     wBand80->setText(Pts80);
                     wBand40->setText(Pts40);
                     wBand20->setText(Pts20);
                     wBand15->setText(Pts15);
                     wBand10->setText(Pts10);
                }
                break;
		
      case CQWPX : {                                           
                     tabWidgetRule->setCurrentIndex(1);        
                     labelPoints_A->setText("");               
                     wPoints_A2->setText("1");
                     labelPoints_B->setText(""); 
                     wPoints_B2->setText("2");
                     labelPoints_C->setText("");
		     wPoints_C2->setText("6");
		     wPoints_D2->setText("1");
		     wPoints_E2->setText("3");
		     
		     wPrefMode2->setText(prefMode);
		     
                     
                     labelMulti_A->setText("Prefix");                 
                     wMulti_A->setText(MultiA);                       
		     labelMultiAinfo->setText("Pro Prefix 1 mal");    
                     labelMulti_B->setText("");
                     wMulti_B->setText(MultiB);
                     wMulti_A2->setText(MultiA);               
                     
                     radioButtonBand_160->setEnabled(true); 
                     radioButtonBand_160->setPalette( QPalette(QColor(180, 210, 200)));  
                     radioButtonBand_160->setText("160"); 
                     radioButtonBand_80->setEnabled(true);
                     radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));   
                     radioButtonBand_80->setText("80 m");
                     radioButtonBand_40->setEnabled(true);
                     radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));   
                     radioButtonBand_40->setText("40 m"); 
                     radioButtonBand_20->setEnabled(true);
                     radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));   
                     radioButtonBand_20->setText("20 m"); 
                     radioButtonBand_20->setChecked(TRUE);
                     confBand = "14010";
                     radioButtonBand_15->setEnabled(true);
                     radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));   
                     radioButtonBand_15->setText("15 m"); 
                     radioButtonBand_10->setEnabled(true);
                     radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));   
                     radioButtonBand_10->setText("10 m"); 
                     radioButtonBand_All->setEnabled(true);
                     radioButtonBand_All->setPalette( QPalette(QColor(180, 210, 200)));   
                     radioButtonBand_All->setText("All"); 
                     radioButtonSsb->setChecked(TRUE);                                    
		     radioButtonSSB();
                     confMode =  radioButtonSsb->text();
		     
                     wBand160->setText(Pts160);
                     wBand80->setText(Pts80);
                     wBand40->setText(Pts40);
                     wBand20->setText(Pts20);
                     wBand15->setText(Pts15);
                     wBand10->setText(Pts10);
                }
                break;
     case CQWAEDC : {                                     
                    tabWidgetRule->setCurrentIndex(0);
                    labelPoints_A->setText("QSO je Band");                  
                    wPoints_A->setText(PointsA);
                    labelPoints_B->setText(""); 
                    wPoints_B->setText(PointsB);
                    labelPoints_C->setText("");
                    wPoints_C->setText(PointsC);
                     
                    labelMulti_A->setText("DXCC");                          
                    wMulti_A->setText(MultiA);                              
		    labelMultiAinfo->setText("");                           
                    labelMulti_B->setText("");
                    wMulti_B->setText(MultiB);
                    
                    radioButtonBand_160->setEnabled(false); 
		    radioButtonBand_160->setPalette(QPalette(QColor(238, 238, 238)));   
                    radioButtonBand_160->setText(""); 
                    radioButtonBand_80->setEnabled(true);
                    radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));   
                    radioButtonBand_80->setText("80 m");
                    radioButtonBand_40->setEnabled(true);
                    radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));   
                    radioButtonBand_40->setText("40 m"); 
                    radioButtonBand_20->setEnabled(true);
                    radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));   
                    radioButtonBand_20->setText("20 m"); 
                    radioButtonBand_20->setChecked(TRUE);
                    confBand = "14010";
                    radioButtonBand_15->setEnabled(true);
                    radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));   
                    radioButtonBand_15->setText("15 m"); 
                    radioButtonBand_10->setEnabled(true);
                    radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));   
                    radioButtonBand_10->setText("10 m"); 
                    radioButtonBand_All->setEnabled(true);
		    radioButtonBand_All->setPalette(QPalette(QColor(238, 238, 238)));   
		    
                    radioButtonBand_All->setText(""); 
                    radioButtonSsb->setChecked(TRUE);                                   
		    radioButtonSSB();
                    confMode = radioButtonSsb->text();
		     
                    wBand160->setText(Pts160);
                    wBand80->setText(Pts80);
                    wBand40->setText(Pts40);
                    wBand20->setText(Pts20);
                    wBand15->setText(Pts15);
                    wBand10->setText(Pts10);
              }
              break;
     case CQWWDX : {                                              
                   tabWidgetRule->setCurrentIndex(0);             
                   labelPoints_A->setText("DX");                  
                   wPoints_A->setText(PointsA);                   
                   labelPoints_B->setText("EU");                  
                   wPoints_B->setText(PointsB);                   
                   labelPoints_C->setText("HomeDx");              
                   wPoints_C->setText(PointsC);                   
                   
                   labelMulti_A->setText("Dxcc");                 
                   wMulti_A->setText(MultiA);                     
		   labelMultiAinfo->setText("");                  
                   labelMulti_B->setText("CQ zone");              
                   wMulti_B->setText(MultiB);                     
                   
                   radioButtonBand_160->setEnabled(true); 
                   radioButtonBand_160->setPalette( QPalette(QColor(180, 210, 200))); 
                   radioButtonBand_160->setText("160");
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80 m");
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(true);
                   radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_20->setText("20 m"); 
                   radioButtonBand_20->setChecked(TRUE);
                   confBand = "14010";
                   radioButtonBand_15->setEnabled(true);
                   radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_15->setText("15 m"); 
                   radioButtonBand_10->setEnabled(true);
                   radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_10->setText("10 m"); 
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   
                   radioButtonSsb->setChecked(TRUE);          
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                }
            break;
     case DTC : {                                             
                   tabWidgetRule->setCurrentIndex(0);         
                   labelPoints_A->setText("QSO");             
                   wPoints_A->setText(PointsA);               
                   labelPoints_B->setText("ClubStation");     
                   wPoints_B->setText(PointsB);               
                   labelPoints_C->setText("");                
                   wPoints_C->setText(PointsC);               
                   
                   labelMulti_A->setText("");                 
                   wMulti_A->setText(MultiA);                 
		   labelMultiAinfo->setText("");              
                   labelMulti_B->setText("");                 
                   wMulti_B->setText(MultiB);                 
                   
                   radioButtonBand_160->setEnabled(false); 
                   radioButtonBand_160->setPalette(QPalette(QColor(238, 238, 238))); 
                   radioButtonBand_160->setText("");
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80 m");
                   radioButtonBand_80->setChecked(TRUE);
                   confBand = "3510";
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(false);
                   radioButtonBand_20->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_20->setText(""); 
                   radioButtonBand_15->setEnabled(false);
                   radioButtonBand_15->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_15->setText(""); 
                   radioButtonBand_10->setEnabled(false);
                   radioButtonBand_10->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_10->setText(""); 
		   
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   
                   radioButtonCw->setChecked(TRUE);                                
		   radioButtonCW();
                   confMode = radioButtonCw->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                }
            break;
  case AGCWHTP : {                                            
                   tabWidgetRule->setCurrentIndex(2);         
		   labelHeader->setText("for classes");
		   
                   labelPoint3A->setText("Class AA");         
                   wPoint3A->setText(PointsA);      
		   
                   labelPoint3B->setText("Class AB");         
                   wPoint3B->setText(PointsB);                
                   labelPoint3C->setText("class AC");         
                   wPoint3C->setText(PointsC);                
		   labelPoint3D->setText("class BB");         
		   wPoint3D->setText(PointsD);                
		   labelPoint3E->setText("class BC");         
		   wPoint3E->setText(PointsE);                
		   labelPoint3F->setText("class CC");         
		   wPoint3F->setText(PointsF);                
                   
                   labelMulti3A->setText("");                 
                   wMulti3A->setText(MultiA);                 
		   labelMultiAinfo->setText("");              
                   labelMulti3B->setText("");                 
                   wMulti3B->setText(MultiB);                 
		   
		   wPrefMode3->setText(prefMode);
                   
                   radioButtonBand_160->setEnabled(false); 
                   radioButtonBand_160->setPalette(QPalette(QColor(238, 238, 238))); 
                   radioButtonBand_160->setText("");
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80 m");
                   radioButtonBand_80->setChecked(TRUE);
                   confBand = "3510";
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(false);
                   radioButtonBand_20->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_20->setText(""); 
                   radioButtonBand_15->setEnabled(false);
                   radioButtonBand_15->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_15->setText(""); 
                   radioButtonBand_10->setEnabled(false);
                   radioButtonBand_10->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_10->setText(""); 
		   
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   
                   radioButtonCw->setChecked(TRUE);                             
		   radioButtonCW();
		   confMode = radioButtonCw->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                }
            break;
    case UBADX : {                                                        
                    tabWidgetRule->setCurrentIndex(0);
                    labelPoints_A->setText("ON Station");                 
                    wPoints_A->setText(PointsA);                          
                    labelPoints_B->setText("EU Station"); 
                    wPoints_B->setText(PointsB);                          
                    labelPoints_C->setText("Other");
                    wPoints_C->setText(PointsC);                          
                     
                    labelMulti_A->setText("ON propvince");                
                    wMulti_A->setText(MultiA);                            
		    labelMultiAinfo->setText("");                         
                    labelMulti_B->setText("ON prefix");
                    wMulti_B->setText(MultiB);                            
                    
                    radioButtonBand_160->setEnabled(false); 
		    radioButtonBand_160->setPalette(QPalette(QColor(238, 238, 238)));   
                    radioButtonBand_160->setText(""); 
                    radioButtonBand_80->setEnabled(true);
                    radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));   
                    radioButtonBand_80->setText("80 m");
                    radioButtonBand_40->setEnabled(true);
                    radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));   
                    radioButtonBand_40->setText("40 m"); 
                    radioButtonBand_20->setEnabled(true);
                    radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));   
                    radioButtonBand_20->setText("20 m"); 
                    radioButtonBand_20->setChecked(TRUE);
                    confBand = "14010";
                    radioButtonBand_15->setEnabled(true);
                    radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));   
                    radioButtonBand_15->setText("15 m"); 
                    radioButtonBand_10->setEnabled(true);
                    radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));   
                    radioButtonBand_10->setText("10 m"); 
                    radioButtonBand_All->setEnabled(true);
		    radioButtonBand_All->setPalette(QPalette(QColor(238, 238, 238)));   
		    
                    radioButtonBand_All->setText(""); 
                    radioButtonSsb->setChecked(TRUE);                                   
		    radioButtonSSB();
                    confMode = radioButtonSsb->text();
		     
                    wBand160->setText(Pts160);
                    wBand80->setText(Pts80);
                    wBand40->setText(Pts40);
                    wBand20->setText(Pts20);
                    wBand15->setText(Pts15);
                    wBand10->setText(Pts10);
              }
              break;
	      
   case UBASP :  {                                        
                   tabWidgetRule->setCurrentIndex(0);     
                   labelPoints_A->setText("Qso");
                   wPoints_A->setText(PointsA);           
                   labelPoints_B->setText("");
                   wPoints_B->setText("");
                   labelPoints_C->setText("");
                   wPoints_C->setText("");
                   
                   labelMulti_A->setText("Province");     
                   wMulti_A->setText(MultiA);             
		   labelMultiAinfo->setText("");          
                   labelMulti_B->setText("");             
                   wMulti_B->setText(MultiB);             
                   
                   radioButtonBand_160->setEnabled(false); 
                   radioButtonBand_160->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_160->setText(""); 
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));
                   radioButtonBand_80->setText("80 m"); 
                   radioButtonBand_80->setChecked(TRUE);   
                   confBand = "3510";
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_40->setText(""); 
                   radioButtonBand_20->setEnabled(false);
                   radioButtonBand_20->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_20->setText(""); 
                   radioButtonBand_15->setEnabled(false);
                   radioButtonBand_15->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_15->setText(""); 
                   radioButtonBand_10->setEnabled(false);
                   radioButtonBand_10->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_10->setText(""); 
		 
                   radioButtonBand_All->setEnabled(false);
		   radioButtonBand_All->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_All->setText(""); 
                   radioButtonSsb->setChecked(TRUE);       
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		 
		   wBand160->setText(Pts160);
		   wBand80->setText(Pts80);
		   wBand40->setText(Pts40);
		   wBand20->setText(Pts20);
		   wBand15->setText(Pts15);
		   wBand10->setText(Pts10);
                 }
           break;
	     
   case UBALB :  {                                        
                   tabWidgetRule->setCurrentIndex(0);     
                   labelPoints_A->setText("Qso");
                   wPoints_A->setText(PointsA);           
                   labelPoints_B->setText("");
                   wPoints_B->setText("");
                   labelPoints_C->setText("");
                   wPoints_C->setText("");
                   
                   labelMulti_A->setText("Province");     
                   wMulti_A->setText(MultiA);             
		   labelMultiAinfo->setText("");          
                   labelMulti_B->setText("");             
                   wMulti_B->setText(MultiB);             
                   
                   radioButtonBand_160->setEnabled(true); 
                   radioButtonBand_160->setPalette( QPalette(QColor(180, 210, 200)));
                   radioButtonBand_160->setText("160 m"); 
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));
                   radioButtonBand_80->setText("80 m"); 
                   radioButtonBand_80->setChecked(TRUE);   
                   confBand = "3510";
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(false);
                   radioButtonBand_20->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_20->setText(""); 
                   radioButtonBand_15->setEnabled(false);
                   radioButtonBand_15->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_15->setText(""); 
                   radioButtonBand_10->setEnabled(false);
                   radioButtonBand_10->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_10->setText(""); 
		 
                   radioButtonBand_All->setEnabled(false);
		   radioButtonBand_All->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_All->setText(""); 
                   radioButtonSsb->setChecked(TRUE);       
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		 
		   wBand160->setText(Pts160);
		   wBand80->setText(Pts80);
		   wBand40->setText(Pts40);
		   wBand20->setText(Pts20);
		   wBand15->setText(Pts15);
		   wBand10->setText(Pts10);
                 }
           break;
	     
   case ONCONT :  {                                       
                   tabWidgetRule->setCurrentIndex(0); 
                   labelPoints_A->setText("Qso");
                   wPoints_A->setText(PointsA);           
                   labelPoints_B->setText("");
                   wPoints_B->setText("");
                   labelPoints_C->setText("");
                   wPoints_C->setText("");
                   
                   labelMulti_A->setText("Province");     
                   wMulti_A->setText(MultiA);             
		   labelMultiAinfo->setText("");          
                   labelMulti_B->setText("");             
                   wMulti_B->setText(MultiB);             
                   
                   radioButtonBand_160->setEnabled(false); 
                   radioButtonBand_160->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_160->setText(""); 
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));
                   radioButtonBand_80->setText("80 m"); 
                   radioButtonBand_80->setChecked(TRUE);   
                   confBand = "3510";
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_40->setText(""); 
                   radioButtonBand_20->setEnabled(false);
                   radioButtonBand_20->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_20->setText(""); 
                   radioButtonBand_15->setEnabled(false);
                   radioButtonBand_15->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_15->setText(""); 
                   radioButtonBand_10->setEnabled(false);
                   radioButtonBand_10->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_10->setText(""); 
		 
                   radioButtonBand_All->setEnabled(false);
		   radioButtonBand_All->setPalette( QPalette(QColor(238, 238, 238)));
                   radioButtonBand_All->setText(""); 
                   radioButtonSsb->setChecked(TRUE);       
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		 
		   wBand160->setText(Pts160);
		   wBand80->setText(Pts80);
		   wBand40->setText(Pts40);
		   wBand20->setText(Pts20);
		   wBand15->setText(Pts15);
		   wBand10->setText(Pts10);
                 }
           break;
	      
     case REFC : {                                                
                   tabWidgetRule->setCurrentIndex(0);             
                   labelPoints_A->setText("F, TM, TK");           
                   wPoints_A->setText(PointsA);                   
                   labelPoints_B->setText("Overseas Pref");            
                   wPoints_B->setText(PointsB);                   
                   labelPoints_C->setText("");                    
                   wPoints_C->setText(PointsC);                   
                   
                   labelMulti_A->setText("Departmens");           
                   wMulti_A->setText(MultiA);                
		   labelMultiAinfo->setText("");                  
                   labelMulti_B->setText("Overseas dxcc");        
                   wMulti_B->setText(MultiB);                     
                   
                   radioButtonBand_160->setEnabled(true); 
                   radioButtonBand_160->setPalette( QPalette(QColor(238, 238, 238))); 
                   radioButtonBand_160->setText("");
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80 m");
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(true);
                   radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_20->setText("20 m"); 
                   radioButtonBand_20->setChecked(TRUE);
                   confBand = "14010";
                   radioButtonBand_15->setEnabled(true);
                   radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_15->setText("15 m"); 
                   radioButtonBand_10->setEnabled(true);
                   radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_10->setText("10 m"); 
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   
                   radioButtonSsb->setChecked(TRUE);              
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                }
            break;
 case RSGB160M : {                                                
                   tabWidgetRule->setCurrentIndex(0);             
                   labelPoints_A->setText("Qso");                 
                   wPoints_A->setText(PointsA);                   
                   labelPoints_B->setText("for first district +");     
                   wPoints_B->setText(PointsB);                   
                   labelPoints_C->setText("");                    
                   wPoints_C->setText(PointsC);                   
		   //
                   labelMulti_A->setText("");                     
                   wMulti_A->setText(MultiA);                
		   labelMultiAinfo->setText("");                  
                   labelMulti_B->setText(""); 
                   wMulti_B->setText(MultiB);
                   
                   radioButtonBand_160->setEnabled(true); 
                   radioButtonBand_160->setPalette( QPalette(QColor(180, 210, 200))); 
                   radioButtonBand_160->setText("160");
		   radioButtonBand_160->setChecked(TRUE);
		   confBand = "1810";
                   radioButtonBand_80->setEnabled(false);
                   radioButtonBand_80->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_80->setText("");
                   radioButtonBand_40->setEnabled(false);
                   radioButtonBand_40->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_40->setText(""); 
                   radioButtonBand_20->setEnabled(false);
                   radioButtonBand_20->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_20->setText("");
                   radioButtonBand_15->setEnabled(false);
                   radioButtonBand_15->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_15->setText(""); 
                   radioButtonBand_10->setEnabled(false);
                   radioButtonBand_10->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_10->setText(""); 
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   
                   radioButtonSsb->setChecked(TRUE);             
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                }
            break;
	    
  case CQWW160M : {                                               
                   tabWidgetRule->setCurrentIndex(0);             
                   labelPoints_A->setText("Own country");         
                   wPoints_A->setText(PointsA);                   
                   labelPoints_B->setText("Own Continent");       
                   wPoints_B->setText(PointsB);                   
                   labelPoints_C->setText("Other continents");    
                   wPoints_C->setText(PointsC);                   
		   //
                   labelMulti_A->setText("State / Province");                     
                   wMulti_A->setText(MultiA);                
		   labelMultiAinfo->setText("");                  
                   labelMulti_B->setText(""); 
                   wMulti_B->setText(MultiB);
                   
                   radioButtonBand_160->setEnabled(true); 
                   radioButtonBand_160->setPalette( QPalette(QColor(180, 210, 200))); 
                   radioButtonBand_160->setText("160");
		   radioButtonBand_160->setChecked(TRUE);
		   confBand = "1810";
                   radioButtonBand_80->setEnabled(false);
                   radioButtonBand_80->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_80->setText("");
                   radioButtonBand_40->setEnabled(false);
                   radioButtonBand_40->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_40->setText(""); 
                   radioButtonBand_20->setEnabled(false);
                   radioButtonBand_20->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_20->setText("");
                   radioButtonBand_15->setEnabled(false);
                   radioButtonBand_15->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_15->setText(""); 
                   radioButtonBand_10->setEnabled(false);
                   radioButtonBand_10->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_10->setText(""); 
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   
                   radioButtonSsb->setChecked(TRUE);             
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
		   
		   qy = "SELECT cqz FROM tla WHERE la='"+settings.value("Dxcc").toString()+"'";  
		   QSqlQuery query(qy);
		   query.next();
		   sent = query.value(0).toString();
                }
            break;
  case NASPRINT : {                                               
                   tabWidgetRule->setCurrentIndex(0);             
                   labelPoints_A->setText("Qso");                 
                   wPoints_A->setText(PointsA);                   
                   labelPoints_B->setText("");                    
                   wPoints_B->setText(PointsB);                   //
                   labelPoints_C->setText("");                    
                   wPoints_C->setText(PointsC);                   
		   //
                   labelMulti_A->setText("NA dxcc country");      
                   wMulti_A->setText(MultiA);                     
		   labelMultiAinfo->setText("without W / VE");    
                   labelMulti_B->setText("state / prov"); 
                   wMulti_B->setText(MultiB);                     
                   
                   radioButtonBand_160->setEnabled(false); 
                   radioButtonBand_160->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_160->setText("");
		   //radioButtonBand_160->setChecked(false);
                   radioButtonBand_80->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80m");
		   radioButtonBand_80->setChecked(TRUE);
		   confBand = "3520";
                   radioButtonBand_40->setEnabled(TRUE);
                   radioButtonBand_40->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_40->setText("40m"); 
                   radioButtonBand_20->setEnabled(TRUE);
                   radioButtonBand_20->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_20->setText("20m");
                   radioButtonBand_15->setEnabled(false);
                   radioButtonBand_15->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_15->setText(""); 
                   radioButtonBand_10->setEnabled(false);
                   radioButtonBand_10->setPalette(QPalette(QColor(238, 238, 238)));  
                   radioButtonBand_10->setText(""); 
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   
                   radioButtonSsb->setChecked(TRUE);             
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                }
            break;
      case PACC : {                                               
                   tabWidgetRule->setCurrentIndex(0);             
                   labelPoints_A->setText("Qso");                 
                   wPoints_A->setText(PointsA);                   
                   labelPoints_B->setText("");                    
                   wPoints_B->setText(PointsB);                   //
                   labelPoints_C->setText("");                    
                   wPoints_C->setText(PointsC);                   
                   
                   labelMulti_A->setText("Province");             
                   wMulti_A->setText(MultiA);                     
		   labelMultiAinfo->setText("");                  
                   labelMulti_B->setText("");                     
                   wMulti_B->setText(MultiB);                     
                   
                   radioButtonBand_160->setEnabled(true); 
                   radioButtonBand_160->setPalette( QPalette(QColor(180, 210, 200))); 
                   radioButtonBand_160->setText("160");
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80 m");
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(true);
                   radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_20->setText("20 m"); 
                   radioButtonBand_20->setChecked(TRUE);
                   confBand = "14010";
                   radioButtonBand_15->setEnabled(true);
                   radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_15->setText("15 m"); 
                   radioButtonBand_10->setEnabled(true);
                   radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_10->setText("10 m"); 
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   
                   radioButtonSsb->setChecked(TRUE);              
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                }
            break;
 
	     
     case RUSSDX : {                                          
                   tabWidgetRule->setCurrentIndex(2);         
		   labelHeader->setText("for QSOs with");
		   
                   labelPoint3A->setText("Russ Station");        
                   wPoint3A->setText(PointsA);                
                   labelPoint3B->setText("own dxcc");         
                   wPoint3B->setText(PointsB);                
                   labelPoint3C->setText("own cont");         
                   wPoint3C->setText(PointsC);                
		   labelPoint3D->setText("DX cont");          
		   wPoint3D->setText(PointsD);                
		   labelPoint3E->setText("");                 
		   wPoint3E->setText(PointsE);                
		   labelPoint3F->setText("");                 
		   wPoint3F->setText(PointsF);                
                   
                   labelMulti3A->setText("dxcc");             
                   wMulti3A->setText(MultiA);                 
		   labelMultiAinfo->setText("");              
                   labelMulti3B->setText("oblast");           
                   wMulti3B->setText(MultiB);                 
		   
		   wPrefMode3->setText(prefMode);
                   
                   radioButtonBand_160->setEnabled(true); 
                   radioButtonBand_160->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_160->setText("160 m");
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80 m");
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(true);
                   radioButtonBand_20->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_20->setText("20 m"); 
		   radioButtonBand_20->setChecked(TRUE);
		   confBand = "14010";                                              
                   radioButtonBand_15->setEnabled(true);
                   radioButtonBand_15->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_15->setText("15 m"); 
                   radioButtonBand_10->setEnabled(true);
                   radioButtonBand_10->setPalette(QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_10->setText(""); 
		   
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   
                   radioButtonSsb->setChecked(TRUE);                             
		   radioButtonSSB();
		   confMode = radioButtonCw->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                }
            break;
  case OCEANIADX : {                                          
                     tabWidgetRule->setCurrentIndex(2);         
		     labelHeader->setText("for bands");
		   
                     labelPoint3A->setText("160M");  
                     wPoint3A->setText(PointsA);           
                     labelPoint3B->setText("80M");         
                     wPoint3B->setText(PointsB);           
                     labelPoint3C->setText("40M");         
                     wPoint3C->setText(PointsC);           
		     labelPoint3D->setText("20M");         
		     wPoint3D->setText(PointsD);           
		     labelPoint3E->setText("15M");         
		     wPoint3E->setText(PointsE);           
		     labelPoint3F->setText("10M");         
		     wPoint3F->setText(PointsF);           
                     
                     labelMulti3A->setText("OC Cont");     
                     wMulti3A->setText(MultiA);            
		     labelMultiAinfo->setText("");         
                     labelMulti3B->setText("");            
                     wMulti3B->setText(MultiB);            
		   
		     wPrefMode3->setText(prefMode);
                   
                     radioButtonBand_160->setEnabled(true); 
                     radioButtonBand_160->setPalette(QPalette(QColor(180, 210, 200)));  
                     radioButtonBand_160->setText("160 m");
                     radioButtonBand_80->setEnabled(true);
                     radioButtonBand_80->setPalette(QPalette(QColor(180, 210, 200)));  
                     radioButtonBand_80->setText("80 m");
                     radioButtonBand_40->setEnabled(true);
                     radioButtonBand_40->setPalette(QPalette(QColor(180, 210, 200)));  
                     radioButtonBand_40->setText("40 m"); 
                     radioButtonBand_20->setEnabled(true);
                     radioButtonBand_20->setPalette(QPalette(QColor(180, 210, 200)));  
                     radioButtonBand_20->setText("20 m"); 
		     radioButtonBand_20->setChecked(TRUE);
		     confBand = "14010";                                              
                     radioButtonBand_15->setEnabled(true);
                     radioButtonBand_15->setPalette(QPalette(QColor(180, 210, 200)));  
                     radioButtonBand_15->setText("15 m"); 
                     radioButtonBand_10->setEnabled(true);
                     radioButtonBand_10->setPalette(QPalette(QColor(180, 210, 200)));  
                     radioButtonBand_10->setText(""); 
		   
                     radioButtonBand_All->setEnabled(false);
                     radioButtonBand_All->setText(""); 
                   
                     radioButtonSsb->setChecked(TRUE);                             
		     radioButtonSSB();
		     confMode = radioButtonCw->text();
		   
                     wBand160->setText(Pts160);
                     wBand80->setText(Pts80);
                     wBand40->setText(Pts40);
                     wBand20->setText(Pts20);
                     wBand15->setText(Pts15);
                     wBand10->setText(Pts10);
                }
            break;
     case HADX : {                                                
                   tabWidgetRule->setCurrentIndex(0);             
                   labelPoints_A->setText("Qso HA");              
                   wPoints_A->setText(PointsA);                   
                   labelPoints_B->setText("QSO DX");              
                   wPoints_B->setText(PointsB);                   
                   labelPoints_C->setText("Home");                
                   wPoints_C->setText(PointsC);                   
                   
                   labelMulti_A->setText("Bezirk");               
                   wMulti_A->setText(MultiA);                     
		   labelMultiAinfo->setText("");                  
                   labelMulti_B->setText("");                     
                   wMulti_B->setText(MultiB);                     
                   
                   radioButtonBand_160->setEnabled(true); 
                   radioButtonBand_160->setPalette( QPalette(QColor(180, 210, 200))); 
                   radioButtonBand_160->setText("160");
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80 m");
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(true);
                   radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_20->setText("20 m"); 
                   radioButtonBand_20->setChecked(TRUE);
                   confBand = "14010";
                   radioButtonBand_15->setEnabled(true);
                   radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_15->setText("15 m"); 
                   radioButtonBand_10->setEnabled(true);
                   radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_10->setText("10 m"); 
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   
                   radioButtonSsb->setChecked(TRUE);              
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                }
            break;
 
      case SPDX :{                                       
                   tabWidgetRule->setCurrentIndex(0);
                   labelPoints_A->setText("je Qso");     
                   wPoints_A->setText(PointsA);
                   labelPoints_B->setText("");
                   wPoints_B->setText("");
                   labelPoints_C->setText("");
                   wPoints_C->setText("");
                   
                   labelMulti_A->setText("voivodeship"); 
                   wMulti_A->setText(MultiA);            
		   labelMultiAinfo->setText("");         
                   labelMulti_B->setText("");            
                   wMulti_B->setText(MultiB);            
                   
                   radioButtonBand_160->setEnabled(true); 
                   radioButtonBand_160->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_160->setText("160m");
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80 m");
		   radioButtonBand_80->setChecked(TRUE);                              
                   confBand = "3510";
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(true);
                   radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_20->setText("20 m"); 
                   radioButtonBand_15->setEnabled(true);
                   radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_15->setText("15 m"); 
                   radioButtonBand_10->setEnabled(true);
                   radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_10->setText("10 m"); 
		   
                   radioButtonBand_All->setEnabled(false);
                   radioButtonBand_All->setText(""); 
                   radioButtonSsb->setChecked(TRUE);                                 
		   radioButtonSSB();
                   confMode = radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
                 }
            break;
	    
     
     case CHECK : {    
                   labelPoints_A->setText("QSO");
                   wPoints_A->setText(PointsA);       
                   labelPoints_B->setText("");
                   wPoints_B->setText("");
                   labelPoints_C->setText("");
                   wPoints_C->setText("");
                   
                   labelMulti_A->setText("");         
                   wMulti_A->setText(MultiA);         
		   labelMultiAinfo->setText("");      
                   labelMulti_B->setText("");         
                   wMulti_B->setText(MultiB);         
                   radioButtonBand_160->setEnabled(true); 
                   radioButtonBand_160->setPalette( QPalette(QColor(180, 210, 200))); 
                   radioButtonBand_160->setText("160");
                   radioButtonBand_80->setEnabled(true);
                   radioButtonBand_80->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_80->setText("80 m");
                   radioButtonBand_40->setEnabled(true);
                   radioButtonBand_40->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_40->setText("40 m"); 
                   radioButtonBand_20->setEnabled(true);
                   radioButtonBand_20->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_20->setText("20 m"); 
                   radioButtonBand_20->setChecked(TRUE);
                   confBand = "14010";
                   radioButtonBand_15->setEnabled(true);
                   radioButtonBand_15->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_15->setText("15 m"); 
                   radioButtonBand_10->setEnabled(true);
                   radioButtonBand_10->setPalette( QPalette(QColor(180, 210, 200)));  
                   radioButtonBand_10->setText("10 m"); 
                   radioButtonBand_All->setEnabled(true);
                   radioButtonBand_All->setText("All"); 
                   radioButtonBand_All->setPalette( QPalette(QColor(180, 210, 200))); 
                   
                   radioButtonSsb->setChecked(TRUE);         
		   radioButtonSSB();
                   confMode =radioButtonSsb->text();
		   
                   wBand160->setText(Pts160);
                   wBand80->setText(Pts80);
                   wBand40->setText(Pts40);
                   wBand20->setText(Pts20);
                   wBand15->setText(Pts15);
                   wBand10->setText(Pts10);
		   
                   labelRuleInfo->hide();
		   tabWidgetRule->setCurrentIndex(0);       
                }
            break;
     }  
}



void contRules::lineContestCb(QString str)
{
   int i;
   lineContest->setText(str);
   
   if(lineContestID->text().count() + str.count() == 20) {
      QMessageBox::information(this,
      tr("Information"),
      tr("\nContet_ID - out of range !"),
      QMessageBox::Ok);
      
      i = str.count();
      QString f = str.left(i -2);
      str = f;
      lineContest->setText(str);
   }
}



void contRules::buttonCreateContestCb()
{
  QString l, e, id;
  
    if(confBand.count() == 0)
      confBand = radioButtonBand_All->text();      
      
    if(confMode.count() == 0)
      confMode = radioButtonMixd->text();          
      
    if(checkBoxInc->isChecked() == TRUE)
      Inc = 1;                                     
    
    QDate d = contestDate->date();                 
    StartDay = d.toString("yyyy-MM-dd");           
    StartTime = wContestStart->text();             
    
    confcontestId += lineContest->text();

    if(confcontestId.count() == 0) {
       QMessageBox::information(this,
       tr("Info"),tr("Missing ContestID"),
       QMessageBox::Ok);
       return;
    }
    switch(cType.toInt()) {
     case CQWWDX :                                 
                  break;
      case CHECK : {
                    if(lineContest->text().indexOf("xx") != -1) { 
                      QMessageBox::information(this,
                      tr("Info"),tr("File name of CHECK LOG needs to be modified"),
                      QMessageBox::Ok);
                     return;
                    }
                 }
                 break;
      case CQWAEDC :
      case     WAG : 
      case    XMAS :
      case  ARRLDX :
      case     HNY :
      case     DTC :
                 break;
     }
     
     QSqlQuery query;
     qy = "SELECT contestid,expLogbook,expFile FROM wContest WHERE contestId='"+confcontestId+"'";
     query.exec(qy);
     if(query.size()) {
       query.next();
       id = query.value(0).toString();
       l = query.value(1).toString();                      
       e = query.value(2).toString();                      
       
       if(e.count() == 0) {
	 QMessageBox::information(this,
         tr("Information"),
         tr("\n\nContest %1 exist - but is not exported").arg(confcontestId),
         QMessageBox::Ok);
         return;
       }
       else
       if(l.count() == 0) {
	 QMessageBox::information(this,
         tr("Information"),
         tr("\n\nContest %1 exist - but is not saved to QtLog").arg(confcontestId),
         QMessageBox::Ok);
         return;
       }
       deleteContest();                                    
     }

     qy = "INSERT INTO wContest VALUES ('"+confcontestId;  
     qy += "','"+cType;                                    
     qy += "','"+confcontestName;                          
     qy += "','";                                          
     qy += "','";                                          
     qy += "','"+StartDay;                                 
     qy += "','"+StartTime;                                
     qy += "','"+RunTime;                                  
     qy += "','0";                                         
     qy += "','"+confBand;                                 
     qy += "','"+confMode;                                 
     qy += "',1";                                          
     qy += ",'"+srst;                                      
     qy += "','"+sent;                                     
     qy += "','"+rrst;                                     
     qy += "','"+settings.value("Call").toString();        
     qy += "','";                                          
     qy += "','";                                          
     qy += "','','','','";                                 
     qy += "','";                                          
     qy += "')";
     query.exec(qy);
    
     n = 1;                                                 
     qy = "INSERT INTO wCband VALUES ('"+confcontestId+"','qsos',0,0,0,0,0,0,0,"+s.setNum(n++)+")";
     query.exec(qy);
     qy = "INSERT INTO wCband VALUES ('"+confcontestId+"','points',0,0,0,0,0,0,0,"+s.setNum(n++)+")";
     query.exec(qy);
     qy = "INSERT INTO wCband VALUES ('"+confcontestId+"','mult1',0,0,0,0,0,0,0,"+s.setNum(n++)+")";
     query.exec(qy);
     qy = "INSERT INTO wCband VALUES ('"+confcontestId+"','mult2',0,0,0,0,0,0,0,"+s.setNum(n++)+")";
     query.exec(qy);
     qy = "INSERT INTO wCband VALUES ('"+confcontestId+"','qsosh',0,0,0,0,0,0,0,"+s.setNum(n++)+")";
     query.exec(qy);
     qy = "INSERT INTO wCband VALUES ('"+confcontestId+"','hscore',0,0,0,0,0,0,0,"+s.setNum(n++)+")";
     query.exec(qy);
     qy = "INSERT INTO wCband VALUES ('"+confcontestId+"','btime',0,0,0,0,0,0,0,"+s.setNum(n++)+")";
     query.exec(qy);
    
     QString home = QDir::homePath(); 
     s = home+"/log/Export/"+confcontestId;
     QDir dir(s);                              
     if(!dir.exists()) {                       
        dir.mkdir(s);                          
        s += "/Archiv";                        
        dir.mkdir(s);  
     }
     stat = 1;
     accept();
}

void contRules::goExit()
{
  stat = 0;                                                            
  accept();
}


void contRules::deleteContest()
{
     QSqlQuery query;                       
     qy = "DELETE FROM wCband WHERE contid='"+confcontestId+"'"; 
     query.exec(qy);
     qy = "DELETE FROM wContest WHERE contestid='"+confcontestId+"'";
     query.exec(qy);
     qy = "DELETE FROM wCqso WHERE contid='"+confcontestId+"'";
     query.exec(qy);
}
