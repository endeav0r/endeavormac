#include "lexer.h"
#include "end.h"



end_lexer_t * end_lexer_create (char * text)
{

	end_lexer_t * lexer;
	
	lexer = (end_lexer_t *) malloc(sizeof(end_lexer_t));
	lexer->text = (char *) malloc(strlen(text) + 1);
	lexer->text[0] = (char) 0;
	strcpy(lexer->text, text);
	lexer->eof = &(lexer->text[strlen(lexer->text)]);
	lexer->current_char = lexer->text;
	lexer->line = 1;
	lexer->tokens = NULL;
	lexer->tokens_last = NULL;
	
	return lexer;

}



void end_lexer_destroy (end_lexer_t * lexer)
{

	end_token_t * token, * token2;
	
	token = lexer->tokens;
	while (token != NULL)
	{
		token2 = token;
		token = token->next;
		free(token2);
	}
	
	free(token->text);
	free(token);

}



end_token_t * end_token_create (char * text, int type, int line)
{

	end_token_t * token;
	
	token = (end_token_t *) malloc(sizeof(end_token_t));
	if (text == NULL)
		token->text = NULL;
	else
	{
		token->text = (char *) malloc(strlen(text) + 1);
		token->text[0] = 0;
		strcpy(token->text, text);
	}
	token->type = type;
    token->line = line;
	return token;

}



end_token_t * end_lexer_string (end_lexer_t * lexer)
{

	int    buf_i;
	size_t buf_size;
	char * buf, c;
	end_token_t * token;

	buf_i = 0;
	buf_size = 8;
	
	buf = (char *) malloc(buf_size);

	while (lexer->current_char < lexer->eof)
	{
		if (buf_i == buf_size - 1)
		{
			buf = realloc(buf, buf_size + 8);
			buf_size += 8;
		}
		c = *(lexer->current_char++);
		switch (c)
		{
			case '\\' :
				if (lexer->current_char + 1 == lexer->eof)
				{
					fprintf(stderr, "ERROR: invalid string literal line %d\n", lexer->line);
					break;
				}
				switch (*(lexer->current_char))
				{
					case 'r' :
						buf[buf_i++] = '\r';
						lexer->current_char++;
						continue;
					case 'n' :
						buf[buf_i++] = '\n';
						lexer->current_char++;
						continue;
					case 't' :
						buf[buf_i++] = '\t';
						lexer->current_char++;
						continue;
					case '\\' :
						buf[buf_i++] = '\\';
						lexer->current_char++;
						continue;
					case '"' :
						buf[buf_i++] = '"';
						lexer->current_char++;
						continue;
				}
				fprintf(stderr, "WARNING: Bad escape sequence line %d\n", lexer->line);
				break;
			case '"' :
				buf[buf_i] = 0;
				token = end_token_create(buf, TOKEN_STRING, lexer->line);
				free(buf);
				return token;
			default :
				buf[buf_i++] = c;
		}
	}
	
	fprintf(stderr, "ERROR: invalid string literal line %d\n", lexer->line);
    exit(-1);
	
	return NULL;
	
}



int end_lexer_text_classify (char * text)
{

	int i;
	int is_number;

    if (strcmp(text, "\n") == 0)
        return TOKEN_TERMINATOR;
	if (strcmp(text, "=") == 0)
		return TOKEN_ASSIGN;
	if (strcmp(text, "+") == 0)
		return TOKEN_ADD;
	if (strcmp(text, "-") == 0)
		return TOKEN_SUBTRACT;
	if (strcmp(text, "==") == 0)
		return TOKEN_EQUAL;
	if (strcmp(text, "(") == 0)
		return TOKEN_PAREN_OPEN;
	if (strcmp(text, ")") == 0)
		return TOKEN_PAREN_CLOSE;
    if (strcmp(text, "<") == 0)
        return TOKEN_LESS_THAN;
    if (strcmp(text, ">") == 0)
        return TOKEN_GREATER_THAN;
    if (strcmp(text, "%") == 0)
        return TOKEN_MODULUS;
    if (strcmp(text, "*") == 0)
        return TOKEN_MULTIPLY;
    if (strcmp(text, "/") == 0)
        return TOKEN_DIVIDE;
    if (strcmp(text, ",") == 0)
        return TOKEN_COMMA;
	if (strcmp(text, "function") == 0)
		return TOKEN_KEYWORD_FUNCTION;
	if (strcmp(text, "end") == 0)
		return TOKEN_KEYWORD_END;
	if (strcmp(text, "if") == 0)
		return TOKEN_KEYWORD_IF;
	if (strcmp(text, "else") == 0)
		return TOKEN_KEYWORD_ELSE;
    if (strcmp(text, "elsif") == 0)
        return TOKEN_KEYWORD_ELSIF;
    if (strcmp(text, "while") == 0)
        return TOKEN_KEYWORD_WHILE;
	if (strcmp(text, "print") == 0)
		return TOKEN_PRINT;
    if (strcmp(text, "return") == 0)
        return TOKEN_KEYWORD_RETURN;
	
	is_number = 1;
	for (i = 0; i < strlen(text); i++)
	{
		if ((text[i] < '0') || (text[i] > '9'))
		{
			is_number = 0;
			break;
		}
	}
	
	if (is_number)
		return TOKEN_NUMBER;
	
	return TOKEN_SYMBOL;

}



