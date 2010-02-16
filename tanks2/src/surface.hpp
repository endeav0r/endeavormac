#ifndef tile_H
#define tile_H

#include <allegro.h>


#include "exception.hpp"


#define SURFACE_DEBUG 2


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
		
		void acquire();
		void release();
		
		BITMAP * get_bitmap();
		int get_width();
		int get_height();
		
		int blitter(Surface * s, int x, int y, bool masked=false);
		
};

#endif
