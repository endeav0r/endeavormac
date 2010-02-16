#ifndef tank_H
#define tank_H

#include <list>

extern "C" {
#include <lua5.1/lua.h>
#include <lua5.1/lauxlib.h>
#include <lua5.1/lualib.h>
}

#include "exception.hpp"

#define TANK_ACTION_NONE 0
#define TANK_ACTION_MOVE 1

#define TANK_ORIENTATION_UP 0
#define TANK_ORIENTATION_LEFT 192
#define TANK_ORIENTATION_DOWN 128
#define TANK_ORIENTATION_RIGHT 64

typedef struct
{
	int type;
	int x;
	int y;
} tank_action;

class Game;

class Tank
{
	private :
		int x;
		int y;
		int orientation;
		lua_State * l_vm;
		Game * game;
		tank_action action;
	
	public :
		Tank (Game * gamestate, char * lua_source);
		~Tank();
		
		void turn ();
		void process_action (int action_type);
		
		void set_x (int x);
		void set_y (int y);
		int get_x();
		int get_y();
		int get_orientation();
		Tank * get_this(); // never thought i'd need this... get it
		
		
		void register_lua_functions ();
		void register_lua_function (char * lua_name, int (*f) (lua_State *));
		void l_pcall (lua_State * l, int args_n, int returns_n);
		
		static int l_move (lua_State * l);
		static int l_location_free (lua_State * l);
		static int l_getx (lua_State *l);
		static int l_gety (lua_State *l);
		static int l_get_game_width (lua_State *l);
		static int l_get_game_height (lua_State *l);
		static int l_get_enemy_locations (lua_State *l);
};

#endif
