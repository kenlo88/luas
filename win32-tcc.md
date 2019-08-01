# Compiling luas with tcc under windows

## Why would I need to do this?
1. Without amdin privilege, msvc compiler cannot be installed.
1. gcc is an overkill and it links to unncessary dll files.

## How to compile using tcc?
1. Download tcc and expand it. No installation is required.
1. Get libmysql_.dll (I got it from gcc win32).
1. Create def files
      tiny_impdef.exe c:/Windows/System32/ws2_32.dll
      tiny_impdef.exe libmysql_.dll
1. Modify Makefile
      CC= d:/soft/tcc32/tcc
      MYCFLAGS= -D__LCC__
      MYLDFLAGS= path_to/libmysql_.def path_to/ws2_32.def
1. tcc does not come with winsock2.h and other windows api headers. You can get a minimum winsock2.h through google.  A version is included here.
1. It should starts you going now. Have fun.
