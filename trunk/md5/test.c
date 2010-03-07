#include <stdio.h>
#include <string.h>

#include "md5.c"



void debug_bytes (unsigned char * data, int bytes)
{
	int i;
	for (i = 0; i < bytes; i++)
	{
		printf("%02hx", data[i]);
		if ((i + 1) % 4 == 0)
			printf("\n");
	}
}



int main ()
{
	int i = 7;
	md5_context * context;
	unsigned char data[] = "test";
	
	context = md5_begin(data, 4);
	
	md5_add_padding(context);
	md5_append_length(context);
	md5_process_block(context);
	
	debug_bytes ((unsigned char *) &(context->A), 4);
	debug_bytes ((unsigned char *) &(context->B), 4);
	debug_bytes ((unsigned char *) &(context->C), 4);
	debug_bytes ((unsigned char *) &(context->D), 4);
	
	return 1;
}
