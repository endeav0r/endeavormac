#include "end.h"



end_t * end_create ()
{

    end_t * end;

    end = (end_t *) malloc(sizeof(end_t));
    
    end->vm = end_vm_create();
    end_vm_symbol_table_stack_push(end->vm);
    end->lexer = NULL;
    end->conditional_noop = 0;
    
    return end;

}



void end_destroy (end_t * end)
{

    if (end->vm != NULL)
        end_vm_destroy(end->vm);
    
    free(end);

}



// the current end->token will be pointing towards an arithmetic operation
// such as + - * / %. This will evaluate the right hand side and perform
// the operation, pushing the result onto the stack
int end_arithmetic (end_t * end)
{

    end_vm_symbol_stack_t * symbol_stack;
    end_symbol_t * result = NULL;
    end_symbol_t * symbol;
    end_token_t * operation_token;
    
    // get the operation, advance token
    operation_token = end->token;
    end->token = end->token->next;
    
    // pop lhs off stack
    symbol_stack = end_vm_stack_pop(end->vm);
    // evaluate rhs
    symbol = end_evaluate(end);

    if (symbol_stack->symbol->type != symbol->type)
    {
        fprintf(stderr, "ERROR, mismatch types %d %d line %d\n",
                symbol_stack->symbol->type, symbol->type, end->token->line);
        #if END_DEBUG
            printf("%p %p\n", symbol_stack->symbol, symbol);
        #endif
    }
    
    switch (operation_token->type)
    {
        case TOKEN_ADD :
            switch (symbol->type)
            {
                case TYPE_NUMBER :
                    result = end_symbol_create(NULL, NULL, TYPE_NUMBER);
                    end_number_add(result->value,
                                   (end_number_t *) symbol_stack->symbol->value,
                                   (end_number_t *) symbol->value);
                    break;
                case TYPE_STRING :
                    result = end_symbol_create(NULL, NULL, TYPE_STRING);
                    end_string_add(result->value, symbol_stack->symbol->value, symbol->value);
                    break;
                default :
                    fprintf(stderr, "invalid type for operation line %d\n", end->token->line);
                    exit(-1);
            }
            break;
        case TOKEN_SUBTRACT :
            switch (symbol->type)
            {
                case TYPE_NUMBER :
                    result = end_symbol_create(NULL, NULL, TYPE_NUMBER);
                    end_number_subtract(result->value,
                                       (end_number_t *) symbol_stack->symbol->value,
                                       (end_number_t *) symbol->value);
                    break;
                default :
                    fprintf(stderr, "invalid type for operation line %d\n", end->token->line);
                    exit(-1);
            }
            break;
        case TOKEN_MODULUS :
            switch (symbol->type)
            {
                case TYPE_NUMBER :
                    result = end_symbol_create(NULL, NULL, TYPE_NUMBER);
                    end_number_modulus(result->value,
                                       (end_number_t *) symbol_stack->symbol->value,
                                       (end_number_t *) symbol->value);
                    break;
                default :
                    fprintf(stderr, "invalid type for operation line %d\n", end->token->line);
                    exit(-1);
            }
            break;
        case TOKEN_DIVIDE :
            switch (symbol->type)
            {
                case TYPE_NUMBER :
                    result = end_symbol_create(NULL, NULL, TYPE_NUMBER);
                    end_number_divide(result->value,
                                      (end_number_t *) symbol_stack->symbol->value,
                                      (end_number_t *) symbol->value);
                    break;
                default :
                    fprintf(stderr, "invalid type for operation line %d\n", end->token->line);
                    exit(-1);
            }
            break;
        case TOKEN_MULTIPLY :
            switch (symbol->type)
            {
                case TYPE_NUMBER :
                    result = end_symbol_create(NULL, NULL, TYPE_NUMBER);
                    end_number_multiply(result->value,
                                       (end_number_t *) symbol_stack->symbol->value,
                                       (end_number_t *) symbol->value);
                    break;
                default :
                    fprintf(stderr, "invalid type for operation line %d\n", end->token->line);
                    exit(-1);
            }
            break;
    
    }
    // clean up, push result
    end_vm_symbol_stack_destroy(symbol_stack);
    end_vm_stack_push(end->vm, result, 1);
    
    return 0;
}



