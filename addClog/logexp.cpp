/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.08.2009
********************************************************************** */
#include <QtGui>
#include <QtSql>
#include <QMessageBox>
#include "logexp.h"
#include "defs.h"
#include "../qtlogDiag/version.h"

logexp::logexp(QWidget * parent) : QDialog(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    int n = settings.value("FontSize").toInt();
    QFont font;
    font.setPointSize(10);
    setFont(font);
    connect(ButtonReturn, SIGNAL(clicked()), this, SLOT(goExit()));
    connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
    connect(ButtonOK, SIGNAL(clicked()), this, SLOT(showExport()));
    connect(ButtonPath, SIGNAL(pressed()), this, SLOT(getDirCb()));
    connect(rButtonCBR3, SIGNAL(pressed()), this, SLOT(rButtonCbr3Cb()));
    connect(rButtonCBR2, SIGNAL(pressed()), this, SLOT(rButtonCbr2Cb()));
    
    connect(rButtonSTF, SIGNAL(pressed()), this, SLOT(rButtonStfCb()));
    connect(rButtonCSV, SIGNAL(pressed()), this, SLOT(rButtonCsvCb()));
    connect(ButtonExport, SIGNAL(pressed()), this, SLOT(exportCb()));
     s = settings.value("Val").toString();
     i = s.indexOf(",");
     s = s.mid(i+1);
     labelContestId->setText(s);
     checkBoxMode->setEnabled(FALSE);                
     checkBoxBand->setEnabled(FALSE);                
     dirsel = 0;
     h = QDateTime::currentDateTime().toUTC();
     dateEdit1->setDate(h.date());                   
     dateEdit2->setDate(h.date());                   
     QSqlQuery query;  
     qy = "SELECT contestid,ctype,name,mode,mycall,club,specify,soapbox FROM wContest WHERE contestid='"+labelContestId->text()+"'";
     n = 0;
     query.exec(qy);  
     query.next();
     s = query.value(n++).toString();                 
     labelContestId->setText(s);
     CNr = query.value(n++).toInt();
     wContestName->setText(query.value(n++).toString());
     mode = query.value(n++).toString();
 
     wCall->setText(query.value(n++).toString());
     wClub->setText(query.value(n++).toString());
     wSpecific->setText(query.value(n++).toString());
     wSoap->setText(query.value(n++).toString());
     
     qy = "SELECT qsoOrder,qtcOrder FROM wCruls WHERE namid='"+labelContestId->text()+"'";
     n = 0;
     query.exec(qy);  
     query.next();     
     qsoOrder = query.value(n++).toString();
     qtcOrder = query.value(n++).toString();
     
     path = QDir::homePath();                         
     path += "/log/Export/"+labelContestId->text();
     path += "/"+settings.value("Call").toString()+"."+lineLog->text();
     linePath->setText(path);
    
     qy = "SELECT subject,phMail,cwMail FROM wCruls WHERE namid='"+labelContestId->text().left(i)+"'";
     query.exec(qy);  
     query.next();
     subJect = query.value(0).toString();
     phMail = query.value(1).toString();
     cwMail = query.value(2).toString();
     
     n = 0;
     qy = "SELECT name,street,qth,country,email FROM toperator WHERE ocall='";
     qy += wCall->text()+"'";
     query.exec(qy); 
     if(query.size()) {
        query.next();
        wName->setText(query.value(n++).toString());     
        wStreet->setText(query.value(n++).toString());   
        wQth->setText(query.value(n++).toString());      
	wCountry->setText(query.value(n++).toString());  
        wMail->setText(query.value(n++).toString());     
     }
     
     rButton = 0;
     switch(CNr) {                                      
       case WAG     :                                   
       case HNY     :                                   
       case XMAS    :
       case DARC10M :
       case DTC     :
       case HHCont  :
       case AGCWHTP :
       case CQPTY   :
                     wClub->setText(settings.value("Club").toString());
                     wSpecific->setText(settings.value("Kenner").toString());
	             rButtonSTF->setChecked(TRUE); 
		     rButtonStfCb();                    
                     rButton = STF;
	             break;
       case CQWAEDC :  
       case  CHECK  :  
                     wClub->setText("");
                     wSpecific->setText(settings.value("Kenner").toString());
                     rButtonSTF->setChecked(TRUE);
                     rButtonStfCb();    
                     rButton = STF;                     
                     break;
       
		     
       case ARRLDX :                                    
        case CQWPX :                                    
       case CQWWDX :                                    
     case CQWW160M : 
       case RUSSDX :
     case RSGB160M :
        case UBASP :
	case UBADX :
	case UBALB :
       case ONCONT :
         case PACC :
     case NASPRINT :
         case HADX :
	 case REFC :
	 case SPDX :
                     wClub->setText("");
                     wSpecific->setText("");
                     rButtonCBR2->setChecked(TRUE);
	             rButtonCbr2Cb();                   
                     rButton = CBR2;
		     break;
		     
    case OCEANIADX :	                                
		     rButtonCBR3->setChecked(TRUE);     
		     rButtonCbr3Cb();                   
		     rButton = CBR3;
	             break; 
     } 
    
     creatSysName();                      
     stackedWidget->setCurrentIndex(0);   
}

logexp::~logexp() {
}

void logexp::goExit()
{
    accept();
}



void logexp::rButtonCbr2Cb()
{
      if(getTemplateForm(CNr,CBR2).count() == 0)  {
         QMessageBox::information(this,
          tr("Information"),
          tr("\n\nCabrillo V 2.0 template for contest  %1 \ndon't exist").arg(labelContestId->text()),
         QMessageBox::Ok);
      }
      rButtonCBR2->setChecked(TRUE);
      rButtonCBR2->setPalette(QPalette(QColor(180, 210, 200)));  
      rButtonCBR3->setPalette(QPalette(QColor(238, 238, 238)));  
      rButtonSTF->setPalette(QPalette(QColor(238, 238, 238)));   
      rButtonCSV->setPalette(QPalette(QColor(238, 238, 238)));   
      lineLog->setText("LOG");
      rButton = CBR2;
      
      s = path;
      i = s.indexOf(".");
      s = s.left(i +1);
      s += lineLog->text();
      linePath->setText(s);
      path = s;
}



void logexp::rButtonCbr3Cb()
{
   if(getTemplateForm(CNr,CBR3).count() == 0)  {
      QMessageBox::information(this,
        tr("Information"),
        tr("\nCabrillo V 3.0 template for contest  %1 \ndon't exist").arg(labelContestId->text()),
      QMessageBox::Ok);
   }
   rButtonCBR3->setPalette(QPalette(QColor(180, 210, 200)));  
   rButtonCBR2->setPalette(QPalette(QColor(238, 238, 238)));  
   rButtonSTF->setPalette(QPalette(QColor(238, 238, 238)));   
   rButtonCSV->setPalette(QPalette(QColor(238, 238, 238)));   
   lineLog->setText("LOG");
   rButton = CBR3;
   s = path;
   i = s.indexOf(".");
   s = s.left(i +1);
   s += lineLog->text();
   linePath->setText(s);
   path = s;
}



void logexp::rButtonStfCb()
{
   rButtonSTF->setPalette(QPalette(QColor(180, 210, 200)));   
   rButtonCBR3->setPalette(QPalette(QColor(238, 238, 238)));  
   rButtonCBR2->setPalette(QPalette(QColor(238, 238, 238)));  
   rButtonCSV->setPalette(QPalette(QColor(238, 238, 238)));   
   lineLog->setText("STF");
   rButton = STF;
   s = path;
   i = s.indexOf(".");
   s = s.left(i +1);
   s += lineLog->text();
   linePath->setText(s);
   path = s;
}



void logexp::rButtonCsvCb()
{      
   rButtonCSV->setPalette(QPalette(QColor(180, 210, 200)));   
   rButtonSTF->setPalette(QPalette(QColor(238, 238, 238)));   
   rButtonCBR3->setPalette(QPalette(QColor(238, 238, 238)));  
   rButtonCBR2->setPalette(QPalette(QColor(238, 238, 238)));  
   rButton = CSV;
   lineLog->setText("txt");
   s = path;
   i = s.indexOf(".");
   s = s.left(i +1);
   s += lineLog->text();
   linePath->setText(s);
   path = s;
}

void logexp::checkBoxBandCb()
{
    if(checkBoxBand->isChecked() == FALSE) {
       checkBoxMode->setEnabled(TRUE);
       checkBoxMode->setChecked(TRUE);
    }
    else {
      checkBoxMode->setEnabled(FALSE);
      checkBoxMode->setChecked(FALSE);
      checkBoxBand->setChecked(TRUE); 
    }
}

void logexp::getDirCb()
{
     QString l;
     l = DirSelector();
     
     l += "/"+settings.value("Call").toString();
     l += "."+lineLog->text();
     linePath->setText(l);
     path = l;
}

QString logexp::DirSelector()
{
   if( ! dirsel ) {
     QString directory = QFileDialog::getExistingDirectory(this,
             tr("Waehle einen Ordner"), QDir::homePath());
     return directory;
   }
   else {
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                        "/home", tr("ADIF ( *.adi *.adif )"));
     return fileName;
   }
}



QString logexp::getTemplateForm(int cnr, int form)
{
   QString q;
     if( form == CBR2 ) {                   
        switch(cnr) {
      case RSGB160M : 
	              q = "SELECT freq,mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,rcvd2,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
	              contest="RSGB-160";
		      break;
        case CQWWDX :                          
                      q = "SELECT freq,mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="CQ-WW";          
                      break;
         case ARRLDX :                          
                      q = "SELECT freq,mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="ARRL-DX";        
                      break;
       case NASPRINT :                          
                      q = "SELECT freq,mode,cdate,ctime,  sent2,  qcall,rcvd,rcvd2,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="NAQO";           
                      break;
          case CQWPX :                          
                      q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="CQ-WPX";         
                      break;
       case CQWW160M : 
	              q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="CQ WW 160 Meter"; 
		      break;
           case PACC : 
	              q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="PACC";            
		      break;  
         case RUSSDX : 
	              q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="RDXC-"+h.date().toString("yyyy");  
		      break;  
	  case UBASP :
	              q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,rcvd2,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="UBA-SPRING";      
		      break;    
	 case UBALB :
		      q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="UBA WINTER";      
		      break;   
	 case UBADX :
	              q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,rcvd2,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="UBA-DX";      
		      break;          
	case ONCONT :
		      q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,rcvd2,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="ON CONTEST";      
		      break;  
	  case HADX :
		      q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="HA-DX";      
		      break;
	  case REFC :
		      q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,multi1,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="REF";      
		      break;         
          case SPDX :
		      q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                      contest="SP-DX";      
		      break;      
		      
		      
        }
     }
     else
     if( form == CBR3 ) {    
	switch(cnr) {
     case OCEANIADX : {                          
	               q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                       contest="OCEANIA-DX";     
		       break;
                     }
      }
    }
    /*
    else
     if( form == CSV ) {    
	switch(cnr) {
     case AGCWHTP: {                          
	               q = "SELECT freq, mode,cdate,ctime,srst,sent,qcall,rrst,rcvd,tn FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
                       contest="OCEANIA-DX";     
		       break;
                   }
                   break;
      }
     }
     */
     return q;       
}



