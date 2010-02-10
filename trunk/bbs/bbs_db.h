#ifndef bbs_db_H
#define bbs_db_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// rows larger than 64k will cause problems!
// it would be 640k, but then that would be enough for everyone
#define DB_MAX_ROW_SIZE 65536


typedef struct _bbs_db_row
{
	int columns_n;
	char ** columns;
} bbs_db_row;


typedef struct
{
	char * filename;
	int rows_n;
	bbs_db_row ** rows;
} bbs_db;


bbs_db_row * create_db_row_from_db_line (char * line);
void         destroy_db (bbs_db * db);
bbs_db *     load_db (char * filename);

#endif
