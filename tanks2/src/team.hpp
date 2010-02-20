#ifndef team_H
#define team_H

#include <cstring>
#include <cstdlib>
#include <list>


typedef struct
{
	int x;
	int y;
} Flag;


class Tank;
class Game;

class Team
{

	private :
		int score;
		int identifier;
		Flag flag;
		Game * game;
		char * source_filename;
		std::list <Tank *> tanks;
		
	public :
		Team (Game * game, char * source_filename, int identifier);
		~Team ();
		
		void turn ();
		void process_action (int action_type);
		void place_flag_randomly ();
		
		void add_tank (int x, int y);
		std::list <Tank *> get_tanks ();
		
		int get_score ();
		int get_flag_x ();
		int get_flag_y ();
		int get_identifier ();
		void set_score (int score);
		void set_flag_x (int x);
		void set_flag_y (int y);
		void increment_score ();

};

#endif
