#ifndef function_HEADER
#define function_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "end.h"
#include "symbol.h"
#include "vm.h"

typedef struct end_function_s
{
    int arguments;
    end_token_t * token;
} end_function_t;



end_symbol_t * end_function_create  (end_t * end);
void           end_function_destroy (end_function_t * function);

int           end_function_assign (end_function_t * function, end_token_t * token);
end_token_t * end_function_get    (end_function_t * function);

// 
int end_function_call (end_t * end, end_symbol_t * symbol);
int end_function_return (end_t * end);

end_function_t * end_function_copy (end_function_t * function);

#endif