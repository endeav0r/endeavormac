#include "tank.hpp"
#include "game.hpp"
#include "team.hpp"


Tank :: Tank (Game * game, Team * team, char * lua_source)
{
	this->x = 0;
	this->y = 0;
	
	int error;
	char * lua_syntax_error_description;
	
	this->team = team;
	this->game = game;
	
	this->action.type = TANK_ACTION_NONE;
	this->action.x = 0;
	this->action.y = 0;
	
	this->orientation = TANK_ORIENTATION_UP;
	
	// this code sets up and loads our lua file
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
	{
		lua_syntax_error_description = (char *) lua_tostring(this->l_vm, 0);
		throw Exception(LUA_SYNTAX_ERROR, lua_syntax_error_description);
		lua_settop(this->l_vm, -1);
	}
	else
		throw Exception(LUA_UNKNOWN_ERROR, "Unknown lua error");
	
	// register lua functions for user
	this->register_lua_functions();
		
}



void Tank :: register_lua_functions ()
{
	this->register_lua_function("move", this->l_move);
	this->register_lua_function("location_free", this->l_location_free);
	this->register_lua_function("getx", this->l_getx);
	this->register_lua_function("gety", this->l_gety);
	this->register_lua_function("get_game_width", this->l_getx);
	this->register_lua_function("get_game_height", this->l_gety);
	this->register_lua_function("get_enemy_locations", this->l_get_enemy_locations);
}



void Tank :: register_lua_function (char * lua_name, int (*f) (lua_State *))
{
	lua_pushlightuserdata(this->l_vm, this);
	lua_pushcclosure(this->l_vm, f, 1);
	lua_setglobal(this->l_vm, lua_name);
}



Tank :: ~Tank ()
{
	lua_close(this->l_vm);	
}



void Tank :: turn ()
{	
	lua_getfield(this->l_vm, LUA_GLOBALSINDEX, "turn");
	this->l_pcall(this->l_vm, 0, 0);
}



