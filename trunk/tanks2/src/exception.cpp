#include "exception.hpp"

Exception :: Exception (int code, std::string description)
{
	this->code = code;
	this->description = description;
}



Exception :: ~Exception ()
{
	printf("~exception");
}



void Exception :: display ()
{
	printf("ERROR: %s\n", this->description.c_str());
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
