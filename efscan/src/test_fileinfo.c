#include "fileinfo.h"
#include "utility.h"

#include <stdio.h>
#include <string.h>



int main (int argc, char * argv[])
{

	int result;
	
	file_info_t fi;
	
	if (argc < 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return 1;
	}
	
	result = file_info(argv[1], &fi);
	
	printf("file_info() returned: %d\n", result);
	
	if (result != 0)
		return 0;
		
	file_info_debug_output(&fi);
	
	return 1;
	
}
