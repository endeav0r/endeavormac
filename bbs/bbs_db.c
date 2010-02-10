#include "bbs_db.h"



bbs_db_row * create_db_row_from_db_line (char * line)
{

	int i;
	int column_size;
	int column_i; // where we are in current column
	int column_n; // how many columns we have filled
	int escape_char = 0;
	char * column;
	char * column_realloc;
	
	bbs_db_row * row;
	
	row = malloc(sizeof(bbs_db_row));
	if (row == NULL)
		return NULL;
		
	
	// count the number of | we have
	escape_char = 0;
	row->columns_n = 1; // we start with 1 because a|b|c has 2 |s, but 3 columns
	for (i = 0; i < strlen(line); i++)
	{
		if (line[i] == '|')
		{
			if (escape_char % 2 == 0)
			{
				row->columns_n++;
			}
		}
		
		if (line[i] == '\\')
			escape_char++;
		else
			escape_char = 0;
	}
	
	row->columns = (char **) malloc(sizeof(char *) * row->columns_n);
	
	// fill columns
	column = malloc(16);
	if (column == NULL)
	{
		free(row->columns);
		free(row);
		return NULL;
	}
	column_size = 16;
	column_i = 0;
	column_n = 0;
	escape_char = 0;
	for (i = 0; i < strlen(line); i++)
	{
		if (line[i] == '|')
		{
			if (escape_char % 2 == 0)
			{
				column[column_i] = 0;
				row->columns[column_n++] = column;
				column_i = 0;
				column = malloc(16);
				column_size = 16;
				continue;
			}
		}
		if (line[i] == '\\')
			escape_char++;
		else
			escape_char = 0;
		
		column[column_i++] = line[i];
		if (column_i == column_size)
		{
			column_realloc = realloc(column, column_size * 2);
			if (column_realloc == NULL)
			{
				for (i = 0; i < column_n; i++)
				{
					free(row->columns[i]);
				}
				free(row->columns);
				free(row);
				return NULL;
			}
			column = column_realloc;
			column_size *= 2;
		}
	}
	
	row->columns[column_n] = column;
	
	return row;
	
}
		
		

void destroy_db (bbs_db * db)
{
	
	int row;
	int column;
	for (row = 0; row < db->rows_n; row++)
	{
		for (column = 0; column < db->rows[row]->columns_n; column++)
		{
			free(db->rows[row]->columns[column]);
		}
		free(db->rows[row]->columns);
		free((db->rows[row]));
	}
	free(db->filename);
	free(db);
	
}



bbs_db * load_db (char * filename)
{

	int bytes_read;
	int i;
	int rows_size = 1;
	int start_of_row;
	char buffer[DB_MAX_ROW_SIZE];
	char * line;
	FILE * fh;
	bbs_db * db;
	bbs_db_row * row;
	bbs_db_row ** rows_realloc;
	
	fh = fopen(filename, "r");
	if (fh == NULL)
		return NULL;
	
	db = malloc(sizeof(bbs_db));
	if (db == NULL)
	{
		return NULL;
		fclose(fh);
	}
		
	db->filename = malloc(strlen(filename));
	if (db->filename == NULL)
	{
		free(db);
		fclose(fh);
		return NULL;
	}
	strncpy(db->filename, filename, strlen(filename));
	db->rows_n = 0;
	db->rows = (bbs_db_row **) malloc (sizeof(bbs_db_row *) * rows_size);
	if (db->rows == NULL)
	{
		free(db->filename);
		free(db);
		fclose(fh);
		return NULL;
	}
	
	bytes_read = fread(buffer, 1, DB_MAX_ROW_SIZE, fh);
	start_of_row = 0;
	while (bytes_read > 0)
	{
		for (i = 0; i < bytes_read; i++)
		{
			printf("%d-", (int)buffer[i]);
			if (buffer[i] == (char) 0)
			{
				// put this row on a line
				line = malloc(i + 2 - start_of_row);
				if (line == NULL)
				{
					destroy_db(db);
					fclose(fh);
					return NULL;
				}
				memset(line, 0, i + 2 - start_of_row);
				strncpy(line, &buffer[start_of_row], i + 2 - start_of_row);
				// create a row from this line
				row = create_db_row_from_db_line(line);
				free(line);
				if (row == NULL)
				{
					destroy_db(db);
					fclose(fh);
					return NULL;
				}
				db->rows[db->rows_n++] = row;
				while (db->rows_n >= rows_size)
				{
					rows_realloc = realloc(db->rows, rows_size * 2);
					if (rows_realloc == NULL)
					{
						destroy_db(db);
						fclose(fh);
						return NULL;
					}
					db->rows = rows_realloc;
					rows_size *= 2;
				}
				start_of_row = i+1;
			}
		}
		// if start_of_row == 0, bad things happened. return null
		if (start_of_row == 0)
		{
			destroy(db);
			fclose(fh);
			return NULL;
		}
		// because the behavior of memcpy is undefined if we try and copy back
		// on ourself...
		for (i = 0; i < bytes_read - start_of_row; i++)
		{
			buffer[i] = buffer[start_of_row + i];
		}
		bytes_read = fread(buffer, 1, DB_MAX_ROW_SIZE, fh);
		start_of_row = 0;
	}
	
	return db;
	
}