void logexp::showExport()
{
    int a;
    a = 0;
    if(rButtonSTF->isChecked() == TRUE)
      a = 1;
     else 
     if(rButtonCBR3->isChecked() == TRUE)
        a = 1;
     else
     if(rButtonCBR2->isChecked() == TRUE)
        a = 1;
     else 
     if(rButtonCSV->isChecked() == TRUE) {
        a = 1;
       exportCb();
       return;
     }
    if( !a ) {
          QMessageBox::information(this,
          tr("Information"),
          tr("\nSet first export format"),
          QMessageBox::Ok);
       return;
    }
   
    QString s;
    BoxCategory->clear();
    BoxBand->clear();
    BoxPwr->clear();
    BoxMode->clear();
    if(rButtonSTF->isChecked() == TRUE) {           
      BoxCategory->insertItem(0,"Single operator"); 
      BoxCategory->insertItem(1,"Multi operator");
      BoxCategory->insertItem(2,"Checklog");
      BoxCategory->setCurrentIndex(0);
      BoxBand->insertItem(0,"mixed");
      BoxBand->setCurrentIndex(0);
      BoxPwr->insertItem(0,"high power");
      BoxPwr->insertItem(1,"low power");
      BoxPwr->insertItem(2,"QRP");
      BoxPwr->setCurrentIndex(0);
      BoxMode->insertItem(0,"CW");
      BoxMode->insertItem(1,"SSB");
      BoxMode->insertItem(2,"RTTY");
      BoxMode->setCurrentIndex(0);
    }
    else 
    if(rButtonCBR2->isChecked() == TRUE) {      
      BoxCategory->insertItem(0,"SINGLE-OP");   
      BoxCategory->insertItem(1,"MULTI-OP");
      BoxCategory->insertItem(2,"CHECKLOG");
      BoxCategory->setCurrentIndex(0);
      BoxBand->insertItem(0,"ALL");
      BoxBand->insertItem(2,"160M");
      BoxBand->insertItem(3,"80M");
      BoxBand->insertItem(4,"40M");
      BoxBand->insertItem(5,"20M");
      BoxBand->insertItem(6,"15M");
      BoxBand->insertItem(7,"10M");
      BoxBand->setCurrentIndex(0);
      BoxTransmitter->insertItem(0,"ONE");
      BoxTransmitter->insertItem(1,"TOW");
      BoxTransmitter->insertItem(2,"UNLIMITED");
      BoxTransmitter->setCurrentIndex(0);
      
      BoxPwr->insertItem(0,"HIGH");
      BoxPwr->insertItem(1,"LOW");
      BoxPwr->insertItem(2,"QRP");
      BoxPwr->setCurrentIndex(0);
      BoxMode->insertItem(0,"SSB");
      BoxMode->insertItem(1,"CW");
      BoxMode->insertItem(2,"PH");
      BoxMode->insertItem(3,"RY");
      BoxMode->insertItem(4,"MIXED");
      BoxMode->setCurrentIndex(0);
    }
    else 
    if(rButtonCBR3->isChecked() == TRUE) {      
      BoxCategory->insertItem(0,"SINGLE-OP");   
      BoxCategory->insertItem(1,"MULTI-OP");
      BoxCategory->insertItem(2,"CHECKLOG");
      BoxCategory->setCurrentIndex(0);
      BoxBand->insertItem(0,"ALL");
      BoxBand->insertItem(2,"160M");
      BoxBand->insertItem(3,"80M");
      BoxBand->insertItem(4,"40M");
      BoxBand->insertItem(5,"20M");
      BoxBand->insertItem(6,"15M");
      BoxBand->insertItem(7,"10M");
      BoxBand->setCurrentIndex(0);
      BoxTransmitter->insertItem(0,"ONE");
      BoxTransmitter->insertItem(1,"TOW");
      BoxTransmitter->insertItem(2,"UNLIMITED");
      BoxTransmitter->setCurrentIndex(0);
      
      BoxPwr->insertItem(0,"HIGH");
      BoxPwr->insertItem(1,"LOW");
      BoxPwr->insertItem(2,"QRP");
      BoxPwr->setCurrentIndex(0);
      BoxMode->insertItem(0,"CW");
      BoxMode->insertItem(1,"SSB");
      BoxMode->insertItem(2,"PH");
      BoxMode->insertItem(3,"RY");
      BoxMode->insertItem(4,"MIXED");
      BoxMode->setCurrentIndex(1);
    }
    stackedWidget->setCurrentIndex(1);            
}



void logexp::exportCb()
{ 
    int form, r;
    QString t;
    r = 0;
    
    if(rButtonSTF->isChecked() == TRUE)
      form = STF;
    else 
    if(rButtonCBR3->isChecked() == TRUE)
       form = CBR3;
    else	
    if(rButtonCBR2->isChecked() == TRUE)
       form = CBR2;
    else 
    if(rButtonCSV->isChecked() == TRUE)
       form = CSV;
    else {
          QMessageBox::information(this,
          tr("Information"),
          tr("\nSet export format"),
          QMessageBox::Ok);
          return;
    }
    if(form != CSV) { 
      qy = "UPDATE toperator SET ";                      
      qy += "name='"+wName->text();
      qy += "',street='"+wStreet->text();
      qy += "',qth='"+wQth->text();
      qy += "',country='"+wCountry->text();
      qy += "',email='"+wMail->text();
      qy += "' WHERE ocall='"+wCall->text()+"'";
      QSqlQuery query(qy); 
    
      qy = "UPDATE wContest SET ";                       //user='"+wName->text();
      qy += "club='"+wClub->text();
      qy += "',specify='"+wSpecific->text();
      qy += "',soapbox='"+wSoap->text();
      qy += "' WHERE contestid='"+labelContestId->text()+"'";
      query.exec(qy);
    }
    
    
    if( form == CBR2 ) {                                  
        switch(CNr) {                                     
        case CQWWDX : {                                   
                       qy = getTemplateForm(CNr,CBR2);    
                       n = workExportCQWW(qy);            
                       stackedWidget->setCurrentIndex(0);
		       QMessageBox::information(this,
                       tr("Information"),
                       tr("\n\nCabrillo file created : %1\n\nMail as attachment :\nSSB log : %2\nCW log : %3\nSubject : your Callsign\n\nIf necessary rename the logfile").arg(path,phMail,cwMail),
		       QMessageBox::Ok);
		       r = 1;
                     }
              break;
	      
         case ARRLDX : {                                   
                        qy = getTemplateForm(CNr,CBR2);    
                        n = workExportCQWW(qy);            
                        stackedWidget->setCurrentIndex(0); 
		        QMessageBox::information(this,
                        tr("Information"),
                        tr("\n\nCabrillo file created : %1\n\nMail as attachment :\nSSB log : %2\nCW log : %3\nSubject : your Callsign\n\nIf necessary rename the logfile").arg(path,phMail,cwMail),
			QMessageBox::Ok);
			r = 1;
                    }
              break;
	      
       case NASPRINT : {                                   
                        qy = getTemplateForm(CNr,CBR2);    
                        n = workExportNASPRINT(qy);        
                        stackedWidget->setCurrentIndex(0); 
		        QMessageBox::information(this,
                        tr("Information"),
                        tr("\n\nCabrillo file created : %1\n\nMail as attachment :\nSSB log : %2\nCW log : %3\nSubject : your Callsign\n\nIf necessary rename the logfile").arg(path,phMail,cwMail),
			QMessageBox::Ok);
			r = 1;
                    }
	      break;
	      
         case CQWPX : {                                    
                        qy = getTemplateForm(CNr,CBR2);    
                        n = workExportCQWW(qy);            
                        stackedWidget->setCurrentIndex(0);
			QMessageBox::information(this,
                        tr("Information"),
                        tr("\n\nCabrillo file created : %1\n\nMail as attachment :\nSSB log : %2\nCW log : %3\nSubject : your Callsign\n\nIf necessary rename the logfile").arg(path,phMail,cwMail),
			QMessageBox::Ok);
			r = 1;
                     }
               break;
	       
      case CQWW160M : {                                    
                        qy = getTemplateForm(CNr,CBR2);    
                        n = workExportCQWW(qy);            
                        stackedWidget->setCurrentIndex(0); 
			QMessageBox::information(this,
		        tr("Information"),		 
			tr("\n\nCabrillo file created : %1\n\nMail as attachment :\nSSB log : %2\nCW log : %3\nSubject : your Callsign\n\nIf necessary rename the logfile").arg(path,phMail,cwMail),
			QMessageBox::Ok);
			r = 1;
                     }
               break;
	       
      case RSGB160M : {                                    
                        qy = getTemplateForm(CNr,CBR2);    
                        n = workExportRSGB160M(qy);        
                        stackedWidget->setCurrentIndex(0); 
		        QMessageBox::information(this,
		        tr("Information"),		 
			tr("\n\nCabrillo file created : %1\n\nEntries must be submitted to : %2\n").arg(path,phMail),
			QMessageBox::Ok);
			r = 1;
                     }
               break;
	       
          case PACC : {                                    
                        qy = getTemplateForm(CNr,CBR2);    
                        n = workExportPACC(qy);            
                        stackedWidget->setCurrentIndex(0); 
			QMessageBox::information(this,
                        tr("Information"),
                        tr("\n\nCabrillo file created : %1\n\neMail with attachment : %2\nSubject : Your Callsign and Category\n\nIf necessary rename this logfile").arg(path,phMail),
                        QMessageBox::Ok);
			r = 1;
                     }
               break;
	 case RUSSDX : {                                    
                        qy = getTemplateForm(CNr,CBR2);     
                        n = workExportRUSSDX(qy);           
                        stackedWidget->setCurrentIndex(0);
                        QMessageBox::information(this,
                        tr("Information"),
                        tr("\n\nCabrillo file created : %1\n\neMail with attachment : %2\nSubject : Your Callsign and Category\n\nIf necessary rename this logfile").arg(path,phMail),
                        QMessageBox::Ok);
			r = 1;
                     }
               break;
	       
	 //case ONCONT :
	  case UBADX :                                     
	              {                                    
                        qy = getTemplateForm(CNr,CBR2);    
                        n = workExportUBADX(qy); 
                        stackedWidget->setCurrentIndex(0);
                        QMessageBox::information(this,			 
			tr("Information"),		 
			tr("\n\nCabrillo file created : %1\n\nMail as attachment :\n SSB log : %2\n CW log : %3\nSubject : your Callsign\n\nIf necessary rename the logfile").arg(path,phMail,cwMail),	 
                        QMessageBox::Ok);
			r = 1;
                     }
               break;
	       
	 case UBALB :                                      
	              {                                    
                        qy = getTemplateForm(CNr,CBR2);    
                        n = workExportUBALB(qy);
                        stackedWidget->setCurrentIndex(0);
                        QMessageBox::information(this,			 
			tr("Information"),		 
			tr("\n\nCabrillo file created : %1\n\nMail as attachment :\n SSB log : %2\n CW log : %3\nSubject : your Callsign\n\nIf necessary rename the logfile").arg(path,phMail,cwMail),	 
                        QMessageBox::Ok);
			r = 1;
                     }
               break;
	       
	  case HADX:                                       
	              {                                    
                        qy = getTemplateForm(CNr,CBR2);    
                        n = workExportUBALB(qy);
                        stackedWidget->setCurrentIndex(0);
                        QMessageBox::information(this,			 
			tr("Information"),		 
			tr("\n\nCabrillo file created : %1\n\nMail as attachment :\n SSB log : %2\n CW log : %3\nSubject : your Callsign\n\nIf necessary rename the logfile").arg(path,phMail,cwMail),	 
                        QMessageBox::Ok);
			r = 1;
                     }
               break;
	       
	   case REFC:                                      
	              {                                    
                        qy = getTemplateForm(CNr,CBR2);    
                        n = workExportREFC(qy);
                        stackedWidget->setCurrentIndex(0);
                        QMessageBox::information(this,			 
			tr("Information"),		 
			tr("\n\nCabrillo file created : %1\n\nMail as attachment :\n SSB log : %2\n CW log : %3\nSubject : your Callsign\n\nIf necessary rename the logfile").arg(path,phMail,cwMail),	 
                        QMessageBox::Ok);
			r = 1;
                     }
               break;
	       
	   case SPDX:                                      
	              {                                    
                        qy = getTemplateForm(CNr,CBR2);    
                        n = workExportSPDX(qy);
                        stackedWidget->setCurrentIndex(0);
                        QMessageBox::information(this,			 
			tr("Information"),		 
			tr("\n\nCabrillo file created : %1\n\nMail as attachment :\n SSB log : %2\n CW log : %3\nSubject : your Callsign\n\nIf necessary rename the logfile").arg(path,phMail,cwMail),	 
                        QMessageBox::Ok);
			r = 1;
                     }
               break;
	       
	      
	      
         default : {
                    stackedWidget->setCurrentIndex(0);    
                    QMessageBox::information(this,
                    tr("Information"),
                    tr("\nDont't have template for this contest type"),
                    QMessageBox::Ok);
		    r = 0;
                  }
                  break;
        }
        if( r ) { 
	  if(path.indexOf(labelContestId->text()) != -1) { 
             i = path.lastIndexOf("/");                    
             p = path.left(i);                             
             s = path.mid(i+1);                            
             i = s.indexOf(".");                           
             
             p += "/Archiv/"+s.left(i)+"_"+h.toString("yyyy-MM-dd")+s.mid(i); 
             t = "cp "+path+" "+p;
             i = system(t.toAscii());
	  }
          QSqlQuery query;    
          qy = "UPDATE wContest SET expFile='"+h.toString("yyyy-MM-dd")+"' WHERE contestId='"+labelContestId->text()+"'";
          query.exec(qy);
       }
    }
    else  
    if( form == CBR3 ) {
	 switch(CNr) {     
  case OCEANIADX : {                                   
                    qy = getTemplateForm(CNr,CBR3);    
                    n = workExportOCEANIADX(qy);       
                    stackedWidget->setCurrentIndex(0);
		    QMessageBox::information(this,
		    tr("Information"),		 
		    tr("\n\nCabrillo file created : %1\n\nMail as attachment \n  SSB log : %2\n  CW log : %3\nSubject : your Callsign\n\nIf necessary rename the logfile").arg(path,phMail,cwMail),
		    QMessageBox::Ok);     
		    r = 1;
                  }
                  break;
	 }
	 if( r ) { 
	   if(path.indexOf(labelContestId->text()) != -1) { 
              i = path.lastIndexOf("/");                    
              p = path.left(i);                             
              s = path.mid(i+1);                            
              i = s.indexOf(".");                           
              
              p += "/Archiv/"+s.left(i)+"_"+h.toString("yyyy-MM-dd")+s.mid(i); 
              t = "cp "+path+" "+p;
              i = system(t.toAscii());
	   }
           QSqlQuery query;    
           qy = "UPDATE wContest SET expFile='"+h.toString("yyyy-MM-dd")+"' WHERE contestId='"+labelContestId->text()+"'";
           query.exec(qy);
       }
    }
    
    else  
    if( form == STF){                                 
        switch(CNr) {                                 
        case WAG : {                                  
                      n = workExportWAG(qsoOrder);    
                      stackedWidget->setCurrentIndex(0);
		      
		      QMessageBox::information(this,
		      tr("Information"),
                      tr("\n\nSTF file created : %1\n\neMail with attachment \nSubject : %2\nlog : %3\n\nIf necessary rename this logfile").arg(path,subJect,phMail),
		      QMessageBox::Ok);		       
		      r = 1;
                   }
                   break;
		   
    case CQWAEDC : {
                      n = workExportWAE(qsoOrder);    
                      stackedWidget->setCurrentIndex(0);
		      
		      QMessageBox::information(this,
                      tr("Information"),
                      tr("\n\nSTF file created : %1\n\neMail with attachment \nSubject : %2\n\nSSB log %3\nCW log %4\n\nIf necessary rename this logfile").arg(path,subJect,phMail,cwMail),
                      QMessageBox::Ok);
		      r = 1;
                   }
                   break;  
		   
        case DTC : {
                      n = workExportDTC(qsoOrder);     
                      
                      QMessageBox::information(this,
                      tr("Information"),
                      tr("\nSTF file created  %1 QSOs").arg(s.setNum(n)),
                      QMessageBox::Ok);
		      r = 1;
                   }
                   break;
		   
        case XMAS :                                   
      case HHCont : {                                 
                       n = workExportXMAS(qsoOrder); 
                       
		       QMessageBox::information(this,
                       tr("Information"),
                       tr("\n\nSTF file created : %1\n\neMail with attachment : %2\nSubject : Callsign \n\nIf necessary rename this logfile").arg(path,phMail),
                       QMessageBox::Ok);
		       r = 1;
                   }
                   break;
       case CQPTY : {                                  
                       n = workExportCQPTY(qsoOrder); 
                       
                       QMessageBox::information(this,
                       tr("Information"),
                       tr("\nFile written:   %1 QSOs\nPath: %2").arg(s.setNum(n),linePath->text()),
                       QMessageBox::Ok);
		       r = 1;
                   }
                   break;
		   
     case DARC10M : { 
	              n = workExportDARC10M(qsoOrder);  
                         
                      QMessageBox::information(this,
                      tr("Information"),
                      tr("\nFile written:   %1 QSOs\nPath: %2").arg(s.setNum(n),linePath->text()),
                      QMessageBox::Ok);
		      r = 1;
	           }
	           break;
		   
	case   HNY : {
	               n = workExportHNY(qsoOrder);
                         
		       QMessageBox::information(this,
                       tr("Information"),
                       tr("\n\nSTF file created : %1\n\neMail with attachment : %2\nSubject : Your Callsign and Category\n\nIf necessary rename this logfile").arg(path,cwMail),
                       QMessageBox::Ok);
		       r = 1;
	           }
	           break;
		   
       case AGCWHTP : {
	                 n = workExportAGCWHTP(qsoOrder);
                         QMessageBox::information(this,
                         tr("Information"),
                         tr("\n\nSTF file created : %1\n\neMail with attachment : %2\nSubject : Callsign\n\nIf necessary rename this logfile").arg(path,cwMail),
                         QMessageBox::Ok);
                         r = 1;
	           }
	           break;
		  /*
         case CHECK : {
                         n = workExportCHECK(qsoOrder);
                         
                         QMessageBox::information(this,
                         tr("Information"),
                         tr("\nFile written:   %1 QSOs\nPath: %2").arg(s.setNum(n),linePath->text()),
                         QMessageBox::Ok);
            
                   }
                   break;
		   */
           default : {                                
                      stackedWidget->setCurrentIndex(0);
                      QMessageBox::information(this,
                      tr("Information"),
                      tr("\nDont't have template for this contest type"),
                      QMessageBox::Ok);
		      r = 0;
                   }
           }
           if( r ) {
	      if(path.indexOf(labelContestId->text()) != -1) { 
                i = path.lastIndexOf("/");          
                p = path.left(i);                   
                s = path.mid(i+1);                  
                i = s.indexOf(".");                 
                
                p += "/Archiv/"+s.left(i)+"_"+h.toString("yyyy-MM-dd")+s.mid(i); 
                t = "cp "+path+" "+p;
                i = system(t.toAscii());
	      }
             QSqlQuery query;    
             qy = "UPDATE wContest SET expFile='"+h.toString("yyyy-MM-dd")+"' WHERE contestId='"+labelContestId->text()+"'";
             query.exec(qy);
	   }
     }
     else 
     if(form == CSV) {
       switch(CNr) {
         
         case CHECK : {
                       n = workExportCSV(qsoOrder);
                       QMessageBox::information(this,
                       tr("Information"),
                       tr("\nFile written:   %1 QSOs\nPath: %2").arg(s.setNum(n),linePath->text()),
                       QMessageBox::Ok);
		       r = 1;
                  }
                  break;
		  
      case AGCWHTP : {
                       n = workExportCSV(qsoOrder);
                       QMessageBox::information(this,
                       tr("Information"),
                       tr("\n\nText file created : %1\n\neMail with attachment : %2\nSubject : Callsign\n\nIf necessary rename this logfile").arg(path,cwMail),
                       QMessageBox::Ok);
                       r = 1;
	          }
	          break; 
		  
		  
         default : {
                    stackedWidget->setCurrentIndex(0);
                    QMessageBox::information(this,
                    tr("Information"),
                    tr("\nDont't have template for this contest type"),
                    QMessageBox::Ok);
		    r = 0;
                  }
                  break;
       }
       if( r ) { 
	  if(path.indexOf(labelContestId->text()) != -1) { 
             i = path.lastIndexOf("/");                    
             p = path.left(i);                             
             s = path.mid(i+1);                            
             i = s.indexOf(".");                           
             
             p += "/Archiv/"+s.left(i)+"_"+h.toString("yyyy-MM-dd")+s.mid(i); 
             t = "cp "+path+" "+p;
             i = system(t.toAscii());
	  }
          QSqlQuery query;    
          qy = "UPDATE wContest SET expFile='"+h.toString("yyyy-MM-dd")+"' WHERE contestId='"+labelContestId->text()+"'";
          query.exec(qy);
       }
     }
     if(n != 0)
       goExit();
}

