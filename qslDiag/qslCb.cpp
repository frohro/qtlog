/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n : 08.11.2008
********************************************************************** */

#include <QtGui>
#include <QtSql>
#include "qsl.h"
#include "../qtlogDiag/dirmngr.h"
#include "mkLquery.h"
#include "mkEquery.h"
#include "../qtlogDiag/version.h"
extern DirMngr dirMngr;
/* MUSTER
   qy = "SELECT id,cept,rufz ..... qth,day,(SELECT awkenner FROM funawd WHERE awtype='DLD' AND id=qid),dxcc, ....  
   FROM funom LEFT JOIN (funqsl,fun) ON (omid=oid AND qsoid=id) WHERE lotqsls='Y' ORDER BY ...";
 -------------------------------------------------------------------------------------------------*/


void qsl::buildHeaderUnreceived()             
{
     forUp = 0;
     changeButtonColor(1);                    
     qy = "SELECT id,cept,rufz,band,mode,name,qth,day,btime,";
     qy += "(SELECT awkenner FROM "+logFile+"awd WHERE awtype='"+BoxAwd->currentText()+"' AND id=qid),";
     if(BoxQslvia->currentIndex() == 0)       
        qy += "qslr,qsls,prflg,qslRvia,qslmgr,dxcc,loc,contactdOp";
     else
      if(BoxQslvia->currentIndex() == 1)      
        qy += "eqslr,eqsls,qslRvia,ag,eqslrd,eqslsd,dxcc,loc,contactdOp,qslmsg";
     else
      if(BoxQslvia->currentIndex() == 2)      
        qy += "lotqslr,lotqsls,qslRvia,lotqslrd,lotqslsd,dxcc,loc,contactdOp";
     qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
     qy += " WHERE ";
     if(BoxQslvia->currentIndex() == 0)      
        qy += "qslr = 'R'";                  
     else
     if(BoxQslvia->currentIndex() == 1)      
        qy += "eqslr = 'R' || eqslr=''";                 
     else
      if(BoxQslvia->currentIndex() == 2)     
        qy += "lotqslr = 'R'";               
     qy += " ORDER BY day DESC";
     sflg = 1;
     awdPos = 9;
     readData(qy,awdPos,0); 
}


void qsl::buildHeaderNotSend()                     
{
     forUp = 0;
     changeButtonColor(2);                         
     qy = "SELECT id,cept,rufz,band,mode,name,qth,day,btime,";
     qy += "(SELECT awkenner FROM "+logFile+"awd WHERE awtype='"+BoxAwd->currentText()+"' AND id=qid),";
     if(BoxQslvia->currentIndex() == 0)             
        qy += "qslr,qsls,prflg,qslRvia,qslmgr,dxcc,loc,contactdOp";
     else
      if(BoxQslvia->currentIndex() == 1)            
        qy += "eqslr,eqsls,qslRvia,ag,eqslrd,eqslsd,dxcc,loc,contactdOp,qslmsg";
     else
      if(BoxQslvia->currentIndex() == 2)            
        qy += "lotqslr,lotqsls,qslRvia,lotqslrd,lotqslsd,dxcc,loc,contactdOp";
     qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
     
     qy += " WHERE ";
     if(BoxQslvia->currentIndex() == 0)             
        qy += "prflg != '' ORDER BY day DESC";      
     else 
      if(BoxQslvia->currentIndex() == 1) {          
         if(checkBoxZeitraum->isChecked() == TRUE) {
           qy += "day BETWEEN '";
           QDate d = dateEditLast->date();
           s = d.toString("yyyy-MM-dd");            
           qy += s;
           qy += "' AND ";
           d = dateEditHeute->date();               
           s = d.toString("yyyy-MM-dd");
           last = s;
           qy += "'"+last+"' AND ";
        }
        qy += "(eqsls='' OR eqsls='R') ORDER BY day DESC";
        
        QSqlQuery query(qy);
        forUp = query.size();
     }
     else
      if(BoxQslvia->currentIndex() == 2) {        
       qy += "lotqsls = 'R'";                     
       if(checkBoxZeitraum->isChecked() == TRUE) {
         qy += " AND day BETWEEN '";
         QDate d = dateEditLast->date();
         s = d.toString("yyyy-MM-dd");            
         qy += s;
         qy += "' AND ";
         d = dateEditHeute->date();               
         s = d.toString("yyyy-MM-dd");
         last = s;
         qy += "'"+last+"'";
       }
       qy += " ORDER BY day DESC";               
       QSqlQuery query(qy);
       forUp = query.size();
     }
     sflg = 1;
     awdPos = 9;
     readData(qy,awdPos,0);
}