void Tank :: process_action (int action_type)
{
	if (action_type == this->action.type)
	{
		this->action.type = TANK_ACTION_NONE;
		
		if (this->action.x < this->x)
			this->orientation = TANK_ORIENTATION_LEFT;
		else if (this->action.x > this->x)
			this->orientation = TANK_ORIENTATION_RIGHT;
		else if (this->action.y < this->y)
			this->orientation = TANK_ORIENTATION_UP;
		else if (this->action.y > this->y)
			this->orientation = TANK_ORIENTATION_DOWN;
		this->x = this->action.x;
		this->y = this->action.y;
	}
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



int Tank :: get_orientation ()
{
	return this->orientation;
}



Tank_Action * Tank :: get_action()
{
	return &(this->action);
}



Tank * Tank :: get_this ()
{
	return this;
}



/********************************
* LUA FUNCTIONS
********************************/

void Tank :: l_pcall (lua_State * l, int args_n, int returns_n)
{
	
	int error;
	char * lua_error_description;
	
	error = lua_pcall(l_vm, args_n, returns_n, 0);
	
	if (error != 0)
	{
		lua_error_description = (char *) lua_tostring(this->l_vm, -1);
		throw Exception(LUA_SYNTAX_ERROR, lua_error_description);
		lua_settop(this->l_vm, -1);
	}
	
}



int Tank :: l_move (lua_State * l)
{
	
	int arguments_n;
	int x;
	int y;
	
	Tank * this_tank;
	
	arguments_n = lua_gettop(l);
	
	if (arguments_n != 2)
	{
		lua_pushstring(l, "Incorrect number of arguments for move");
		lua_error(l);
	}
	
	this_tank = (Tank *) lua_topointer(l, lua_upvalueindex(1));
	
	x = lua_tonumber(l, 1);
	y = lua_tonumber(l, 2);
	
	x %= 2;
	y %= 2;
	if ((x != 0) && (y != 0))
	{
		lua_pushstring(l, "Only x or y can be not equal to zero, not both");
		lua_error(l);
	}
		
	if (this_tank->game->location_free(this_tank->x + x, this_tank->y + y))
	{
		this_tank->action.type = TANK_ACTION_MOVE;
		this_tank->action.x = this_tank->x + x;
		this_tank->action.y = this_tank->y + y;
		lua_pushboolean(l, 1);
	}
	else
		lua_pushboolean(l, 0);
	
	return 1;

}



int Tank :: l_location_free (lua_State * l)
{
	
	int arguments_n;
	int x;
	int y;
	
	Tank * this_tank;
	
	arguments_n = lua_gettop(l);
	
	if (arguments_n != 2)
	{
		lua_pushstring(l, "Incorrect number of arguments for move");
		lua_error(l);
	}
	
	this_tank = (Tank *) lua_topointer(l, lua_upvalueindex(1));
	
	x = lua_tonumber(l, 1);
	y = lua_tonumber(l, 2);
	
	if (this_tank->game->location_free(x, y) == true)
		lua_pushboolean(l, 1);
	else
		lua_pushboolean(l, 0);
	
	return 1;

}



int Tank :: l_getx (lua_State * l)
{
	
	Tank * this_tank;
	
	this_tank = (Tank *) lua_topointer(l, lua_upvalueindex(1));
	
	lua_pushinteger(l, this_tank->get_x());
	
	return 1;

}



int Tank :: l_gety (lua_State * l)
{
	
	Tank * this_tank;
	
	this_tank = (Tank *) lua_topointer(l, lua_upvalueindex(1));
	
	lua_pushinteger(l, this_tank->get_y());
	
	return 1;

}



int Tank :: l_get_game_width (lua_State * l)
{
	
	Tank * this_tank;
	
	this_tank = (Tank *) lua_topointer(l, lua_upvalueindex(1));
	
	lua_pushinteger(l, GAME_WIDTH);
	
	return 1;

}



int Tank :: l_get_game_height (lua_State * l)
{
	
	Tank * this_tank;
	
	this_tank = (Tank *) lua_topointer(l, lua_upvalueindex(1));
	
	lua_pushinteger(l, GAME_HEIGHT);
	
	return 1;

}



int Tank :: l_get_enemy_locations (lua_State * l)
{
	int table_index = 0;
	Tank * this_tank;
	std::list <Team *> teams;
	std::list <Team *> :: iterator team_i;
	std::list <Tank *> tanks;
	std::list <Tank *> :: iterator tank_i;
	
	this_tank = (Tank *) lua_topointer(l, lua_upvalueindex(1));
	
	teams = this_tank->game->get_teams();
	
	// create new table (referred to as enemy_locations in these comments)
	lua_newtable(l);
	
	for (team_i = teams.begin(); team_i != teams.end(); team_i++)
	{
		if (*team_i == this_tank->team)
			continue;
			
		tanks = (*team_i)->get_tanks();
	
		for (tank_i = tanks.begin(); tank_i != tanks.end(); tank_i++)
		{
			if (*tank_i != this_tank)
			{
				// top, bottom (these comments keep track of what's on the stack)
				// enemy_locations
				lua_pushinteger(l, table_index++); // table_index, enemy_locations
				lua_newtable(l); // new_table_row, table_index, enemy_locations
				lua_pushinteger(l, 0); // 0, new_table_row, table_index, enemy_locations
				lua_pushinteger(l, (*tank_i)->get_x()); // x, 0, new_table_row, table_index, enemy_locations
				lua_settable(l, -3); // new_table_row, table_index, enemy_locations
				lua_pushinteger(l, 1);
				lua_pushinteger(l, (*tank_i)->get_y());
				lua_settable(l, -3); // new_table_row, table_index, enemy_locations
				lua_settable(l, -3); // enemy_locations
			}
		}
	}
	
	return 1;

}




