#include "game.hpp"
#include "tank.hpp"


Game :: Game ()
{
	
	this->map = new Map(GAME_WIDTH, GAME_HEIGHT);
	this->map->randomize(NONPASSABLE_FREQUENCY);
	
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
	std::list <Tank *> :: iterator tank_i;
		
	delete this->map;
	delete this->screen_buffer;
	
	for (i = 0; i < PASSABLE_TILES_N; i++)
		delete this->passable_tiles[i];
		
	for (i = 0; i < NONPASSABLE_TILES_N; i++)
		delete this->nonpassable_tiles[i];
		
	for (i = 0; i < TANK_TILES_N; i++)
		delete this->tank_tiles[i];
		
	for (tank_i = this->tanks.begin(); tank_i != this->tanks.end(); tank_i++)
	{
		delete *tank_i;
	}
		
}
	
	



void Game :: entry ()
{
	
	std::list <Tank *> :: iterator tank_i;
	
	char key = 0;
	
	while (key != 27)
	{
		for (tank_i = this->tanks.begin(); tank_i != this->tanks.end(); tank_i++)
			(*tank_i)->turn();
		
		this->draw_screen_buffer();
		acquire_screen();
		blit(this->screen_buffer->get_bitmap(), screen, 0, 0, 0, 0, this->screen_buffer->get_width(), this->screen_buffer->get_height());
		release_screen();
		
		key = (char) readkey();
		//rest(250);
	}
	
	debug("entry");
	
}



void Game :: draw_screen_buffer ()
{
	int x;
	int y;
	int random_tile;
	
	std::list <Tank *> :: iterator tank_i;
	int tank_tile_i = 0;
	
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
	
	for (tank_i = this->tanks.begin(); tank_i != this->tanks.end(); tank_i++)
	{
		this->screen_buffer->blitter(this->tank_tiles[tank_tile_i],
		                             (*tank_i)->get_x() * TILE_WIDTH,
		                             (*tank_i)->get_y() * TILE_HEIGHT,
		                             true);
		if (++tank_tile_i > TANK_TILES_N)
			tank_tile_i = 0;
	}
}



bool Game :: add_tank (char * source_filename)
{
	int x;
	int y;
	
	Tank * t;
	
	x = rand() % GAME_WIDTH;
	y = rand() % GAME_HEIGHT;
	
	while (this->location_free(x, y) != true)
	{
		x = rand() % GAME_WIDTH;
		y = rand() % GAME_HEIGHT;
	}
	
	t = new Tank(this, source_filename);
	t->set_x(x);
	t->set_y(y);
	
	this->tanks.push_back(t);
	return true;
	
}
	
	

bool Game :: location_free (int x, int y)
{
	
	std::list <Tank *> :: iterator i;
	
	if ((x < 0) || (x >= GAME_WIDTH))
		return false;
	if ((y < 0) || (y >= GAME_HEIGHT))
		return false;
	
	if (this->map->get_location(x, y) != MAP_PASSABLE)
		return false;
		
	for (i = this->tanks.begin(); i != this->tanks.end(); i++)
	{
		if (((*i)->get_x() == x) && ((*i)->get_y() == y))
			return false;
	}
	
	return true;
	
}



std::list <Tank *> Game :: get_tanks ()
{
	return this->tanks;
}