int logexp::getClaimed(QString contest, QString val)
{
    QSqlQuery query; 
    qy = "SELECT total FROM wCband WHERE contid='"+contest+"' AND val='"+val+"'";
    query.exec(qy);
    query.next();
    return  query.value(0).toInt();
}





int logexp::workExportCQWW(QString q)
{
    QString val, p, hfeld;
    int col, idx, cnt, tpoints, tmult1, tmult2;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    out << "START-OF-LOG: 2.0\n";
    if(wArrlSec->text().count() != 0)
      out << "ARRL-SECTION: "+wArrlSec->text()+"\n";
    out << "CONTEST: "+contest+"-"+mode+"\n";
    out << "CALLSIGN: "+wCall->text()+"\n";
    out << "CATEGORY: "+BoxCategory->currentText()+" "+BoxBand->currentText()+" "+BoxPwr->currentText()+"\n";
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    tmult2 = getClaimed(labelContestId->text(),"mult2");
    int sum;
    sum = tpoints * (tmult1 + tmult2);
    
    out << "CLAIMED-SCORE: "+s.setNum(sum)+"\n";
    s = VERSION;
    out << "CREATED-BY: QtLog "+s+"- DL1HBD  ( "+h.date().toString("yyyy-MM-dd")+" )\n";
    if(wClub->text().count() != 0)                
      out << "CLUB: "+wClub->text()+"\n"; 
    out << "NAME: "+wName->text()+"\n";
    out << "ADDRESS: "+wStreet->text()+"\n";
    out << "ADDRESS: "+wQth->text()+"\n";
    out << "ADDRESS: "+wCountry->text()+"\n";
    out << "EMAIL: "+wMail->text()+"\n"; 
    if(wSoap->text().count() != 0)
      out << "SOAPBOX: "+wSoap->text()+"\n";

    QSqlQuery query;
    query.exec(q);  
    cnt = query.size();                               
    col = query.record().count();                     
    while(query.next()) {
      idx = 0;
      p = "QSO: ";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);        
        val = query.value(idx).toString();
	
        if(hfeld.compare("freq") == 0) {              
         int n = val.toInt();
         n = n/100 * 100;
         val.setNum(n);
         if(val.count() != 5) {
            p += val.rightJustified(5,' ');           
            p += " ";
           }
           else
             p += val.leftJustified(6,' ');           
        }
        
        else
        if(hfeld.compare("mode") == 0) {              
          val = BoxMode->currentText();
	  if(val.compare("ssb"))
	    val = "PH";
          p += val.leftJustified(3,' ');
        }
        else
        if(hfeld.compare("cdate") == 0) 
          p += val.leftJustified(11,' ');
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(6,' ');
          p += val;
          p += wCall->text().leftJustified(12,' ');   
        }     
        else
        if(hfeld.compare("srst") == 0) 
          p += val.leftJustified(4,' ');
        if(hfeld.compare("sent") == 0) 
          p += val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0) 
          p += val.leftJustified(14,' ');
        else
        if(hfeld.compare("rrst") == 0) 
          p += val.leftJustified(10,' ');  
        else
        if(hfeld.compare("rcvd") == 0) 
          p += val.leftJustified(7,' ');
        else
        if(hfeld.compare("tn") == 0) 
          p += val;
        idx++;
      }
      out << p+"\n";
    }
    out << "END-OF-LOG:\n";
    file.close();
    return cnt;
}  



