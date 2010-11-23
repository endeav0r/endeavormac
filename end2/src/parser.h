#ifndef parser_HEADER
#define parser_HEADER

#include "vm.h"
#include "lexer.h"
#include "variable.h"

typedef struct parser_s {
    int parens;
    token_t * token;
    vm_t * vm;
} parser_t;

parser_t *   parser_create  (token_t * token);
void         parser_destroy (parser_t * parser);

int          parser_parse (parser_t * parser);

#endif