void qsl::buildHeaderNotConfirmed()            
{    
     changeButtonColor(3);                     
     forUp = 0;
     qy = "SELECT id,cept,rufz,band,mode,name,qth,day,btime,";
     qy += "(SELECT awkenner FROM "+logFile+"awd WHERE awtype='"+BoxAwd->currentText()+"' AND id=qid),";
     if(BoxQslvia->currentIndex() == 0)        
        qy += "qslr,qsls,prflg,qslRvia,qslmgr,dxcc,loc,contactdOp";
     else 
      if(BoxQslvia->currentIndex() == 1)       
        qy += "eqslr,eqsls,qslRvia,ag,eqslrd,eqslsd,dxcc,loc,contactdOp,qslmsg";
     else
      if(BoxQslvia->currentIndex() == 2)       
        qy += "qslRvia,lotqslr,lotqsls,lotqslrd,lotqslsd,dxcc,loc,contactdOp";
     qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
     qy += " WHERE ";
     if(BoxQslvia->currentIndex() == 0)             
        qy += "qslr ='-' OR qslr ='I'";        
     else
      if(BoxQslvia->currentIndex() == 1)       
       qy += "eqsls = 'U'";                    
     else
      if(BoxQslvia->currentIndex() == 2)       
        qy += "lotqsls = 'U'";                 
     qy += " ORDER BY day DESC";
 
     sflg = 1;
     awdPos = 9;
     readData(qy,awdPos,0);                    
}


void qsl::buildHeaderReceived()                
{    
     changeButtonColor(4);                     
     forUp = 0;
     qy = "SELECT id,cept,rufz,band,mode,name,qth,day,btime,";
     qy += "(SELECT awkenner FROM "+logFile+"awd WHERE awtype='"+BoxAwd->currentText()+"' AND id=qid),";
     if(BoxQslvia->currentIndex() == 0)        
        qy += "qslr,qsls,prflg,qslRvia,qslmgr,dxcc,loc,contactdOp";
     else 
      if(BoxQslvia->currentIndex() == 1)       
        qy += "eqslr,eqsls,qslRvia,ag,eqslrd,eqslsd,dxcc,loc,contactdOp,qslmsg";
     else
      if(BoxQslvia->currentIndex() == 2)       
        qy += "qslRvia,lotqslr,lotqsls,lotqslrd,lotqslsd,dxcc,loc,contactdOp";
     qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
     qy += " WHERE ";
     if(BoxQslvia->currentIndex() == 0)                
       qy += "qslr = 'Y' ORDER BY qslrd DESC";         
     else
     if(BoxQslvia->currentIndex() == 1)                
       qy += "eqslr = 'Y' ORDER By eqslrd DESC";       
     else
     if(BoxQslvia->currentIndex() == 2)                
        qy += "lotqslr = 'Y' ORDER BY lotqslrd DESC";  
     sflg = 1;
     awdPos = 9;
     readData(qy,awdPos,0);                            
}





//-----------------------------------------------------------------------------------------------
void qsl::readLastDownLoad(int fl, QString sel)
{                      
    if(BoxQslvia->currentIndex() == 1)
      ButtonNotSent->setText(QString::fromUtf8("eQSL Report"));
    else 
      ButtonNotSent->setText(QString::fromUtf8("Lotw Report"));   
    changeButtonColor(2); 
    
    forUp= 0;
    awdPos = 9;
    qy = "SELECT id,cept,rufz,band,mode,name,qth,day,btime,";
    qy += "(SELECT awkenner FROM "+logFile+"awd WHERE awtype='"+BoxAwd->currentText()+"' AND id=qid),";
    if(BoxQslvia->currentIndex() == 1)       
      qy += "eqslr,eqsls,qslRvia,eqslrd,eqslsd,dxcc,loc,contactdOp";
    if(BoxQslvia->currentIndex() == 2)       
      qy += "lotqslr,lotqsls,qslRvia,lotqslrd,lotqslsd,dxcc,loc,contactdOp";
    qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
    if( fl ) {
      qy += " WHERE sel='"+sel;              
      qy += "' ORDER BY day DESC";
      readData(qy, awdPos, fl);              
    }
    else {
     qy += " WHERE lotqslr='Y'";             
     readData(qy, awdPos, 0);
    }
}



void qsl::changeButtonColor(int but)           
{
   selectedbut = but;
   if( but == 1) {                             
     ButtonUnReceived->setPalette( QPalette(QColor(180, 210, 200)));       
     ButtonNotSent->setPalette( QPalette(QColor(224, 224, 224)));
     ButtonUnConfirmed->setPalette( QPalette(QColor(224, 224, 224)));
     ButtonReceived->setPalette( QPalette(QColor(224, 224, 224)));
   }
   else
   if(but == 2) {                               
     ButtonNotSent->setPalette( QPalette(QColor(180, 210, 200)));          
     ButtonUnReceived->setPalette( QPalette(QColor(224, 224, 224)));
     ButtonUnConfirmed->setPalette( QPalette(QColor(224, 224, 224)));
     ButtonReceived->setPalette( QPalette(QColor(224, 224, 224)));
   }
   else
   if(but == 3) {                                
     ButtonUnConfirmed->setPalette( QPalette(QColor(180, 210, 200)));      
     ButtonNotSent->setPalette( QPalette(QColor(224, 224, 224)));
     ButtonUnReceived->setPalette( QPalette(QColor(224, 224, 224)));
     ButtonReceived->setPalette( QPalette(QColor(224, 224, 224)));
   }
   else
   if(but == 4) {                                
     ButtonReceived->setPalette( QPalette(QColor(180, 210, 200)));         
     ButtonUnConfirmed->setPalette( QPalette(QColor(224, 224, 224)));
     ButtonNotSent->setPalette( QPalette(QColor(224, 224, 224)));
     ButtonUnReceived->setPalette( QPalette(QColor(224, 224, 224)));
   }
}