int logexp::workExportNASPRINT(QString q)
{
    QString val, p, hfeld;
    int col, idx, cnt, tpoints, tmult1, tmult2;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    out << "START-OF-LOG: 2.0\n";
    if(wArrlSec->text().count() != 0)
      out << "ARRL-SECTION: "+wArrlSec->text()+"\n";
    out << "CONTEST: "+contest+"-"+mode+"\n";
    out << "CALLSIGN: "+wCall->text()+"\n";
    
    out << "CATEGORY: "+BoxCategory->currentText()+" "+BoxBand->currentText()+" "+BoxPwr->currentText()+"\n";
    
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    tmult2 = getClaimed(labelContestId->text(),"mult2");
    int sum;
    sum = tpoints * (tmult1 + tmult2);
    
    out << "CLAIMED-SCORE: "+s.setNum(sum)+"\n";
    s = VERSION;
    out << "CREATED-BY: QtLog "+s+"- DL1HBD  (" +h.date().toString("yyyy-MM-dd")+" )\n";
    if(wClub->text().count() != 0)                
      out << "CLUB: "+wClub->text()+"\n"; 
    out << "NAME: "+wName->text()+"\n";
    out << "ADDRESS: "+wStreet->text()+"\n";
    out << "ADDRESS: "+wQth->text()+"\n";
    out << "ADDRESS: "+wCountry->text()+"\n";
    out << "EMAIL: "+wMail->text()+"\n"; 
    if(wSoap->text().count() != 0)
      out << "SOAPBOX: "+wSoap->text()+"\n";

    QSqlQuery query;
    query.exec(q);  
    cnt = query.size();                               
    col = query.record().count();                     
    while(query.next()) {
      idx = 0;
      p = "QSO: ";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);        
        val = query.value(idx).toString();
	
        if(hfeld.compare("freq") == 0) {              
         int n = val.toInt();
         n = n/100 * 100;
         val.setNum(n);
         if(val.count() != 5) {
            p += val.rightJustified(5,' ');           
            p += " ";
           }
           else
             p += val.leftJustified(6,' ');           
        }
        
        else
        if(hfeld.compare("mode") == 0) {              
          val = BoxMode->currentText();
	  if(val.compare("ssb"))
	    val = "PH";
          p += val.leftJustified(3,' ');
        }
        else
        if(hfeld.compare("cdate") == 0) 
          p += val.leftJustified(11,' ');
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(6,' ');
          p += val;
          p += wCall->text().leftJustified(10,' '); 
        }     
        else
        if(hfeld.compare("sent2") == 0) {           
          i = val.indexOf("/");
	  s = val.left(i);
	  s = s.leftJustified(8,' ');
          p += s;
	  s = val.mid(i+1);
	  s = s.leftJustified(8,' ');               
	  p += s;
	}
        else
        if(hfeld.compare("qcall") == 0)   
          p += val.leftJustified(10,' ');
        else
        if(hfeld.compare("rcvd2") == 0) { 
          i = val.indexOf("/");
	  s = val.left(i);
	  s = s.leftJustified(8,' ');
          p += s;
	  s = val.mid(i+1);
	  s = s.leftJustified(5,' ');     
	  p += s;
	}
        else
        if(hfeld.compare("tn") == 0) 
          p += val;
        idx++;
      }
      out << p+"\n";
    }
    out << "END-OF-LOG:\n";
    file.close();
    return cnt;
}  



int logexp::workExportRSGB160M(QString q)
{  
    QString val, p, hfeld;
    int col, idx, cnt, tpoints, tmult1;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    out << "START-OF-LOG: 2.0\n";
    if(wArrlSec->text().count() != 0)
      out << "ARRL-SECTION: "+wArrlSec->text()+"\n";
    out << "CONTEST: "+contest+"-"+mode+"\n";
    out << "CALLSIGN: "+wCall->text()+"\n";
    out << "CATEGORY: "+BoxCategory->currentText()+" "+BoxBand->currentText()+" "+BoxPwr->currentText()+" "+BoxMode->currentText()+"\n";
    
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    //tmult2 = getClaimed(labelContestId->text(),"mult2");
    int sum;
    //sum = tpoints * (tmult1 + tmult2);
    sum = tpoints + tmult1;
    out << "CLAIMED-SCORE: "+s.setNum(sum)+"\n";
    s = VERSION;
    out << "CREATED-BY: QtLog "+s+" - DL1HBD ( "+h.date().toString("yyyy-MM-dd")+" )\n";
    if(wClub->text().count() != 0)
      out << "CLUB: "+wClub->text()+"\n"; 
    out << "NAME: "+wName->text()+"\n";
    out << "ADDRESS: "+wStreet->text()+"\n";
    out << "ADDRESS: "+wQth->text()+"\n";
    out << "ADDRESS: "+wCountry->text()+"\n";
    out << "EMAIL: "+wMail->text()+"\n"; 
    if(wSoap->text().count() != 0)
      out << "SOAPBOX: "+wSoap->text()+"\n";

    QSqlQuery query;
    query.exec(q);  
    cnt = query.size();                               
    col = query.record().count();                     
    
    while(query.next()) {
      idx = 0;
      p = "QSO: ";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);        
        val = query.value(idx).toString();
	
        if(hfeld.compare("freq") == 0) {              
          int n = val.toInt();
          n = n/100 * 100;
          val.setNum(n);
          if(val.count() != 5) {
             p += val.rightJustified(5,' ');          
             p += " ";
           }
           else
             p += val.leftJustified(6,' ');           
        }
        
        else
        if(hfeld.compare("mode") == 0) {              
          val = BoxMode->currentText();
	  if(val.compare("ssb"))
	    val = "PH";
          p += val.leftJustified(3,' ');
        }
        else
        if(hfeld.compare("cdate") == 0) 
          p += val.leftJustified(11,' ');
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(5,' ');
          p += val;
          p += wCall->text().leftJustified(14,' ');   
        }     
        else
        if(hfeld.compare("srst") == 0) 
          p += val.leftJustified(4,' ');
        if(hfeld.compare("sent") == 0) 
          p += val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0) 
          p += val.leftJustified(14,' ');
        else
        if(hfeld.compare("rrst") == 0) 
          p += val.leftJustified(10,' ');  
        else
	  
	if(hfeld.compare("rcvd") == 0) 
        p += val.leftJustified(7,' '); 
	  
        if(hfeld.compare("rcvd2") == 0) 
          p += val.leftJustified(7,' ');
        else
        if(hfeld.compare("tn") == 0) 
          p += val;
        idx++;
      }
      out << p+"\n";
    }
    out << "END-OF-LOG:\n";
    file.close();
    
    return cnt;
}  



int logexp::workExportPACC(QString q)
{
    QString val, p, hfeld;
    int col, idx, cnt, tpoints, tmult1;
    
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    
    QTextStream out(&file);
    out << "START-OF-LOG: 2.0\n";
    if(wArrlSec->text().count() != 0)
      out << "ARRL-SECTION: "+wArrlSec->text()+"\n";
    out << "CONTEST: "+contest+"-"+mode+"\n";
    out << "CALLSIGN: "+wCall->text()+"\n";
    out << "CATEGORY: "+BoxCategory->currentText()+" "+BoxBand->currentText()+" "+BoxPwr->currentText()+"\n";
    
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    //tmult2 = getClaimed(labelContestId->text(),"mult2");
    int sum;
    //sum = tpoints * (tmult1 + tmult2);
    sum = tpoints * tmult1;
    out << "CLAIMED-SCORE: "+s.setNum(sum)+"\n";
    s = VERSION;
    out << "CREATED-BY: QtLog "+s+" - DL1HBD\n";
    
    if(wClub->text().count() != 0)
      out << "CLUB: "+wClub->text()+"\n"; 
    
    out << "NAME: "+wName->text()+"\n";
    out << "ADDRESS: "+wStreet->text()+"\n";
    out << "ADDRESS: "+wQth->text()+"\n";
    out << "ADDRESS: "+wCountry->text()+"\n";
    out << "EMAIL: "+wMail->text()+"\n"; 
    if(wSoap->text().count() != 0)
      out << "SOAPBOX: "+wSoap->text()+"\n";

    QSqlQuery query;
    query.exec(q);  
    cnt = query.size();                               
    col = query.record().count();                     
    while(query.next()) {
      idx = 0;
      p = "QSO: ";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);        
        val = query.value(idx).toString();
	
        if(hfeld.compare("freq") == 0) {              
          int n = val.toInt();
          n = n/100 * 100;
          val.setNum(n);
          if(val.count() != 5) {
             p += val.rightJustified(5,' ');          
             p += " ";
           }
           else
             p += val.leftJustified(6,' ');           
        }
        
        else
        if(hfeld.compare("mode") == 0) {              
          val = BoxMode->currentText();
	  if(val.compare("ssb"))
	    val = "PH";
          p += val.leftJustified(3,' ');
        }
        else
        if(hfeld.compare("cdate") == 0) 
          p += val.leftJustified(11,' ');
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(5,' ');
          p += val;
          p += wCall->text().leftJustified(14,' ');   
        }     
        else
        if(hfeld.compare("srst") == 0) 
          p += val.leftJustified(4,' ');
        if(hfeld.compare("sent") == 0) 
          p += val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0) 
          p += val.leftJustified(14,' ');
        else
        if(hfeld.compare("rrst") == 0) 
          p += val.leftJustified(10,' ');  
        else
	  
	if(hfeld.compare("rcvd") == 0) 
        p += val.leftJustified(7,' '); 
	  
        if(hfeld.compare("rcvd2") == 0) 
          p += val.leftJustified(7,' ');
        else
        if(hfeld.compare("tn") == 0) 
          p += val;
        idx++;
      }
      out << p+"\n";
    }
    out << "END-OF-LOG:\n";
    file.close();
    
    return cnt;
}  



int logexp::workExportRUSSDX(QString q)
{
    QString val, p, hfeld;
    int col, idx, cnt, tpoints, tmult1, tmult2;
    
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    
    QTextStream out(&file);
    out << "START-OF-LOG: 2.0\n";
    if(wArrlSec->text().count() != 0)
      out << "ARRL-SECTION: "+wArrlSec->text()+"\n";
    //out << "CONTEST: "+contest+"-"+mode+"\n";
    out << "CONTEST: "+contest+"\n";
    out << "CALLSIGN: "+wCall->text()+"\n";
    out << "CATEGORY: "+BoxCategory->currentText()+" "+BoxBand->currentText()+" "+BoxPwr->currentText()+"\n";
    
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    tmult2 = getClaimed(labelContestId->text(),"mult2");
    int sum;
    sum = tpoints * (tmult1 + tmult2);
    out << "CLAIMED-SCORE: "+s.setNum(sum)+"\n";
    s = VERSION;
    out << "CREATED-BY: QtLog "+s+" - DL1HBD\n";
    
    if(wClub->text().count() != 0)
      out << "CLUB: "+wClub->text()+"\n"; 
    
    out << "NAME: "+wName->text()+"\n";
    out << "ADDRESS: "+wStreet->text()+"\n";
    out << "ADDRESS: "+wQth->text()+"\n";
    out << "ADDRESS: "+wCountry->text()+"\n";
    out << "EMAIL: "+wMail->text()+"\n"; 
    if(wSoap->text().count() != 0)
      out << "SOAPBOX: "+wSoap->text()+"\n";

    QSqlQuery query;
    query.exec(q);  
    cnt = query.size();                               
    col = query.record().count();                     
    while(query.next()) {
      idx = 0;
      p = "QSO: ";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);        
        val = query.value(idx).toString();
	
        if(hfeld.compare("freq") == 0) {              
          int n = val.toInt();
          n = n/100 * 100;
          val.setNum(n);
          if(val.count() != 5) {
             p += val.rightJustified(5,' ');          
             p += " ";
           }
           else
             p += val.leftJustified(6,' ');           
        }
        
        else
	if(hfeld.compare("mode") == 0) {              
          val = BoxMode->currentText();
          p += val.leftJustified(3,' ');
        }
        else  
        if(hfeld.compare("cdate") == 0) 
          p += val.leftJustified(11,' ');
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(5,' ');
          p += val;
          p += wCall->text().leftJustified(14,' ');   
        }     
        else
        if(hfeld.compare("srst") == 0) 
          p += val.leftJustified(4,' ');
        if(hfeld.compare("sent") == 0) 
          p += val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0) 
          p += val.leftJustified(14,' ');
        else
        if(hfeld.compare("rrst") == 0) 
          p += val.leftJustified(10,' ');  
        else
	  
	if(hfeld.compare("rcvd") == 0) 
        p += val.leftJustified(7,' '); 
	  
        if(hfeld.compare("rcvd2") == 0) 
          p += val.leftJustified(7,' ');
        else
        if(hfeld.compare("tn") == 0) 
          p += val;
        idx++;
      }
      out << p+"\n";
    }
    out << "END-OF-LOG:\n";
    file.close();
    
    return cnt;
}  



