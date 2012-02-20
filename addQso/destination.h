/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <math.h>
#include <qstring.h>
#ifndef DEST_H
#define DEST_H
class Dest
{
  public:
      Dest();
     ~Dest();
     void setHomeCoordinaten(QString, QString);  
     QString getDisRicht(QString, QString);
     void EntfRichtung(double, double, double, double);
     float GradDezimal(QString );                
     int getRichtung();

     void setHomeCoordinatenFromLoc(QString);
     QString calculateToLL(QString);
     bool validateLocator(const char *);
     void calcLatLong(char *, float &, float &);
     int Distanz;
     int Richtung;
  private:
     QString EntfRicht;
     float remBr;           
     float remLg;           
     float homeBr;          
     float homeLg;          
};
#endif
