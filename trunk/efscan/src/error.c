#include "error.h"



void error_print_description (int description)
{
	
	switch (description)
	{
		case NSRL_ALLOCATE_ERROR :
			printf("NSRL ALLOCATE ERROR\n");
			break;
		case NSRL_OPEN_FILE_ERROR :
			printf("NSRL OPEN FILE ERROR\n");
			break;
		case NSRL_READ_ERROR :
			printf("NSRL READ ERROR\n");
			break;
		case NSRL_EOF :
			printf("NSRL EOF\n");
			break;
		case NSRL_LINE_BUFFER_OVERFLOW_ERROR :
			printf("NSRL LINE BUFFER OVERFLOW ERROR\n");
			break;
		case NSRL_GET_LINE_FAIL :
			printf("NSRL GET LINE FAIL\n");
			break;
	}
	
}