int logexp::workExportOCEANIADX(QString q)
{
    QString val, p, hfeld;
    int col, idx, cnt, tpoints, tmult1;
    
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    
    out << "START-OF-LOG: 3.0\n";
    if(wArrlSec->text().count() != 0)
      out << "ARRL-SECTION: "+wArrlSec->text()+"\n";
    
    out << "CONTEST: "+contest+"-"+mode+"\n";  
    out << "CALLSIGN: "+wCall->text()+"\n";
    out << "CATEGORY-OPERATOR: "+BoxCategory->currentText()+"\n";
    out << "CATEGORY-BAND: "+BoxBand->currentText()+"\n";
    out << "CATEGORY-POWER: "+BoxPwr->currentText()+"\n";
    out << "CATEGORY-TRANSMITTER: "+BoxTransmitter->currentText()+"\n";  
    
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    //tmult2 = getClaimed(labelContestId->text(),"mult2");
    int sum;
    //sum = tpoints * (tmult1 + tmult2);
    sum = tpoints * tmult1;
    out << "CLAIMED-SCORE: "+s.setNum(sum)+"\n";
    s = VERSION;
    out << "CREATED-BY: QtLog "+s+" - DL1HBD ( "+h.date().toString("yyyy-MM-dd")+" )\n";
  
    out << "NAME: "+wName->text()+"\n";
    out << "ADDRESS: "+wStreet->text()+"\n";
    out << "ADDRESS-CITY: "+wQth->text()+"\n";
    out << "ADDRESS-COUNTRY: "+wCountry->text()+"\n";
    out << "EMAIL: "+wMail->text()+"\n"; 
    out << "SOAPBOX: "+wSoap->text()+"\n";

    QSqlQuery query;
    query.exec(q);  
    cnt = query.size();                               
    col = query.record().count();                     
    while(query.next()) {
      idx = 0;
      p = "QSO: ";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);        
        val = query.value(idx).toString();
	
        if(hfeld.compare("freq") == 0) {              
          int n = val.toInt();
          n = n/100 * 100;
          val.setNum(n);
          if(val.count() != 5) {
             p += val.rightJustified(5,' ');          
             p += " ";
           }
           else
             p += val.leftJustified(6,' ');           
        }
        
        else
        if(hfeld.compare("mode") == 0) {              
          val = BoxMode->currentText();
          p += val.leftJustified(3,' ');
        }
        else
        if(hfeld.compare("cdate") == 0)               
          p += val.leftJustified(11,' ');
        else
        if(hfeld.compare("ctime") == 0) {             
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(5,' ');
          p += val;
          p += wCall->text().leftJustified(14,' ');   
        }     
        else
        if(hfeld.compare("srst") == 0)                
          p += val.leftJustified(4,' ');
        if(hfeld.compare("sent") == 0)                
          p += val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0)               
          p += val.leftJustified(14,' ');
        else
        if(hfeld.compare("rrst") == 0)                
          p += val.leftJustified(4,' ');              
        else
	  
	if(hfeld.compare("rcvd") == 0)                
        p += val.leftJustified(7,' '); 
	  
        if(hfeld.compare("rcvd2") == 0)               
          p += val.leftJustified(7,' ');
        else
        if(hfeld.compare("tn") == 0) 
          p += val;
        idx++;
      }
      out << p+"\n";                                  
    }
    out << "END-OF-LOG:\n";
    file.close();
    return cnt;
}  



int logexp::workExportUBALB(QString q)
{
    QString val, p, hfeld;
    int col, idx, cnt, tpoints, tmult1, tmult2;
    
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    
    QTextStream out(&file);
    out << "START-OF-LOG: 2.0\n";
    if(wArrlSec->text().count() != 0)
      out << "ARRL-SECTION: "+wArrlSec->text()+"\n";
    //out << "CONTEST: "+contest+"-"+mode+"\n";
    out << "CONTEST: "+contest+"\n";  
    out << "CALLSIGN: "+wCall->text()+"\n";
    out << "CATEGORY: "+BoxCategory->currentText()+" "+BoxBand->currentText()+" "+BoxPwr->currentText()+" "+BoxMode->currentText()+"\n";
    
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    tmult2 = getClaimed(labelContestId->text(),"mult2");
    int sum;
    sum = tpoints * (tmult1 + tmult2);
    out << "CLAIMED-SCORE: "+s.setNum(sum)+"\n";
    s = VERSION;
    out << "CREATED-BY: QtLog "+s+" - DL1HBD  "+h.date().toString("yyyy-MM-dd")+"\n";
    
    if(wClub->text().count() != 0)
      out << "CLUB: "+wClub->text()+"\n"; 
    
    out << "NAME: "+wName->text()+"\n";
    out << "ADDRESS: "+wStreet->text()+"\n";
    out << "ADDRESS: "+wQth->text()+"\n";
    out << "ADDRESS: "+wCountry->text()+"\n";
    out << "EMAIL: "+wMail->text()+"\n"; 
    if(wSoap->text().count() != 0)
      out << "SOAPBOX: "+wSoap->text()+"\n";

    QSqlQuery query;
    query.exec(q);  
    cnt = query.size();                               
    col = query.record().count();                     
    while(query.next()) {
      idx = 0;
      p = "QSO: ";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);        
        val = query.value(idx).toString();
	
        if(hfeld.compare("freq") == 0) {              
          int n = val.toInt();
          n = n/100 * 100;
          val.setNum(n);
          if(val.count() != 5) {
             p += val.rightJustified(5,' ');          
             p += " ";
           }
           else
             p += val.leftJustified(6,' ');           
        }
        
        else
        if(hfeld.compare("mode") == 0) {              
          val = BoxMode->currentText();
	  if(val.compare("ssb"))
	    val = "PH";
          p += val.leftJustified(3,' ');
        }
        else
        if(hfeld.compare("cdate") == 0) 
          p += val.leftJustified(11,' ');
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(5,' ');
          p += val;
          p += wCall->text().leftJustified(14,' ');   
        }     
        else
        if(hfeld.compare("srst") == 0) 
          p += val.leftJustified(4,' ');
        if(hfeld.compare("sent") == 0) 
          p += val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0) 
          p += val.leftJustified(14,' ');
        else
        if(hfeld.compare("rrst") == 0) 
          p += val.leftJustified(5,' ');  
        else
	if(hfeld.compare("rcvd") == 0) 
	  p += val.leftJustified(7,' '); 
	else
	//if(hfeld.compare("multi2") == 0) 
        
        //else
        if(hfeld.compare("tn") == 0) 
          p += val;
        idx++;
      }
      out << p+"\n";
    }
    out << "END-OF-LOG:\n";
    file.close();
    
    return cnt;
}  



int logexp::workExportUBADX(QString q)
{
    QString val, p, hfeld;
    int col, idx, cnt, tpoints, tmult1, tmult2;
    
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    
    QTextStream out(&file);
    out << "START-OF-LOG: 2.0\n";
    if(wArrlSec->text().count() != 0)
      out << "ARRL-SECTION: "+wArrlSec->text()+"\n";
    //out << "CONTEST: "+contest+"-"+mode+"\n";
    out << "CONTEST: "+contest+"\n";  
    out << "CALLSIGN: "+wCall->text()+"\n";
    out << "CATEGORY: "+BoxCategory->currentText()+" "+BoxBand->currentText()+" "+BoxPwr->currentText()+" "+BoxMode->currentText()+"\n";
    
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    tmult2 = getClaimed(labelContestId->text(),"mult2");
    int sum;
    sum = tpoints * (tmult1 + tmult2);
    out << "CLAIMED-SCORE: "+s.setNum(sum)+"\n";
    s = VERSION;
    out << "CREATED-BY: QtLog "+s+" - DL1HBD  "+h.date().toString("yyyy-MM-dd")+"\n";
    
    if(wClub->text().count() != 0)
      out << "CLUB: "+wClub->text()+"\n"; 
    
    out << "NAME: "+wName->text()+"\n";
    out << "ADDRESS: "+wStreet->text()+"\n";
    out << "ADDRESS: "+wQth->text()+"\n";
    out << "ADDRESS: "+wCountry->text()+"\n";
    out << "EMAIL: "+wMail->text()+"\n"; 
    if(wSoap->text().count() != 0)
      out << "SOAPBOX: "+wSoap->text()+"\n";

    QSqlQuery query;
    query.exec(q);  
    cnt = query.size();                               
    col = query.record().count();                     
    while(query.next()) {
      idx = 0;
      p = "QSO: ";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);        
        val = query.value(idx).toString();
	
        if(hfeld.compare("freq") == 0) {              
          int n = val.toInt();
          n = n/100 * 100;
          val.setNum(n);
          if(val.count() != 5) {
             p += val.rightJustified(5,' ');          
             p += " ";
           }
           else
             p += val.leftJustified(6,' ');           
        }
        
        else
        if(hfeld.compare("mode") == 0) {              
          val = BoxMode->currentText();
	  if(val.compare("ssb"))
	    val = "PH";
          p += val.leftJustified(3,' ');
        }
        else
        if(hfeld.compare("cdate") == 0) 
          p += val.leftJustified(11,' ');
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(5,' ');
          p += val;
          p += wCall->text().leftJustified(14,' ');   
        }     
        else
        if(hfeld.compare("srst") == 0) 
          p += val.leftJustified(4,' ');
        if(hfeld.compare("sent") == 0) 
          p += val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0) 
          p += val.leftJustified(14,' ');
        else
        if(hfeld.compare("rrst") == 0) 
          p += val.leftJustified(5,' ');  
        else
	if(hfeld.compare("rcvd") == 0) 
	  p += val.leftJustified(7,' '); 
	else
	if(hfeld.compare("rcvd2") == 0) 
          p += val.leftJustified(7,' ');
        else
        if(hfeld.compare("tn") == 0) 
          p += val;
        idx++;
      }
      out << p+"\n";
    }
    out << "END-OF-LOG:\n";
    file.close();
    
    return cnt;
}  



