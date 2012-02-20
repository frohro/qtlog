/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n : 30.07.09
********************************************************************** */

#include <QtGui>
#include <QtSql>
#include <QTimer>
#include "contLog.h"
#include "contRules.h"
#include "defs.h"
#include "rule.h"
#include "editLog.h"
extern cBase * myContest;

int Qpts;                 
int Mul1p;                
int Mul2p;                
QString Multi1;           
QString Multi2;           

contLog::contLog(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    connect(actionAll, SIGNAL(triggered(bool)), this, SLOT(newContestLogAc()));             
    connect(actionContinue, SIGNAL(triggered(bool)), this, SLOT(continueAc()));             
    connect(actionSentLog, SIGNAL(triggered(bool)), this, SLOT(sentLogAc()));               
    connect(actionConvertToQtLog, SIGNAL(triggered(bool)), this, SLOT(convertToQtLogAc())); 
    connect(actionDeleteContest, SIGNAL(triggered(bool)), this, SLOT(deleteContestAc()));   
    
    connect(actionCWkeyer, SIGNAL(triggered(bool)), this, SLOT(cwkeyerAc()));            
    connect(actionRigControl, SIGNAL(triggered(bool)), this, SLOT(hlibAc()));            
    connect(actionReadMasterFile, SIGNAL(triggered(bool)), this, SLOT(readMasterAc()));  
    
  
    connect(actionHilfeContestLog, SIGNAL(triggered(bool)), this, SLOT(hilfeContestLogAc()));   
    connect(actionHilfeRigControl, SIGNAL(triggered(bool)), this, SLOT(hilfeRigCtlAc()));       
    connect(actionHilfeCwDaemon, SIGNAL(triggered(bool)), this, SLOT(hilfeCwDaemonAc()));       
    
    connect(actionExit, SIGNAL(triggered(bool)), this, SLOT(goExit()));
    
    
    

    //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("host").toString());                         
    db.setDatabaseName(settings.value("dbname").toString());
    db.setUserName(settings.value("dbuser").toString());
    db.setPassword(settings.value("dbpasswd").toString());
    if(!db.open()) {
       qDebug() << db.lastError();          
    }
    
    FreqToBand.insert("1","160");           
    FreqToBand.insert("3","80");
    FreqToBand.insert("7","40");
    FreqToBand.insert("14","20");
    FreqToBand.insert("21","15");
    FreqToBand.insert("28","10");
                                            
    BandToFreq.insert("b160",1820);         
    BandToFreq.insert("b80",3510);          
    BandToFreq.insert("b40",7010);          
    BandToFreq.insert("b20",14010);         
    BandToFreq.insert("b15",21010);         
    BandToFreq.insert("b10",28010);         
    bandIdx.insert("b160",1);               
    bandIdx.insert("b80",2);                
    bandIdx.insert("b40",3);                
    bandIdx.insert("b20",4);                
    bandIdx.insert("b15",5);                
    bandIdx.insert("b10",6);                
    
    FreqUp.insert(1,3510);                  
    FreqUp.insert(3,7010); 
    FreqUp.insert(7,14010); 
    FreqUp.insert(14,21010); 
    FreqUp.insert(21,28010); 
    FreqUp.insert(28,1820); 
    FreqDown.insert(28,21010);              
    FreqDown.insert(21,14010); 
    FreqDown.insert(14,7010); 
    FreqDown.insert(7,3510); 
    FreqDown.insert(3,1820); 
    FreqDown.insert(1,28010); 
    
    connect(stackButtonOk, SIGNAL(clicked()), this, SLOT(stackButtonOkCb()));
    connect(stackButtonReturn, SIGNAL(clicked()), this, SLOT(goExit()));
    
    connect(lineCallsign, SIGNAL(textEdited(QString)), this, SLOT(lineCallsignCb(QString)));
    connect(lineCallsign, SIGNAL(returnPressed()), this, SLOT(lineCallsignCrCb()));
    connect(lineExchange, SIGNAL(textEdited(QString)), this, SLOT(lineExchangeBackCb(QString)));
    connect(lineExchange, SIGNAL(returnPressed()), this, SLOT(lineExchangeCb())); 
    connect(lineEditSent, SIGNAL(textEdited(QString)), this, SLOT(lineEditedSentCb(QString)));
    
    //connect(ButtonReceiveQtc, SIGNAL(clicked()), this, SLOT(ReceiveQtcCb()));
    connect(ButtonEditLog, SIGNAL(clicked()), this, SLOT(buttonEditLogCb()));
    connect(ButtonInfo, SIGNAL(clicked()), this, SLOT(ButtonInfoCb()));
    connect(ButtonRemoveQso, SIGNAL(clicked()), this, SLOT(removeQsoCb()));    
    connect(ButtonUhr, SIGNAL(clicked()), this, SLOT(uhrStopCb()));
    connect(contestWorkedList, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(contWorkedListCb(QTreeWidgetItem*,int)));
    //connect(workedList, SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)), this, SLOT(worckedListClickedCb(QTreeWidgetItem*,int)));
    resize(settings.value("contLog/Size",sizeHint()).toSize());
    restoreState(settings.value("contLog/Properties").toByteArray());
    splitter->restoreState(settings.value("contLog/splitterSizes").toByteArray());
    
    workedList->setColumnWidth(0,120);        
    workedList->setColumnWidth(1,90);         
    workedList->setColumnWidth(2,90);         
    workedList->setColumnWidth(3,90);         
    workedList->setColumnWidth(4,90);         
    workedList->setColumnWidth(5,90);         
    workedList->setColumnWidth(6,90);         
    workedList->setColumnWidth(7,90);         
    workedList->setColumnWidth(8,90);         
    workedList->setColumnWidth(9,90);         
    workedList->setColumnWidth(10,60);        
   
    qsoPointsList->setColumnWidth(0,80);      
    qsoPointsList->setColumnWidth(1,80);      
    qsoPointsList->setColumnWidth(2,80);      
    qsoPointsList->setColumnWidth(3,80);      
    qsoPointsList->setColumnWidth(4,80);      
    qsoPointsList->setColumnWidth(5,80);      
    qsoPointsList->setColumnWidth(6,80);      
    qsoPointsList->setColumnWidth(7,60);      
   
    contestWorkedList->setColumnWidth(0,100); 
    contestWorkedList->setColumnWidth(1,280); 
    contestWorkedList->setColumnWidth(2,80);  
    contestWorkedList->setColumnWidth(3,80);  
    contestWorkedList->setColumnWidth(4,90);  
    contestWorkedList->setColumnWidth(5,90);  
     
    s = settings.value("Breite").toString();       
    t = settings.value("Laenge").toString();
    distanz.setHomeCoordinaten(s,t);
    
    bMin = 0;                                      
    mMin = 0;
    min = 0;
    
    h = QDateTime::currentDateTime().toUTC();      
    s = h.time().toString("hh:mm");
    cTime->setText(s);
    s = h.date().toString("yyyy-MM-dd");
    cDate->setText(s);
    timeflg = 1;                                   
    
    ModeToMod.insert("LSB","SSB");
    ModeToMod.insert("USB","SSB");
    ModeToMod.insert("SSB","LSB");
    ModeToMod.insert("CW","CW");
   
    runflg = 0;
    rigpolling = 0;                                 
    retry = 0;                                      
    QSqlQuery query;  
    qy = "SELECT host,port,retry,runing FROM wproz WHERE runing=1"; 
    query.exec(qy);
    if(query.next()) {
      tcphost = query.value(0).toString();          
      tcpport = query.value(1).toString();          
      retry = query.value(2).toInt();               
      runflg = query.value(3).toInt();              
      rigpolling = runflg;
    }
    
    connect(freqBox, SIGNAL(valueChanged(int)), this, SLOT(BandChangedCb(int))); 
    connect(modeBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ModeChangedCb(QString)));
   
    timer = new QTimer(this);                      
    connect(timer,SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);                            
    showTime();                                    
 
    timerR = new QTimer(this);                     
    connect(timerR,SIGNAL(timeout()), this, SLOT(repeaterTimerCb()));   
    tcpSocket = new QTcpSocket(this);               
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(sentCommand()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readRigReply()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                                            this, SLOT(displayError(QAbstractSocket::SocketError)));
    stimer = new QTimer(this);                      
    connect(stimer,SIGNAL(timeout()), this, SLOT(requestRigNew()));
    
    udpPort = 6789;
    udpSocket = new QUdpSocket(this);              
    udpSocket->bind(QHostAddress::LocalHost,6799); 
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
    
    
    Qpts = 0;                                      
    Multi1 = "";
    Multi2 = "";
    Mul1p = 0;
    Mul2p = 0;
    
    QString path = QDir::homePath();  
    s = path+"/log/icon/Won-Ton-Soup-3.jpg";
    labelBild->setPixmap(QPixmap(s));
                         
    ButtonReceiveQtc->hide();
    ButtonRemoveQso->hide();
    ButtonLog->hide();
    ButtonLock->hide();
    
    qsoItem = NULL;
    Item = NULL;
    CNr = 0;                                
    
    get = 0;
    set = 0;
    cmdFlg = 0;
    QString cmdStr;
    
    eflg = 0;
    rigpolling = 0;
    
    found = 0;
    stxNr = 0;                              
    escflg = 0;                             
    eflg = 0;
    sflg = 0;
    qtcflg = 0;
    bandruning = 0;
    
    laPref = 0;
    cPref = 0;
    scPref = 0;
    nlen = 0;
    olen = 0;
    qNrlen = 0;
    numflg = 1;                            
    clFlg = 0;                             
    idx = 0;
    lastF = 0;                             
    lineCallsign->setFocus();
    stackedWidget->setCurrentIndex(2);     
   
    appString="app5 1 addClog";
    i = appString.indexOf(" ");
    s = "QtLog 1.5 App "+appString.mid(i +1,1)+" ";
    qy = "SELECT appV FROM tApplist WHERE appid='"+appString.left(i)+"'";  
    query.exec(qy);
    query.next();
    i = appString.mid(i +1,1).toInt();
    n = query.value(0).toInt();
    
    s = "QtLog V1.5 App 0 ";
    //if(i != n)
    
    //qDebug() << i  << n;
    
    
    
    //s = "QtLog 1.5 App "+appString.mid(i +1,1)+" ";
    labelApp->setText(s);
}

