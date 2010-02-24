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
	
	this->flag_tiles[0] = new Surface("images/flag_blue.bmp");
	this->flag_tiles[1] = new Surface("images/flag_red.bmp");
	this->flag_tiles[2] = new Surface("images/flag_green.bmp");
	this->flag_tiles[3] = new Surface("images/flag_gray.bmp");
	
	this->turret_tile = new Surface("images/turret.bmp");
	
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
		
	for (i = 0; i < FLAG_TILES_N; i++)
		delete this->flag_tiles[i];
	
	delete this->turret_tile;
		
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

		// team turns (run lua scripts)
		for (team_i = this->teams.begin(); team_i != this->teams.end(); team_i++)
			(*team_i)->turn();
	
		// move teams
		for (team_i = this->teams.begin(); team_i != this->teams.end(); team_i++)
			(*team_i)->process_action(TANK_ACTION_MOVE);
		
		// check for flag captures
		this->check_for_flag_captures();
		
		rest((unsigned int) 200);
		
		if (keypressed())
			key = (char) readkey();
		
	}
	
}



void Game :: check_for_flag_captures ()
{
	std::list <Team *> :: iterator team_i;
	std::list <Team *> :: iterator flag_team_i;
	
	std::list <Tank *> tanks;
	std::list <Tank *> :: iterator tank_i;
	
	for (team_i = this->teams.begin(); team_i != this->teams.end(); team_i++)
	{
		tanks = (*team_i)->get_tanks();
		for (tank_i = tanks.begin(); tank_i != tanks.end(); tank_i++)
		{
			for (flag_team_i = this->teams.begin(); flag_team_i != this->teams.end(); flag_team_i++)
			{
				if ((*flag_team_i) != (*team_i))
				{
					if (((*flag_team_i)->get_flag_x() == (*tank_i)->get_x())
					    && ((*flag_team_i)->get_flag_y() == (*tank_i)->get_y()))
					{
						(*team_i)->increment_score();
						(*flag_team_i)->place_flag_randomly();
					}
				}
			}
		}
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
	
	
	// draw map tiles
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
	
	// draw teams
	for (team_i = this->teams.begin(); team_i != this->teams.end(); team_i++)
	{
		// draw tanks
		tanks = (*team_i)->get_tanks();
		for (tank_i = tanks.begin(); tank_i != tanks.end(); tank_i++)
		{
			this->screen_buffer->blitter(this->tank_tiles[team_tile_i],
										 (*tank_i)->get_x() * TILE_WIDTH,
										 (*tank_i)->get_y() * TILE_HEIGHT,
										 true,
										 (*tank_i)->get_orientation());
			this->screen_buffer->blitter(this->turret_tile,
										 (*tank_i)->get_x() * TILE_WIDTH,
										 (*tank_i)->get_y() * TILE_HEIGHT,
										 true,
										 (*tank_i)->get_orientation());
										 							 
		}
		// draw flags
		this->screen_buffer->blitter(this->flag_tiles[team_tile_i],
		                             (*team_i)->get_flag_x() * TILE_WIDTH,
		                             (*team_i)->get_flag_y() * TILE_HEIGHT,
		                             true);
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
	
	team = new Team(this, source_filename, this->teams.size());
	
	this->teams.push_back(team);
	
	for (tank_i = 0; tank_i < tanks_n; tank_i++)
	{
		x = rand() % GAME_WIDTH;
		y = rand() % GAME_HEIGHT;
		while (this->location_free(x, y, true) == false)
		{
			x = rand() % GAME_WIDTH;
			y = rand() % GAME_HEIGHT;
		}
		
		team->add_tank(x, y);
	}
	
	team->place_flag_randomly();
	
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
	

bool Game :: location_free (int x, int y, bool include_flags)
{
	
	std::list <Tank *> tanks;
	std::list <Team *> :: iterator team_i;
	std::list <Tank *> :: iterator tank_i;
	Tank_Action * action;
	
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
			action = (*tank_i)->get_action();
			if (action->type == TANK_ACTION_MOVE)
			{
				if ((action->x == x) && (action->y == y))
					return false;
			}
			else if (((*tank_i)->get_x() == x) && ((*tank_i)->get_y() == y))
				return false;
		}
		if (include_flags == true)
		{
			if (((*team_i)->get_flag_x() == x) && ((*team_i)->get_flag_y() == y))
				return false;
		}
	}
	return true;
	
}



std::list <Team *> Game :: get_teams ()
{
	return this->teams;
}
