#ifndef token_HEADER
#define token_HEADER

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define TOKEN_TERMINATOR 1
#define TOKEN_SYMBOL 2
#define TOKEN_ASSIGN 3
#define TOKEN_NUMBER 5
#define TOKEN_STRING 6
#define TOKEN_EQUAL 7
#define TOKEN_ADD 8
#define TOKEN_SUBTRACT 9
#define TOKEN_PAREN_OPEN 10
#define TOKEN_PAREN_CLOSE 11
#define TOKEN_LESS_THAN 12
#define TOKEN_GREATER_THAN 13
#define TOKEN_MODULUS 14
#define TOKEN_DIVIDE 15
#define TOKEN_MULTIPLY 16
#define TOKEN_COMMA 17
#define TOKEN_KEYWORD_FUNCTION 100
#define TOKEN_KEYWORD_END 101
#define TOKEN_KEYWORD_IF 102
#define TOKEN_KEYWORD_ELSE 103
#define TOKEN_KEYWORD_ELSIF 104
#define TOKEN_KEYWORD_WHILE 105
#define TOKEN_KEYWORD_RETURN 106
#define TOKEN_EOF 1000

// for now, PRINT is its own token
#define TOKEN_PRINT 2000



typedef struct end_token_s
{
	char * text;
	int    type;
    int line; // the line this token was found on
	struct end_token_s * next;
} end_token_t;


typedef struct end_lexer_s
{
	int    line;
	char * text;
	char * current_char;
	char * eof;
	end_token_t * tokens;
	end_token_t * tokens_last;
} end_lexer_t;



end_lexer_t * end_lexer_create  (char * text);
void          end_lexer_destroy (end_lexer_t * lexer);

end_token_t * end_token_create        (char * text, int type, int line);
end_token_t * end_lexer_string        (end_lexer_t * lexer);
int           end_lexer_text_classify (char * text);
void          end_lexer_append_token  (end_lexer_t * lexer, end_token_t * token);
end_token_t * end_lexer_next          (end_lexer_t * lexer);

int           end_lexer_eof           (end_lexer_t * lexer);

int end_token_type_to_end_type (end_token_t * token);

#endif