contLog::~contLog()
{
    if(CNr) {
      saveAll();
      settings.setValue("contLog/Size",size());
      settings.setValue("contLog/Properties",saveState());
      settings.setValue("contLog/splitterSizes",splitter->saveState());
    }
}

void contLog::goExit()
{
   qApp->quit();
}



void contLog::hilfeContestLogAc()
{
    settings.setValue("Val","Qso Contest");
    i = system("hilfedb &");
}


void contLog::hilfeRigCtlAc()
{
    settings.setValue("Val","Rig Control-Hamlib");
    i = system("hilfedb &");
}


void contLog::hilfeCwDaemonAc()
{
    settings.setValue("Val","CwDaemon");
    i = system("hilfedb &");
}



//---------------------------------------------------------------------------------
void contLog::keyPressEvent( QKeyEvent * event )
{
   QString cq, c;
   int z;
   z = 0;
   switch ( event->key() ) {
    
     case Qt::Key_F1 : {
               cq = checkMacro(getMacro("F1"));
               if( repeat ) {
                  pause = getPause(cq, lineWpm->text().toInt(), repeat);
                  timerR->start(pause * 1000);          
               }
               processWriteDatagram(cq, udpPort);
               escflg = 1;
             }
             break;
     case Qt::Key_F2 :
               cq = getMacro("F2");
               processWriteDatagram(checkMacro(cq),udpPort); 
             break;
     case Qt::Key_F3 :
               cq = getMacro("F3");
               processWriteDatagram(checkMacro(cq),udpPort); 
             break;
     case Qt::Key_F4 : 
               cq = getMacro("F4");
               processWriteDatagram(checkMacro(cq),udpPort); 
             break;        
     case Qt::Key_F5 :
               cq = getMacro("F5");
               processWriteDatagram(checkMacro(cq),udpPort); 
             break;    
     case Qt::Key_F6 : 
               cq = getMacro("F6");
               processWriteDatagram(checkMacro(cq),udpPort); 
             break;  
     case Qt::Key_F7 : 
               cq = getMacro("F7");
               processWriteDatagram(checkMacro(cq),udpPort); 
             break;   
     case Qt::Key_F8 :   
               cq = getMacro("F8");
               processWriteDatagram(checkMacro(cq),udpPort); 
             break;
     case Qt::Key_F9 :
               cq = getMacro("F9");
               processWriteDatagram(checkMacro(cq),udpPort); 
             break;
     case Qt::Key_F10 :
               cq = getMacro("F10");
               processWriteDatagram(checkMacro(cq),udpPort); 
             break;
	   
     case Qt::Key_PageUp : {
               i = lineWpm->text().toInt();
               i++;
               lineWpm->setText(s.setNum(i));
               processWriteDatagram(ESC, '2', s.setNum(i), udpPort);
               }
             break;
     case Qt::Key_PageDown : {
               i = lineWpm->text().toInt();
               i--;
               lineWpm->setText(s.setNum(i));
               processWriteDatagram(ESC, '2', s.setNum(i), udpPort);
               }
             break;
	
     case Qt::Key_Escape : {
                  processWriteDatagram(ESC, '4',"0", udpPort);  
                  timerR->stop();                               
                  if( ! escflg ) {
                    if( eflg ) {                                  
                      ButtonRemoveQso->hide();                    
                      lineCallsign->setEnabled(true);
                      ButtonEditLog->setEnabled(true);
                      setColorUhr("WEISS");                       
                      setTimeDate();
                      lineEditSent->setText(sentSave);            
                    }
                    eflg = 0;                                     
                      rigpolling = runflg;                        
                      timer->start(1000);                         
                      clearAll();
                      setTimeDate();
                      showAllQso();
                  }
                  escflg = 0;
                }
            break;
	    
     case Qt::Key_Up : {                                 
                 if(eflg) break;                         
                 z = freqBox->value();
                 if( rigpolling ) {                      
                   cmdStr = "F "+c.setNum(setfreqUp(z/1000))+"000";
                   cmdFlg = 1;
                 }
                 else 
                  freqBox->setValue(setfreqUp(z/1000));  
                }
            break;
     case Qt::Key_Down : {                                
                 if(eflg) break;
                 z = freqBox->value();
                 if( rigpolling ) {                       //falls hamlib läuft, ...
                   cmdStr = "F "+c.setNum(setfreqDown(z/1000))+"000";
                   cmdFlg = 1;
                 }
                 else
                   freqBox->setValue(setfreqDown(z/1000));
                }
            break;
	   
   }
   lineCallsign->setFocus();
}


float contLog::getPause(QString cq, float wpm, int repeat)
{
   float rtime;
   int slen;
       slen = cq.count();
       rtime = slen / wpm * 10;
     return rtime + repeat;
}


QString contLog::checkMacro(QString str)
{
     int idx;
     QString sentout, m;
     while(1) {
       idx = str.indexOf("$");          
       if(idx == -1) break;             
       sentout += str.left(idx);
       str.remove(0, idx +1);
       idx = str.indexOf(" ");          
       if(idx == -1) {                  
          str += " ";                   
          idx = str.indexOf(" ");       
       }
       m = str.left(idx);               
       if(m.compare("mcall") == 0) {                   
         sentout += settings.value("Call").toString(); 
         str.remove(0,idx);
       }
       else
       if(m.compare("qcall") == 0) {                   
         sentout += lineCallsign->text();
         str.remove(0,idx);
       }
       else
       if(m.compare("rst") == 0) {
         m = lineEditSrst->text();
         sentout += m.left(1);
         if(m.count() == 3)
           sentout += "nn";
         else
           sentout += "n";
         str.remove(0,idx);
       }
       else
       if(m.compare("sent") == 0) {
         sentout += lineEditSent->text();
         str.remove(0,idx);
       }
     }
     sentout += str;
     return sentout;
}



