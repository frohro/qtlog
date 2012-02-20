/****************************************************************************
   File       : dbrow.h
   copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email      : dl1hbd@darc.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   V 1.5  - 20.09.2009
 ***************************************************************************/
#ifndef ROW_H
#define ROW_H
#include <sys/types.h>
#include <mysql.h>

class Row {
 private:
  MYSQL_RES *result;
  MYSQL_ROW fields;
  int nfields;
 public:
  Row(MYSQL_RES *, MYSQL_ROW);
  ~Row();
  char *GetField(int);
  int GetFieldCount();
  int IsClosed();
  void Close();
};
#endif 
