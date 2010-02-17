#include "game.hpp"


Game :: Game ()
{
	
	srand((unsigned int) time(NULL));
	
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
	std::list <Team *> :: iterator team_i;
		
	delete this->map;
	delete this->screen_buffer;
	
	for (i = 0; i < PASSABLE_TILES_N; i++)
		delete this->passable_tiles[i];
		
	for (i = 0; i < NONPASSABLE_TILES_N; i++)
		delete this->nonpassable_tiles[i];
		
	for (i = 0; i < TANK_TILES_N; i++)
		delete this->tank_tiles[i];
		
	for (team_i = this->teams.begin(); team_i != this->teams.end(); team_i++)
	{
		delete *team_i;
	}
		
}
	
	



void Game :: entry ()
{
	
	std::list <Team *> :: iterator team_i;
	
	char key = 0;
	
	while (key != 27)
	{
		


		this->draw_screen_buffer();
		acquire_screen();
		blit(this->screen_buffer->get_bitmap(), screen, 0, 0, 0, 0, this->screen_buffer->get_width(), this->screen_buffer->get_height());
		release_screen();

		for (team_i = this->teams.begin(); team_i != this->teams.end(); team_i++)
			(*team_i)->turn();
					
		key = (char) readkey();
		
		//rest(250);
	}
	
}



void Game :: draw_screen_buffer ()
{
	int x;
	int y;
	int random_tile;
	
	
	std::list <Tank *> :: iterator tank_i;
	std::list <Tank *> tanks;
	std::list <Team *> :: iterator team_i;
	int team_tile_i = 0; // tile for a team
	
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
	
	// move teams
	for (team_i = this->teams.begin(); team_i != this->teams.end(); team_i++)
	{
		(*team_i)->process_action(TANK_ACTION_MOVE);
	}
	
	// draw tanks
	for (team_i = this->teams.begin(); team_i != this->teams.end(); team_i++)
	{
		tanks = (*team_i)->get_tanks();
		for (tank_i = tanks.begin(); tank_i != tanks.end(); tank_i++)
		{
			this->screen_buffer->blitter(this->tank_tiles[team_tile_i],
										 (*tank_i)->get_x() * TILE_WIDTH,
										 (*tank_i)->get_y() * TILE_HEIGHT,
										 true,
										 (*tank_i)->get_orientation());
		}
		if (++team_tile_i > TANK_TILES_N)
			team_tile_i = 0;
	}
}



void Game :: add_team (char * source_filename, int tanks_n)
{
	
	Team * team;
	
	int x;
	int y;
	
	int tank_i;
	
	team = new Team(this, source_filename);
	
	this->teams.push_back(team);
	
	for (tank_i = 0; tank_i < tanks_n; tank_i++)
	{
		x = rand() % GAME_WIDTH;
		y = rand() % GAME_HEIGHT;
		while (this->location_free(x, y) == false)
		{
			x = rand() % GAME_WIDTH;
			y = rand() % GAME_HEIGHT;
		}
		
		team->add_tank(x, y);
	}
	
	
}
		

/*
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
*/
	

bool Game :: location_free (int x, int y)
{
	
	std::list <Tank *> tanks;
	std::list <Team *> :: iterator team_i;
	std::list <Tank *> :: iterator tank_i;
	
	if ((x < 0) || (x >= GAME_WIDTH))
		return false;
	if ((y < 0) || (y >= GAME_HEIGHT))
		return false;
	
	if (this->map->get_location(x, y) != MAP_PASSABLE)
		return false;
	
	for (team_i = this->teams.begin(); team_i != this->teams.end(); team_i++)
	{
		tanks = (*team_i)->get_tanks();
		for(tank_i = tanks.begin(); tank_i != tanks.end(); tank_i++)
		{
			if (((*tank_i)->get_x() == x) && ((*tank_i)->get_y() == y))
				return false;
		}
	}
	return true;
	
}



std::list <Team *> Game :: get_teams ()
{
	return this->teams;
}
