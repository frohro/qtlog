/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5.n : 01.12.2007
********************************************************************** */

#include <QtGui>
#include <QtSql>
#include <QTimer>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "diglog.h"
#include "../qtlogDiag/dirmngr.h"
extern DirMngr dirMngr;

diglog::diglog(QWidget * parent) : QMainWindow(parent),
settings(QSettings::IniFormat, QSettings::UserScope,"QtLog", "qtlog")
{
    setupUi(this);
    int n = settings.value("FontSize").toString().toInt();
    QFont font;
    font.setPointSize(n);
    setFont(font);
    
    connect(actionENDE, SIGNAL(triggered(bool)), this, SLOT(goExit()));
    connect(ButtonExit, SIGNAL(clicked()), this, SLOT(goExit()));
    
    iptimer = new QTimer(this);                  
    connect(iptimer,SIGNAL(timeout()), this, SLOT(getipcData()));   
    connect(lineEditSearch, SIGNAL (textEdited(QString)), this, SLOT (searchQso(QString)));
    connect(lineEditSearch, SIGNAL (returnPressed()), this, SLOT (getHistory()));
    
    connect(qsoList, SIGNAL(itemClicked(QTreeWidgetItem *,int)), this, SLOT(qsoListClicked(QTreeWidgetItem*,int)));
    connect (awdList, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updateAwdItem(QTableWidgetItem * )));
    
    connect(ButtonUpdate, SIGNAL(clicked()), this, SLOT(updateQso()));
    
    
    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabwidgetChanged(int)));
    
    
    
    
    //db = QSqlDatabase::addDatabase(settings.value("qsqlDatabase").toString());
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(settings.value("host").toString());   
    db.setDatabaseName(settings.value("dbname").toString());
    db.setUserName(settings.value("dbuser").toString());
    db.setPassword(settings.value("dbpasswd").toString());
    if(! db.open()) {
      QSqlError err = db.lastError ();                   
      s = err.databaseText();
      s += tr("\n\nFehlermeldung vom MySQL-Server !\nDie Zugangsberechtigung ist beschaedigt\nBetrieb ist nicht moeglich");
      QMessageBox::information( this,
         tr("Server-Datenbank INFO"),
         tr(s.toAscii()),
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
      exit(0);                                          
    }
    
    QDateTime h = QDateTime::currentDateTime().toUTC();
    toDay = h.date().toString("yyyy-MM-dd");
    
 
    QSqlQuery query;
    
    //BandBox->clear();
    QString band,mBand, hFreq;
    i = 0;                                        
    qy = "SELECT band,mband,hfreq FROM wband WHERE work!=0";
    query.exec(qy);
    while(query.next()) {
      band = query.value(0).toString();   
      mBand = query.value(1).toString();  
      hFreq = query.value(2).toString();  
       Freqto_mBand.insert(hFreq,mBand);   
       Freqto_Band.insert(hFreq,band);     
      mBandTo_Band.insert(mBand,band);    
      BandTo_mBand.insert(band,mBand);    
    }
   
    int c = 0;
    qy = "SELECT atype FROM wawdlist WHERE aset !='0' ORDER BY aset";
    query.exec(qy);
    qDebug() << "AWD_List" << query.size();
    awdList->clear();                    
    awdList->setColumnWidth(0,50);       
    awdList->setColumnWidth(1,70);       
    QTableWidgetItem *hItem0 = new QTableWidgetItem();
    hItem0->setText(QApplication::translate("QtLog","Type", 0, QApplication::UnicodeUTF8));
    awdList->setHorizontalHeaderItem(0, hItem0);
    QTableWidgetItem *hItem1 = new QTableWidgetItem();
    hItem1->setText(QApplication::translate("QtLog","Kenner", 0, QApplication::UnicodeUTF8));
    awdList->setHorizontalHeaderItem(1,hItem1);
   
    n = query.size();
    awdList->setRowCount(n);                              
    col = 0;
    row = 0;
    while(query.next()) {
        c = 0;
        QTableWidgetItem *newItem0 = new QTableWidgetItem(tr("%1").arg((row)*(c))); //Type
        newItem0->setText(query.value(0).toString());     
        awdList->setItem(row,c++,newItem0);               
	qDebug() << newItem0->text() << row << c;
        QTableWidgetItem *newItem1 = new QTableWidgetItem(tr("%1").arg((row)*(c))); //Kenner
        newItem1->setText("");                            
        awdList->setItem(row++,c,newItem1);               
	//qDebug() << row << n << query.value(0).toString();
    }
   
    qsoList->setColumnWidth(0,90);   
    qsoList->setColumnWidth(1,70);   
    qsoList->setColumnWidth(2,80);   
    qsoList->setColumnWidth(3,80);   
    qsoList->setColumnWidth(4,90);   
    qsoList->setColumnWidth(5,110);  
    qsoList->setColumnWidth(6,50);   
    qsoList->setColumnWidth(7,40);   
    qsoList->setColumnWidth(8,50);   
    qsoList->setColumnWidth(9,70);   
    qsoList->setColumnWidth(10,80);  
   //qsoList->setColumnWidth(10,60); 
    logFile = settings.value("logfile").toString();   
    wLogFile->setText(logFile);
    
    s = settings.value("Breite").toString();   
    t = settings.value("Laenge").toString();
     distanz.setHomeCoordinaten(s,t);
    
    sekCnt = 10;
    id = 0;
    omId = 0;
    msgid = -1;
    qslflg = 1;                      
    setup_ipcData();
    getHistory();
    lineEditSearch->setFocus(); 
    
    QTimer::singleShot(50, this, SLOT(InfoCb())); 
    
}