int logexp::workExportREFC(QString q)
{
    QString val, p, hfeld;
    int col, idx, cnt, tpoints, tmult1, tmult2;
    
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    
    QTextStream out(&file);
    out << "START-OF-LOG: 2.0\n";
    if(wArrlSec->text().count() != 0)
      out << "ARRL-SECTION: "+wArrlSec->text()+"\n";
    out << "CONTEST: "+contest+"-"+mode+"\n";
    //out << "CONTEST: "+contest+"\n";  
    out << "CALLSIGN: "+wCall->text()+"\n";
    out << "CATEGORY: "+BoxCategory->currentText()+" "+BoxBand->currentText()+" "+BoxPwr->currentText()+" "+BoxMode->currentText()+"\n";
    
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    tmult2 = getClaimed(labelContestId->text(),"mult2");
    int sum;
    sum = tpoints * (tmult1 + tmult2);
    out << "CLAIMED-SCORE: "+s.setNum(sum)+"\n";
    s = VERSION;
    out << "CREATED-BY: QtLog "+s+" - DL1HBD  "+h.date().toString("yyyy-MM-dd")+"\n";
    
    if(wClub->text().count() != 0)
      out << "CLUB: "+wClub->text()+"\n"; 
    
    out << "NAME: "+wName->text()+"\n";
    out << "ADDRESS: "+wStreet->text()+"\n";
    out << "ADDRESS: "+wQth->text()+"\n";
    out << "ADDRESS: "+wCountry->text()+"\n";
    out << "EMAIL: "+wMail->text()+"\n"; 
    if(wSoap->text().count() != 0)
      out << "SOAPBOX: "+wSoap->text()+"\n";

    QSqlQuery query;
    query.exec(q);  
    cnt = query.size();                               
    col = query.record().count();                     
    while(query.next()) {
      idx = 0;
      p = "QSO: ";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);        
        val = query.value(idx).toString();
	
        if(hfeld.compare("freq") == 0) {              
          int n = val.toInt();
          n = n/100 * 100;
          val.setNum(n);
          if(val.count() != 5) {
             p += val.rightJustified(5,' ');          
             p += " ";
           }
           else
             p += val.leftJustified(6,' ');           
        }
        
        else
        if(hfeld.compare("mode") == 0) {              
          val = BoxMode->currentText();
	  if(val.compare("ssb"))
	    val = "PH";
          p += val.leftJustified(3,' ');
        }
        else
        if(hfeld.compare("cdate") == 0) 
          p += val.leftJustified(12,' ');
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(5,' ');
          p += val;
          p += wCall->text().leftJustified(14,' ');   
        }     
        else
        if(hfeld.compare("srst") == 0) 
          p += val.leftJustified(4,' ');
        if(hfeld.compare("sent") == 0) 
          p += val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0) 
          p += val.leftJustified(14,' ');
        else
        if(hfeld.compare("rrst") == 0) 
          p += val.leftJustified(5,' ');  
        else
	if(hfeld.compare("rcvd") == 0) 
	  p += val.leftJustified(6,' '); 
	else
	if(hfeld.compare("multi1") == 0) 
          p += val.leftJustified(6,' ');
        else
        if(hfeld.compare("tn") == 0) 
          p += val;
        idx++;
      }
      out << p+"\n";
    }
    out << "END-OF-LOG:\n";
    file.close();
    
    return cnt;
}  



int logexp::workExportSPDX(QString q)
{
    QString val, p, hfeld;
    int col, idx, cnt, tpoints, tmult1, tmult2;
    
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    
    QTextStream out(&file);
    out << "START-OF-LOG: 2.0\n";
    if(wArrlSec->text().count() != 0)
      out << "ARRL-SECTION: "+wArrlSec->text()+"\n";
    out << "CONTEST: "+contest+"-"+mode+"\n";
    //out << "CONTEST: "+contest+"\n";  
    out << "CALLSIGN: "+wCall->text()+"\n";
    out << "CATEGORY: "+BoxCategory->currentText()+" "+BoxBand->currentText()+" "+BoxPwr->currentText()+" "+BoxMode->currentText()+"\n";
    
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    tmult2 = getClaimed(labelContestId->text(),"mult2");
    int sum;
    sum = tpoints * (tmult1 + tmult2);
    out << "CLAIMED-SCORE: "+s.setNum(sum)+"\n";
    s = VERSION;
    out << "CREATED-BY: QtLog "+s+" - DL1HBD  "+h.date().toString("yyyy-MM-dd")+"\n";
    
    if(wClub->text().count() != 0)
      out << "CLUB: "+wClub->text()+"\n"; 
    
    out << "NAME: "+wName->text()+"\n";
    out << "ADDRESS: "+wStreet->text()+"\n";
    out << "ADDRESS: "+wQth->text()+"\n";
    out << "ADDRESS: "+wCountry->text()+"\n";
    out << "EMAIL: "+wMail->text()+"\n"; 
    if(wSoap->text().count() != 0)
      out << "SOAPBOX: "+wSoap->text()+"\n";

    QSqlQuery query;
    query.exec(q);  
    cnt = query.size();                               
    col = query.record().count();                     
    while(query.next()) {
      idx = 0;
      p = "QSO: ";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);        
        val = query.value(idx).toString();
	
        if(hfeld.compare("freq") == 0) {              
          int n = val.toInt();
          n = n/100 * 100;
          val.setNum(n);
          if(val.count() != 5) {
             p += val.rightJustified(5,' ');          
             p += " ";
           }
           else
             p += val.leftJustified(6,' ');           
        }
        
        else
        if(hfeld.compare("mode") == 0) {              
          val = BoxMode->currentText();
	  if(val.compare("ssb"))
	    val = "PH";
          p += val.leftJustified(3,' ');
        }
        else
        if(hfeld.compare("cdate") == 0) 
          p += val.leftJustified(12,' ');
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(5,' ');
          p += val;
          p += wCall->text().leftJustified(14,' ');   
        }     
        else
        if(hfeld.compare("srst") == 0) 
          p += val.leftJustified(4,' ');
        if(hfeld.compare("sent") == 0) 
          p += val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0) 
          p += val.leftJustified(14,' ');
        else
        if(hfeld.compare("rrst") == 0) 
          p += val.leftJustified(5,' ');  
        else
	if(hfeld.compare("rcvd") == 0) 
	  p += val.leftJustified(6,' '); 
	else
	if(hfeld.compare("multi1") == 0) 
          p += val.leftJustified(6,' ');
        else
        if(hfeld.compare("tn") == 0) 
          p += val;
        idx++;
      }
      out << p+"\n";
    }
    out << "END-OF-LOG:\n";
    file.close();
    
    return cnt;
}  






void logexp::creatSysName()
{
    int i;
    QString a,t;
    QString h="Contest,contid;Trs,trans;Date,cdate;Time,ctime;Band,band;Freq,freq;Mode,mode;Call,qcall;SRst,srst;Sent,sent;Sent2,sent2;RRst,rrst;Rcvd,rcvd;Rcvd2,rcvd2;Pts,pts;WAE,wae;Dxcc,dxcc;Cnt,cnt;Mult,multi1;Mult2,multi2;Qtcflg,qtcflg;";
    while(1) {
      i = h.indexOf(";");
      if( i  != -1) {
         t = h.left(i);       
         h.remove(0, ++i);
         i = t.indexOf(",");
         a = t.left(i);
         t.remove(0, ++i);
         toSysName.insert(a,t);
      }
      else 
       break;                 
    }
}



int logexp::workExportWAG(QString q)
{
    QString ts, t;
    int x, tqsos, tpoints, tmult1;
    h = QDateTime::currentDateTime().toUTC();   
    t = h.date().toString("yyyy-MM-dd");
    t += " / ";
    t += h.time().toString("hh:mm:ss");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    s = VERSION;
    out << "STF1\r\n";
    out << "# Created by QtLog "+s+" - DL1HBD\r\n";
    out << "# File written "+t+" UTC\r\n";
    out << "Header\r\n";
    out << "Contest   WAG\r\n";
    out << "MyCall    "+wCall->text()+"\r\n";
    out << "Category  "+BoxCategory->currentText()+", "+BoxBand->currentText()+", "+BoxPwr->currentText()+"\r\n";
    out << "Club      "+wClub->text()+"\r\n";
    out << "Specific  "+wSpecific->text()+"\r\n";
    out << "MailAddress "+wName->text()+"\r\n";       
    out << "MailAddress "+wStreet->text()+"\r\n";     
    out << "MailAddress "+wQth->text()+"\r\n";        
    out << "MailAddress "+wMail->text()+"\r\n";       
    if(wCountry->text().count() != 0)
      out << "MailAddress "+wCountry->text()+"\r\n";  
    out << "Soapbox  "+wSoap->text()+"\r\n";
    tqsos = getClaimed(labelContestId->text(),"qsos");
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    out << "ClaimedQso   "+s.setNum(tqsos)+"\r\n";
    out << "ClaimedPts   "+s.setNum(tpoints)+"\r\n";
    out << "ClaimedMult  "+s.setNum(tmult1)+"\r\n"; 
    x = tpoints * tmult1;
    out << "ClaimedScore "+s.setNum(x)+"\r\n";

    out << "QsoOrder "+qsoOrder+"\r\n"; 
    out << "EndHeader\r\n\r\n";

    out << "QsoList \r\n";
    file.close();
    
    return readQsoData(path, q);
}



int logexp::workExportDTC(QString q)
{
    QString ts, t;
    int tqsos, tpoints;
    h = QDateTime::currentDateTime().toUTC();   
    t = h.date().toString("yyyy-MM-dd");
    t += " / ";
    t += h.time().toString("hh:mm:ss");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    s = VERSION;
    out << "STF1\r\n";
    out << "# Created by QtLog "+s+" - DL1HBD\r\n";
    out << "# File written "+t+" UTC\r\n";
    out << "Header\r\n";
    out << "Contest   "+labelContestId->text()+"\r\n";
    out << "MyCall    "+wCall->text()+"\r\n";
    out << "Category  "+BoxCategory->currentText()+", "+BoxBand->currentText()+", "+BoxPwr->currentText()+"-POWER\r\n";
    out << "MailAddress "+wName->text()+"\r\n";        
    out << "MailAddress "+wStreet->text()+"\r\n";      
    out << "MailAddress "+wQth->text()+"\r\n";         
    out << "MailAddress "+wMail->text()+"\r\n";        
    
    if(wCountry->text().count() != 0)
      out << "MailAddress "+wCountry->text()+"\r\n"; 
    if(wSoap->text().count() != 0)
      out << "Soapbox  "+wSoap->text()+"\r\n";
    tqsos = getClaimed(labelContestId->text(),"qsos");
    tpoints = getClaimed(labelContestId->text(),"points");
    out << "ClaimedQso  "+s.setNum(tqsos)+"\r\n";
    out << "ClaimedPts  "+s.setNum(tpoints)+"\r\n";
    out << "QsoOrder "+qsoOrder+"\r\n"; 
    out << "EndHeader\r\n\r\n";
    out << "QsoList \r\n";
    file.close();
    
    return readQsoData(path, q);
}




int logexp::workExportXMAS(QString q)
{
    QString ts, t;
    int  tqsos, tpoints, tmult1, tmult2, sum;
    h = QDateTime::currentDateTime().toUTC();   
    t = h.date().toString("yyyy-MM-dd");
    t += " / ";
    t += h.time().toString("hh:mm:ss");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    s = VERSION;
    out << "STF1\r\n";
    out << "# Created by QtLog "+s+" - DL1HBD\r\n";
    out << "# File written "+t+" UTC\r\n";
    out << "Header\r\n";
    out << "Contest   XMAS - "+wContestName->text()+"\r\n";
    out << "MyCall    "+wCall->text()+"\r\n";
    out << "Specific  "+wSpecific->text()+"\r\n";
    out << "Category  "+BoxCategory->currentText()+", "+BoxBand->currentText()+", "+BoxPwr->currentText()+"\r\n";
    out << "MailAddress "+wName->text()+"\r\n";        
    out << "MailAddress "+wStreet->text()+"\r\n";      
    out << "MailAddress "+wQth->text()+"\r\n";         
    out << "MailAddress "+wMail->text()+"\r\n";        
    if(wCountry->text().count() != 0)
      out << "MailAddress "+wCountry->text()+"\r\n";  
    if(wSoap->text().count() != 0)
      out << "Soapbox  "+wSoap->text()+"\r\n";
    tqsos = getClaimed(labelContestId->text(),"qsos");
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    tmult2 = getClaimed(labelContestId->text(),"mult2");
    out << "ClaimedQso  "+s.setNum(tqsos)+"\r";
    out << "ClaimedPts  "+s.setNum(tpoints)+"\r\n";
    out << "ClaimedMult  "+s.setNum(tmult1)+"\r\n";
    out << "ClaimedMult2 "+s.setNum(tmult2)+"\r\n";
    sum = tpoints * (tmult1 + tmult2);
    out << "ClaimedScore "+s.setNum(sum)+"\r\n";

    out << "QsoOrder "+qsoOrder+"\r\n";
    out << "EndHeader\r\n\r\n";
    out << "QsoList \r\n";
    file.close();
    
    return readQsoData(path, q);
}




