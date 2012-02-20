/*****************************************************************************
   File       : dbresult.cpp 
   copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email      : dl1hbd@darc.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   V 1.5  - 20.09.2009
 ***************************************************************************/
#include <QDebug>
#include "dbresult.h"
#include <stdio.h>

Result::Result(MYSQL_RES *res) {
  result = res;
}

Result::~Result() {
  result = (MYSQL_RES *) NULL;
}

int Result::Next() {
 MYSQL_ROW row;
    if( result == ( MYSQL_RES *) NULL ) {
      qDebug() << "return ??";
      return 0;                         //throw "Result set closed";
    }
    row = mysql_fetch_row(result);
    if( !row ) {
      current_row = (Row *)NULL;
      return 0;
    }
    else {
      current_row = new Row(result, row);
      return 1;
    }
}

Row *Result::GetCurrentRow() {
  if( result == (MYSQL_RES *)NULL ) {
    return NULL;                       //throw "Result set closed";
  }
  return current_row;
}

void Result::Close() {
  if( result == (MYSQL_RES *)NULL ) {
    return;
  }
  
  mysql_free_result(result);
  result = (MYSQL_RES *)NULL;
}

int Result::GetRowCount() {
    row_count = mysql_num_rows(result);
    return row_count;
}

int Result::GetFieldCount() {
    field_count = mysql_num_fields(result);
    return field_count;
}

char * Result::GetFieldName() {
     field  = mysql_fetch_field(result);
     return field->name;
}
