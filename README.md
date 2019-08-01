# luas
a handy cgi tool built especially for the fmd-lunch project

## what is it for?
MySQL is added to [lua](http://ww.lua.org) version 5.2.3.  As lua changes its
API from 5.1 to 5.2 and from 5.2 to 5.3, slight amendment to the code is
required to makde it works in other versions.

Quick and easy methods are added to access mySQL. Example:

```lua
conn=vsql:connect('mySQL_server','user','password','database_name',3306)
sql = 'SELECT CURDATE(), CURRENT_TIMESTAMP'
r = conn:execsql(sql)
print(r.nrow)    // should print 1, number of rows in result set
print(r.ncol)    // should print 2, number of columns in result set
print(r[1][1])   // should print 2019-08-01 (or today)
print(r[1][2])   // should print 2019-08-01 13:00:32 (or timestamp now)
```

## There are many good mySQL pacakges.  Why another?
I just need a few methods.  The good packages are overkills.  Furthermore,
this package can be compiled with [tcc](https://bellard.org/tcc/) under 
windows.  See [win32-tcc.md](win32-tcc.md) for details.

## Install
Copy vsql.c linit.c lualib.h Makefile to the src directory of lua-5.2.3.  In
the src directory, type

```bash
make
```

## Changes to lua core files
Only minor changes.

```bash
$ diff deps/lua-5.2.3/src/linit.c ./linit.c
39a40
>   {LUA_VSQLLIBNAME, luaopen_vsql},

$ diff deps/lua-5.2.3/src/lualib.h ./lualib.h
43a44,46
> #define LUA_VSQLLIBNAME "vsql"
> LUAMOD_API int (luaopen_vsql) (lua_State *L);
>

$ diff deps/lua-5.2.3/src/Makefile ./Makefile
24c24
< MYLIBS=
---
> MYLIBS= -lmysql
36c36
<       lmathlib.o loslib.o lstrlib.o ltablib.o loadlib.o linit.o
---
>       lmathlib.o loslib.o lstrlib.o ltablib.o loadlib.o linit.o vsql.o
39c39
< LUA_T=        lua
---
> LUA_T=        luas
42c42
< LUAC_T=       luac
---
> LUAC_T=       luasc
186a187
> vsql.o: vsql.c lua.h lualib.h linit.c
```
