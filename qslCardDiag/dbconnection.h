/****************************************************************************
   File       : dbconnection.h
   copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email      : dl1hbd@darc.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   V 1.3 -  20.05.2009
 ***************************************************************************/
#ifndef CONNECTION_H
#define CONNECTION_H
#include <mysql.h>
#include <sys/time.h>
#include <sys/types.h>
#include <qstring.h>
#include "dbresult.h"
class Connection  {
 public: 
  Connection(char *, char *, char *, char *);
  ~Connection();
  void Close();
  void Connect(char *, char *, char *, char *);
   int GetAffectedRows();
   const char *GetError();
   char *GetConnectInfo();
   int IsConnected();
   int GetState(); 
   Result * Query(QString);            	         
   Result * Real_Query(QString,unsigned long);   
   QString GetOneField(QString);                 
   int insertdbId();
   char * escapeString(char *);
   QString EscapeQString( char * );
 private:
   int state;            		
   int affected_rows;
   char *enc_str;
   int enc_cnt;
  char connectInfo[120];
  QString var;
  MYSQL mysql;
  MYSQL *connection;
};
#endif 
