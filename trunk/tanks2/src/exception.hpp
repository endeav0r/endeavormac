#ifndef exception_H
#define exception_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

#define MALLOC_FAIL 1

#define MAP_BAD_SIZE 1001

#define SURFACE_FILE_LOAD_ERROR 2001

#define LUA_P_CALL_ERROR 3001
#define LUA_SYNTAX_ERROR 3002
#define LUA_UNKNOWN_ERROR 3003

class Exception
{

	private :
		int code;
		char * description;
		
	
	public :
		Exception (int code, char * description);
		~Exception();
		void display ();

};

void debug (char * s);

#endif
