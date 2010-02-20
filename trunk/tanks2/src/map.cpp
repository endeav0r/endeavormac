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
	if (this->locations != NULL)
		free(this->locations);
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
	
	for (y = 0; y < this->height; y++)
	{
		row = y * this->width;
		for (x = 0; x < this->width; x++)
		{
			rand_value = (int) (rand() / 10);
			rand_value %= 100;
			if (rand_value <= frequency)
			{
				this->locations[row + x] = MAP_NONPASSABLE;
			}
			else
			{
				this->locations[row + x] = MAP_PASSABLE;
			}
		}
	}
	
	
}



int Map :: get_location (int x, int y)
{
	return this->locations[(y * this->width) + x];
}
