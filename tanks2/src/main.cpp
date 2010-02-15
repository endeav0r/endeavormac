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
	
	Game game;
	game.entry();
	
	debug("final");
	
	return 1;

}

END_OF_MAIN()
