#include <stdio.h>
#include <string.h>

#include "bbs_db.h"

int main() 
{

	int i;
	int row;
	int column;
	int testdb_length = 0;
	char testdb[] = "a|b|cdef0123|45|6780";
	FILE * fh;
	bbs_db * db;
	
	testdb_length = strlen(testdb);
	
	for (i = 0; i < testdb_length; i++)
	{
		if (testdb[i] == '0')
			testdb[i] = (char) 0;
	}
	
	fh = fopen("test.db", "w");
	if (fh == NULL)
		printf("Error opening test.db\n");
	fwrite(testdb, 1, testdb_length, fh);
	fclose(fh);
	
	db = load_db("test.db");
	for (row = 0; row < db->rows_n; row++)
	{
		printf("ROW %d\n", row);
		for (column = 0; column < db->rows[row]->columns_n; column++)
		{
			printf("%s\n", db->rows[row]->columns[column]);
		}
	}
	
	return 1;
	
}