diglog::~diglog()
{
       QString home, p;
       home = getenv("HOME"); 
       p = "ps -e | grep fldig > "+home+"/.qtlog/rigstatus";
       i = system(p.toAscii());          
       home += "/.qtlog/rigstatus";      
       QFile iniFile(home);
       iniFile.open(QIODevice::ReadOnly);
       QTextStream istream( &iniFile);   
       p = istream.readLine(0);          
       iniFile.close();
       
       if(p.indexOf("fldig") != -1) {    
         p.remove(0,1);                  
         i = p.indexOf(" ");
         s = p.left(i);                  
         p = "kill "+s;
         i = system(p.toAscii());        
       }
}

void diglog::goExit()
{
  qApp->quit();
}
void diglog::InfoCb()
{
      QString f = "\nZur Zeit keine Funktion\nDie Fldigi - Einbindung ist in der Entwicklung";
      QMessageBox::information(this,
      tr("Information"),f,QMessageBox::Ok);
}


//--------------------------------------------------------------
void diglog::keyPressEvent( QKeyEvent * event )
{
     switch ( event->key() ) {
       case Qt::Key_Escape : { 
                              lineEditSearch->setText("");
                              labelCont->setText("");
                              labelBeaming->setText("");
                              lineEditSearch->setFocus(); 
                              tabWidget->setCurrentIndex(0);
                              lineEditSearch->show();
                              getHistory();
                              id = 0;
                           break;
                          }
       case Qt::Key_F2 : {
                           if(tabWidget->currentIndex() == 1) {
                             updateQso();
                             getHistory();
                             lineEditSearch->setFocus(); 
                             tabWidget->setCurrentIndex(0);
                             id = 0;
                           }
                          break;
                        }
       case Qt::Key_F3 : {
	                   qslflg++;
			   if(qslflg == 5) qslflg = 1;
                           if(qslflg == 1) {
			     checkBoxQslB->setChecked(TRUE); 
			     checkBoxEqsl->setChecked(TRUE); 
			     labelQslVia->setText("(F3) QSL via B+E");
			   }
			   else
			     if(qslflg == 2)  {
			       checkBoxQslB->setChecked(TRUE);  
			       checkBoxEqsl->setChecked(FALSE); 
			       labelQslVia->setText("(F3) QSL via B -");
			     }
			   else
			     if(qslflg == 3)  {
			       checkBoxQslB->setChecked(FALSE);  
			       checkBoxEqsl->setChecked(TRUE);   
			       labelQslVia->setText("(F3) QSL via - E");
			     }
			   else
			     if(qslflg == 4)  {
			       checkBoxQslB->setChecked(FALSE); 
			       checkBoxEqsl->setChecked(FALSE); 
			       labelQslVia->setText("(F3) QSL via - -");
			     }
                        }
                        break;
     }
}