void qsl::takeSelectedButton()
{
    switch(selectedbut) {
      case 1 : buildHeaderUnreceived();
               break;
      case 2 : buildHeaderNotSend();
               break;
      case 3 : buildHeaderNotConfirmed();
               break;
      case 4 : buildHeaderReceived();
               break;
      case 5:  sucheCb();
               break;
    }
}

QString qsl::DirSelector()
{
   QString path =  QDir::homePath();
   if( !dirsel ) {
     QString directory = QFileDialog::getExistingDirectory(this,
             tr("Waehle einen Ordner"), QDir::homePath());
     return directory;
   }
   else {
     QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                        path, tr("ADIF ( *.adi *.adif *.cfm *.cc)"));
     return fileName;
   }
}




void qsl::uploadEqslAc()
{
  QString t, id;
     if(BoxQslvia->currentText().compare("Eqsl") != 0) {
        QMessageBox::information( this,tr("eQSL Upload"),tr("\nStelle QSL via auf  ' Eqsl '.\n"),
        QMessageBox::Ok);
       return;
     }
     if(!forUp) {
        QMessageBox::information(this,tr("Lotw Upload"),
        tr("\nEs ist sicherer die QSO's vor dem Upload anzusehen\nund den Zeitraum zu pruefen.\n\nDruecke den Button  ' Nicht gesendet '"),
        QMessageBox::Ok);
       return;
     }
     if(BoxQslvia->currentIndex() == 1) {            
       s.setNum(forUp);
       t = "\n"+s+"  QSOs - zum upload bereit.\nDatei  < ";
       t += settings.value("Call").toString();
       t += ".adi >  erzeugen  ?\n\nDer Status eQSL_sent wird auf  'U' (upload) gestellt\n";
       int status = QMessageBox::question( this,
           tr("eQSL Datei.adi upload"),t,
           QMessageBox::Yes,QMessageBox::No);
       if(status != QMessageBox::Yes)
          return;
     }
     Err = 0;
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     mkEqslUploadAdi();                               
     if(Err) {                                        
         Error += "\n\nDie Datensaetze werden uebersprungen\nVorgang abbrechen ?";
         int st = QMessageBox::question(this,tr("eQSL ADIF Datei"),Error,
         QMessageBox::Yes,QMessageBox::No);
         if( st == QMessageBox::Yes)
           return; 
     }
     r = qslTable->rowCount();                        
     QTableWidgetItem * v;
     int x = 0;
     QSqlQuery query;                                 //uptate: 
     while( x != r) {
       v = qslTable->verticalHeaderItem(x); 
       id = v->text();
       if(getErrId(id).count() == 0) {                
          qy = "SELECT qslSvia FROM "+logFile+" WHERE id="+id;
          query.exec(qy);
          query.next();
          s = query.value(0).toString();               
          if(s.indexOf("E") == -1)                     
             s += "E";
          qy = "UPDATE "+logFile+" SET qslSvia='"+s+"' WHERE id="+id;
          query.exec(qy);
          
          qy = "SELECT eqslr FROM "+logFile+"qsl WHERE qsoid="+id;
          QSqlQuery query; 
          query.exec(qy);
          query.next();
          s = query.value(0).toString();
          
          qy = "UPDATE "+logFile+"qsl SET ";
          if(s.indexOf("Y") != -1)                     
             qy += "eqsls='Y',eqslsd='"+sentDay+"' WHERE qsoid="+id;
          else
             qy += "eqslr='R',eqsls='U',eqslsd='"+sentDay+"' WHERE qsoid="+id;
          query.exec(qy);
       }
       x++;
     }
     settings.setValue("EqslLastUp",last);             
     QApplication::restoreOverrideCursor();
     p = "\n"+path;
     p += tr("\n\nFertig zum Upload - ueber einen HTML Bowser");
     QMessageBox::information( this,tr("Datei upload"),p,
     QMessageBox::Ok);
     buildHeaderNotConfirmed();               
}



