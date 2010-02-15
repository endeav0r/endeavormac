#include "game.hpp"



Game :: Game ()
{
	
	this->map = new Map(GAME_WIDTH, GAME_HEIGHT);
	this->screen_buffer = new Surface(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	this->passable_tiles[0] = new Surface("images/passable_1.bmp");
	this->passable_tiles[1] = new Surface("images/passable_2.bmp");
	this->passable_tiles[2] = new Surface("images/passable_3.bmp");
	
	this->nonpassable_tiles[0] = new Surface("images/nonpassable_1.bmp");
	
	this->tank_tiles[0] = new Surface("images/tank_blue.bmp");
	this->tank_tiles[1] = new Surface("images/tank_red.bmp");
	this->tank_tiles[2] = new Surface("images/tank_green.bmp");
	this->tank_tiles[3] = new Surface("images/tank_gray.bmp");
	
}



Game :: ~Game ()
{
	
	int i;
		
	delete this->map;
	delete this->screen_buffer;
	
	for (i = 0; i < PASSABLE_TILES_N; i++)
		delete this->passable_tiles[i];
		
	for (i = 0; i < NONPASSABLE_TILES_N; i++)
		delete this->nonpassable_tiles[i];
		
	for (i = 0; i < TANK_TILES_N; i++)
		delete this->tank_tiles[i];
		
}
	
	



void Game :: entry ()
{
	
	char key = 0;
	
	this->map->randomize(NONPASSABLE_FREQUENCY);
	
	while (key != 27)
	{
		this->draw_screen_buffer();
		acquire_screen();
		blit(this->screen_buffer->get_bitmap(), screen, 0, 0, 0, 0, this->screen_buffer->get_width(), this->screen_buffer->get_height());
		release_screen();
		key = (char) readkey();
		rest(250);
	}
	
	debug("entry");
	
}



void Game :: draw_screen_buffer ()
{
	int x;
	int y;
	int random_tile;
	
	for (x = 0; x < GAME_WIDTH; x++)
	{
		for (y = 0; y < GAME_HEIGHT; y++)
		{
			switch (this->map->get_location(x, y))
			{
				case MAP_PASSABLE :
					random_tile = rand() % PASSABLE_TILES_N;
					this->screen_buffer->blitter(this->passable_tiles[random_tile], x * TILE_WIDTH, y * TILE_HEIGHT);
					break;
				case MAP_NONPASSABLE :
					random_tile = rand() % NONPASSABLE_TILES_N;
					this->screen_buffer->blitter(this->nonpassable_tiles[random_tile], x * TILE_WIDTH, y * TILE_HEIGHT);
					break;
			}
		}
	}	
} 
	
	
