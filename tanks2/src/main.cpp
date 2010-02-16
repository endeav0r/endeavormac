#include <allegro.h>

#include <cstdio>

#include "game.hpp"
#include "exception.hpp"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

int main ()
{

	allegro_init();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
	set_palette(desktop_palette);
	install_keyboard();
	
	try
	{
		Game game;
		game.add_tank("tanks/square.lua");
		game.add_tank("tanks/square.lua");
		game.entry();
	}
	catch (Exception e)
	{
		(e).display();
	}
	
	debug("final");
	
	return 1;

}

END_OF_MAIN()
