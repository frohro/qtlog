/****************************************************************************
   File       : dbrow.cpp 
   copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email      : dl1hbd@darc.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   V 1.5  - 20.05.2009
 ***************************************************************************/
#include <malloc.h>
#include "dbrow.h"
#include <stdio.h>

Row::Row(MYSQL_RES *res, MYSQL_ROW row) {
  fields = row;
  result = res;
}

Row::~Row() {
  if( !IsClosed() ) {
    Close();
  }
}

void Row::Close() {
  if( IsClosed() ) {
    return;                  //throw "Row closed";
  }
  fields = (MYSQL_ROW)NULL;
  result = (MYSQL_RES *)NULL;
}

int Row::GetFieldCount() {
  if( IsClosed()) {
    return 0;                //throw "Row closed";
  }
  nfields = mysql_num_fields(result);
  return nfields;
}

char *Row::GetField(int field) {
  if( IsClosed() ) {
    return 0;                
  }
  if( field < 1 || field > GetFieldCount() ) {
    return 0;                
  }
  return fields[field-1];
}

int Row::IsClosed() {
    return (fields == (MYSQL_ROW)NULL);
}
