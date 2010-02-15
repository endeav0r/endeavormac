#ifndef map_H
#define map_H

#include <cstdlib>

#define MAP_PASSABLE 0
#define MAP_NONPASSABLE 1

class map
{

	private :
		int width;
		int height;
		int * locations;
	
	public :
		map (int width, int height);
		void randomize_map ();
		int get_location (int x, int y);
}
		
#endif
