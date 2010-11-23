#include <stdio.h>

#inclue "parser.h"



int main () {
    char example [] = "print 1 + 2";
    
    parser_t * parser;
    lexer_t * lexer;
    
    lexer = lexer_create(example);
    parser = parser_create(lexer->tokens);
    parser_parse(parser);
    
    return 0;
}