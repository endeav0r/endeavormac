#ifndef game_H
#define game_H

#include <list>
#include <ctime>

#include "map.hpp"
#include "surface.hpp"
#include "team.hpp"
#include "tank.hpp"

#define GAME_WIDTH 20
#define GAME_HEIGHT 15

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define NONPASSABLE_FREQUENCY 0.15

#define PASSABLE_TILES_N 3
#define NONPASSABLE_TILES_N 1
#define TANK_TILES_N 4

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

class Game
{

	private :
	
		Map * map;
		Surface * screen_buffer;
		
		// oh no, hard coded object pointers, wah wah cry me a river. man up
		Surface * passable_tiles[PASSABLE_TILES_N];
		Surface * nonpassable_tiles[NONPASSABLE_TILES_N];
		Surface * tank_tiles[TANK_TILES_N];
		
		std::list <Team *> teams;

	public :
		Game ();
		~Game ();
		void entry();
		void draw_screen_buffer ();
		
		void add_team(char * source_filename, int tanks_n);
		
		bool location_free(int x, int y);
		
		std::list <Team *> get_teams ();

};
#endif 
