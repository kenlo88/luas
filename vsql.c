/*
   vsql is a tiny lua interface to mysql client
   (c) Copyright 2015-2019 by kenlo88@gmail.com
*/

#include <stdio.h>
#ifdef WIN32
/*#include <winsock2.h>*/
#endif
#include <mysql/mysql.h>

#include "lua.h"
#include "lauxlib.h"

typedef struct {
    short   closed;
    int     env;
    MYSQL   *con;
} vsql_con;

#define VCON "Vcon"

vsql_con* vsql_getcon (lua_State *L) {
  vsql_con *this_con = (vsql_con *)luaL_checkudata (L, 1, VCON);
  luaL_argcheck (L, this_con != NULL, 1, "connection expected");
  luaL_argcheck (L, !this_con->closed, 1, "connection is closed");
  return this_con;
}

int lua_vsql_connect(lua_State *L) {
  char *host, *usr, *passwd, *db;
  int port;
  host   = (char*) lua_tostring(L, -5);
  usr    = (char*) lua_tostring(L, -4);
  passwd = (char*) lua_tostring(L, -3);
  db     = (char*) lua_tostring(L, -2);
  port   = (int) lua_tonumber(L, -1);

  MYSQL *con = mysql_init(NULL);
  con = mysql_init(NULL);
  if (con == NULL) 
  {
      fprintf(stderr, "mysql_init() failed\n");
      return 0;
  }
  mysql_options(con, MYSQL_SET_CHARSET_NAME, "utf8");
  mysql_options(con, MYSQL_INIT_COMMAND, "SET NAMES utf8");
  if (mysql_real_connect(con, host, usr, passwd, db, port, NULL, 0) == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      return 0;
  }  

  vsql_con *this_con = (vsql_con *)lua_newuserdata(L, sizeof(vsql_con));
  luaL_getmetatable(L,VCON);
  lua_setmetatable(L,-2);
  this_con->con = con;
  this_con->closed = 0;
  return 1;
}

int lua_vsql_execsql(lua_State *L) {
  MYSQL *con = 0;
  char *sql;

  vsql_con *this_con = vsql_getcon (L);
  sql = (char *)luaL_checkstring(L, 2);
  con = this_con->con;
  if (mysql_query(con, sql)) 
  {
      lua_pushnil(L);
      lua_pushstring(L,mysql_error(con));
      return 2;
  }
  MYSQL_RES *result = mysql_store_result(con);
  if (result == NULL) 
  {
      lua_pushnil(L);
      lua_pushstring(L,mysql_error(con));
      return 2;
  }

  unsigned int ncol = mysql_num_fields(result);
  unsigned int nrow = mysql_num_rows(result);
  MYSQL_ROW row;
  int i = 1, j;

  lua_newtable(L);
  lua_pushstring(L,"nrow");
  lua_pushnumber(L,nrow);
  lua_settable(L,-3);
  lua_pushstring(L,"ncol");
  lua_pushnumber(L,ncol);
  lua_settable(L,-3);

  MYSQL_FIELD *field;
  lua_pushstring(L,"col");
  lua_newtable(L);
  for(j = 0; j < ncol; j++)
  {
      field = mysql_fetch_field_direct(result, j);
      lua_pushnumber(L,j+1);
      lua_pushstring(L, field->name);
      lua_settable(L,-3);
  }
  lua_settable(L,-3);

  while ((row = mysql_fetch_row(result))) 
  { 
      lua_pushnumber(L,i++);
      lua_newtable(L);
      for(j = 0; j < ncol; j++) 
      {
          lua_pushnumber(L,j+1);
          if (row[j] == NULL)
          {
               lua_pushnil(L);
          }
          else
          {
               lua_pushstring(L,row[j]);
          }
          lua_settable(L,-3);
      }
      lua_settable(L,-3);

  }
  mysql_free_result(result);
  return 1;
}

int lua_vsql_close(lua_State *L) {
  MYSQL *con = 0;
  vsql_con *this_con = vsql_getcon (L);
  con = this_con->con;
  this_con->closed = 1;
  mysql_close(con);
  return 0;
}

int luaopen_vsql (lua_State *L) { 
  struct luaL_Reg vsql[] = {
    { "connect",   lua_vsql_connect },
    { NULL, NULL },
  };
  struct luaL_Reg vcon[] = {
    { "execsql",   lua_vsql_execsql },
    { "close",   lua_vsql_close },
    { NULL, NULL },
  };
  luaL_newmetatable(L, VCON);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, vcon, 0);
  luaL_newlibtable(L,vsql);
  luaL_setfuncs(L,vsql,0);
  return 1;
}