void diglog::tabwidgetChanged(int idx)
{
    if(idx == 1 && id == 0) {
        QMessageBox::information( this,
        tr("I n f o"),
        tr("\n\nKein QSO selektiert !"),
        QMessageBox::Ok);
        tabWidget->setCurrentIndex(0);  
      return;
    }
}

void diglog::getHistory()
{
  qDebug() << "getHistory";
   qsoList->clear();
   QString t = "US-CA";      
   int x;
   qy = "SELECT day,btime,band,cept,rufz,name,qth,mode,rstr,(SELECT awkenner FROM ";
   qy += logFile+"awd WHERE awtype = '"+t+"' AND id=qid),loc,contestid,ntext ";
   qy += "FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
   qy += " WHERE day <= '"+toDay+"' AND mode='"+ModeBox->currentText()+"' ORDER BY day DESC,btime DESC LIMIT 30";
   QSqlQuery query(qy);
   x = query.size();
   while(query.next()) {
      n = 0;
      i = 0;
      QTreeWidgetItem *item = new QTreeWidgetItem(qsoList);
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,getmyBand(query.value(n++).toString())); 
      s = query.value(n++).toString();                   
      s += query.value(n++).toString();                  
      item->setText(i++,s);                              
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
      item->setText(i++,query.value(n++).toString());    
   }
   labelCount->setText(s.setNum(x));
}


void diglog::searchQso(QString str)
{
     i = getla(str);
     
     if( i == 1 ) {
       qsoList->clear();
       QString t = "DLD";          
       int x;
       QSqlQuery query;
       qy = "SELECT day,btime,band,cept,rufz,name,qth,mode,rstr,(SELECT awkenner FROM ";
       qy += logFile+"awd WHERE awtype = '"+t+"' AND id=qid),loc,contestid,ntext";
       qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
       qy += " WHERE rufz LIKE '"+str+"%' ORDER by day DESC,btime DESC LIMIT 30";
       query.exec(qy);
       x = query.size();
       while(query.next()) {        
         n = 0;
         i = 0;
         QTreeWidgetItem *item = new QTreeWidgetItem(qsoList);
         item->setText(i++,query.value(n++).toString());    
         item->setText(i++,query.value(n++).toString());    
	 item->setText(i++,getmyBand(query.value(n++).toString())); 
         s = query.value(n++).toString();                   
         s += query.value(n++).toString();                  
         item->setText(i++,s);                              
         item->setText(i++,query.value(n++).toString());    
         item->setText(i++,query.value(n++).toString());    
         item->setText(i++,query.value(n++).toString());    
         item->setText(i++,query.value(n++).toString());    
         item->setText(i++,query.value(n++).toString());    
         item->setText(i++,query.value(n++).toString());    
         item->setText(i++,query.value(n++).toString());    
         item->setText(i++,query.value(n++).toString());    
      }
      labelCount->setText(s.setNum(x));
    }
}



int diglog::getla(QString str)
{
    if(str.count() == 0) {
       labelBeaming->setText("");
       labelCont->setText("");
      return 0;
    }
    int i;
    QString wCntry, f, Ca;
    n = 0;
    i = 0;
    QSqlQuery query;                                       
    qy = "SELECT pref FROM tpref WHERE spref='"+str+"'";   
    query.exec(qy);
    i = query.size();                                      
    if( i ) {                                              
       query.next();
       s = query.value(0).toString();
       qy = "SELECT lcn, ldxcc, br,lg, lname,timez FROM tla WHERE la='"+s+"'"; 
       query.exec(qy);
       while(query.next()) {
          Ca = query.value(0).toString();                  
          Ca += "   "+query.value(1).toString();           
	  La = query.value(1).toString();                  
          s = query.value(2).toString();                   
          f = query.value(3).toString();                   
          labelBeaming->setText(distanz.getDisRicht(s,f)); 
	  Ca += +" - "+query.value(4).toString();
	  Ca += "   UTC  "+query.value(5).toString();
	  labelCont->setText(Ca);
         }
      }
      return i;
}



