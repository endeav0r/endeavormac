#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define GET_LINE_BUFFER_SIZE 128

char GET_LINE_BUFFER[GET_LINE_BUFFER_SIZE];



// returns 1 if c is a printable/acceptable character
int char_is_printable (char c)
{
	if ((c >= 'a') && (c <= 'z'))
		return 1;
	if ((c >= 'A') && (c <= 'Z'))
		return 1;
	if ((c >= '0') && (c <= '9'))
		return 1;
	if ((c == '\r') || (c == '\n') || (c == '\t') || (c == ' '))
		return 1;
	if ((c == '\'') || (c == '"') || (c == ',') || (c == '.'))
		return 1;
	if ((c == '>') || (c == '<') || (c == ';') || (c == '='))
		return 1;
	return 0;
}




char * get_line (int maxlen)
{

	char c;
	int line_buffer_i = 0;
	int i;
	
	memset(GET_LINE_BUFFER, 0, GET_LINE_BUFFER_SIZE);
	
	for (i = 0; i < GET_LINE_BUFFER_SIZE - 1; i++)
	{
		c = getchar();
		if ((c == '\r') || (c == '\n'))
			return GET_LINE_BUFFER;
		if (char_is_printable(c))
			GET_LINE_BUFFER[line_buffer_i++] = c;
	}
	
	return NULL;
	
}
	


int login ()
{

	char username[21];
	char password[21];

	while (1)
	{
		printf("Login: ");
		if (get_line(20) == NULL)
			printf("Username was too long\n");
		else
			break;
	}
	strncpy(username, GET_LINE_BUFFER, 21);
	printf("Logging in with username: %s\n", username);
	
	return 1;

}

int main (int argc, char * argv[])
{

	printf("Welcome to the BBS\n");
	login();
	
	return 1;
	
}
