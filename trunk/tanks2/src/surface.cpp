#include "surface.hpp"



Surface :: Surface (char * filename)
{
	
	#if SURFACE_DEBUG == 2
		char buf[1024];
		snprintf(buf, 1024, "loading surface %s", filename);
		debug(buf);
	#endif
	this->bitmap = NULL;
	this->bitmap = load_bitmap(filename, NULL);
	if (this->bitmap == NULL)
	{
		this->width = 0;
		this->height = 0;
		throw Exception(SURFACE_FILE_LOAD_ERROR, "Error loading image file");
	}
	else
	{
		this->width = (int) this->bitmap->w;
		this->height = (int) this->bitmap->h;
	}
}



Surface :: Surface (int width, int hei

	this->bitmap = create_bitmap(width, height);
	clear_bitmap(this->bitmap);
	if (this->bitmap == NULL)
	{
		throw Exception(0, "Failed to create bitmap");
	}
	else
	{
		this->width = width;
		this->height = height;
	}
}



Surface :: ~Surface ()
{
	if (this->bitmap != NULL)
		destroy_bitmap(this->bitmap);
}



void Surface :: acquire ()
{
	acquire_bitmap(this->bitmap);
}



void Surface :: release ()
{
	release_bitmap(this->bitmap);
}



BITMAP * Surface :: get_bitmap ()
{
	return this->bitmap;
}



int Surface :: get_width ()
{
	return this->width;
}



int Surface :: get_height ()
{
	return this->height;
}



int Surface :: blitter (Surface * s, int x, int y)
{
	
	#if SURFACE_DEBUG == 1
		char buf[1024];
		snprintf(buf, 1024, "blit x %d, y %d, s->width %d, s->height %d\n", x, y, s->width, s->height);
		debug("blitter");
		debug(buf);
	#endif
	
	if (s->bitmap == NULL)
		throw Exception(0, "blitter: s->bitmap == NULL");
	if (this->bitmap == NULL)
		throw Exception(0, "blitter: this->bitmap == NULL");
	
	this->acquire();
	s->acquire();
	blit(s->bitmap, this->bitmap, 0, 0, x, y, s->width, s->height);
	s->release();
	this->release();
	
	return 1;
        
}