void diglog::qsoListClicked(QTreeWidgetItem *item, int col)
{
  int i = col;
  int n;
  QString s;
  
      qy = "SELECT id,oid,day,btime,rufz,name,qth,band,mode,";
      qy += "freq,loc,ntext,qslmgr";
      
      qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id)";
      qy += " WHERE band = '";       
      s = item->text(2);             
      i = s.indexOf("m");            
      if(i != -1)
        qy += getsBand(s);           
      else
	qy += getBand(s);            
      qy += "' AND rufz = '"+item->text(3)+"' AND day = '";
      qy += item->text(0)+"' AND btime = '"+item->text(1)+"'";
      
      QSqlQuery query(qy);                                  
      if(query.size() == 1) {
        query.next();                                       
        i = 0;
        id = query.value(i++).toInt();                      
        omId = query.value(i++).toInt();                    
 
        qDay = query.value(i++).toString();                 
        qTime = query.value(i++).toString();                
        lineEditCall->setText(query.value(i++).toString()); 
        lineEditName->setText(query.value(i++).toString()); 
        lineEditQth->setText(query.value(i++).toString());  
	s = query.value(i++).toString();
        lineEditFreq->setText(getmyBand(s));                
        n = ModeBox->findText(query.value(i++).toString()); 
	ModeBox->setCurrentIndex(n);		              	       
        qFreq = query.value(i++).toString();
        lineEditLoc->setText(query.value(i++).toString());   
        lineEditNotiz->setText(query.value(i++).toString()); 
        wQslMgr->setText(query.value(i++).toString());       
        
	setAwdKenner();
	tabWidget->setCurrentIndex(1);
	lineEditSearch->hide();
      }
}


void diglog::setAwdKenner()
{
  int r, c;
  QString awdtype, kenner;
  
        qy = "SELECT awtype,awkenner FROM "+logFile+"awd WHERE qid="+s.setNum(id);
        QSqlQuery query(qy); 
        QTableWidgetItem * aItem;        
        n = awdList->rowCount();         
        if( query.size() ) {             
          while(query.next()) {
            r = 0;
            i = 0;
            awdtype = query.value(r++).toString();          
            kenner = query.value(r).toString();             
            aItem = awdList->item(i,0);                     
            while( i != n ) {                               
              if((awdtype.compare(aItem->text())) == 0 ) {  
                 c = 1;
                 QTableWidgetItem *kItem = new QTableWidgetItem(tr("%1").arg((row)*(c))); 
                 kItem->setText(kenner);                    
                 awdList->setItem(i, 1, kItem);             
                 break;                                     
              }
              if( i == n -1 ) {                             
                 awdtype = "\nDer AwdType ' "+awdtype+" ' ist unbekannt.\nBitte anlegen.";
                 QMessageBox::information( this,
                 tr("AWD Fehler"),awdtype,
                 QMessageBox::Ok | QMessageBox::Default,
                 QMessageBox::NoButton, QMessageBox::NoButton);
                return;                               
              }
              i++;
              aItem = awdList->item(i,0);             
          }
        }
      }
      else {                                          
        r = 0;
        i = 0;
        kenner = "";                                  
        aItem = awdList->item(i,0);                   
        while( i != n  ) {                            
            c = 1;
            QTableWidgetItem *kItem = new QTableWidgetItem(tr("%1").arg((row)*(c))); 
            kItem->setText(kenner);                   
            awdList->setItem(i, 1, kItem);            
            if(i == n) break;
                i++;
            aItem = awdList->item(i,0);               
        }
      }
}
  




