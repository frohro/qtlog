/* *******************************************************************
   QtLog copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email            :  diek@mycall.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the free Software Foundation; either version 2 of the License, or
   any later version.
   V 1.5 : 01.12.2007
********************************************************************** */

#include <QHash>
#ifndef CONVERT_H
#define CONVERT_H
//#include <string.h>
class conVert
{
  public:
    conVert();
    ~conVert();
   int i;
   int n;
   int RefEntries;
   QString qy;
   void loadRefnamen();
   QString getdbField(QString );
  typedef class {
    public:
    char Field[20];      
    char Refnam[20];     
  }REFNAMEN;
   REFNAMEN * RefNamen;
/*
   QString qy;
   QHash <QString, QString> dbHash;
   QHash <QString, QString> uHash;
   void loadRefnamen();
   int getdbField(QString name);
*/
};
#endif 
