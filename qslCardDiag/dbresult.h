/*****************************************************************************
   File       : dbresult.h
   copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email      : dl1hbd@darc.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   V 1.5  - 20.05.2009
 ***************************************************************************/
#ifndef RESULT_H
#define RESULT_H
#include <sys/time.h>
#include <mysql.h>
#include "dbrow.h"
class Result {
 private:
  int row_count;
  int field_count;
  MYSQL_FIELD * field;
  MYSQL_RES *result;
  Row *current_row;
 public:
  Result(MYSQL_RES *);
  ~Result();
  void Close();
  Row * GetCurrentRow();
  int GetRowCount();
  int GetFieldCount();
  char * GetFieldName();
  int Next();
};
#endif 
