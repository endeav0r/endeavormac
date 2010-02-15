#ifndef tank_H
#define tank_H

#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>

#include "exception.hpp"

class Tank
{
	private :
		lua_State * l_vm;
		int x;
		int y;
	
	public :
		Tank (char * lua_source);
		~Tank();
		void set_x (int x);
		void set_y (int y);
		int get_x();
		int get_y();
};

#endif