void qsl::uploadLotwAc()
{
   QString n, sentDay, last, id, datumheute, t; 
   int x;
   if(settings.value("LotPasswd").toString().count() == 0) {
       QMessageBox::information( this,tr("LotW Upload"),tr("\nDas Passwort fuer den Zugang zum ARRL Server fehlt.\nDieses muss im Config Menue eingetragen sein"),
       QMessageBox::Ok);
      return;
   }
   if(BoxQslvia->currentText().compare("LotW") != 0) {
       QMessageBox::information( this,tr("LotW Upload"),tr("\nErst QSL via auf  ' LotW '  stellen.\nDann den Zeitraum fuer den Upload pruefen/setzen"),
       QMessageBox::Ok);
     return;
   }
   if( !forUp ) {
     QMessageBox::information(this,tr("Lotw Upload"),
     tr("\nEs ist sicherer die QSO's vor dem Upload anzusehen\nund den Upload-Zeitraum zu pruefen.\n\nDruecke den Button  ' Nicht gesendet '"),
     QMessageBox::Ok);
    return;
   }
   if(BoxQslvia->currentIndex() == 2) {       
     QDate d = dateEditHeute->date(); 
     s = d.toString("yyyy-MM-dd");            
     last = s;
     d = dateEditLast->date(); 
     s = d.toString("yyyy-MM-dd");            
     sentDay = s;           
     n.setNum(forUp);
     t = "\n"+n+" QSOs  - zum upload bereit.\n";
     t += "Vom: "+last+" bis: "+sentDay+"\n";
     t += "Datei : < "+settings.value("Call").toString()+"xxx.adi > erzeugen ?\n\n";
     t += "Der Status LotSent wird auf 'U' (upload) gestellt.";
     int status = QMessageBox::question( this,
         tr("Datei.adi upload"),t,
         QMessageBox::Yes,QMessageBox::No);
     if(status != QMessageBox::Yes)
       return;
     Err = 0;
     QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
     mkLotwUploadAdi(settings.value("LotdataSet").toInt()); 
     QApplication::restoreOverrideCursor();
     if( Err ) {                                            
         Error += "\n\nDie Datensaetze werden uebersprungen\n  Abbrechen ?";
         int st = QMessageBox::question(this,tr("Lotw Upload"),Error,
         QMessageBox::Yes,QMessageBox::No);
         if( st == QMessageBox::Yes)
           return; 
     }
     s = "tqsl -d "+path+" -x ";                   
     StartProcess(s.toAscii());
     
     d = QDate::currentDate();                             
     sentDay = d.toString("yyyy-MM-dd");   
     
     i = path.indexOf('.',0);                              
     s = path.left(i +1);                                  
     s += "tq8";
     QFile file(s);
     if(file.exists() == TRUE ) {
        p = "\n"+s;
        p += tr("\nFertig zum Upload - ueber einen HTML Bowser");
        QMessageBox::information( this,tr("Datei upload"),p,
        QMessageBox::Ok);
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        r = qslTable->rowCount();                         
        QTableWidgetItem * v;
        x = 0;
        while( x != r) {
           v = qslTable->verticalHeaderItem(x); 
           id = v->text();
           if(getErrId(id).count() == 0) {                
             QSqlQuery query;                             //uptate: 
             qy = "SELECT qslSvia FROM "+logFile+" WHERE id="+id;
             query.exec(qy);
             query.next();
             s = query.value(0).toString();               
             if(s.count() == 0) 
                s = "L";
             else
             if(s.compare("B") == 0 || s.compare("E") == 0)
               s += "L";
             qy = "UPDATE "+logFile+" SET qslSvia='"+s+"' WHERE id="+id;
             query.exec(qy);
             
             qy = "UPDATE "+logFile+"qsl SET lotqslr='R', lotqsls='U',lotqslsd='"+sentDay+"' WHERE qsoid="+id;
             query.exec(qy);
          }
          x++;
        }
        settings.setValue("LotLastUp",last);             
        QDate d = QDate::fromString (last,"yyyy-MM-dd");
        dateEditLast->setDate(d);                        
        QApplication::restoreOverrideCursor();
        buildHeaderNotConfirmed();                       
     }
     else {
        QMessageBox::information(this,tr("Lotw Upload"),
        tr("\n'TQSL' hat keine '*.tq8' Datei erzeugt.\nTQSL ist defekt oder fehlt !!"),
        QMessageBox::Ok);
      return;
     }
   }
}