void contLog::contWorkedListCb(QTreeWidgetItem *item,int row)
{
   int x;
   x = row;                                   
   Item = item;                               
   labelContestName->setText(Item->text(1));
   labelContestId->setText(Item->text(0));
}



void contLog::showTime()                             
{
    h = QDateTime::currentDateTime().toUTC();        
    mm = h.time().toString("mm").toInt();            
    if(mm != min) {                                  
        min = mm;
	u = h.time().toString("hh:mm");              
        cTime->setText(u);                           
       if(bflg) {                                    
	 lineBandMin->setText(u.setNum(++bMin));     
	 if(bMin == 10) {                            
	   u = ">"+u.setNum(bMin);                   
	   lineBandMin->setText(u);                  
	   bflg = 0;                                 //
	   setColorBandMin("WEISS");                 
	 }
	 hbtime.insert(band,bMin);                   
       }
       else                                          
	lineMultMin->setText(u.setNum(++mMin));      
    }
}

void contLog::setTimeDate()
{
      h = QDateTime::currentDateTime().toUTC();         
      u = h.time().toString("hh:mm");                   
      cTime->setText(u);                                
      cDate->setText( h.date().toString("yyyy-MM-dd")); 
}


void contLog::uhrStopCb()
{
  if(timeflg) {
    timer->stop();
    setColorUhr("GELB");
    setColorButtonUhr("ROT");
    timeflg = 0;
    ButtonUhr->setText("Stop");
  }
  else {
    timer->start(1000);
    setColorUhr("WEISS");
    setColorButtonUhr("WEISS");
    timeflg = 1;
    ButtonUhr->setText("UTC");
  }
}



void contLog::repeaterTimerCb()
{
   QString cq;
   if(repeat != 0) {
     cq = checkMacro(getMacro("F1"));
     pause = getPause(cq,lineWpm->text().toInt(),repeat);
     timerR->start(pause * 1000);
     processWriteDatagram(cq,udpPort); 
   }
}



void contLog::buttonEditLogCb()
{
     saveAll();                              
     s = labelContestId->text()+",0,"+t.setNum(CNr)+","+band; 
     settings.setValue("Val",s);             
     delete myContest;                       
     
     editLog * eLog = new editLog();
     eLog->exec();
     setContinueValues();                    
}



void contLog::lineEditedSentCb(QString str)
{
     str = str.toUpper();                     
     i = str.indexOf("-");
     if(i != -1)                              
       str[i] = '/';                          
     lineEditSent->setText(str);
     
     if(methode == 6) {                       
       i = 0;
       sentVal = "";
       while(1) {
          if(str.at(i) >= QChar('0') && str.at(i) <= QChar('9')) 
            i++;                                                 
        else
          break;
       }
       if(i) 
         sentVal = str.mid(i);                
     }
     else {
       i = str.indexOf("/");
       s = str.left(i);
       sentVal = str.mid(i +1);
     }
}




void contLog::lineCallsignCb(QString C)
{
     QString str;
     if(!CNr) {
       lineCallsign->setText("*"); 
      return;
     }
     if(C.indexOf(" ") != -1) {                       
       str = C.trimmed();                             
       lineCallsign->setText(str);                    
       lineExchange->setFocus();                      
       return;
     }

     if(C.count() == 0) {                             
        QPalette palette7;
        QBrush brush8(QColor(255, 255, 255));         
        palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
        wMultNew->setPalette(palette7);
        wMult2->setPalette(palette7);
	clearAll();
	showAllQso();
      }
      else {
        str = C.toUpper();
        lineCallsign->setText(str);
        Callsign = str;
	i = lineCallsign->text().indexOf("/");       
        if(i != -1 && i >= 3) {                                     
          if((lineCallsign->text().mid(i +1).compare("MM")) == 0) { 
            myContest->getqsoPoints(9,dxcc);                        
	    return;
	  }
	}
	
        checkMultPrefix(Callsign);                   
                                                     
        if( eflg == 1 ) {                            
           eflg = 3;                                 
        }
      }
}



void contLog::checkMultPrefix(QString str)
{
      nlen = str.count();
      if( !prefMode ) {                         
	 i = getLaValues(str);                  
	 if(laPref) {                           
	   setContestValues();                  
	   laPref = 0;
	 }
      }
      else {                                    
         if( olen > nlen ) {                    
	    if(laPref > nlen ) {                
	      laPref = 0;
	      labelInfo->setText(""); 
	      cpref = "";
	      i = getLaValues(str);             
	      setContestValues();               
	      showMasterCallsign(str);
	      olen = nlen;
	      return;
	    }
	    i = getLaValues(str); 
	    if(cPref > nlen || cPref == 0) {    
	      cPref = 0;
	      cpref = str;
	      setContestValues();
	    }
	    olen = nlen;
	    showMasterCallsign(str);
	    return;
         }
         
         olen = nlen;                   
	 if(laPref) {                   
            i = str.count() -1;
	    if( !cPref ) {              
	       if(str.at(i) >= QChar('0') && str.at(i) <= QChar('9')) {  
                  cpref = str;
	          setContestValues();   
	          cPref = i;            
	       }
	    }
	 }
	 i = getLaValues(str);          
      }
      showMasterCallsign(str);
}

int contLog::getLaValues(QString str)
{
       QSqlQuery query;                           
       qy = "SELECT ldxcc,lname,lwae,lcn,ituz,cqz,br,lg FROM tpref,tla WHERE pref=la AND spref='"+str+"'";
       query.exec(qy);
       if( query.size() == 1 ) {                  
          n = 0;
          query.next();                           
          dxcc = query.value(n++).toString();     
          lname = query.value(n++).toString();    
          wae = query.value(n++).toString();      
          cnt = query.value(n++).toString();      
	  ituz = query.value(n++).toString();     
          cqz = query.value(n++).toString();      
 
          br = query.value(n++).toString();       
          lg = query.value(n++).toString();       
	  
	  s = "   "+dxcc+"  -  "+lname+"       "+cnt+"     ITUz -";
	  s += ituz+"     CQz -"+cqz+"    -  "+distanz.getDisRicht(br,lg); 
          labelInfo->setText(s);                  
	  
	  myContest->setDxContinent(cnt);         
	  laPref =  str.count();                  
	  cPref = 0;
	  scPref = 1;                             
	  labelWarnung2->setText("");
	  setContestValues();                     
          return 1;                               
       }
       return 0;                                  
}

void contLog::setContestValues()                             
{                                                            
        switch(methode) {
	  case 2: {
                    i = myContest->getqsoPoints(0,cpref,dxcc); 
		    if( i == 2 )                             
		      lineExchange->setText(cqz);            
		    else
		    if( i == 3 )                             
                      lineExchange->setText(Multi1);         
	        }
	        break;
		
	  case 3: 
	         i = myContest->getqsoPoints(0,dxcc,ituz); 
	        break;
	  case 4: {                                         
                    if(cqz.count() == 2) {
		      lineExchange->setText(cqz);
                      i = myContest->getqsoPoints(0,dxcc,cqz); 
		      
		      wMult2->setText(Multi2);
		      setColorMult2(Mul2p);
		    }
		    else
		       i = myContest->getqsoPoints(0,dxcc,""); 
	        }
		break;
	  case 1:     
	         i = myContest->getqsoPoints(0,cpref);  
		break;
	  case 0: 
	         i = myContest->getqsoPoints(0,dxcc);  
	        break;
		
	  case 6: {                                                       
                    t = lineEditSent->text();  
		    i = 0;
		    while(1) {
	               if(t.at(i) >= QChar('0') && t.at(i) <= QChar('9')) 
                         i++;                                             
		       else
			 break;
		    }
                    if(t.at(i) == QChar('/'))                             
		      myContest->getqsoPoints(0,lineEditSent->text().mid(i +1));
                      else                                                
		      myContest->getqsoPoints(0,lineEditSent->text().mid(i));
	        }
	        break;
	 
         
	 
       }
       
       if(i != -1) {                                   
	 lineEditPoints->setText(s.setNum(Qpts));      
	 wMultNew->setText(Multi1);
         setColorMult(Mul1p);                          
       }
       else {
	 labelWarnung2->setText(myContest->getInfo()); 
       }
}