int logexp::workExportCQPTY(QString q)
{
    QString ts, t;
    int  tqsos, tpoints, tmult1, tmult2;
    h = QDateTime::currentDateTime().toUTC();   
    t = h.date().toString("yyyy-MM-dd");
    t += " / ";
    t += h.time().toString("hh:mm:ss");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    s = VERSION;
    out << "STF1\r\n";
    out << "# Created by QtLog "+s+" - DL1HBD\r\n";
    out << "# File written "+t+" UTC\r\n";
    out << "Header\r\n";
    out << "Contest   "+wContestName->text()+"\r\n";
    out << "MyCall    "+wCall->text()+"\r\n";
    out << "Specific  "+wSpecific->text()+"\r\n";
    out << "Category  "+BoxCategory->currentText()+", "+BoxBand->currentText()+", "+BoxPwr->currentText()+"\r\n";
    out << "MailAddress "+wName->text()+"\r\n";        
    out << "MailAddress "+wStreet->text()+"\r\n";      
    out << "MailAddress "+wQth->text()+"\r\n";         
    out << "MailAddress "+wMail->text()+"\r\n";        
    if(wCountry->text().count() != 0)
      out << "MailAddress "+wCountry->text()+"\r\n"; 
    if(wSoap->text().count() != 0)
      out << "Soapbox  "+wSoap->text()+"\r\n";
    tqsos = getClaimed(labelContestId->text(),"qsos");
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    tmult2 = getClaimed(labelContestId->text(),"mult2");
    out << "ClaimedQso  "+s.setNum(tqsos)+"\r";
    out << "ClaimedPts  "+s.setNum(tpoints)+"\r\n";
    out << "ClaimedMult  "+s.setNum(tmult1)+"\r\n";
    //out << "ClaimedMult2 "+s.setNum(tmult2)+"\r\n";
    //sum = tpoints * (tmult1 + tmult2);
    //out << "ClaimedScore "+s.setNum(sum)+"\r\n";
    out << "ClaimedScore "+s.setNum(tpoints)+"\r\n";

    out << "QsoOrder "+qsoOrder+"\r\n";
    out << "EndHeader\r\n\r\n";
    out << "QsoList \r\n";
    file.close();
    
    return readQsoData(path, q);
}



int logexp::workExportDARC10M(QString q)
{
    QString ts, t;
    int  tqsos, tpoints, tmult1, tmult2, sum;
    h = QDateTime::currentDateTime().toUTC();   
    t = h.date().toString("yyyy-MM-dd");
    t += " / ";
    t += h.time().toString("hh:mm:ss");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    s = VERSION;
    out << "STF1\r\n";
    out << "# Created by QtLog "+s+" - DL1HBD\r\n";
    out << "# File written "+t+" UTC\r\n";
    out << "Header\r\n";
    out << "Contest   "+labelContestId->text()+"\r\n";
    out << "MyCall    "+wCall->text()+"\r\n";
    out << "Category  "+BoxCategory->currentText()+", "+BoxBand->currentText()+", "+BoxPwr->currentText()+"\r\n";
    out << "MailAddress "+wName->text()+"\r\n";        
    out << "MailAddress "+wStreet->text()+"\r\n";      
    out << "MailAddress "+wQth->text()+"\r\n";         
    out << "MailAddress "+wMail->text()+"\r\n";        
    
    if(wCountry->text().count() != 0)
      out << "MailAddress "+wCountry->text()+"\r\n"; 
    if(wSoap->text().count() != 0)
      out << "Soapbox  "+wSoap->text()+"\r\n";
    tqsos = getClaimed(labelContestId->text(),"qsos");
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    tmult2 = getClaimed(labelContestId->text(),"mult2");
    out << "ClaimedQso  "+s.setNum(tqsos)+"\r\n";
    out << "ClaimedPts  "+s.setNum(tpoints)+"\r\n";
    out << "ClaimedMult  "+s.setNum(tmult1)+"\r\n";
    out << "ClaimedMult2 "+s.setNum(tmult2)+"\r\n";
    sum = tpoints * (tmult1 + tmult2);
    out << "ClaimedScore "+s.setNum(sum)+"\r\n";

    out << "QsoOrder "+qsoOrder+"\r\n"; 
    out << "EndHeader\r\n\r\n";
    out << "QsoList \r\n";
    file.close();
    
    return readQsoData_2(path, q);
}



int logexp::workExportHNY(QString q)
{
    QString ts, t;
    int  tqsos, tpoints, tmult1, tmult2, sum;
    h = QDateTime::currentDateTime().toUTC();   
    t = h.date().toString("yyyy-MM-dd");
    t += " / ";
    t += h.time().toString("hh:mm:ss");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    s = VERSION;
    out << "STF1\r\n";
    out << "# Created by QtLog "+s+" - DL1HBD\r\n";
    out << "# File written "+t+" UTC\r\n";
    out << "Header\r\n";
    out << "Contest   AGCW "+labelContestId->text()+"\r\n";
    out << "MyCall    "+wCall->text()+"\r\n";
    out << "Category  "+BoxCategory->currentText()+", "+BoxBand->currentText()+", "+BoxPwr->currentText()+"\r\n";
    out << "MailAddress "+wName->text()+"\r\n";        
    out << "MailAddress "+wStreet->text()+"\r\n";      
    out << "MailAddress "+wQth->text()+"\r\n";         
    out << "MailAddress "+wMail->text()+"\r\n";        
    if(wCountry->text().count() != 0)
      out << "MailAddress "+wCountry->text()+"\r\n"; 
    if(wSoap->text().count() != 0)
      out << "Soapbox  "+wSoap->text()+"\r\n";
    tqsos = getClaimed(labelContestId->text(),"qsos");
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    tmult2 = getClaimed(labelContestId->text(),"mult2");
    out << "ClaimedQso  "+s.setNum(tqsos)+"\r";
    out << "ClaimedPts  "+s.setNum(tpoints)+"\r\n";
    out << "ClaimedMult  "+s.setNum(tmult1)+"\r\n";
    out << "ClaimedMult2 "+s.setNum(tmult2)+"\r\n";
    sum = tpoints * (tmult1 + tmult2);
    out << "ClaimedScore "+s.setNum(sum)+"\r\n";

    out << "QsoOrder "+qsoOrder+"\r\n";
    out << "EndHeader\r\n\r\n";
    out << "QsoList \r\n";
    file.close();
    
    return readQsoData_2(path, q);
}



int logexp::workExportAGCWHTP(QString q)
{
    QString ts, t, f;
    int  tqsos, tpoints, tmult1, tmult2, sum;
    h = QDateTime::currentDateTime().toUTC();   
    t = h.date().toString("yyyy-MM-dd");
    t += " / ";
    t += h.time().toString("hh:mm:ss");
    
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    s = VERSION;
    out << "STF1\r\n";
    out << "# Created by QtLog "+s+" - DL1HBD\r\n";
    out << "# File written "+t+" UTC\r\n";
    out << "Header\r\n";
    
    s = labelContestId->text();
    i = labelContestId->text().indexOf("_");
    f = labelContestId->text().left(i);
    t = h.date().toString("MM");
    if(t.toInt() < 6)
      f += " ( HTP80 )";
    else
      f += " ( HTP40 )";
    out << "Contest   "+f+"\r\n";
    out << "MyCall    "+wCall->text()+"\r\n";
    out << "Category  "+BoxCategory->currentText()+", "+BoxBand->currentText()+", "+BoxPwr->currentText()+"\r\n";
    out << "MailAddress "+wName->text()+"\r\n";        
    out << "MailAddress "+wStreet->text()+"\r\n";      
    out << "MailAddress "+wQth->text()+"\r\n";         
    out << "MailAddress "+wMail->text()+"\r\n";        
    if(wCountry->text().count() != 0)
      out << "MailAddress "+wCountry->text()+"\r\n"; 
    if(wSoap->text().count() != 0)
      out << "Soapbox  "+wSoap->text()+"\r\n";
    tqsos = getClaimed(labelContestId->text(),"qsos");
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = 0;    //getClaimed(labelContestId->text(),"mult1");
    tmult2 = 0;    //getClaimed(labelContestId->text(),"mult2");
    out << "ClaimedQso  "+s.setNum(tqsos)+"\r";
    out << "ClaimedPts  "+s.setNum(tpoints)+"\r\n";
    //out << "ClaimedMult  "+s.setNum(tmult1)+"\r\n";
    //out << "ClaimedMult2 "+s.setNum(tmult2)+"\r\n";
    //sum = tpoints * (tmult1 + tmult2);
    sum = tpoints;
    //out << "ClaimedScore "+s.setNum(sum)+"\r\n";

    out << "QsoOrder "+qsoOrder+"\r\n";
    out << "EndHeader\r\n\r\n";
    out << "QsoList \r\n";
    file.close();
    
    return readQsoData_2(path, q);
}
/*



int logexp::workExportCHECK(QString q)
{
    QString ts, t;
    int  tqsos;
    h = QDateTime::currentDateTime().toUTC();   
    t = h.date().toString("yyyy-MM-dd");
    t += " / ";
    t += h.time().toString("hh:mm:ss");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    s = VERSION;
    out << "STF1\r\n";
    out << "# Created by QtLog "+s+" - DL1HBD\r\n";
    out << "# File written "+t+" UTC\r\n";
    out << "Header\r\n";
    out << "MyCall    "+wCall->text()+"\r\n";
    out << "Category  "+BoxCategory->currentText()+", "+BoxBand->currentText()+", "+BoxPwr->currentText()+"\r\n";
    out << "MailAddress "+wName->text()+"\r\n";        
    out << "MailAddress "+wStreet->text()+"\r\n";      
    out << "MailAddress "+wQth->text()+"\r\n";         
    out << "MailAddress "+wMail->text()+"\r\n";        
    out << "Soapbox  "+wSoap->text()+"\r\n";
    tqsos = getClaimed(labelContestId->text(),"qsos");
    out << "ClaimedQso  "+s.setNum(tqsos)+"\r";

    out << "QsoOrder "+qsoOrder+"\r\n";
    out << "EndHeader\r\n\r\n";
    out << "QsoList \r\n";
    file.close();
    return readQsoData(path, q);
}
*/



int logexp::readQsoData(QString fpath, QString q)
{
    QString ts, t;
    QString val, p, hfeld;
    int x, col, idx,row;
 
    QFile file(fpath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "FEHLER" << fpath;
       return 0;
    }
    QTextStream out(&file);
    q = qsoOrder.simplified();           
    
    QSqlQuery query;
    qy = "SELECT ";
    while( 1 ) {
       i = q.indexOf(" ");
       if(i == -1) break;
       ts = q.left(i);
       qy += getSysName(ts)+",";
       q.remove(0,i +1);
    }
    qy += getSysName(q);                          
    qy += " FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
    query.exec(qy);
    row = query.size();                          
    col = query.record().count();
 
    while(query.next()) {
      idx = 0;
      p = "";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);   
        val = query.value(idx).toString();
        if(hfeld.compare("cdate") == 0) 
          val = val.remove(QChar('-'), Qt::CaseInsensitive);
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(6,' ');
        }
        else
        if(hfeld.compare("band") == 0)
          val = val.leftJustified(3,' ');
        else
        if(hfeld.compare("mode") == 0)
          val = val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0)
          val = val.leftJustified(14,' ');
        else
        if(hfeld.compare("sent") == 0)
          val = val.leftJustified(6,' ');
        else
        if(hfeld.compare("rrst") == 0)
          val = val.leftJustified(4,' ');
	else
        if(hfeld.compare("rcvd") == 0)
          val = val.leftJustified(10,' ');
	else 
	if(hfeld.compare("pts") == 0)
          val = val.rightJustified(4,' ');
        else
        if(hfeld.compare("multi1") == 0) {
          i = val.count();
          x = 8;
          val = val.leftJustified(6,' ');
        }
        else
        if(hfeld.compare("multi2") == 0) {
          if(val.count() == 0)
            val = "-";
          else 
            val = val.leftJustified(6,' ');
        }
        p += val += " ";
        idx++;
      }                              
      out << p+"\r\n";               
    }                                
    out << "EndQsoList" << "\r\n";   
    file.close();
    return row;
}