void qsl::adifLotwImportAc()
{
    if(BoxQslvia->currentText().compare("LotW") != 0) {
         QMessageBox::information( this,tr("Check lotw"),tr("\nQSL via : Steht nicht auf LotW"),
         QMessageBox::Ok);
       return;
    }
    dirsel = 1;
    l = DirSelector();                         
    if(l.count() == 0) return;
    if(l.indexOf("lotwreport.adi") == -1) {    
       QMessageBox::information( this,tr("Check report"),tr("\nDatei ist nicht < lotwreport.adi >"),
       QMessageBox::Ok);
      return;
    }
    newQsl = 0;
    adif adi;
    adi.loadRefList();                         
    adi.setBandTable();                        
    n = settings.value("LastDownNo").toString().toInt();
    n++;                                          
    adi.setLastDownNr(s.setNum(n));
    settings.setValue("LastDownNo",s.setNum(n)); 

    adi.readQslReport(logFile, l,"lotw");         
    n = adi.getState();                        
    if( n ) {                                  
      if( n == 2 ) {
        s = tr("Falsches Logbook ???\n\n");    
        s += adi.getErrorInfo();
      }
     else
       if(n == 9) {
         s =  adi.getErrorInfo();  
         QMessageBox::critical(this, qApp->applicationName(),s,
         QMessageBox::Ok);
         return;
       }
       s =  adi.getErrorInfo();               
       QMessageBox::critical(this, qApp->applicationName(),s,
       QMessageBox::Ok);
    }
    newQsl = adi.getNewqslCnt();              
    settings.setValue("Val",newQsl);          
    adi.closeRefList();
    adi.clearBandTable();
    readLastDownLoad(1,settings.value("LastDownNo").toString()); 
}



void qsl::adifLotwQslRecordsAc()
{
   if(BoxQslvia->currentText().compare("LotW") != 0) {
         QMessageBox::information( this,tr("Check lotw"),tr("\nQSL via : Steht nicht auf LotW"),
         QMessageBox::Ok);
       return;
   }
   mkLquery *mkLq = new mkLquery();    
   mkLq->exec();
   if( ! mkLq->getRec )                
     return;                           
   
   newQsl = 0;
   QString url = mkLq->qyrec;          
   path = "downloadmgrdb "+url+" &";
   StartProcess(path.toAscii());       
   
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}




void qsl::adifEqslImportAc()
{
    if(BoxQslvia->currentText().compare("Eqsl") != 0) {
         QMessageBox::information( this,tr("Check Eqsl"),tr("\nStelle QSL via auf : Eqsl  "),
         QMessageBox::Ok);
       return;
    }
    dirsel = 1;
    l = DirSelector();                           
    if(l.count() == 0) return;
    s = "\nDie Importdatei  xxxxxx.adi  muss von eQsl.cc sein.\nBei einem Fehler !!\nKoennte die Logtabelle beschaedigt werden.\n\nImportiern ?";
    int st = QMessageBox::question(this,tr("Info"),s,
         QMessageBox::Yes,QMessageBox::No);
    if( st != QMessageBox::Yes)
        return; 
    QDate d = dateEditHeute->date(); 
    s = d.toString("yyyy-MM-dd");            
    newQsl = 0;
    adif adi;
    adi.loadRefList();                         
    adi.setBandTable();                        
    adi.setdateHeute(s);
    n = settings.value("LastDownNo").toString().toInt();
    n++;                                         
    adi.setLastDownNr(s.setNum(n));
    settings.setValue("LastDownNo",s.setNum(n)); 

    adi.readQslReport(logFile, l,"eqsl");        
    n = adi.getState();                          
    if( n ) {                                  
      if( n == 2 || n == 1 ) {
        p =  QDir::homePath();                     
        p += "/eQsl/ErrorMessage.txt";
        QFile file(p);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
           qDebug() << "FileERROR";
        }
        QTextStream out(&file);
        out << "\n";
        out << adi.getErrorInfo();
        file.close();
      }
      else
       if(n == 9) {
         s =  adi.getErrorInfo();  
         QMessageBox::critical(this, qApp->applicationName(),s,
         QMessageBox::Ok);
         return;
       }
       s =  adi.getErrorInfo();               
       QMessageBox::critical(this, qApp->applicationName(),s,
       QMessageBox::Ok);
    }
    newQsl = adi.getNewqslCnt();              
    settings.setValue("Val",newQsl);          
    adi.closeRefList();
    adi.clearBandTable();
    readLastDownLoad(1,settings.value("LastDownNo").toString()); 
}



void qsl::adifEqslRecordsAc()
{
   if(BoxQslvia->currentText().compare("Eqsl") != 0) {
         QMessageBox::information( this,tr("Check Eqsl"),tr("\nStelle QSL via auf : Eqsl  "),
         QMessageBox::Ok);
      return;
   }
   QString url;
   mkEquery *mkEq = new mkEquery();    
   mkEq->exec();

   if(mkEq->box == 6) {                
      checkQsoVerification();
      return;
   }
   if(mkEq->box == 7) {                
     checkQslCardstatus();
     return;
   }
   if( ! mkEq->getRec )                
     return;                           
   newQsl = 0;
   url = mkEq->qyrec;                  
   path = "downloadmgrdb "+url+" &";
   StartProcess(path.toAscii());       
   
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
}



