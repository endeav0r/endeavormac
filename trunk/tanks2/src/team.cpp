#include "team.hpp"
#include "tank.hpp"
#include "game.hpp"

Team :: Team (Game * game, char * source_filename, int identifier)
{
	this->score = 0;
	this->flag.x = 0;
	this->flag.y = 0;
	this->game = game;
	this->identifier = identifier;
	
	this->source_filename = (char *) malloc(strlen(source_filename) + 1);
	memset(this->source_filename, 0, strlen(source_filename) + 1);
	if (this->source_filename == NULL)
		throw Exception(MALLOC_FAIL, "Failed allocating memory for team source_filename");
	strcpy(this->source_filename, source_filename);
}



Team :: ~Team ()
{
	std::list <Tank *> :: iterator tank_i;
	for (tank_i = this->tanks.begin(); tank_i != this->tanks.end(); tank_i++)
	{
		delete *tank_i;
	}
}



void Team :: turn ()
{
	
	std::list <Tank *> :: iterator tank_i;
	
	for (tank_i = this->tanks.begin(); tank_i != this->tanks.end(); tank_i++)
	{
		(*tank_i)->turn();
	}

}



void Team :: process_action (int action_type)
{
	
	std::list <Tank *> :: iterator tank_i;
	
	for (tank_i = this->tanks.begin(); tank_i != this->tanks.end(); tank_i++)
	{
		(*tank_i)->process_action(action_type);
	}
	
}



void Team :: place_flag_randomly ()
{
	int x;
	int y;
	
	x = rand() % GAME_WIDTH;
	y = rand() % GAME_HEIGHT;
	
	while (this->game->location_free(x, y, true) == false)
	{
		x = rand() % GAME_WIDTH;
		y = rand() % GAME_HEIGHT;
	}
	
	this->flag.x = x;
	this->flag.y = y;
}



void Team :: add_tank (int x, int y)
{
	int new_identifier;
	
	Tank * tank;
	
	if (this->tanks.size() == 0)
		new_identifier = 0;
	else
	{
		new_identifier = (this->tanks.back())->get_identifier();
		new_identifier++;
	}	
	
	tank = new Tank(this->game, this, this->source_filename, new_identifier);
	
	tank->set_x(x);
	tank->set_y(y);
	
	this->tanks.push_back(tank);
}



std::list <Tank *> Team :: get_tanks()
{
	return this->tanks;
}



int Team :: get_score()
{
	return this->score;
}



int Team :: get_flag_x()
{
	return this->flag.x;
}



int Team :: get_flag_y()
{
	return this->flag.y;
}



int Team :: get_identifier ()
{
	return this->identifier;
}



void Team :: set_score(int score)
{
	this->score = score;
}



void Team :: set_flag_x(int x)
{
	this->flag.x = x;
}



void Team :: set_flag_y(int y)
{
	this->flag.y = y;
}



void Team :: increment_score()
{
	this->score++;
}
