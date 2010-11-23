#include <stdio.h>

#include "parser.h"


int main (int argc, char * argv[]) {
    int bytes_read;
    FILE * fh;
    char buf[4096];
    lexer_t * lexer;
    parser_t * parser;

    if (argc != 2) {
        printf("usage: %s <script filename>\n", argv[0]);
        exit(1);
    }
    
    fh = fopen(argv[1], "r");
    if (fh == NULL) {
        fprintf(stderr, "bad filename: %s\n", argv[1]);
        exit(1);
    }
    
    bytes_read = fread(buf, 1, 4096, fh);
    if (bytes_read == 4096) {
        fprintf(stderr, "alpha takes scripts < 4kb\n");
        exit(1);
    }
    
    buf[bytes_read] = 0;
    
    lexer = lexer_create(buf);
    while (lexer_eof(lexer) == 0)
        lexer_next(lexer);
    parser = parser_create(lexer->tokens);
    parser_parse(parser);
    
    parser_destroy(parser);
    lexer_destroy(lexer);
    
    return 0;
}