void qsl::readrecordFile(QString formdata, QString fname)
{
    if(formdata.compare("lotw") == 0)
      s = "/lotw/"+fname;
    else
      s = "/eQsl/"+fname;
    path =  QDir::homePath();
    path += s;                                    
    QDate d = dateEditHeute->date(); 
    s = d.toString("yyyy-MM-dd");                 
    newQsl = 0;
    adif adi;
    adi.loadRefList();                            
    adi.setBandTable();                           
    adi.setdateHeute(s);
    n = settings.value("LastDownNo").toString().toInt();
    n++;                                          
    adi.setLastDownNr(s.setNum(n));               
    settings.setValue("LastDownNo",s.setNum(n));  

    
    adi.readQslReport(logFile, path, formdata);   
    QApplication::restoreOverrideCursor();        
    n = adi.getState();                        
    if( n ) {                                  
      if( n == 2 || n == 1 ) {
        p =  QDir::homePath();                     
        p += "/eQsl/ErrorMessage.txt";
        QFile file(p);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
           qDebug() << "FileERROR";
        }
        QTextStream out(&file);
        out << "\n";
        out << adi.getErrorInfo();
        file.close();
      }
     else
       if(n == 9) {
         s =  adi.getErrorInfo();  
         QMessageBox::critical(this, qApp->applicationName(),s,
         QMessageBox::Ok);
         return;
       }
      s =  adi.getErrorInfo();               
      s += "\n\nDie Errordatei ist hier : "+p;
      QMessageBox::critical(this, qApp->applicationName(),s,
      QMessageBox::Ok);
    }
    newQsl = adi.getNewqslCnt();            
    settings.setValue("Val",newQsl);        
    adi.closeRefList();
    adi.clearBandTable();
    
    readLastDownLoad(1,settings.value("LastDownNo").toString());
}



void qsl::checkQslCardstatus()
{
 int row, tlen;
    if(BoxQslvia->currentText().compare("Eqsl") != 0) {
         QMessageBox::information( this,tr("Check Eqsl"),tr("\nQSL via : Steht nicht auf Eqsl"),
         QMessageBox::Ok);
       return;
    }
    if(selectedbut != 4) {
       QMessageBox::information( this,tr("Check Eqsl"),tr("\nStelle Button 4 auf  - Qsl erhalten -\n\nQslKarten koennen nur fuer bereits bestaetigte QSOs nach installiert werden ! ( eQSLrcvd = Y )"),
       QMessageBox::Ok);
      return;
    }
    
    QSqlQuery query;
    n = settings.value("LastDownNo").toString().toInt();
    n++;                                                 
    settings.setValue("LastDownNo",s.setNum(n));         
    row = 0;
    QTableWidgetItem * v;                                
    v = qslTable->verticalHeaderItem(row);

    tlen = qslTable->rowCount();                         
    tabLen = tlen;
    row = 0;
    i = 0;
    while(tlen-- != 0) {                                 
       v = qslTable->verticalHeaderItem(row);            
       qy = "SELECT qcall FROM "+logFile+"card WHERE qsocid="+v->text();
       query.exec(qy);
       n = query.size();
       if( !n ) {                                        
         qy = "UPDATE "+logFile+"qsl SET sel='"+settings.value("LastDownNo").toString()+"' WHERE qsoid="+v->text();
         
         query.exec(qy);
         i++;                                            
       }
       row++;
    }
    tabLen = i;                                         
    if(!i) {                                            
       s = "\nAlle QSOs haben eine QSL-Karte";
       QMessageBox::information( this,tr("QSL card check"),s,
       QMessageBox::Ok);
       return;
    }
    else {
      s = "\n"+p.setNum(i);
      s += " QSOs haben keine QSL-Karte.\n\nDie fehlenden Karten runter laden ?";
      int status = QMessageBox::question(this,tr("QSL Karten check"),s,
          QMessageBox::Yes | QMessageBox::No);
      if(status != QMessageBox::Yes)
        return;
    }
    
    readLastDownLoad(2,settings.value("LastDownNo").toString()); 

    adi.setModeTabel();
    tabRow = 0;                             
    
    labelRun->setText("load QSL cards");
    labelRun->show();
    progressBar->setRange(tabRow,tabLen);
    progressBar->show();
    
    urlmode = 1;
    QString u = "downloadmgrdb GeteQSL.cfm &";
    StartProcess(u.toAscii());              
    mtimer->start(800);                     
    
    
}



