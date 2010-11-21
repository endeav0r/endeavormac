#include "function.h"



// creating a function is a bit trickier than most types.
// end->token should be TOKEN_KEYWORD_FUNCTION
// this will return the function symbol, ready to be pushed on to
// takes end->token to the terminator following TOKEN_FUNCTION. it's up to the interpreter
// to then skip over code until the corresponding end is found
// the symbol table
end_symbol_t * end_function_create (end_t * end)
{

    #ifdef FUNCTION_DEBUG
        printf("end_function_create ... ");
    #endif
    
    end_symbol_t * symbol;
    end_function_t * function;
    
    if (end->token->next->type != TOKEN_SYMBOL)
    {
        fprintf(stderr, "function not followed by symbol line %d\n", end->token->line);
        exit(1);
    }
    
    function = (end_function_t *) malloc(sizeof(end_function_t));
    function->arguments = 0;
    function->token = end->token;
    
    end->token = end->token->next;
    
    symbol = end_symbol_create (end->token->text, NULL, TYPE_FUNCTION);
    symbol->value = function;
    end->token = end->token->next;
    
    if (end->token->type != TOKEN_PAREN_OPEN)
    {
        fprintf(stderr, "syntax error 03 line %d\n", end->token->line);
        exit(1);
    }
    end->token = end->token->next;
    while (end->token->type != TOKEN_PAREN_CLOSE)
    {
        
        if ((end->token->type == TOKEN_TERMINATOR) || (end->token->type == TOKEN_EOF))
        {
            fprintf(stderr, "no closing paren line %d\n", end->token->line);
            exit(1);
        }
        if (end->token->type != TOKEN_COMMA)
            function->arguments++;
        end->token = end->token->next;
    }
    
    while ((end->token->type != TOKEN_TERMINATOR) && (end->token->type != TOKEN_EOF))
    {
        end->token = end->token->next;
    }
    
    #ifdef FUNCTION_DEBUG
        printf("done\n");
    #endif
    
    return symbol;
    
}



void end_function_destroy (end_function_t * function)
{
    free(function);
}



int end_function_assign (end_function_t * function, end_token_t * token)
{
    function->token = token;
    return 0;
}



end_token_t * end_function_get (end_function_t * function)
{
    return function->token;
}



int end_function_call (end_t * end, end_symbol_t * symbol)
{
    end_function_t * function = (end_function_t *) symbol->value;
    end_token_t * function_token;
    end_symbol_t * argument;
    
    // we now have a new symbol table for the scope of this function
    end_vm_symbol_table_stack_push(end->vm);
    
    // set up our function to our first argument
    function_token = function->token;
    // we're going to assume that because this function is in the symbol-table,
    // it's valid, and we can skip to first argument (or closing paren)
    function_token = function_token->next->next->next;
    
    // time to read in those arguments
    // but if the user didn't follow the function with a TOKEN_PAREN_OPEN
    end->token = end->token->next;
    if (end->token->type != TOKEN_PAREN_OPEN)
    {
        // there better be no goddam arguments
        if (function->arguments > 0)
        {
            fprintf(stderr, 
                    "function called with no arguments, it NEEDS them line %d\n",
                    end->token->line);
            exit(-1);
        }
    }
    else
    {
        // skip pass the TOKEN_PAREN_OPEN
        end->token = end->token->next;
        
        // for all those awesome arguments
        while (end->token->type != TOKEN_PAREN_CLOSE)
        {
            // oh no, if the function has a closing paren, that's bad news!
            if (function_token->type == TOKEN_PAREN_CLOSE)
            {
                fprintf(stderr, "incorrect number of arguments line %d\n", end->token->line);
                exit(-1);
            }
            #ifdef FUNCTION_DEBUG
                printf("evaluating argument: %s\n", end->token->text);
            #endif
            // evaluate the argument
            argument = end_evaluate(end);
            // set their name to what the function is expecting
            end_symbol_name_set(argument, function_token->text);
            // put this symbol in the symbol table locally for the function
            end_vm_symbol_set_local(end->vm, argument);
            // advance tokens
            if ((end->token->type == TOKEN_COMMA) && (end->token->type != TOKEN_PAREN_CLOSE))
                end->token = end->token->next;
            function_token = function_token->next;
            if ((function_token->type == TOKEN_COMMA)
                && (function_token->type != TOKEN_PAREN_CLOSE))
                function_token = function_token->next;
        }
        if (function_token->type != TOKEN_PAREN_CLOSE)
        {
            fprintf(stderr,
                    "incorrect number of arguments line %d\n",
                    end->token->line);
            exit(-1);
        }
        // end->token is the closing paren of the function call, so set it to
        // be the right after the closing paren
        
        end->token = end->token->next;
    }
    
    // end->token is the closing paren of 
    
    // now we need to push on to the execution stack
    end_vm_control_stack_push(end->vm, function->token);
    end_vm_control_stack_set_retraddr(end->vm, end->token);
    
    // now we need to carry function_token out to one beyond terminator and
    // make it the next token to be executed
    while (function_token->type != TOKEN_TERMINATOR)
    {
        if (function_token->type == TOKEN_EOF)
        {
            fprintf(stderr, 
                    "strange errors, should not happen, 294kda line %d\n",
                    end->token->line);
            exit(-1);
        }
        function_token = function_token->next;
    }
    end->token = function_token->next;
    
    return 0;
    
}



// end->token souuld be of type TOKEN_KEYWORD_RETURN
int end_function_return (end_t * end)
{
    #ifdef FUNCTION_DEBUG
        printf("function_return ... ");
    #endif
    end_symbol_t * symbol;
    
    // if return is followed by a symbol, we're returning that symbol and
    // we need to push it onto the stack
    if ((end->token->next->type != TOKEN_TERMINATOR) && (end->token->next->type != TOKEN_EOF))
    {
        end->token = end->token->next;
        symbol = end_evaluate(end);
        
        if (symbol == NULL)
        {
            fprintf(stderr, 
                    "could not return %s, symbol not found line %d\n",
                    symbol->name,
                    end->token->line);
            exit(-1);
        }
        // this symbol will be destroyed when this symbol_table goes out of
        // scope, so we need to copy it
        symbol = end_symbol_copy (symbol);
        // now push this copy on to the stack
        end_vm_stack_push(end->vm, symbol, 1);
    }
    
    // pop symbol_table off stack
    end_vm_symbol_table_stack_pop(end->vm);
    
    // jump back to where we were before
    end->token = end_vm_control_stack_get_retraddr(end->vm);
    #ifdef FUNCTION_DEBUG
        printf(" %d %s %d %s end\n", end->token->type, end->token->text, 
        end->token->next->type, end->token->next->text);
    #endif
    return 0;
}



end_function_t * end_function_copy (end_function_t * function)
{

    end_function_t * copy = (end_function_t *) malloc(sizeof(end_function_t));
    memcpy(copy, function, sizeof(end_function_t));
    return copy;

}