void contLog::lineCallsignCrCb()
{
     lineExchangeCb();                                
}




void contLog::lineExchangeBackCb(QString str)  
{
 
     if(str.indexOf(" ") != -1) {               
       str = str.trimmed();                     
       lineExchange->setText(str);              
       lineCallsign->setFocus();                
       return;
     }
     if(str.count() == 0) {                     
         //numflg = 1;
         return;
     }
     /*
     if(str.count() <= 3)
	   numflg = 1;
*/
     if(eflg == 1)                              
       eflg = 3;                                
       
     st = str.toUpper();                        
     lineExchange->setText(st);                 
     
     i = st.indexOf("-");                       
     if(i != -1 ) {
       st[i] = '/';
       lineExchange->setText(st);
     }
     
     
     switch(exchp) {
       case 0:                          
                 i = 0;
	  break;
       case 1:                          
	       {                                                         
		 if( numflg ) { 
		   i = st.count() -1;             
		   if(st.at(i) >= QChar('0') && st.at(i) <= QChar('9')) {  
                      qNrlen = i;                                          
		      break;                                               
		   }
		   else
		     numflg = 0;                                           
		 }
                 if( !qNrlen ) {                           
		    if(st.count() >= 2) {                  
                       numflg = 0;
		       i = myContest->getqsoPoints(1,st);  
                       wMultNew->setText(Multi1);
                       setColorMult(Mul1p);                
		       break;
		    }
		    else {
		      if(CNr == SPDX ) {                   
			 i = myContest->getqsoPoints(1,st);
		         wMultNew->setText(Multi1);
                         setColorMult(Mul1p);              
		        break;
		      }
		    }
		 }
		 else {                                     
                        if(st.indexOf("/") != -1) {              
                           if(st.mid(qNrlen +2).count() >= 1) {  
		              i = myContest->getqsoPoints(1,st.mid(qNrlen +2));
			      wMultNew->setText(Multi1);
                              setColorMult(Mul1p);               
		           }
			}
                        else {                                   
		          if(st.mid(qNrlen + 1).count() >= 2) {  
		            i = myContest->getqsoPoints(1,st.mid(qNrlen +1));
			    wMultNew->setText(Multi1);
                            setColorMult(Mul1p);                 
		          }
		        }
		  }
		}
	   break;
	 
       case 2:                 
	       {                                  
	        if( numflg ) { 
		   i = st.count() -1;             
		   if(st.at(i) >= QChar('0') && st.at(i) <= QChar('9')) {  
                      qNrlen = i;                                          
		      break;                                               
		   }
		   else
		     numflg = 0;                                           
		}
		if( !qNrlen ) {                            
		    if(st.count() >= 2) {                  
                       numflg = 0;
		       i = myContest->getMulti2Points(st); 
                       wMult2->setText(Multi2);
                       setColorMult2(Mul2p);               
		    }
		    break;
		 }
		 else {                                    
		   if(st.indexOf("/") != -1) {             //und mit '/' eingegeben 
		      if(st.mid(qNrlen +2).count() >= 2) {                
                        i = myContest->getMulti2Points(st.mid(qNrlen +2));  
		        wMult2->setText(Multi2);
                        setColorMult2(Mul2p);              
		      }
		   }
		   else {
		     if(st.mid(qNrlen + 1).count() >= 2) { 
		       i = myContest->getMulti2Points(st.mid(qNrlen + 1));
		       wMult2->setText(Multi2);
                       setColorMult2(Mul2p);               
		     }
		   }
	         }
	         if(i == -1)
		   labelWarnung2->setText(myContest->getInfo()); 
	       }
	  break;
	 
       case 3:                
	      { 
		idx = st.count() -1;                       
		if(st.at(idx) >= QChar('0') && st.at(idx) <= QChar('9')) {  
                   if(idx < qNrlen + 1) 
		     lineEditPoints->setText("-");                          
                   qNrlen = idx;
		   break;                                                   
		}
		if( CNr == NASPRINT) {     
                   if( qNrlen - (idx +1) == 1) {           
		     break;                                
		   }
		   idx = st.lastIndexOf("/");              
		   if(idx > 3 ) {                          
		      if(dxcc.compare("VE") == 0) {
			st += wMultNew->text();
		        lineExchange->setText(st);         
		      }
		      if((st.count() -idx) >= 2) {	   
		         i = myContest->getqsoPoints(1,st.mid(idx + 1)); 
                         wMultNew->setText(Multi1);
                         setColorMult(Mul1p);                            
		         break;
		      }
		    }
	        }
	        else
		if(CNr == AGCWHTP) {        
                   if(idx -qNrlen <= 2) {
		     if((st.at(idx) != QChar('/') && (st.at(idx) >= QChar('A') && st.at(idx) <= QChar('C')))) {
                       i = myContest->getqsoPoints(1,st.mid(idx));
		       if( i == 1 )                                   
		         lineEditPoints->setText(s.setNum(Qpts));     
		     }
		     else
		       idx++;
		   }
		 }
	       }
	  break; 
	  
       case 4 : {                                          
	          if(st.count() >= 2) {                    
		     i = myContest->getMulti2Points(st);
		     wMult2->setText(Multi2);
                     setColorMult2(Mul2p);                 
		  }
               }
          break;
	  
     }
     if(i == -1)
        labelWarnung2->setText(myContest->getInfo()); 
}   
     

void contLog::switchExchangePolicy(QString st)       
{
     QString x = st;
}




void contLog::lineExchangeCb() 
{
    if(lineCallsign->text().count() <= 2) {        
        labelWarnung2->setText("   Check Callsign ??");
       return;
    }
    
    if(lineExchange->text().count() == 0) {
        labelWarnung2->setText("   Check Exchange ??");
     return;
    }
    labelWarnung2->setText("");
   
    if ( eflg == 1 ) {                            
       qy = "UPDATE wCqso SET "; 
       qy += "ctime='"+cTime->text()+":00";
       qy += "',mode='"+modeBox->currentText();
       qy += "',srst='"+lineEditSrst->text();
       
       s = lineEditSent->text();
       i = s.indexOf("/");
       if(i != -1) {                              
         qy += "',sent='"+s.left(i);
         qy += "',sent2='"+s.mid(i +1);
       }
       else
         qy += "',sent='"+lineEditSent->text();
       qy += "',rrst='"+lineEditRrst->text();
       qy += "' WHERE contid='"+ContestID+"' AND cqsoid="+d_Id;
       QSqlQuery query(qy); 
    
       labelInfo->setText("");
       ButtonRemoveQso->hide();
       ButtonEditLog->setEnabled(true);
       lineCallsign->setEnabled(true);
       clearAll();                             
       showAllQso();                           
       eflg = 0;
       timer->start(1000);
       setTimeDate();
       setColorUhr("WEISS");                   
       lineEditSent->setText(sentSave);        
       rigpolling = runflg;                     
     return;
    }
    else                                       
     if( eflg == 3 ) {                         
        lcompl = lineExchange->text();         
        removeQso(d_Id);                       
	int nn, ii;                            
        QString st, xc;                        
        Call = lineCallsign->text();           
	ii = 1;
        nn = Call.count();
        while( 1 ) {                           
           xc = Call.left(ii); 
           checkMultPrefix(xc); 
           if(ii == nn)
             break;
           ii++;
        }
	
	st = lineExchange->text();
        ii = 1;
        nn = st.count();
        while( 1 ) {                            
           xc = st.left(ii); 
           lineExchangeBackCb(xc);  
           if(ii == nn)
             break;
           ii++;
        }
	
        insertNewQso();                        
                                               
        ButtonRemoveQso->hide();               
        ButtonEditLog->setEnabled(true);
        labelInfo->setText("");
        eflg = 0;
        timer->start(1000);                    
        setTimeDate();
        lineEditSent->setText(sentSave);       
        rigpolling = runflg;                   
       return;
     }
     
     insertNewQso();
}



