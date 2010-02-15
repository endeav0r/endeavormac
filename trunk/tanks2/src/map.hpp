#ifndef map_H
#define map_H

#include <cstdlib>

#include "exception.hpp"

#define MAP_POSSIBILITIES 2
#define MAP_PASSABLE 0
#define MAP_NONPASSABLE 1

class Map
{

	private :
		int width;
		int height;
		int * locations;
	
	public :
		Map (int width, int height);
		~Map();
		void randomize (double nonpassable_frequency);
		int get_location (int x, int y);
};
		
#endif
