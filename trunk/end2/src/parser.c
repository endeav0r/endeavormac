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


token_t * find_block_end (token_t * token) {
    int block = 1;
    token_t * next;
    
    next = token->next;
    while (block > 0) {
        switch (next->type) {
            case TOKEN_KEYWORD_END :
                block--;
                break;
            case TOKEN_KEYWORD_IF :
                block++;
                break;
            case TOKEN_KEYWORD_WHILE :
                block++;
                break;
            case TOKEN_KEYWORD_ELSE :
                block--;
            case TOKEN_KEYWORD_LOOP :
                block--;
        }
        next = next->next;
    }
    return next;
}


token_t * find_loop_begin (token_t * token) {
    int block = 1;
    token_t * previous;
    
    previous = token->previous;
    while (block > 0) {
        switch (previous->type) {
            case TOKEN_KEYWORD_END :
                block++;
                break;
            case TOKEN_KEYWORD_LOOP :
                block++;
                break;
            case TOKEN_KEYWORD_IF :
                block--;
                break;
            case TOKEN_KEYWORD_WHILE :
                block--;
                break;
        }
        previous = previous->previous;
    }
    return previous;
}

int parser_parse (parser_t * parser) {
    symbol_t * symbol;
    token_t * token_jmp;
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
                
            case TOKEN_STRING :
                vm_stack_push(parser->vm, variable_create(parser->token->text, TYPE_STRING));
                break;
                
            case TOKEN_ADD :
                vm_opcodes_push(parser->vm, OPCODE_ADD);
                break;
                
            case TOKEN_SUBTRACT :
                vm_opcodes_push(parser->vm, OPCODE_SUBTRACT);
                break;
                
            case TOKEN_DIVIDE :
                vm_opcodes_push(parser->vm, OPCODE_DIVIDE);
                break;
                
            case TOKEN_MULTIPLY :
                vm_opcodes_push(parser->vm, OPCODE_MULTIPLY);
                break;
                
            case TOKEN_MODULUS :
                vm_opcodes_push(parser->vm, OPCODE_MODULUS);
                break;
            
            case TOKEN_KEYWORD_LOOP :
                vm_stack_push(parser->vm, find_loop_begin(parser->token));
                vm_opcodes_push(parser->vm, OPCODE_JMP);
                break;
                
            case TOKEN_KEYWORD_ELSE :
                vm_stack_push(parser->vm, find_block_end(parser->token));
                vm_opcodes_push(parser->vm, OPCODE_JMP);
                break;
                
            case TOKEN_KEYWORD_IF :
                vm_stack_push(parser->vm, find_block_end(parser->token));
                break;
                
            case TOKEN_KEYWORD_WHILE :
                vm_stack_push(parser->vm, find_block_end(parser->token));
                break;
                
            case TOKEN_EQUAL :
                vm_opcodes_push(parser->vm, OPCODE_EQ);
                break;
                
            case TOKEN_LESS_THAN :
                vm_opcodes_push(parser->vm, OPCODE_LT);
                break;
            
            case TOKEN_GREATER_THAN :
                vm_opcodes_push(parser->vm, OPCODE_GT);
                break;
                
            case TOKEN_ASSIGN :
                vm_opcodes_push(parser->vm, OPCODE_ASSIGN);
                break;
                
            case TOKEN_PRINT :
                vm_opcodes_push(parser->vm, OPCODE_PRINT);
                break;
                
            case TOKEN_TERMINATOR :
            case TOKEN_EOF :
                token_jmp = vm_execute(parser->vm);
                if (token_jmp != NULL) {
                    parser->token = token_jmp;
                }
                break;
        }
        if (parser->token->type == TOKEN_EOF)
            break;
        parser->token = parser->token->next;
    }
    return 0;
}