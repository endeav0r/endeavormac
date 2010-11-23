#ifndef lexer_HEADER
#define lexer_HEADER

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



typedef struct token_s {
	char * text;
	int    type;
    int    line; // the line this token was found on
	struct token_s * next;
} token_t;


typedef struct lexer_s {
	int    line;
	char * text;
	char * current_char;
	char * eof;
	token_t * tokens;
	token_t * tokens_last;
} lexer_t;



lexer_t *  lexer_create  (char * text);
void       lexer_destroy (lexer_t * lexer);

token_t * token_create        (char * text, int type, int line);
token_t * lexer_string        (lexer_t * lexer);
int       lexer_text_classify (char * text);
void      lexer_append_token  (lexer_t * lexer, token_t * token);
token_t * lexer_next          (lexer_t * lexer);

int       lexer_eof           (lexer_t * lexer);

int token_type_to_variable_type (token_t * token);

#endif