void diglog::updateAwdItem(QTableWidgetItem *item)
{
  int row;
  
      if (item != awdList->currentItem())                  
         return;
      if(awdList->column( item ) == 0)                     
         return;                   
      row = awdList->row( item );
      QTableWidgetItem * kItem = awdList->item( row, 0);  
      QSqlQuery query;                                    
      qy = "SELECT awkenner FROM "+logFile+"awd WHERE qid="+s.setNum(id)+" AND awtype='";
      qy += kItem->text()+"'";
      query.exec(qy);
     
      if( query.size() == 1 ) {                                 
	 if(item->data(Qt::EditRole).toString().count() != 0) { 
            qy = "UPDATE "+logFile+"awd SET awkenner='"+item->data(Qt::EditRole).toString();
            qy += "' WHERE qid="+s.setNum(id)+" AND awtype='"+kItem->text()+"'";
            query.exec(qy);
         }
         else {                                                 
	   qy = "DELETE FROM "+logFile+"awd WHERE awtype='"+kItem->text()+"' AND qid="+s.setNum(id);
	   query.exec(qy);
	 }
      }
      else {                                                    
         qy = "SELECT rufz FROM "+logFile+","+logFile+"om WHERE id="+s.setNum(id);
         qy += " AND omid=oid";
         query.exec(qy);
         query.next();                                                
         qy = "INSERT INTO "+logFile+"awd VALUES (0,"+s.setNum(id);   
         qy += ",'"+query.value(0).toString();                        
         qy += "','"+kItem->text();                                   
         qy += "','"+item->data(Qt::EditRole).toString();             
         qy += "')";
         query.exec(qy);
     }
     awdList->clearSelection(); 
     lineEditSearch->setFocus(); 
}