QString qsl::getUrlForCard()
{
    QTableWidgetItem * v;      
    n = 0;
    if(tabRow != tabLen) {
      v = qslTable->verticalHeaderItem(tabRow);  
      qid = v->text();
      QTableWidgetItem * item;
      url = "http://www.eqsl.cc/qslcard/GeteQSL.cfm?UserName="+settings.value("EqslUser").toString();
      url += "&Password="+settings.value("EqslPasswd").toString();
      item = qslTable->item(tabRow,0);           
      call = item->text();
      url += "&CallsignFrom="+call;              
      item = qslTable->item(tabRow,1);           
      band = item->text();                       
      url += "&QSOBand="+adi.getAdiBand(band);   
      item = qslTable->item(tabRow,2);           
      mode = item->text();                       
      url += "&QSOMode="+adi.getAdiMode(mode);   
      item = qslTable->item(tabRow,5);           
      datum = item->text();
      s = datum.left(4);
      url += "&QSOYear="+s;                      
      s = datum.mid(5,2);
      url += "&QSOMonth="+s;                     
      s = datum.mid(8,2);
      url += "&QSODay="+s;                       
      item = qslTable->item(tabRow,6);           
      ontime = item->text();
      s = ontime.left(2);
      url += "&QSOHour="+s;                      
      s = ontime.mid(3,2);
      url += "&QSOMinute="+s;                    
      tabRow++;
      return url;
    }
    else {
      adi.clearModeTable();
      QApplication::restoreOverrideCursor();
      progressBar->hide();
      labelRun->hide();
      return "";                                 
    }
}



void qsl::checkQsoVerification()
{
    if(!tabLen) {
        QMessageBox::information( this,tr("Check Eqsl"),tr("\nDie Tabelle ist leer !!"),
        QMessageBox::Ok);
      return;
    }
    
    
    adi.setModeTabel();
    urlmode = 2;
    tabRow = 0;                             
    callFrom = settings.value("Call").toString();
    
    labelRun->setText("QSL Verification");
    labelRun->show();
    
    progressBar->setRange(tabRow,tabLen);
    progressBar->setValue(tabRow);
    progressBar->show();
    
    yes = 0;
    no = 0;
    err = 0;
    ignor = 0;
 
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString u = "downloadmgrdb VerifyQSO.cfm &";
    StartProcess(u.toAscii());              
    mtimer->start(800);                     
}



QString qsl::getUrlForVerifyQSO()
{
    QTableWidgetItem * v;
    n = 0;
    if(tabRow != tabLen) {
      v = qslTable->verticalHeaderItem(tabRow);
      qid = v->text();
      QTableWidgetItem * item;
      url = "http://www.eqsl.cc/qslcard/VerifyQSO.cfm?";
      url += "CallsignFrom="+callFrom;           
      item = qslTable->item(tabRow,0);           
      call = item->text();
      url += "&CallsignTo="+call;                
      item = qslTable->item(tabRow,1);           
      band = item->text();                       
      url += "&QSOBand="+adi.getAdiBand(band);   
      item = qslTable->item(tabRow,5);           
      datum = item->text();
      s = datum.left(4);
      url += "&QSOYear="+s;                      
      s = datum.mid(5,2);
      url += "&QSOMonth="+s;                     
      s = datum.mid(8,2);
      url += "&QSODay="+s;                       
      item = qslTable->item(tabRow,2);           
      mode = item->text();                       
      url += "&QSOMode="+adi.getAdiMode(mode);   
      tabRow++;
      return url;
    }
    else {
      adi.clearModeTable();
      return "";                                 
    }
}

void qsl::timertransferUrl()
{
     mtimer->stop();
     if(urlmode == 1)
        processWriteDatagram(18, getUrlForCard(), 45500);      
     else
       processWriteDatagram(20, getUrlForVerifyQSO(), 45500);  
}




