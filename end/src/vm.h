#ifndef vm_HEADER
#define vm_HEADER

#include "lexer.h"
#include "number.h"
#include "symbol.h"


// temporary store for variables when evaluating expressions, returning, etc
// works pretty much exactly like a real stack
typedef struct end_vm_symbol_stack_s
{
	end_symbol_t *                 symbol;
	int                            free;
	struct end_vm_symbol_stack_s * next;
} end_vm_symbol_stack_t;


// symbol stack is used to keep track of NAMED symbols
// as scope changes, we push a new symbol_table on to the symbol_table stack
// by search through the stack we can search outwards in scope
typedef struct end_vm_symbol_table_stack_s
{
	end_symbol_table_t *                 symbols;
	struct end_vm_symbol_table_stack_s * next;
} end_vm_symbol_table_stack_t;


// the execution stack is used to help keep track of control blocks
// when we have a new control block, we push the token which begins this control
// block on to the stack
// when we encounter a TOKEN_END, we take a look at the top of the stack to
// decide what to do
// the type of the control stack can be determined by looking at the type of the
// first token
// auto_pop says when the element above this is popped, automatically pop this
// element too. This is how we deal with nested if else if else if else end
// statements.
typedef struct end_vm_control_stack_s
{
    int autopop;
    end_token_t * token; // token that started this control block
    end_token_t * retraddr; // return address for functions
    struct end_vm_control_stack_s * next;
} end_vm_control_stack_t;


typedef struct end_vm_s
{
	end_vm_symbol_table_stack_t  * symbol_table_stack;
	end_vm_symbol_stack_t * symbol_stack;
    end_vm_control_stack_t * control_stack;
	
	//end_symbol_t * result; // think of it like a result register
} end_vm_t;



end_vm_t * end_vm_create  ();
void       end_vm_destroy (end_vm_t * vm);
int        end_vm_set_result (end_vm_t * vm, end_symbol_t * result);

end_symbol_t * end_vm_symbol_get (end_vm_t * vm, char * name);
int            end_vm_symbol_table_stack_pop  (end_vm_t * vm);
int            end_vm_symbol_table_stack_push (end_vm_t * vm);
int            end_vm_symbol_set (end_vm_t * vm, end_symbol_t * symbol);
int            end_vm_symbol_set_local (end_vm_t * vm, end_symbol_t * symbol);

void end_vm_symbol_table_stack_destroy (end_vm_symbol_table_stack_t * stack);

int                     end_vm_stack_push           (end_vm_t * vm, end_symbol_t * symbol, int free);
end_symbol_t *          end_vm_stack_get_symbol     (end_vm_t * vm);
end_vm_symbol_stack_t * end_vm_stack_pop            (end_vm_t * vm);
void                    end_vm_symbol_stack_destroy (end_vm_symbol_stack_t * stack);

int           end_vm_control_stack_push (end_vm_t * vm, end_token_t * token);
int           end_vm_control_stack_set_retraddr (end_vm_t * vm, end_token_t * token);
int           end_vm_control_stack_set_autopop (end_vm_t * vm, int autopop);
end_token_t * end_vm_control_stack_get (end_vm_t * vm);
end_token_t * end_vm_control_stack_get_retraddr (end_vm_t * vm);
void          end_vm_control_stack_pop (end_vm_t * vm);

#endif