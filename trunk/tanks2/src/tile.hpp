#ifndef tile_H
#define tile_H

#include <allegro.h>

class Surface
{

	private :
		int width;
		int height;
		BITMAP * bitmap;
	
	public :
		Surface(char * filename);
		Surface(int width, int height);
		~Surface();
		BITMAP * get_bitmap();
		
};

#endif
