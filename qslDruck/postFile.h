/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#ifndef POSTFILE_H
#define POSTFILE_H
#include "decl.h"
#include "translate.h"
#include <QtGui>
#include <stdio.h>
#include "qslPr.h"
typedef int Type;
class PostFile
{
 public:
   PostFile();                    
   ~PostFile();                   
    QString qy;
   
    QSettings settings;
    translate trans;              
    QString log;
    QString traeger;              
    QString form;                 
      int col;                    
      int row;                    
    float randx;                  
    float randy;
    float breite;
    float hoehe;
    float zwx;                    
    float zwy;
    int sel;
    float tposx;                  
    float tposy;
    float xsum, ysum;
    QString printerStr;
    int zuf;
    float frame;
    int just;
    int startlab;
    int post;
    int numr;
    char userCall[CALL +1];
    char operatorCall[CALL+1];
    char userName[NAME];
    char buero[30];
    char locator[LOCATOR];
    char hdxcc[DXCC];
    char qth[QTH];
    char mail[255];
    char www[255];
  
    QString ErrInfo;
    int err;
    QString GetErrMessage();
    int GetState();
    void LoadMedium( QString );
    void Label( QString, int, QString, int, int, int, char *);
    void create_Label(QString, int, int, int, char *);
    void create_Card(QString, int, int, char *);
    //void createLabel_SWL(FILE *, char *,char *,float,float,float,float);
    //void createCard_SWL(FILE *, char *, char *);
    //void create_Label_SWL(QString, int, int, char *);      
    //create_Label_SWL(QString, int, int, char *, float, float, float, float) 
    void create_Label_SWL(QString, QString, char *, float, float, float, float);
    void create_Card_SWL(QString, char *);                  
 private:

    class Qso
    {
      public:
       Qso( Type W ) {
        Wert = W;
       }
     char Call[CALL +1];    
     char Name[NAME +1];      
     char Dxcc[DXCC +1];
     char Pref[PREF+1];
     char Cept[PREF];
     //char Suf[SUF +1];      
     char Day[DAY +1];
     char Btim[START +1];
     char Band[BAND +1];
     char Mod[MOD +1];
     char Freq[FREQ +1];
     char Qslr[QSL +1];
     char Rsts[RST +1];
     char Qslmgr[QSLMGR +1];
     char Rig[RIG+1];
     char TxPwr[PWR+1];
     char RxPwr[PWR +1];
     char Ktext[KTEXT];
      int QuarterId;
      int Operator;
     char Swl[SWL];
      Type Wert;
   };
   Qso * q;
   
    class Rig 
    {
      public:
       Rig( Type W ) {
       Wert = W;
       }
      char Rigtype[8];
      char Trans[30];
      char Ant[50];
      char Op[36];
      char Spez[50];
      Type Wert;
    };
    Rig * r;
   
    class Qslk 
    {
      public:
       Qslk( Type W, Qslk *N = 0 ) {
       Wert = W;
       next = N;
       }
      char Name[12];
      char Ktype[8];
      char Dear[12];
      char Well[70];
      char Cqz[70];
      char Text1[70];
      char Text2[70];
      char Text3[70];
      Type Wert;
      Qslk *next;
    };
    Qslk * qk;
    Qslk *ktop;
    Qslk *k;
};
#endif 
 