void diglog::insertQso()
{
  QString Call, Cept, t, pr, Freq, Date;
  int n, i, x;
  
       omId = 0;
       Cept = "";
       Call = getRecValue("call").simplified();
      
       n = Call.count();
       i = 1;
       while( n-- ) {              
         t = Call.left(i++);
         x = getla(t);
       }
       QSqlQuery query; 
       QSqlField field("feld",QVariant::String);              
       qy = "SELECT omid,name FROM "+logFile+"om WHERE rufz='"+Call+"'";
       query.exec(qy);
       i = query.size();
       if( i ) {                                              
         query.next();
         omId = query.value(0).toInt();                       
       }
       else {                                                 
         qy = "INSERT INTO "+logFile+"om VALUES (0,'";
         qy += Call+"',";                                     
         field.setValue(getRecValue("name").simplified());    
         qy += db.driver()->formatValue(field,false);         
         qy += ")";
         query.exec(qy);
         omId = query.lastInsertId().toInt();          
       }
  
       
       qy = "INSERT INTO "+logFile+" VALUES (0,";         
       qy += t.setNum(omId);                              
       qy += ",'"+Cept+"',";                              
       field.setValue(getRecValue("qth").simplified());   
       qy += db.driver()->formatValue(field,false);       
       qy += ",'"+getRecValue("locator").simplified();    
       
       Freq = getRecValue("mhz").simplified();
       i = Freq.indexOf(".");                             
       s = Freq.left(i);
 
       qy += "','"+getBand(s);                            
       qy += "','"+getRecValue("mhz").simplified();       
       qy += "','"+getRecValue("mode");                   
       qy += "','";
      
       Date = getRecValue("date");                  
       Date.insert(4,"-");                          
       Date.insert(7,"-");                          
       qy += Date;                                  
       qy += "','";
       
       t = getRecValue("time");                     
       t.insert(2,":");                             
       t.append(":");                               
       t.append(s.setNum(sekCnt));                  
       qy += t;                                     
       qy += "','";
       qy += t;                                     
       if(sekCnt++ == 60)
	 sekCnt = 10;
       
       s = getRecValue("rx");
       qy += "','"+s.trimmed();                     
       s = getRecValue("tx");
       qy += "','"+s.trimmed();                     
       qy += "','";                                 
      
       
       if(checkBoxQslB->isChecked() == true) {      
         qy += "','R','R";                          
         pr = "B";                                  
       }
       else {
         qy += "','-','-";                          
         pr = "";                                   
       }
       
       qy += "','0000-00-00";                       
       qy += "','0000-00-00";                       
       qy += "','"+wQslMgr->text();                 
       qy += "','"+RigBox->currentText();           
       qy += "','"+getRecValue("pwr");              
       
       
       t = "SELECT ldxcc,lwae,lwpx,ituz,cqz,lcn FROM tla WHERE la = '"+La+"'";
       query.exec(t);
       if(query.size()) {                           
         while(query.next()) {
            n = 0;
            qy += "','"+query.value(n++).toString(); 
            qy += "','"+query.value(n++).toString(); 
            qy += "','"+query.value(n++).toString(); 
            qy += "','"+query.value(n++).toString(); 
            qy += "','"+query.value(n++).toString(); 
            qy += "','"+query.value(n++).toString(); 
         }
       }
       else {                              
         qy += "','";                              
         qy += "','";                              
         qy += "','";                              
         qy += "','";                              
         qy += "','";                              
         qy += "','";                              
       }
       qy += "','','','";                           
       qy += pr;                                    
       field.setValue(getRecValue("notes"));        
       qy += "',"+db.driver()->formatValue(field,false); 
       qy += ",'";                                  
       qy += "','";                                 
       qy += "','KW";                               
       qy += "','1',0)";                            
       qDebug() << qy; 
       query.exec(qy);                              
       id = query.lastInsertId().toInt();           
    
       
       qy = "INSERT INTO "+logFile+"qsl VALUES (0,";
       qy += t.setNum(id);                          
       qy += ",'"+Call;                             
       qy += "','"+Date;                            
       qy += "','";                                 
       qy += "','";                                 
       
       qy += "','','";                        
       if(checkBoxEqsl->isChecked() == true)  
         qy += "R";                           
       else 
         qy += "-";                           
       qy += "','";                      
       qy += "','";                      
       
       qy += "','";                      
       qy += "','";
       qy += "R";                        
       qy += "','";                      
       qy += "','";                      
       
       qy += "','";                      
       qy += "','";                      
       qy += "','";                      
       qy += "','";                      
       qy += "','";                      
       qy += "','";                      
       qy += "','";                      
       qy += "','";                      
       qy += "','"; //+wCustom1->text();     
       qy += "','"; //+wCustom2->text();     
       qy += "','')";                    
       query.exec(qy);
       qDebug() << qy;
       qDebug() << query.lastError();
       
       
       if(getRecValue("state").count() != 0) {
          qy = "INSERT INTO "+logFile+"awd VALUES (0,";
          qy += t.setNum(id);                     
          qy += ",'"+Call;                        
          qy += "','US-CA";                       
          qy += "','"+getRecValue("state");       
          qy += "')";
          query.exec(qy);                        
	  lineEditState->setText(getRecValue("state"));
	  qDebug() << qy;
	  qDebug() << query.lastError();
       }
   
       
       if(getRecValue("province").count() != 0) {
          qy = "INSERT INTO "+logFile+"awd VALUES (0,";
          qy += t.setNum(id);                     
          qy += ",'"+Call;                        
          qy += "','WAVE";                        
          qy += "','"+getRecValue("province");    
          qy += "')";
          query.exec(qy);                       
	  
	  qDebug() << qy;
	  qDebug() << query.lastError();
       }
       
       
       qsoList->clear();
       t = "DLD";           
       
       qy = "SELECT day,btime,band,cept,rufz,name,qth,mode,rstr,(SELECT awkenner FROM ";
       qy += logFile+"awd WHERE awtype = '"+t+"' AND id=qid),loc,contestid,ntext";
       qy += " FROM "+logFile+"om LEFT JOIN ("+logFile+"qsl,"+logFile+") ON (omid=oid AND qsoid=id) WHERE id="+s.setNum(id);
       qDebug() << qy;
       query.exec(qy);
       
       qDebug() << query.lastError();
       x = query.size();
       qDebug() << "X" << x;
       while(query.next()) {        
         n = 0;
         i = 0;
         QTreeWidgetItem *item = new QTreeWidgetItem(qsoList);
         item->setText(i++,query.value(n++).toString());     
         item->setText(i++,query.value(n++).toString());     
	 item->setText(i++,getmyBand(query.value(n++).toString())); 
         s = query.value(n++).toString();                    
	
         s += query.value(n++).toString();                   
         item->setText(i++,s);                               
	 lineEditCall->setText(s);
	
         item->setText(i++,query.value(n).toString());        
	 lineEditName->setText(query.value(n++).toString());  
	
         item->setText(i++,query.value(n).toString());        
	 lineEditQth->setText(query.value(n++).toString());   
	
         item->setText(i++,query.value(n++).toString());      
         item->setText(i++,query.value(n++).toString());      
         item->setText(i++,query.value(n++).toString());      
	
         item->setText(i++,query.value(n).toString());        
	 lineEditLoc->setText(query.value(n++).toString());   
	
         item->setText(i++,query.value(n++).toString());      
         item->setText(i++,query.value(n).toString());        
	 lineEditNotiz->setText(query.value(n++).toString()); 
      }
       
      
      setAwdKenner();                         
      id = 0;
      omId = 0;
      
      
      if(La.count()) {                                
	QString xband;
        qy = "SELECT cntx,b"+getBand(qFreq)+" FROM "+logFile+"dx ";
        qy += "WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
        query.exec(qy);
        query.next();
        if(query.size() != 0) {                       
          n = query.value(0).toInt();                 
          xband = query.value(1).toString();          
          n++;
          qy = "UPDATE "+logFile+"dx SET cntx=";
          qy += t.setNum(n);
          if(xband.compare("e") != 0) {               
            qy += ", b"+getBand(qFreq);               
            qy += "='x' WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
          }
          else {                                      
            qy += " WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
          }
          query.exec(qy);
        }
        else {
          qy = "INSERT INTO "+logFile+"dx VALUE ('";
          qy += La;                                   
          qy += "','"+ModeBox->currentText();         
          qy += "',1,0,";                             
          qy += "'','','','','','','','','','','','','','','','','','','')";
          query.exec(qy);
          qy = "UPDATE "+logFile+"dx SET b"+getBand(qFreq);
          qy += "='x' WHERE dxpref='"+La+"' AND mode='"+ModeBox->currentText()+"'";
          query.exec(qy);
        }
      }
      tabWidget->setCurrentIndex(1);      
}



