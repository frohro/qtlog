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
#include <QString>
#include "destination.h"
float Acs(double );
float Cosinus(double );
float Sinus(double );
double fsqr(double );

Dest::Dest()
{
      homeBr   = 0.0;                     // home 
      homeLg   = 0.0;                     // home 
}

Dest::~Dest(){
}

// Entfernung und Richtung aus Remote_Locator berechnen und zurueckgeben
// ---------------------------------------------------------------------
QString Dest::calculateToLL(QString str)
{
  char temp[255];
  float latRem, longRem;
  strcpy(temp,str.toAscii());
  if( ! validateLocator(temp)) {
     return "UNBEKANNT";                    // im Dialog anzeigen
  }
  else {                                    // kalkuliere remote lat/long
    calcLatLong(temp, latRem,longRem);
    EntfRichtung(homeBr, homeLg, latRem, longRem);
    return  EntfRicht.sprintf("%d Km / %d Grad",Distanz,Richtung);
  }
}

// Ueberpruefe den Remote_Locator auf korr. Eingabe
// ------------------------------------------------
bool Dest::validateLocator(const char *st)
{
   if((st[0]>='A') && (st[0]<='R') && (st[1]>='A') &&( st[1]<='R')) {
     if( isdigit(st[2]) && isdigit(st[3]) ) {
       if(st[4]==0) {
         return TRUE;
       }
       else
        if((st[4] >= 'A') && (st[4] <= 'X') && (st[5] >= 'A') && (st[5] <= 'X')) {
          return TRUE;
        }
    }
  }
  return FALSE;
}

// Kalkuliere die Breite und Laenge in Grad.dezimal aus dem Locator
// ----------------------------------------------------------------
void Dest::calcLatLong(char *st, float &lat, float &lon)
{
   if(st[4] != 0) {
      lon=(20.*((float)st[0]-65.) +2.*((float)st[2]-48.) +5.*((float)st[4]-65.)/60.)-180.;
      lat=(10.*((float)st[1]-65.) +((float)st[3]-48.) +5.*(st[5]-65.)/120.)-90.;
   }
   else {
    lon=(20.*((float)st[0]-65.) +2.*((float)st[2]-48.)) -180.;
    lat=(10.*((float)st[1]-65.) + ((float)st[3]-48.)) -90.;
   }
}


// -----------------------------------------------------------------------------------------
// IN: grad.mm.ss  -  return: grad.dezimal
// ---------------------------------------------------
float Dest::GradDezimal( QString str)   // str = "53.20.46"
{                                       // str = "53"
 float grad, min, sec;
 QString s,f;
    s = str;
    f = s.section('.',0,0);
    grad = f.toFloat();
    f = s.section('.',1,1);
    min = f.toFloat();
    f = s.section('.',2,2);
    sec = f.toFloat();
   return grad +(min/60) +(sec/3600);
}

// HOME geog.coordinaten  Breite / Laenge  aus Locator berechnen und setzen
// ------------------------------------------------------------------------
void Dest::setHomeCoordinatenFromLoc(QString loc)
{
  char temp[255];
  float latHom, longHom;
    strcpy(temp,loc.toAscii());
    validateLocator(temp);
    calcLatLong(temp, latHom,longHom);
    homeBr = latHom;    // jetzt geogrph. Coordinaten aus Locator als HOME setzen
    homeLg = longHom;
}

// HOME geog.coordinaten  Breite / Laenge  gem. config setzen
// ----------------------------------------------------------
void Dest::setHomeCoordinaten(QString Br, QString Lg)
{
   homeBr = GradDezimal( Br );
   homeLg = GradDezimal( Lg );
}

// HOME geog.coordinaten  Breite / Laenge  gem. config setzen
// ----------------------------------------------------------
int Dest::getRichtung()
{
   if(Richtung <= 180)
       return Richtung + 180;
   else 
       return Richtung -180;
}

// Entfernug und Richtung aus Remote Br/Lg berechnen und als string zurückgeben
// ============================================================================
QString Dest::getDisRicht(QString Br, QString Lg)
{
    //remBr = GradDezimal( Br );
    //remLg = GradDezimal( Lg );
    //qDebug() << homeBr << homeLg << Br << Lg;
    EntfRichtung(homeBr, homeLg, Br.toDouble(), Lg.toDouble()); 
    
    //EntfRichtung(homeBr, homeLg, remBr, remLg);
    return EntfRicht;
}


// ----------------------------------------------------------------------------
// Grosskreis / Entfernung und Richtung ( Grundlagen -Sat.Bahnberechnung DC9ZP )
// geograf. Koordinaten in Grad.dezimal( z.B - B1: 53.nnnn)
//
// Home.Koordin.  : B1 - L1   Ziel.Koordin: B2 - L2
// nautische Form : Laenge WEST und Breite SUED  : mit negativem Vorzeichen
//                  Laenge OST  und Breite NORD  : ohne Vorzeichen ( posetiv )
// Genauigkeit    : ca. 50m in Meereshoehe
//----------------------------------------------------------------------------
void Dest::EntfRichtung(double B1,double L1,double B2,double L2)
{
 float re, degr;
 float fl;
 float s1,s2,s3,s4,br1,br2,l,e1,e2,zw,az,distanz,richtung,diff;
#define  PI 3.14159265358979
     re = 6378.14;
     degr = 180/PI;
     fl = 1/298.257;
     br1 = (B1+B2)/2.0;
     br2 = (B1-B2)/2.0;
     l   = (L1-L2)/2.0;
     e1 = fsqr(Sinus(br2))*fsqr(Cosinus(l))+fsqr(Cosinus(br1))*fsqr(Sinus(l));
     e2 = fsqr(Cosinus(br2))*fsqr(Cosinus(l))+fsqr(Sinus(br1))*fsqr(Sinus(l));
     zw = atan(sqrt(e1/e2));
     s1 = sqrt(e1*e2)/zw;
     s2 = 2*zw*re;
     s3 = (3*s1-1)/(2*e2);
     s4 = (3*s1+1)/(2*e1);
     //Entfernung 
     distanz = s2*(1+fl*s3*fsqr(Sinus(br1))*fsqr(Cosinus(br2))-fl*s4 *
               fsqr(Cosinus(br1))*fsqr(Sinus(br2)));
     Distanz = abs( (int)(distanz + 0.5) );         // keine Nachkommastellen
     //Richtung
     diff = L1-L2;
     zw = Acs(Cosinus(B1)*Cosinus(diff)*Cosinus(B2)+Sinus(B1)*Sinus(B2));
     az = Acs((Sinus(B2)-Sinus(B1)*Cosinus(zw))/(Cosinus(B1)*Sinus(zw)));
     if(Sinus(diff) < 0) richtung = az;
      else richtung = 360-az;
     Richtung = abs( (int)(richtung + 0.5) );       // keine Nachkommastellen
     EntfRicht.sprintf("%d Km / %d Grad",Distanz,Richtung);
}

// -----------------
float Sinus(double x)
{
 float de;
   de  = 180/PI;
 return(sin(x/de));
}

// -----------------
float Cosinus(double x)
{
 float de;
    de = 180/PI;
  return(cos(x/de));
}

// ------------------
float Acs(double x)
{
 float dgr = 180/PI;
  return( (-atan( x/sqrt(-x * x+1)) *dgr) +90.0 );
}

// ------------------
double fsqr(double x)
{
  return (x * x);
}