int logexp::readQsoData_2(QString fpath, QString q)
{
    QString ts, t;
    QString val, l, m, p, hfeld;
    int col, idx,row;
 
    QFile file(fpath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "FEHLER" << fpath;
       return 0;
    }
    QTextStream out(&file);
    q = qsoOrder.simplified();           
    qy = "SELECT ";
    while( 1 ) {
       i = q.indexOf(" ");
       if(i == -1) break;
       ts = q.left(i);
       qy += getSysName(ts)+",";
       q.remove(0,i +1);
    }
    qy += getSysName(q);                          
    QSqlQuery query;
    qy += " FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
    query.exec(qy);
    row = query.size();                          
    col = query.record().count();
    while(query.next()) {
      idx = 0;
      p = "";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);   
        val = query.value(idx).toString();
        if(hfeld.compare("cdate") == 0) 
          val = val.remove(QChar('-'), Qt::CaseInsensitive);
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(6,' ');
        }
        else
        if(hfeld.compare("band") == 0)
          val = val.leftJustified(3,' ');
        else
        if(hfeld.compare("mode") == 0)
          val = val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0)
          val = val.leftJustified(12,' ');
	
        else
        if(hfeld.compare("sent") == 0)         
	   val = val.leftJustified(4,' ');
	else
	if(hfeld.compare("sent2") == 0)        
	   val = val.leftJustified(12,' ');
        else
        if(hfeld.compare("rrst") == 0)
          val = val.leftJustified(3,' ');
	else
          
        if(hfeld.compare("rcvd") == 0)         
           val = val.leftJustified(5,' ');
        else
        if(hfeld.compare("rcvd2") == 0) {       
           if(CNr == AGCWHTP)
	     val = val.leftJustified(14,' ');
	   else
	    val = val.leftJustified(10,' ');
	}
        else
        if(hfeld.compare("multi1") == 0) 
          val = val.leftJustified(4,' ');
        else
        if(hfeld.compare("multi2") == 0) {
          if(val.count() == 0) {
            val = "-";
          }
          val = val.leftJustified(4,' ');
        }
        p += val += " ";
        idx++;
      }                              
      out << p+"\r\n";               
    }                                
    out << "EndQsoList" << "\r\n";   
    file.close();
    return row;
}

int logexp::getMulti1poitsBand(QString Contest,QString blist)  
{
  int i, n;
     QSqlQuery query;
     qy = "SELECT "+blist +" FROM wCband WHERE contid='"+Contest+"' AND val='mult1'";
     query.exec(qy);
     i = 0;
     while(query.next()) {
       n = 0;
       i += query.value(n++).toInt();
     }
     return i;
}



int logexp::workExportWAE(QString q)
{
    QString ts, t;
    QString val, p, hfeld;
    int x, col, idx,row, tqsos, tpoints, tmult1;
    h = QDateTime::currentDateTime().toUTC();   
    t = h.date().toString("yyyy-MM-dd");
    t += " / ";
    t += h.time().toString("hh:mm:ss");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    s = VERSION;
    out << "STF1\r\n";
    out << "# Created by QtLog "+s+" - DL1HBD\r\n";
    out << "# File written "+t+" UTC\r\n";
    out << "Header\r\n";
    out << "Contest    CQ-WAEDC\r\n";           
    out << "MyCall    "+wCall->text()+"\r\n";
    out << "Category  "+BoxCategory->currentText()+", "+BoxBand->currentText()+", "+BoxPwr->currentText()+"-POWER\r\n";
    if(wClub->text() != 0) 
       out << "Club      "+wClub->text()+"\r\n";
    out << "Specific  "+wSpecific->text()+"\r\n";
    out << "MailAddress "+wName->text()+"\r\n";    
    out << "MailAddress "+wStreet->text()+"\r\n";  
    out << "MailAddress "+wQth->text()+"\r\n";     
    if(wCountry->text().count() != 0)
      out << "MailAddress "+wCountry->text()+"\r\n"; 
    out << "MailAddress "+wMail->text()+"\r\n";      
    if(wSoap->text().count() != 0)
      out << "Soapbox  "+wSoap->text()+"\r\n";       

    tqsos = getClaimed(labelContestId->text(),"qsos");
    tpoints = getClaimed(labelContestId->text(),"points");
    tmult1 = getClaimed(labelContestId->text(),"mult1");
    
    
    int sum = getMulti1poitsBand(labelContestId->text(),"b80");     
    int mSum = sum *4;
    sum = getMulti1poitsBand(labelContestId->text(),"b40");         
    mSum += sum * 3; 
    sum = getMulti1poitsBand(labelContestId->text(),"b20,b15,b10");  
    mSum += sum * 2;
    
    out << "ClaimedQso   "+s.setNum(tqsos)+"\r\n";
    out << "ClaimedPts   "+s.setNum(tpoints)+"\r\n";
    out << "ClaimedMult  "+s.setNum(tmult1)+"\r\n";
    x = tqsos * mSum;
    out << "ClaimedScore "+s.setNum(x)+"\r\n";

    QDate d;
    d = dateEdit1->date();                      
    s =  d.toString("dd-MM-yy");
    out << "OFFTIME "+s+" "+wBegin1->text()+" "+wEnd1->text()+"\r\n";
    d = dateEdit2->date();                      
    s =  d.toString("dd-MM-yy");
    out << "OFFTIME "+s+" "+wBegin2->text()+" "+wEnd2->text()+"\r\n";
    
    if(wEnd1->text().compare("00:00") == 0) { 
        QMessageBox::information(this,
        tr("Information"),
        tr("\nSet off time"),
        QMessageBox::Ok);
       return 0;
    }
//--
    out << "QsoOrder "+qsoOrder+"\r\n";
    out << "QtcOrder "+qtcOrder+"\r\n";
    out << "EndHeader\r\n\r\n";

    out << "QsoList \r\n";
    q = qsoOrder.simplified();              
    qy = "SELECT ";
    while( 1 ) {
       i = q.indexOf(" ");
       if(i == -1) break;
       ts = q.left(i);
       qy += getSysName(ts)+",";
       q.remove(0,i +1);
    }
    qy += getSysName(q);
    QSqlQuery query;
    qy += " FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
    query.exec(qy);
    row = query.size();                                 
    col = query.record().count();
 
    while(query.next()) {
      idx = 0;
      p = "";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx);          
        val = query.value(idx).toString();
        if(hfeld.compare("cdate") == 0) 
          val = val.remove(QChar('-'), Qt::CaseInsensitive);
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(6,' ');
        }
        else
        if(hfeld.compare("band") == 0)
          val = val.leftJustified(3,' ');
        else
        if(hfeld.compare("mode") == 0)
          val = val.leftJustified(6,' ');
        else
        if(hfeld.compare("qcall") == 0)
          val = val.leftJustified(12,' ');
        else
        if(hfeld.compare("sent") == 0)
          val = val.leftJustified(6,' ');
        else
        if(hfeld.compare("rrst") == 0)
          val = val.leftJustified(4,' ');
        else
        if(hfeld.compare("rcvd") == 0)
          val = val.leftJustified(12,' ');
        else
        if(hfeld.compare("multi1") == 0) {
          //i = val.count();
          //x = 8;
          val = val.leftJustified(6,' ');
        }
        else
        if(hfeld.compare("multi2") == 0) {
          if(val.count() == 0)
            val = "-";
          else 
            val = val.leftJustified(6,' ');
        }
        //p += val += "  ";
        p += val+"  ";
        idx++;
      }                                  
      out << p+"\r\n";                   
    }                                    
    out << "EndQsoList" << "\r\n\r\n";   

    
    
    
    file.close();
    return row;
}




int logexp::workExportCSV(QString q)
{
    QString ts, t;
    QString val, p, hfeld;
    int x, col, idx,row;
    
    h = QDateTime::currentDateTime().toUTC();   
    t = h.date().toString("yyyy-MM-dd");
    t += " / ";
    t += h.time().toString("hh:mm:ss");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "FEHLER" << path;
       return 0;
    }
    QTextStream out(&file);
    t = qsoOrder.simplified();           
                                         //Date Time Band Mode Call SRst Sent Sent2 RRst Rcvd Rcvd2 Pts 

    while( 1 ) {                         
       i = t.indexOf(" ");
       if(i == -1) break;
       val = t.left(i);
       if(val.compare("Date") == 0) {
         ts = val.leftJustified(14,' '); 
         val = "  "+ts;
       }
       else
       if(val.compare("Time") == 0)
         val = val.leftJustified(7,' ');
       else
       if(val.compare("Freq") == 0)
         val = val.leftJustified(8,' ');
       else
       if(val.compare("Band") == 0)
         val = val.leftJustified(8,' ');
       else
       if(val.compare("Mode") == 0)
         val = val.leftJustified(9,' ');
       else
       if(val.compare("Call") == 0)
         val = val.leftJustified(15,' ');
       else
       if(val.compare("SRst") == 0)
         val = val.leftJustified(5,' ');
       else
       if(val.compare("Sent") == 0)
         val = val.leftJustified(8,' ');
       else
       if(val.compare("Sent2") == 0)
         val = val.leftJustified(11,' ');
       else
       if(val.compare("RRst") == 0)
         val = val.leftJustified(6,' ');
       else
       if(val.compare("Rcvd") == 0)
         val = val.leftJustified(8,' ');
       else
       if(val.compare("Rcvd2") == 0)
         val = val.leftJustified(11,' ');
       else
       if(val.compare("Pts") == 0)
         val = val.leftJustified(4,' ');
       out << val;
       t.remove(0,i +1);
    }
    val = t;
    out << val;

    n  = 0;
    out << "\r\n";
    p = "-";
    while( n != 107) {                     
      out << p;
      n++;
    }
    out << "\r\n";

    q = qsoOrder.simplified();            
    qy = "SELECT ";                        
    while( 1 ) {
       i = q.indexOf(" ");
       if(i == -1) break;
       ts = q.left(i);
       qy += getSysName(ts)+",";
       q.remove(0,i +1);
    }
    qy += getSysName(q);                    
    QSqlQuery query;
    qy += " FROM wCqso WHERE contid='"+labelContestId->text()+"' ORDER BY cdate,ctime";
    query.exec(qy);
    row = query.size();                     
    col = query.record().count();
 
    while(query.next()) {
      idx = 0;
      p = "";
      while( idx != col ) {  
        hfeld = query.record().fieldName(idx); 
        val = query.value(idx).toString();
        if(hfeld.compare("cdate") == 0) {
         t = val.leftJustified(13,' ');
         val = "  "+t;
        }
        else
        if(hfeld.compare("ctime") == 0) {
          val = val.remove(QChar(':'), Qt::CaseInsensitive);
          val = val.left(4);
          val = val.leftJustified(6,' ');
        }
        else
        if(hfeld.compare("freq") == 0)
          val = val.leftJustified(8,' ');
        else
        if(hfeld.compare("band") == 0)
          val = val.leftJustified(8,' ');
        else
        if(hfeld.compare("mode") == 0)
          val = val.leftJustified(7,' ');
        else
        if(hfeld.compare("qcall") == 0)
          val = val.leftJustified(14,' ');
        else
        if(hfeld.compare("srst") == 0)
          val = val.leftJustified(5,' ');
        else
        if(hfeld.compare("sent") == 0)
          val = val.leftJustified(6,' ');
	
	else
        if(hfeld.compare("sent2") == 0)
          val = val.leftJustified(10,' ');
	
        else
        if(hfeld.compare("rrst") == 0)
          val = val.leftJustified(5,' ');
        if(hfeld.compare("rcvd") == 0)
          val = val.leftJustified(7,' ');
        else
	  
	if(hfeld.compare("rcvd2") == 0)
          val = val.leftJustified(11,' ');
        else
	 
        if(hfeld.compare("multi1") == 0) {
          i = val.count();
          x = 8;
          val = val.leftJustified(8,' ');
        }
        else
        if(hfeld.compare("multi2") == 0) {
          if(val.count() == 0)
            val = "-";
          else 
            val = val.leftJustified(6,' ');
        }
        else 
        if(hfeld.compare("pts") == 0)
          val = val.leftJustified(4,' ');
    
        p += val += " ";
        idx++;
      }                            
      out << p+"\r";               
    }                              
    out << "\r\n";                 
    file.close();
    return row;
}