void contLog::insertNewQso()
{
  QString p;
    
      insertQso();
                                                   
      if(incNr) {                                  
         stxNr++;                                  
         if(stxNr <= 9)
	   p = "00"+s.setNum(stxNr);
         else
         if(stxNr <= 99)
	    p = "0"+s.setNum(stxNr);
         else
	   p = s.setNum(stxNr);
	 
         
	 i = lineEditSent->text().indexOf("/");
	 if( i != -1 ) {
            if(CNr == DARC10M || CNr == WAG || CNr == HNY)  
	      p += "/"+sentVal;
	    else
	    if(CNr == AGCWHTP || CNr == HNY) {              
	      p += sentVal;
	    } 
	 }
         lineEditSent->setText(p);
      }
      
      clearAll();                                   
      showAllQso();                                 
}



void contLog::showAllQso()
{ 
    headItem = workedList->headerItem();
    headItem->setText(8,"Rcvd");
    headItem->setText(9,"Mult1");
    headItem->setText(10,"Multi2");
    headItem->setText(0,"Last  logged");
    
    labelWarnung2->setText("");
    QSqlQuery query; 
    workedList->clear();                                  
    qy = "SELECT cdate,ctime,qcall,band,mode,srst,sent,rrst,rcvd,multi1,multi2 FROM wCqso ";
    qy += "WHERE contid='"+ContestID;
    qy += "' AND qcall LIKE '"+ lineCallsign->text()+"%' ORDER BY cdate DESC, ctime DESC LIMIT 20";
    query.exec(qy);  
    rows = query.size(); 
    while(query.next()) {
      n = 0;
      i = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(workedList);
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
}



int contLog::showMasterCallsign(QString str)
{
    QString call;
    int found;
    QString err = "  ##  -   DOUBLE   -  ##";
    
    headItem = workedList->headerItem();
    headItem->setText(8,"");
    headItem->setText(9,"");
    headItem->setText(10,"");
    headItem->setText(0,"Master Call");
     
    QBrush brush(QColor(211,056,053));              
    QBrush nbrush(QColor(0,255,255));               
    
    QSqlQuery query; 
    workedList->clear(); 
    qy = "SELECT * FROM wCmaster ";
    qy += "WHERE callsign LIKE '"+str+"%' ORDER BY callsign LIMIT 200";
    query.exec(qy);  
    rows = query.size(); 
    if( rows == 0) {
      if(labelWarnung2->text().indexOf("W") == -1)  
         labelWarnung2->setText("");                
    }
    while( rows ) {
      n = 0;
      i = 0;
      found = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(workedList);
      while(query.next()) {
	  rows--;
	  n = 0;
	  call =  query.value(n++).toString();
          item->setText(i,call);                                   
	                                                           
	   if(query.value(n).toInt()) {                            
              if(query.record().fieldName(n).compare(band) == 0) { 
                 item->setForeground(i,brush);                     
		 found++;
		 if(i == 0 && rows == 0) {
		   labelWarnung2->setText(err);                    
		   return rows;
		 }
		 else
		   labelWarnung2->setText("");                     
	      }
              else 
		if(!found)
		 item->setForeground(i,nbrush);        
	   }
	   
	   n++;
	   if(query.value(n).toInt()) { 
              if(query.record().fieldName(n).compare(band) == 0) { 
                 item->setForeground(i,brush);                     
		 found++;
		 if( i == 0 && rows == 0) {
		   labelWarnung2->setText(err);
		   return rows;
		 }
		 else
		   labelWarnung2->setText("");
	      }
              else 
		if(!found)
		 item->setForeground(i,nbrush);                   
	   }
	   
	   n++;
	   if(query.value(n).toInt()) { 
              if(query.record().fieldName(n).compare(band) == 0) { 
                 item->setForeground(i,brush);                     
		 found++;
		 if( i == 0 && rows == 0 ) {
		   labelWarnung2->setText(err);
		   return rows;
		 }
		 else
		   labelWarnung2->setText("");
	      }
              else 
		if(!found)
		 item->setForeground(i,nbrush);                    
	   } 
           
	   n++;
	   if(query.value(n).toInt()) {
              if(query.record().fieldName(n).compare(band) == 0) { 
                 item->setForeground(i,brush);                     
		 found++;
		 if( i == 0 && rows == 0) {
		   labelWarnung2->setText(err);
		   return rows;
		 }
		 else
		   labelWarnung2->setText("");
	      }
              else 
		if(!found)
		 item->setForeground(i,nbrush);                    
	   }
	   
	   n++;
	   if(query.value(n).toInt()) {
              if(query.record().fieldName(n).compare(band) == 0) { 
                 item->setForeground(i,brush);                     
		 found++;
		 if( i == 0 && rows == 0 ) {
		    labelWarnung2->setText(err);
		    return rows;
		  }
		  else
		    labelWarnung2->setText("");
	      }
              else 
	       if(!found)
		 item->setForeground(i,nbrush);                     
	   }
	   
	   n++;
	   if(query.value(n).toInt()) {
              if(query.record().fieldName(n).compare(band) == 0) { 
                 item->setForeground(i,brush);                     
		 found++;
		 if( i == 0 && rows == 0) {
		   labelWarnung2->setText(err);
		   return rows;
		 }
		 else
		   labelWarnung2->setText("");
	      }
              else 
		if(!found)
		 item->setForeground(i,nbrush);                   
	   }
	  //if(i == 10 ) break;
	  if(i == 9) break;
	   i++;                                 
      }
    }
    return 0;
}

//worckedListClickedCb(QTreeWidgetItem* item, int r)



void contLog::worckedListClickedCb(QTreeWidgetItem* item, int r)
{
     int b;
     b = r;
     QString cb;
     ButtonEditLog->setEnabled(false);
     freq = freqBox->value();                    
     s = "b"+item->text(3);
   
     if(s.compare(band) != 0) {                  
       labelWarnung2->setText("   Check band ! ?");
       return;
     }
     eflg = 1;                                  
     rigpolling = 0;                            
     
     sentSave = lineEditSent->text();           
     timer->stop();                             
     labelWarnung2->setText("   Edit mode");
                                                
     qsoItem = item;                            
     
     cDate->setText(item->text(0));             
     cTime->setText(item->text(1));             
     lineCallsign->setText(item->text(2));      
     lineEditSrst->setText(item->text(5));      
     lineEditSent->setText(item->text(6));      
     lineEditRrst->setText(item->text(7));      
     
     lineExchange->setText(item->text(8));      
     wMultNew->setText(item->text(9));          
     wMult2->setText(item->text(10));           
     
     ButtonRemoveQso->show();                  
                                               
     d_Date = item->text(0);                   
     d_Time = item->text(1);                   
     cTime->setText(d_Time.left(5));           
     setColorUhr("GELB");                      
     
     d_Call = item->text(2);                   
     d_Band = item->text(3);                   
     d_Mode = item->text(4);                   
     d_Srst = item->text(5);                   
     d_Sent = item->text(6);                   
     d_Rrst = item->text(7);                   
     d_Rcvd = item->text(8);                   
     d_Multi1 = item->text(9);                 
     d_Multi2 = item->text(10);                
     
     
     qy = "SELECT cqsoid,dxcc,cpref,rcvd,rcvd2,pts,multi1,mul1,mul2,qtcflg FROM wCqso ";
     qy += "WHERE contid='"+ContestID+"' AND cdate='"+d_Date+"' AND qcall='"+d_Call;
     qy += "' AND band='"+d_Band+"' AND ctime='"+d_Time+"'";
     QSqlQuery query(qy); 
     query.next();
    
     n = 0;
     d_Id = query.value(n++).toString();             
     d_Dxcc = query.value(n++).toString();           
     d_Pref = query.value(n++).toString();           
     
     d_Rcvd = query.value(n++).toString();           
     d_Rcvd2 = query.value(n++).toString();          
   
     d_Pts  = query.value(n).toInt();                
     lineEditPoints->setText(query.value(n++).toString());
     d_Multi1 = query.value(n++).toString();         
     d_Mul1p = query.value(n++).toInt();             
     d_Mul2p = query.value(n++).toInt();             
     d_Qtcflg = query.value(n++).toInt();            
    
    
    
    
    
    
}



void contLog::removeQsoCb()
{
    if(eflg == 2) {
       QMessageBox::information(this,
       tr("Info"),
       tr("Change band !!"),
       QMessageBox::Ok);
       return;
    }
    int status = QMessageBox::question(this,tr(" QSO  remove ?"),  
           tr("\n\nQSO - %1   band: %2  time: %3")
           .arg(qsoItem->text(2),qsoItem->text(3), qsoItem->text(1)  ),
           QMessageBox::Yes, QMessageBox::No);
    if(status != QMessageBox::Yes) {
       labelWarnung2->setText("");
       eflg = 0;
       return;
    }
    
   removeQso(d_Id);
   labelWarnung2->setText("");
   labelInfo->setText("");
   ButtonRemoveQso->hide();
   ButtonEditLog->setEnabled(true);
   lineCallsign->setEnabled(true);
   lineEditSent->setText(sentSave);        
   setColorUhr("WEISS");                   
   eflg = 0;
   timer->start(1000);                     
   clearAll();
   showAllQso();                           
}

void contLog::ModeChangedCb(QString m)
{
     if(m.compare("CW") == 0) {
         lineEditSrst->setText("599");          
         lineEditRrst->setText("599");          
	 prepCwdaemon();                        
       }
       else {
	 lineEditSrst->setText("59");           
	 lineEditRrst->setText("59");           
       }
}



void contLog::BandChangedCb(int freqz)
{
     n  = freqz/1000;                             
     if( n != lastF ) {
       lastF = n;
       s.setNum(n);
       MHzFreq = s;                              
       lineBandMHz->setText(s);                  
       band = "b"+getBand(s);                    
       setBandValues(band);                      
     }
}


void contLog::setBandValues(QString band)         
{
     qsoPointsList->clear();                      
     i = 0;
     QTreeWidgetItem *item = new QTreeWidgetItem(qsoPointsList); 
     item->setText(i++,"QSOs"); 
     if(hqsos.value("b160") != 0)
       item->setText(i,s.setNum(hqsos.value("b160")));
     else 
       item->setText(i,"");
     item->setTextAlignment(i++,Qt::AlignCenter);
     if(hqsos.value("b80") != 0)
        item->setText(i,s.setNum(hqsos.value("b80")));
     else
       item->setText(i,"");
     item->setTextAlignment(i++,Qt::AlignCenter);
     if((hqsos.value("b40")) != 0) 
       item->setText(i,s.setNum(hqsos.value("b40")));
     else
       item->setText(i,"");
     item->setTextAlignment(i++,Qt::AlignCenter);
     if(hqsos.value("b20") != 0)
        item->setText(i,s.setNum(hqsos.value("b20")));
     else
       item->setText(i,"");
     item->setTextAlignment(i++,Qt::AlignCenter);
     if(hqsos.value("b15") != 0)
       item->setText(i,s.setNum(hqsos.value("b15")));
     else
       item->setText(i,"");
     item->setTextAlignment(i++,Qt::AlignCenter);
     if(hqsos.value("b10") != 0)
       item->setText(i,s.setNum(hqsos.value("b10"))); 
     else
       item->setText(i,"");
     item->setTextAlignment(i++,Qt::AlignCenter);
     item->setText(i,s.setNum(hqsos.value("total")));
     item->setTextAlignment(i,Qt::AlignCenter);

     i = 0;
     QTreeWidgetItem *itemp = new QTreeWidgetItem(qsoPointsList); 
     itemp->setText(i++,"Points");
     if(hpoints.value("b160") != 0)
       itemp->setText(i,s.setNum(hpoints.value("b160")));
     else
       itemp->setText(i,"");
     itemp->setTextAlignment(i++,Qt::AlignCenter);
     if(hpoints.value("b80") != 0)
       itemp->setText(i,s.setNum(hpoints.value("b80")));
     else
       itemp->setText(i,"");
     itemp->setTextAlignment(i++,Qt::AlignCenter);
     if(hpoints.value("b40") != 0) 
       itemp->setText(i,s.setNum(hpoints.value("b40")));
     else
       itemp->setText(i,"");
     itemp->setTextAlignment(i++,Qt::AlignCenter);
     if(hpoints.value("b20") != 0)
       itemp->setText(i,s.setNum(hpoints.value("b20")));
     else
       itemp->setText(i,"");
     itemp->setTextAlignment(i++,Qt::AlignCenter);
     if(hpoints.value("b15") != 0)
       itemp->setText(i,s.setNum(hpoints.value("b15")));
     else
       itemp->setText(i,"");
     itemp->setTextAlignment(i++,Qt::AlignCenter);
     if(hpoints.value("b10") != 0)
       itemp->setText(i,s.setNum(hpoints.value("b10"))); 
     else
       itemp->setText(i,"");
     itemp->setTextAlignment(i++,Qt::AlignCenter);
     
     itemp->setText(i,s.setNum(hpoints.value("total")));
     itemp->setTextAlignment(i,Qt::AlignCenter);

     i = 0;  
     QTreeWidgetItem *itemm1 = new QTreeWidgetItem(qsoPointsList); 
     itemm1->setText(i++,"Mult1"); 
     if(hmult1.value("b160") != 0)
       itemm1->setText(i,s.setNum(hmult1.value("b160")));
     else
       itemm1->setText(i,"");
     itemm1->setTextAlignment(i++,Qt::AlignCenter);
     if(hmult1.value("b80") != 0)
       itemm1->setText(i,s.setNum(hmult1.value("b80")));
     else
       itemm1->setText(i,"");
     itemm1->setTextAlignment(i++,Qt::AlignCenter);
     if(hmult1.value("b40") != 0)
       itemm1->setText(i,s.setNum(hmult1.value("b40")));
     else
       itemm1->setText(i,"");
     itemm1->setTextAlignment(i++,Qt::AlignCenter);
     if(hmult1.value("b20") != 0)
       itemm1->setText(i,s.setNum(hmult1.value("b20")));
     else
       itemm1->setText(i,"");
     itemm1->setTextAlignment(i++,Qt::AlignCenter);
     if(hmult1.value("b15") != 0)
       itemm1->setText(i,s.setNum(hmult1.value("b15")));
     else
       itemm1->setText(i,"");
     itemm1->setTextAlignment(i++,Qt::AlignCenter);
     if(hmult1.value("b10") != 0)
       itemm1->setText(i,s.setNum(hmult1.value("b10"))); 
     else
       itemm1->setText(i,"");
     itemm1->setTextAlignment(i++,Qt::AlignCenter);
     itemm1->setText(i,s.setNum(hmult1.value("total")));
     itemm1->setTextAlignment(i,Qt::AlignCenter);

     i = 0; 
     QTreeWidgetItem *itemm2 = new QTreeWidgetItem(qsoPointsList); 
     itemm2->setText(i++,"Mult2"); 
     if(hmult2.value("b160") != 0)
       itemm2->setText(i,s.setNum(hmult2.value("b160")));
     else
       itemm2->setText(i,"");
     itemm2->setTextAlignment(i++,Qt::AlignCenter);
     if(hmult2.value("b80") != 0)
       itemm2->setText(i,s.setNum(hmult2.value("b80")));
     else
       itemm2->setText(i,"");
     itemm2->setTextAlignment(i++,Qt::AlignCenter);
     if(hmult2.value("b40") != 0)
       itemm2->setText(i,s.setNum(hmult2.value("b40")));
     else
       itemm2->setText(i,"");
     itemm2->setTextAlignment(i++,Qt::AlignCenter);
     if(hmult2.value("b20") != 0)
       itemm2->setText(i,s.setNum(hmult2.value("b20")));
     else
       itemm2->setText(i,"");
     itemm2->setTextAlignment(i++,Qt::AlignCenter);
     if(hmult2.value("b15") != 0)
       itemm2->setText(i,s.setNum(hmult2.value("b15")));
     else
       itemm2->setText(i,"");
     itemm2->setTextAlignment(i++,Qt::AlignCenter);
     if(hmult2.value("b10") != 0)
       itemm2->setText(i,s.setNum(hmult2.value("b10"))); 
     else
       itemm2->setText(i,"");
     itemm2->setTextAlignment(i++,Qt::AlignCenter);
     itemm2->setText(i,s.setNum(hmult2.value("total")));
     itemm2->setTextAlignment(i,Qt::AlignCenter);

     i = 0; 
     QTreeWidgetItem *itemqs = new QTreeWidgetItem(qsoPointsList); 
     itemqs->setText(i++,"QSOsh");
     if(hqsosh.value("b160") != 0)
       itemqs->setText(i,s.setNum(hqsosh.value("b160")));
     else
       itemqs->setText(i,"");
     itemqs->setTextAlignment(i++,Qt::AlignCenter);
     if(hqsosh.value("b80") != 0)
       itemqs->setText(i,s.setNum(hqsosh.value("b80")));
     else
       itemqs->setText(i,"");
     itemqs->setTextAlignment(i++,Qt::AlignCenter);
     if(hqsosh.value("b40") != 0)
       itemqs->setText(i,s.setNum(hqsosh.value("b40")));
     else
       itemqs->setText(i,"");
     itemqs->setTextAlignment(i++,Qt::AlignCenter);
     if(hqsosh.value("b20") != 0)
       itemqs->setText(i,s.setNum(hqsosh.value("b20")));
     else
      itemqs->setText(i,"");
     itemqs->setTextAlignment(i++,Qt::AlignCenter);
     
     if(hqsosh.value("b15") != 0)
       itemqs->setText(i,s.setNum(hqsosh.value("b15")));
     else
       itemqs->setText(i,"");
     itemqs->setTextAlignment(i++,Qt::AlignCenter);
     if(hqsosh.value("b10") != 0)
       itemqs->setText(i,s.setNum(hqsosh.value("b10"))); 
     else
       itemqs->setText(i,"");
     itemqs->setTextAlignment(i++,Qt::AlignCenter);
     itemqs->setText(i,s.setNum(hqsosh.value("total")));
     itemqs->setTextAlignment(i,Qt::AlignCenter);

     i = 0; 
     QTreeWidgetItem *itemsc = new QTreeWidgetItem(qsoPointsList); 
     itemsc->setText(i++,"Score");
     itemsc->setText(i,s.setNum(hscore.value("b160")));
     itemsc->setTextAlignment(i++,Qt::AlignCenter);
     itemsc->setText(i,s.setNum(hscore.value("b80")));
     itemsc->setTextAlignment(i++,Qt::AlignCenter);
     itemsc->setText(i,s.setNum(hscore.value("b40")));
     itemsc->setTextAlignment(i++,Qt::AlignCenter);
     itemsc->setText(i,s.setNum(hscore.value("b20")));
     itemsc->setTextAlignment(i++,Qt::AlignCenter);
     itemsc->setText(i,s.setNum(hscore.value("b15")));
     itemsc->setTextAlignment(i++,Qt::AlignCenter);
     itemsc->setText(i,s.setNum(hscore.value("b10"))); 
     itemsc->setTextAlignment(i++,Qt::AlignCenter);
     itemsc->setText(i,s.setNum(hscore.value("total")));
     itemsc->setTextAlignment(i,Qt::AlignCenter);
     
     if( band.count() ) {
       QBrush brush(QColor(217,213,162));                  
       i = getBindex(band);                                
       int row = qsoPointsList->topLevelItemCount();       
       n = 0;
       while(row != n ) { 
           Item =  qsoPointsList->topLevelItem(n++);       
           Item->setBackground(i,brush);                   
           Item->setTextAlignment(i,Qt::AlignCenter);  
       }
 
       myContest->setQsoBand(band);                        
       n = getBandPoint(band);                             
       if(!n)
	 labelWarnung2->setText("##     WRONG - band     ##");
       else 
	 labelWarnung2->setText("");
       myContest->setBandPoint(n);                         
       
       myContest->setmul1p(setMultiA);                     
       myContest->setmul2p(setMultiB);                     
     
       labelInfo->setText("");
     
       bqsos = hqsos.value(band);                          
       bpoints = hpoints.value(band);                      
       bmulti1p = hmult1.value(band);                      
       bmulti2p = hmult2.value(band);                      
     
       //bMin = hbtime.value(band);                        
       bMin = 0;
       bQso = 0;
       bflg = 0;
       lineBandMin->setText(s.setNum(bMin));               
       
       
     }
}



void contLog::processWriteDatagram(QString str, int port)
{
    datagram.clear();
    datagram.append(str);
    udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, port);    
}



