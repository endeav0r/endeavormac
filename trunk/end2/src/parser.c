#include "parser.h"


parser_t * parser_create (token_t * token) {
    parser_t * parser;
    
    parser = (parser_t *) malloc(sizeof(parser_t));
    parser->token = token;
    parser->vm = vm_create();
    
    return parser;
}


void parser_destroy (parser_t * parser) {
    vm_destroy(parser->vm);
    free(parser);
};


int parser_parse (parser_t * parser) {
    symbol_t * symbol;
    while (1) {
        switch (parser->token->type) {
            case TOKEN_SYMBOL :
                symbol = vm_symbol_get(parser->vm, parser->token->text);
                if (symbol == NULL) {
                    symbol = symbol_create(parser->token->text,
                                           variable_create(NULL, TYPE_NONE));
                    variable_set_symbol(symbol->value, symbol);
                    vm_symbol_insert(parser->vm, symbol);
                }
                vm_stack_push(parser->vm, variable_copy(symbol->value));
                break;
                
            case TOKEN_NUMBER :
                vm_stack_push(parser->vm, 
                              variable_create(parser->token->text, TYPE_NUMBER));
                break;
                
            case TOKEN_ADD :
                vm_opcodes_push(parser->vm, OPCODE_ADD);
                break;
                
            case TOKEN_ASSIGN :
                vm_opcodes_push(parser->vm, OPCODE_ASSIGN);
                break;
                
            case TOKEN_PRINT :
                vm_opcodes_push(parser->vm, OPCODE_PRINT);
                break;
                
            case TOKEN_TERMINATOR :
            case TOKEN_EOF :
                vm_execute(parser->vm);
                break;
        }
        if (parser->token->type == TOKEN_EOF)
            break;
        parser->token = parser->token->next;
    }
    return 0;
}