void diglog::updateQso()
{
  
  if(id) {
    QSqlQuery query; 
    qy = "UPDATE "+logFile+"om SET name='"+lineEditName->text();
    qy += "' WHERE omid="+s.setNum(omId);
    query.exec(qy);
    
    qy = "UPDATE "+logFile+" SET qth='"+lineEditQth->text()+"',loc='"+lineEditLoc->text()+"',qslmgr='"+wQslMgr->text();
    if(checkBoxQslB->isChecked() == TRUE)
      qy += "',qsls='R',prflg='B'";
    else 
      qy += "',qsls='-',prflg=''";
    qy += ",ntext='"+lineEditNotiz->text();
    qy += "' WHERE id="+s.setNum(id);
    query.exec(qy);
 
    
    
    qy = "UPDATE "+logFile+"qsl SET eqsls='";
    if(checkBoxEqsl->isChecked() == TRUE)
      qy += "R'";
    else
      qy += "-'";
    qy += " WHERE qsoid="+s.setNum(id);
    query.exec(qy);
    
    tabWidget->setCurrentIndex(0);
    lineEditSearch->show();
    getHistory();
  }
  else {
     QMessageBox::information( this,
         tr("Info"),
         tr("\n\nKein QSO selectiert"),
         QMessageBox::Ok | QMessageBox::Default,
         QMessageBox::NoButton, QMessageBox::NoButton);
	 tabWidget->setCurrentIndex(0);
     return;
  }
}