void qsl::processPendingDatagram()
{
   while(udpSocket->hasPendingDatagrams()) {
     datagram.resize(udpSocket->pendingDatagramSize());
     QHostAddress sender;
     quint16 senderPort;
     udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
     i = datagram.indexOf (",",0);
     QByteArray Left = datagram.left(i);             
     QByteArray Mid = datagram.mid(i+1);             
     int com = Left.toInt();                         
     s = Mid;
     if(com == 10) {                                 
       if(s.compare("FAILED") == 0) {                
          QApplication::restoreOverrideCursor();
          QMessageBox::critical(this,"Download Message",
              tr("Kontakt zum ARRL server fehlgeschlagen.\nPruefe das Netzwerk"),
              QMessageBox::Ok);
          return;
       }
       else
         if(s.compare("lotwreport.adi") == 0)        
            readrecordFile("lotw",s);                
     }
     else
     if(com == 15) {                                 
        if(Mid.count() != 0)
          readrecordFile("eqsl",Mid);                
        else {
           QApplication::restoreOverrideCursor();
           mess = "Keine Logeintraege vorhanden";
           QMessageBox::information( this,
           tr("UploadMgr Lotw"),(mess),
           QMessageBox::Default,
           QMessageBox::NoButton, QMessageBox::NoButton);
           return;
        } 
     }        
     else
     if(com == 2) {                               
        if(s.indexOf("ERROR") == -1) {
          mess = "\nUpload: "+s+" erfolgt";
          QMessageBox::information( this,
          tr("UploadMgr Eqsl"),(mess),QMessageBox::Default);
          NotSentCb();                            
        }
        else {                                    
          QMessageBox::information( this,
          tr("UploadMgr Eqsl"),("ERROR: - No match on eQSL_User/eQSL_Pswd -\nUploadMgr is OFF."),
          QMessageBox::Default);   
        }
     }
     else
     if(com == 3) {                               
        cardview = 0;
     }
     else
     if(com == 9) {                               
       mess = "\nEin Netzwerk Timeout Signal ist eingegangen.\nDas Netzwerk ist gestoert, der eQSL.cc Server meldet sich nicht\nUpload:  "+s+"  abgebrochen - pruefe das Netzwerk !";
       QMessageBox::information( this,
       tr("UploadMgr Eqsl"),(mess),
       QMessageBox::Default,
       QMessageBox::NoButton, QMessageBox::NoButton);
     }
     else
     if(com == 18) {                              
       
       QFile file(Mid);                           
       band = adi.getsysBand(band);
       QSqlQuery query;                           
       qy = "INSERT INTO "+logFile+"card VALUES (0,"+qid;
       qy += ",'"+call+"','"+datum+"','"+band+"','"+mode+"','"+ontime+"','E',"+s.setNum(file.size())+",'')";
       
       query.exec(qy);
       qy = "UPDATE "+logFile+"card SET qcard=LOAD_File ('"+Mid;
       qy += "') WHERE qsocid="+qid;              
       
       query.exec(qy);
       
       
       if(tabRow != tabLen) {                     
         progressBar->setValue(tabRow);           
         mtimer->start(600);                      
         
         
         
       }
       else {                                     
         processWriteDatagram(-1, "-1", 45500);   
         QApplication::restoreOverrideCursor();
         
         mess = mess.setNum(tabLen);
         mess += " QSL Karten geladen";
         QMessageBox::information( this,tr("QSL card upload"),mess,
         QMessageBox::Ok);
         labelRun->hide();
         progressBar->hide();
       }
     }
     else
     if(com == 20) {                                  
        qslr= "";
        qsls = "";
        p = "";
        QFile datei(Mid);                             
        datei.open(QIODevice::ReadOnly);
        QTextStream istream( &datei);
        while( istream.atEnd() != true ) { 
            l = istream.readLine(0);
            if(l.indexOf("<BODY>") != -1) break;
        }
        while( istream.atEnd() != true)  {            
           l = istream.readLine(0);
           if(l.count() != 0) break; 
        }

        if(l.indexOf("Result - QSO on file") != -1) {                     
           p = "Y";
           l = istream.readLine(0);                                       
           while( istream.atEnd() != true)  {                             
             l = istream.readLine(0);
             if(l.count() != 0) break; 
           }
           if(l.indexOf("Information - Authenticity Guaranteed") != -1)   
              p = "Y";
           yes++;
        }
        else                                                              
        if(l.indexOf("Information - CallsignTo not on file") != -1) {
           p = "N";
           l = istream.readLine(0);                                       
           while( istream.atEnd() != true)  {                             
             l = istream.readLine(0);
             if(l.count() != 0) break; 
           }
           if(l.indexOf("Result - QSO on file") != -1)                    
              qslr = "N";
           if(l.indexOf("Error - Result: QSO not on file") != -1)         
              qsls = "N";
           no++;
        }
        else                                                              
        if(l.indexOf("Error - Result: QSO rejected by recipient") != -1) {
             qslr = "I";                                                  
             qsls = "I";                                                  
             ignor++;
        }
        else                                                              
        if(l.indexOf("Error - Result: QSO not on file") != -1) {          
             qsls = "N";
             err++;
        }
        qy = "UPDATE "+logFile+"qsl SET ag='"+p;
        if(qslr.count() != 0)
          qy += "',eqslr='"+qslr;
        if(qsls.count() != 0)
          qy += "',eqsls='"+qsls;
        qy += "' WHERE qsoid="+qid;
        
        QSqlQuery query;
        query.exec(qy);
        datei.close();
        
        if(tabRow != tabLen) {
          
          progressBar->setValue(tabRow); 
          mtimer->start(600);            
        }
        else {
          processWriteDatagram(-1, "-1", 45500);  
          QApplication::restoreOverrideCursor();
          mess = "\n"+s.setNum(tabLen)+" QSOs mit eQSL verifiziert\n\n";
          mess += "Y="+s.setNum(yes);
          mess += "  N="+s.setNum(no);
          mess += "  Err="+s.setNum(err);
          mess += "  I="+s.setNum(ignor)+"\n";
          QMessageBox::information( this,tr("QSL Verification"),mess,
          QMessageBox::Ok);
          
          progressBar->hide();
          labelRun->hide();
          takeSelectedButton();
        }
     }
  }
}



void qsl::processWriteDatagram(int dNr, QString mess, int port)
{
    datagram = QByteArray::number(dNr);
    datagram.append(",");
    datagram.append(mess);
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, port);       
}