void contLog::processPendingDatagram()
{
   QString m, tm;
   while(udpSocket->hasPendingDatagrams()) {
     datagram.resize(udpSocket->pendingDatagramSize());
     QHostAddress sender;
     quint16 senderPort;
     udpSocket->readDatagram(datagram.data(),datagram.size(),&sender, &senderPort);
     i = datagram.indexOf (",",0);
     QByteArray Left = datagram.left(i);              
     QByteArray Mid = datagram.mid(i+1);              
     int com = Left.toInt();                          
     tm = Mid;                                        
     if(com == 15) {                                  
       prepCwdaemon();
   }
  }
}

void contLog::prepCwdaemon()
{ 
  QString m, tm;
  
      QString h = QDir::homePath();                   
      m = h+"/log/iniFiles/cwtext.ini";
      QFile file(m);
      if(!file.open(QIODevice::ReadOnly)) {
          qDebug() << "FEHLER cwtext.ini fehlt" << m; 
      }
    
      QTextStream istream( &file);
      while(1) {
        m = istream.readLine();
        if(m.count() == 0) break;    
        i = m.indexOf("=");
        tm = m.mid(i +1);
        m = m.left(i);
        Macros.insert(m, tm);
      }
      file.close();
      m = getMacro("wpm");
      lineWpm->setText(m);
      tm = getMacro("repeat");
      repeat = tm.toInt();
      processWriteDatagram(ESC, '2', m, udpPort);   
}

