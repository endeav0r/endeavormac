#include "tank.hpp"



Tank :: Tank (char * lua_source)
{
	int error;
	
	this->l_vm = luaL_newstate();
	luaL_openlibs(this->l_vm);
	
	error = luaL_loadfile(this->l_vm, lua_source);
	
	if (error == 0)
	{
		error = lua_pcall(l_vm, 0, 0, 0);
		if (error != 0)
			throw Exception(LUA_P_CALL_ERROR, "lua_p_call returned an error");
	}
	else if (error == LUA_ERRSYNTAX)
		throw Exception(LUA_SYNTAX_ERROR, "Error in lua syntax");
	else
		throw Exception(LUA_UNKNOWN_ERROR, "Unknown lua error");
}



Tank :: ~Tank ()
{
	lua_close(this->l_vm);	
}



void Tank :: set_x (int x)
{
	this->x = x;
}



void Tank :: set_y (int y)
{
	this->y = y;
}



int Tank :: get_x ()
{
	return this->x;
}



int Tank :: get_y ()
{
	return this->y;
}
