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
	
	// for flood fill
	char * data;
	
	
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
	
	/*
	 * flood fill to check if this map is valid
	 */
	
	data = (char *) malloc (this->width * this->height);
	memset(data, 0, this->width * this->height);
	
	// we need to find a starting place
	for (x = 0; x < this->width; x++)
	{
		if (this->locations[x] == MAP_PASSABLE)
			break;
	}
	
	if (x == this->width)
		this->randomize(nonpassable_frequency);
	else
	{
		this->flood_fill(x, 0, data);
		for (x = 0; x < this->width * this->height; x++)
		{
			if ((this->locations[x] == MAP_PASSABLE) && (data[x] != 1))
			{
				this->randomize(nonpassable_frequency);
				break;
			}
		}
	}
	
	free(data);
	
}



void Map :: flood_fill (int x, int y, char * data)
{
	if (data[y * this->width + x] == 1)
		return;
	
	if (this->locations[y * this->width + x] == MAP_PASSABLE)
		data[y * this->width + x] = 1;
	
	if ((x > 0) && (this->locations[y * this->width + x - 1] == MAP_PASSABLE))
		this->flood_fill(x - 1, y, data);
	if ((x < this->width - 1) && (this->locations[y * this->width + x + 1] == MAP_PASSABLE))
		this->flood_fill(x + 1, y, data);
	if ((y > 0) && (this->locations[(y-1) * this->width + x] == MAP_PASSABLE))
		this->flood_fill(x, y - 1, data);
	if ((y < this->height - 1) && (this->locations[(y+1) * this->width + x] == MAP_PASSABLE))
		this->flood_fill(x, y + 1, data);
}



int Map :: get_location (int x, int y)
{
	return this->locations[(y * this->width) + x];
}
