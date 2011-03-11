#ifndef parser_HEADER
#define parser_HEADER

#include "lexer.h"

typedef struct parser_value_stack_s {
    void * data;
    int type;
    struct parser_value_stack_s * next;
} parser_value_stack_t;


typedef struct parser_action_stack_s {
    int type;
    struct parser_action_stack_s * next;
} parser_action_stack_t;


typedef struct parser_s {
    lexer_t * lexer;
    token_t * token;
    parser_value_stack_t * parser_value_stack;
    parser_action_stack_t * parser_action_stack;
} parser_t;


parser_t * parser_create  (lexer_t * lexer);
void       parser_destroy (parser_t * parser);


#endif