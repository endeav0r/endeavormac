#ifndef parser_HEADER
#define parser_HEADER

#include "lexer.h"
#include "number.h"
#include "string.h"
#include "symbol.h"
#include "vm.h"


#define TYPE_NONE     1
#define TYPE_FUNCTION 2
#define TYPE_NUMBER   3
#define TYPE_STRING   4


typedef struct end_s
{
    int conditional_noop; // when this is 1, we are not executing instructions
                          // because of a conditional that evaluated FALSE
                          // it is incremented for every additional block, and
                          // when it reaches 0 again, we know to start executing
                          // again
	end_vm_t * vm;
	end_lexer_t * lexer;
	end_token_t * token; // current token we're executing ... instruction pointer
} end_t;



end_t * end_create ();
void    end_destroy (end_t * end);

end_symbol_t * end_evaluate (end_t * end);
int end_statement (end_t * end);

int end_parse (end_t * end, char * text);

int end_subtract (end_t * end);
int end_add (end_t * end);
int end_evaluate_conditional (end_t * end);
int end_if (end_t * end);

#endif