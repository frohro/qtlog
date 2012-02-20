/****************************************************************************
   File       : dbconnection.cpp 
   copyright  : (C) 2000 by Dieter Kumpies DL1HBD
   email      : dl1hbd@darc.de
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   V 1.5 - 20.05.2009
 ***************************************************************************/
#include <QDebug>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "dbconnection.h"   
#define ENCSTR 65200                           

Connection::Connection(char *host, char *db, char *uid, char *pw) {
   connection = (MYSQL *)NULL;
   Connect(host, db, uid, pw);
}

void Connection::Connect(char *host, char *db, char *uid, char *pw) {
   mysql_init( &mysql );
   connection = mysql_real_connect( &mysql, host, uid, pw, db, 0, 0, 0 );
   if( IsConnected() ) {
       sprintf(connectInfo,"%s@%s - database %s",uid,host,db);
       enc_str = (char *)malloc(ENCSTR);           
   }
   else
       sprintf(connectInfo,"%s@ database  - FAULT -",uid);
}

Connection::~Connection() {
   if( IsConnected() ) {
     Close();
   }
}

void Connection ::Close() {
   if( !IsConnected() ) {
     return;                                
   }
   delete enc_str;
   mysql_close(connection);
   connection = (MYSQL *) NULL;
}

Result * Connection::Query(QString qy) {
MYSQL_RES *res;
    if( !IsConnected() ) {           
      qDebug() << "Not connected";   
    }
    state = mysql_query( connection, qy.toAscii());  
    if( state != 0 ) 
      return (Result *) NULL;              
    res = mysql_store_result(connection);  
    if( res == (MYSQL_RES *) NULL ) {      
      affected_rows = mysql_affected_rows(connection); 
      
      return (Result *) NULL;               
    }
    return new Result(res);                 
}

Result *Connection::Real_Query(QString sql, unsigned long len) {
 MYSQL_RES *res;
    if( !IsConnected() ) {                 
      qDebug() << "Not connected";         
    }
    state = mysql_real_query( connection, sql.toAscii(), len); 
    if( state < 0 ) 
       return (Result *) NULL;             
   
    res = mysql_store_result(connection);  
    if( res == (MYSQL_RES *)NULL ) { 
      affected_rows = mysql_affected_rows(connection);
      return (Result *) NULL;              
    }
    return new Result(res);                
}


QString Connection::GetOneField(QString sql) {
    var = "";
    Result *res = Query(sql.toAscii());
    if((res->GetRowCount()) != 0) {       
      res->Next();
      Row *row = res->GetCurrentRow();
      var = row->GetField(1);
    }
    res->Close();
    return var;
}

int Connection::GetAffectedRows() {
   return affected_rows;
}

const char * Connection::GetError() {
  if( IsConnected() ) {
    return mysql_error( connection );
  }
  else {
     return mysql_error( &mysql );
  }
}

char *Connection::GetConnectInfo() {
   return &connectInfo[0];
}

int Connection::IsConnected() {
   return !(!connection);
}

int Connection::GetState() { 
   return state;                     
}

int Connection::insertdbId() {       
   return mysql_insert_id( connection );
}

char * Connection::escapeString( char * von ) {
   enc_cnt = mysql_escape_string(enc_str, von, strlen(von));
   return enc_str;
}

QString Connection::EscapeQString( char * von ) {
   enc_cnt = mysql_escape_string( enc_str, von, strlen( von ) );
   return enc_str;
}