// a = b OPERATION c
end_symbol_t * end_evaluate (end_t * end)
{

    int while_break = 0;
    end_vm_symbol_stack_t * symbol_stack;
    end_symbol_t * result, * symbol;
    
    while (   (end->token->type != TOKEN_TERMINATOR) 
           && (end->token->type != TOKEN_EOF)
           && (while_break == 0)
          )
    {
        switch (end->token->type)
        {
        
            case TOKEN_SYMBOL :
                #ifdef END_DEBUG
                    printf("end_evaluate TOKEN_SYMBOL\n");
                #endif
                symbol = end_vm_symbol_get(end->vm, end->token->text);
                if (symbol == NULL)
                {
                    fprintf(stderr, "ERROR, %s not found line %d\n", end->token->text, end->token->line);
                    exit(-1);
                }
                if (symbol->type == TYPE_FUNCTION)
                {
                    end_function_call(end, symbol);
                    end_execute(end);
                    while_break = 1;
                }
                else
                    end_vm_stack_push(end->vm, symbol, 0);
                break;
        
            case TOKEN_NUMBER :
                #ifdef END_DEBUG
                    printf("end_evaluate TOKEN_NUMBER\n");
                #endif
                symbol = end_symbol_create(NULL, end->token->text, end_token_type_to_end_type(end->token));
                end_vm_stack_push(end->vm, symbol, 1);
                break;
            
            case TOKEN_STRING :
                #ifdef END_DEBUG
                    printf("end_evalute TOKEN_STRING\n");
                #endif
                symbol = end_symbol_create(NULL, end->token->text, end_token_type_to_end_type(end->token));
                end_vm_stack_push(end->vm, symbol, 1);
                break;
            
            case TOKEN_ADD :
            case TOKEN_SUBTRACT :
            case TOKEN_MODULUS :
            case TOKEN_DIVIDE :
            case TOKEN_MULTIPLY :
                end_arithmetic(end);
                while_break = 1;
                break;
            
            default :
                while_break = 1;
                break;
        }
        if (while_break == 0)
            end->token = end->token->next;
    }
    
    symbol_stack = end_vm_stack_pop(end->vm);
    result = end_symbol_copy(symbol_stack->symbol);
    end_vm_symbol_stack_destroy(symbol_stack);
    
    #ifdef END_DEBUG
        printf("end_evalute returning %d\n", result->type);
    #endif
    return result;

}



// returns 1 if conditional is true, 0 if conditional is false
int end_evaluate_conditional (end_t * end)
{

    end_symbol_t * lhs, * rhs;
    end_token_t * comparison_token;
    int comparison = 0;
    
    lhs = end_evaluate (end);
    while (
           (end->token->type != TOKEN_EQUAL)
           && (end->token->type != TOKEN_LESS_THAN)
           && (end->token->type != TOKEN_GREATER_THAN)
          )
    {
        end->token = end->token->next;
        if ((end->token->type == TOKEN_TERMINATOR) || (end->token->type == TOKEN_EOF))
        {
            fprintf(stderr, "ERROR in conditional on line %d\n", end->token->line);
            exit(-1);
        }
    }
    comparison_token = end->token;
    end->token = end->token->next;
    rhs = end_evaluate (end);
    
    if (lhs->type != rhs->type)
    {
        fprintf(stderr, "ERROR type mismatch in conditional on line %d\n", end->token->line);
        exit(-1);
    }
    
    switch (lhs->type)
    {
        case TYPE_NUMBER :
            comparison = end_number_compare(lhs->value, rhs->value);
            break;
        case TYPE_STRING :
            comparison = end_string_compare(lhs->value, rhs->value);
            break;
    }
    
    if ((comparison_token->type == TOKEN_EQUAL) && (comparison == 0))
        return 1;
    else if ((comparison_token->type == TOKEN_LESS_THAN) && (comparison == -1))
        return 1;
    else if ((comparison_token->type == TOKEN_GREATER_THAN) && (comparison == 1))
        return 1;
    else
        return 0;

}
    
    


