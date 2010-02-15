#ifndef game_H
#define game_H

#include "map.hpp"
#include "surface.hpp"

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
		Surface * passable_tiles[3];
		Surface * nonpassable_tiles[1];
		Surface * tank_tiles[4];

	public :
		Game ();
		~Game ();
		void entry();
		void draw_screen_buffer ();

};
#endif 
