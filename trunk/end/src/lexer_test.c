#include <stdio.h>

#include "lexer.h"

int main ()
{

	char * test_string = "if something == 29443 and something_else == "
	                     "\"this_thing\" then\nprint something end";

	struct end_lexer_s * lexer;
	struct end_token_s * token;
	
	lexer = end_lexer_create(test_string);
	
	token = end_lexer_next(lexer);
	printf ("lexer->text: %s\n", lexer->text);
	while (token->type != TOKEN_EOF)
	{
		printf("%d %s\n", token->type, token->text);
		token = end_lexer_next(lexer);
	}
	
	return 0;

}