int end_statement (end_t * end)
{

    #ifdef END_DEBUG
        printf("end_statement begin\n");
    #endif

    end_symbol_t * symbol, * rhs;
    end_token_t * control_stack_top;
    
    while ((end->token->type != TOKEN_EOF) && (end->token->type != TOKEN_TERMINATOR))
    {
        if (end->conditional_noop == 0)
        {
            switch (end->token->type)
            {
                case TOKEN_SYMBOL :
                    if (end->token->next == NULL)
                    {
                        fprintf(stderr, "syntax error 01 line %d\n", end->lexer->line);
                        exit(-1);
                    }
                    // are we assigning something to a symbol?
                    if (end->token->next->type == TOKEN_ASSIGN)
                    {
                        #ifdef END_DEBUG
                            printf("end_statement ASSIGN\n");
                        #endif
                        if (end->token->next->next == NULL)
                        {
                            fprintf(stderr, "syntax error 02 line %d\n", end->token->line);
                            exit(-1);
                        }
                        // create a symbol to save the name
                        symbol = end_symbol_create(end->token->text, NULL, TYPE_NONE);
                        // advance token
                        end->token = end->token->next->next;
                        // get right hand side
                        rhs = end_evaluate(end);
                        // rename rhs
                        end_symbol_name_set(rhs, symbol->name);
                        // set the symbol in the VM
                        end_vm_symbol_set(end->vm, rhs);
                        // delete name cache symbol
                        end_symbol_destroy(symbol);
                        return 0;
                    }
                    // maybe it's a function we are calling
                    else
                    {
                        symbol = end_vm_symbol_get (end->vm, end->token->text);
                        if (symbol->type == TYPE_FUNCTION)
                        {
                            #ifdef END_DEBUG
                                printf("end_statement FUNCTION %s\n", symbol->name);
                            #endif
                            // call the function
                            end_function_call(end, symbol);
                            end_execute(end);
                            continue;
                        }
                    }
                    break;
                    
                case TOKEN_PRINT :
                    #ifdef END_DEBUG
                        printf("end_statement PRINT\n");
                    #endif
                    end->token = end->token->next;
                    symbol = end_evaluate(end);
                    #ifdef END_DEBUG
                        printf("PRINT: ");
                    #endif
                    switch (symbol->type)
                    {
                        case TYPE_NUMBER :
                            printf("%s\n", end_number_get(symbol->value));
                            break;
                        case TYPE_STRING :
                            printf("%s\n", end_string_get(symbol->value));
                            break;
                    }
                    end_symbol_destroy(symbol);
                    return 0;
                    
            }
        }
        switch (end->token->type)
        {
            // while and if work exactly the same way for setup
            case TOKEN_KEYWORD_WHILE :
            case TOKEN_KEYWORD_IF :
                end_vm_control_stack_push(end->vm, end->token);
                end->token = end->token->next;
                if (end->conditional_noop)
                    end->conditional_noop++;
                else if (end_evaluate_conditional(end) == 0)
                    end->conditional_noop = 1;
                break;
            
            case TOKEN_KEYWORD_FUNCTION :
                symbol = end_function_create(end);
                end_vm_symbol_set(end->vm, symbol);
                // push on to the control stack so when we pop, no segfault :)
                end_vm_control_stack_push(end->vm, NULL);
                end->conditional_noop++;
                break;
            
            case TOKEN_KEYWORD_RETURN :
                if (end->conditional_noop)
                {
                    end->conditional_noop--;
                    // if this return has a result it returns, we
                    // need to skip all the way to TOKEN_TERMINATOR
                    if (end->conditional_noop == 0)
                    {
                        while ((end->token->type != TOKEN_TERMINATOR)
                               && (end->token->type != TOKEN_EOF))
                            end->token = end->token->next;
                        continue;
                    }
                }
                else
                {
                    control_stack_top = end_vm_control_stack_get(end->vm);
                    // this better be a function at the top of the control stack
                    if (control_stack_top->type != TOKEN_KEYWORD_FUNCTION)
                    {
                        fprintf(stderr,
                                "return not matched with function line %d\n",
                                end->token->line);
                        exit(-1);
                    }
                    end_function_return(end);
                    end_vm_control_stack_pop(end->vm);
                    // break out of the end_execute statement, which will
                    // return control to the end_evaluate statement where
                    // the function_call took place
                    return 1;
                }
                break;
            case TOKEN_KEYWORD_END :
                if (end->conditional_noop)
                {
                    #if END_DEBUG
                        printf("end->conditional_noop-- %d\n", end->conditional_noop);
                    #endif
                    end->conditional_noop--;
                }
                else
                {
                    // look at the top of the control stack. is it a while loop?
                    control_stack_top = end_vm_control_stack_get(end->vm);
                    if (control_stack_top != NULL)
                    {
                        if (control_stack_top->type == TOKEN_KEYWORD_WHILE)
                        {
                            // reset execution to the beginning of the while loop
                            end->token = control_stack_top;
                            end_vm_control_stack_pop(end->vm);
                            continue; // don't let interpeter skip over loop token
                        }
                    }
                }
                end_vm_control_stack_pop(end->vm);
                break;
                
            case TOKEN_KEYWORD_ELSE :
                if (end->conditional_noop == 0)
                    end->conditional_noop = 1;
                else if (end->conditional_noop == 1)
                    end->conditional_noop = 0;
                if (end->token->next->type == TOKEN_KEYWORD_IF)
                    end_vm_control_stack_set_autopop(end->vm, 1);
        }
    
        end->token = end->token->next;
    }
                
    return 0;

}



int end_execute (end_t * end)
{


    while (end->token->type != TOKEN_EOF)
    {
        if (end_statement(end))
        {
            if (end->token->type == TOKEN_TERMINATOR)
                end->token = end->token->next;
            break;
        }
        #ifdef END_DEBUG
            printf("statement execution complete\n");
        #endif
        if (end->token->type == TOKEN_TERMINATOR)
            end->token = end->token->next;
    }
        
    return 0;
    
}


int end_parse (end_t * end, char * text)
{
    
    if (end->lexer != NULL)
        end_lexer_destroy(end->lexer);
    
    end->lexer = end_lexer_create(text);
    
    #ifdef END_DEBUG
        printf("begin lexing\n");
    #endif
    // just lex everything
    while (end_lexer_eof(end->lexer) == 0)
        end_lexer_next(end->lexer);
    #ifdef END_DEBUG
        printf("end lexing\n");
    #endif

    // now set token to beginning
    end->token = end->lexer->tokens;

    end_execute(end);
    
    return 0;

}