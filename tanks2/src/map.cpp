#include "map.hpp"

Map :: Map (int width, int height)
{
	
	this->width = width;
	this->height = height;
	this->locations = NULL;
	this->locations = (int *) malloc(sizeof(int) * width * height);
	if (this->locations == NULL)
		throw Exception(MALLOC_FAIL, "Failed allocating memory for Map->locations");
	
}



Map :: ~Map ()
{
	debug("~map");
	//if (this->locations != NULL)
		//free(this->locations);
}



void Map :: randomize (double nonpassable_frequency)
{
	
	int x;
	int y;
	int row;
	int frequency;
	int rand_value;
	
	
	if (this->width == 0)
		throw Exception(MAP_BAD_SIZE, "Map->width == 0");
	if (this->height == 0)
		throw Exception(MAP_BAD_SIZE, "Map->height == 0");
	
	frequency = ((int) (nonpassable_frequency * 100)) % 100;
	//frequency = 15;
	
	//srand((int) clock());
	
	for (x = 0; x < this->width; x++)
	{
		row = x * this->width;
		for (y = 0; y < this->height; y++)
		{
			rand_value = (int) (rand() / 10);
			rand_value %= 100;
			if (rand_value <= frequency)
			{
				this->locations[row + y] = MAP_NONPASSABLE;
			}
			else
			{
				this->locations[row + y] = MAP_PASSABLE;
			}
		}
	}
	
}



int Map :: get_location (int x, int y)
{
	return this->locations[(x * this->width) + y];
}