// doesn't copy token, so don't free it after this!
void end_lexer_append_token (end_lexer_t * lexer, end_token_t * token)
{
	if (lexer->tokens == NULL)
	{
		lexer->tokens = token;
		lexer->tokens_last = token;
	}
	else
	{
		lexer->tokens_last->next = token;
		lexer->tokens_last = lexer->tokens_last->next;
	}
}



end_token_t * end_lexer_next (end_lexer_t * lexer)
{

	int buf_i, buf_size;
	char * buf, c;
	end_token_t * token;

	buf_i = 0;
	buf_size = 8;
	buf = (char *) malloc(buf_size);
	
	while (lexer->current_char < lexer->eof)
	{
		if (buf_i == buf_size - 1)
		{
			buf = realloc(buf, buf_size + 8);
			buf_size += 8;
		}
		c = *(lexer->current_char++);
		switch (c)
		{
            // these are 1 character tokens
			case '\n' :
            case '(' :
            case ')' :
				if (buf_i > 0)
				{
					// this will go back a char, making sure to hit this char
					// next time end_lexer_next is called
					lexer->current_char--;
					buf[buf_i] = 0;
					token = end_token_create(buf, end_lexer_text_classify(buf), lexer->line);
					end_lexer_append_token(lexer, token);
					#ifdef LEXER_DEBUG
						printf("end_lexer_next %d %s\n", token->type, token->text);
					#endif
					return token;
				}
				else
				{
                    if (c == '\n')  
                        lexer->line++;
                    buf[0] = c;
                    buf[1] = 0;
					token = end_token_create(buf, end_lexer_text_classify(buf), lexer->line);
					end_lexer_append_token(lexer, token);
					#ifdef LEXER_DEBUG
						printf("end_lexer_next %d %s\n", token->type, token->text);
					#endif
					return token;
				}
				break; // never reached
			case ' ' :
			case '\t' :
			case '\r' :
				if (buf_i > 0)
				{
					buf[buf_i] = 0;
					token = end_token_create(buf, end_lexer_text_classify(buf), lexer->line);
					end_lexer_append_token(lexer, token);

					#ifdef LEXER_DEBUG
						printf("end_lexer_next %d %s\n", token->type, token->text);
					#endif
					return token;
				}
				break;
			case '"' :
				end_lexer_append_token(lexer, end_lexer_string(lexer));
                #ifdef LEXER_DEBUG
                    printf("end_lexer_next %d %s\n", lexer->tokens_last->type, lexer->tokens_last->text);
                #endif
				return lexer->tokens_last;
			default :
				buf[buf_i++] = c;
		}
	}
	
	if (buf_i > 0)
	{
		buf[buf_i] = 0;
		token = end_token_create(buf, end_lexer_text_classify(buf), lexer->line);
		end_lexer_append_token(lexer, token);
		#ifdef LEXER_DEBUG
			printf("end_lexer_next %d %s\n", token->type, token->text);
		#endif
		return token;
	}
	if (lexer->tokens_last != NULL)
	{
		if (lexer->tokens_last->type == TOKEN_EOF)
			return lexer->tokens_last;
	}
	
	token = end_token_create(NULL, TOKEN_EOF, lexer->line);
	end_lexer_append_token(lexer, token);
	
	#ifdef LEXER_DEBUG
		printf("end_lexer_next %d %s\n", token->type, token->text);
	#endif
	return token;

}



int end_lexer_eof (end_lexer_t * lexer)
{
	if (lexer->tokens_last != NULL)
    {
        if (lexer->tokens_last->type == TOKEN_EOF)
            return 1;
    }
	return 0;
}
	

	
int end_token_type_to_end_type (end_token_t * token)
{
	if (token->type == TOKEN_NUMBER)
		return TYPE_NUMBER;
    else if (token->type == TOKEN_STRING)
        return TYPE_STRING;
	else
		return TYPE_NONE;
}