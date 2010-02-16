#include "exception.hpp"


Exception :: Exception (int code, char * description)
{
	this->code = code;
	this->description = (char *) malloc(strlen(description));
	if (this->description == NULL)
		// throwing an exception here seems like such a bad idea
		throw 1;
	strcpy(this->description, description);
}



Exception :: ~Exception ()
{
	//free(this->description);
}



void Exception :: display ()
{
	printf("ERROR: %s\n", this->description);
}



void debug (char * s)
{
	printf("%s\n", s);
	FILE * fh;
	fh = fopen("debug.txt", "a");
	fwrite(s, 0, strlen(s), fh);
	fwrite("\n", 0, 1, fh);
	fclose(fh);
}