void contLog::processWriteDatagram(int esc, char com, QString val, int uport)

{
      datagram.clear();
      datagram[0] = esc;                                    
      datagram[1] = com;                                    
      datagram.append(val);                                 
      udpSocket->writeDatagram(datagram.data(),datagram.size(),
                       QHostAddress::LocalHost, uport);     
}




void contLog::requestRigNew()
{
   if( rigpolling ) {                
     tcpSocket->abort();
     tcpSocket->connectToHost(tcphost,tcpport.toInt());
   }
}



void contLog::sentCommand()
{
    QString message;
    if(rigpolling) {
      message = cmdStr;              
      tcpSocket->write(message.toAscii());
      tcpSocket->flush();
     
      if(cmdFlg == 1) {             
        get = FREQZ;
        cmdStr = "f";
        set = 0; 
      }
    }
}



void contLog::readRigReply()
{
    QString st;
    QString message(tcpSocket->readAll());     
    
    if(message.indexOf("RPRT") != -1) {
      stimer->stop();                         
      rigpolling = 0;
      runflg = 0;
      modeBox->setEnabled(true);                   
      QMessageBox::information(this,tr("Hamlib Client"),
                   tr("\nError ocuured timedout.\n"
                   "Check the transceiver connection.\n"
                   "Hamlib server stopped."));
      return;
    }
    
    if(get == FREQZ) {                         
      i = message.count();
      st = message.left(i -4);                 
      
      if(lastFreq.toInt() != st.toInt())
        freqBox->setValue(st.toInt());         
      lastFreq = st;
      if(cmdFlg == 1)
        cmdFlg = 0;
    }
    else {                                     
      i = message.indexOf("\n");
      st = message.left(i);
      if(st.compare(lastMode) != 0) {
        n = modeBox->findText(getMode(st));
        cmdFlg = 2;
        modeBox->setCurrentIndex(n);
        lastMode = st;
      }
    }
  
    if(get == FREQZ) {                        
      get = MODE;
      cmdStr = "m";
      set = 0;
    }
    else {
      get = FREQZ;
      cmdStr = "f";
      set = 0;
    }
}


