#include "team.hpp"
#include "tank.hpp"
#include "game.hpp"

Team :: Team (Game * game, char * source_filename)
{
	this->score = 0;
	this->flag.x = 0;
	this->flag.y = 0;
	this->game = game;
	
	this->source_filename = (char *) malloc(strlen(source_filename));
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



void Team :: add_tank (int x, int y)
{
	Tank * tank;
	
	tank = new Tank(this->game, this, this->source_filename);
	
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
