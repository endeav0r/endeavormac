#include <stdio.h>

#include "end.h"

int main (int argc, char * argv[])
{

	int bytes_read;
	char buf[4096];
	end_t * end;
	FILE * fh;

	if (argc != 2)
	{
		printf("usage: %s <filename>\n", argv[0]);
		return 0;
	}
	
	fh = fopen(argv[1], "r");
	if (fh == NULL)
	{
		printf("error opening file %s\n", argv[1]);
		return -1;
	}
	
	bytes_read = fread(buf, 1, 4096, fh);
	if (bytes_read == 4096)
	{
		printf("early prototype accepts files <4k only\n");
		return -1;
	}
	
	buf[bytes_read] = 0;
	
	end = end_create();
	end_parse(end, buf);
	//end_destroy(end);
	
	return 0;

}