void contLog::displayError(QAbstractSocket::SocketError socketError)
{
  rigpolling = 0;                 
  stimer->stop();                 
  switch(socketError) {
    case QAbstractSocket::RemoteHostClosedError:
      break;
    case QAbstractSocket::HostNotFoundError:
         QMessageBox::information(this,tr("Hamlib Client"),
                                  tr("\nThe host was not found.\n"
                                  "Config Hamlib and restart contest logging."));
      break;
    case QAbstractSocket::ConnectionRefusedError:
         QMessageBox::information(this,tr("Hamlib Client"),
                                  tr("\nThe connection was refused by the peer.\n\n"
                                  "Make sure the hamlib server is running,\n"
                                  "check that the host name and port\n"
                                  "settings are correct\n"
                                  "or connect the transceiver."));
      break;
    default: 
         QMessageBox::information(this,tr("Hamlib Client"),
                                  tr("\nThe folloing error ocuured: %1.")
                                  .arg(tcpSocket->errorString()));
  }
}

void contLog::starthamlibServer()
{
     QString home, p;                       
     int ok;
     ok = 1;
     home = getenv("HOME"); 
     p = "ps -e | grep rigctld > "+home+"/.qtlog/rigstatus";
     i = system(p.toAscii());               
     home += "/.qtlog/rigstatus";           
     QFile iniFile(home);
     iniFile.open(QIODevice::ReadOnly);
     QTextStream istream( &iniFile);        
     p = istream.readLine(0);               
     iniFile.close();
    
     if(p.indexOf("rigctld") == -1) {       
       QSqlQuery query;                     
       qy = "SELECT rigpid,rigdev,baud,pttdev,rts,dtr FROM wproz WHERE runing=1";
       query.exec(qy);
       query.next();
       
       p = "rigctld -m "+query.value(0).toString();       
       p += " -r "+query.value(1).toString();             
       p += " -s "+query.value(2).toString();             
       p += " -p "+query.value(3).toString();             
       p += " -C rts_state="+query.value(4).toString();   
       p += " -C dtr_state="+query.value(5).toString();   
       p += " &";
       i = system(p.toAscii()); 
     }
     set = 0;
     get = FREQZ;
     cmdFlg = 0;
     cmdStr = "f";
     stimer->start(retry);                                 
}




void contLog::saveAll()
{
    if(CNr) {                                          
      QSqlQuery query; 
      
      qy = "UPDATE wCband SET b160="+s.setNum(hqsos.value("b160"));  
      qy += ",b80="+s.setNum(hqsos.value("b80"));
      qy += ",b40="+s.setNum(hqsos.value("b40"));
      qy += ",b20="+s.setNum(hqsos.value("b20"));
      qy += ",b15="+s.setNum(hqsos.value("b15"));
      qy += ",b10="+s.setNum(hqsos.value("b10"));
      qy += ",total="+s.setNum(hqsos.value("total")); 
      qy += " WHERE contid='"+ContestID+"' AND val='qsos'";
      query.exec(qy);
      qy = "UPDATE wCband SET b160="+s.setNum(hpoints.value("b160")); //hpoints
      qy += ",b80="+s.setNum(hpoints.value("b80"));
      qy += ",b40="+s.setNum(hpoints.value("b40"));
      qy += ",b20="+s.setNum(hpoints.value("b20"));
      qy += ",b15="+s.setNum(hpoints.value("b15"));
      qy += ",b10="+s.setNum(hpoints.value("b10"));
      qy += ",total="+s.setNum(hpoints.value("total")); 
      qy += " WHERE contid='"+ContestID+"' AND val='points'";
      query.exec(qy);
      qy = "UPDATE wCband SET b160="+s.setNum(hmult1.value("b160"));  //hmult1
      qy += ",b80="+s.setNum(hmult1.value("b80"));
      qy += ",b40="+s.setNum(hmult1.value("b40"));
      qy += ",b20="+s.setNum(hmult1.value("b20"));
      qy += ",b15="+s.setNum(hmult1.value("b15"));
      qy += ",b10="+s.setNum(hmult1.value("b10"));
      qy += ",total="+s.setNum(hmult1.value("total"));
      qy += " WHERE contid='"+ContestID+"' AND val='mult1'";
      query.exec(qy);
      qy = "UPDATE wCband SET b160="+s.setNum(hmult1.value("b160"));  //hmult2
      qy += ",b80="+s.setNum(hmult2.value("b80"));
      qy += ",b40="+s.setNum(hmult2.value("b40"));
      qy += ",b20="+s.setNum(hmult2.value("b20"));
      qy += ",b15="+s.setNum(hmult2.value("b15"));
      qy += ",b10="+s.setNum(hmult2.value("b10"));
      qy += ",total="+s.setNum(hmult2.value("total"));
      qy += " WHERE contid='"+ContestID+"' AND val='mult2'";
      query.exec(qy);
      qy = "UPDATE wCband SET b160="+s.setNum(hbtime.value("b160")); 
      qy += ",b80="+s.setNum(hbtime.value("b80"));
      qy += ",b40="+s.setNum(hbtime.value("b40"));
      qy += ",b20="+s.setNum(hbtime.value("b20"));
      qy += ",b15="+s.setNum(hbtime.value("b15"));
      qy += ",b10="+s.setNum(hbtime.value("b10"));
      qy += ",total="+s.setNum(hbtime.value("total"));
      qy += " WHERE contid='"+ContestID+"' AND val='btime'";
      query.exec(qy);
      
      i = freqBox->value();                                 
      freqBox->setValue(setfreqDown(i/1000));   
      
      qy = "UPDATE wContest SET qsoid="+s.setNum(qsoRow);   
      qy += ",view='"+s.setNum(viewtiming);
      qy += "',mode='"+modeBox->currentText();
      qy += "',freqband='"+s.setNum(freqBox->value());      
      qy += "',srst='"+lineEditSrst->text();
      qy += "',sent='"+lineEditSent->text();
      qy += "',rrst='"+lineEditRrst->text();
      qy += "' WHERE contestid='"+ContestID+"'";
      query.exec(qy);
   }
}
