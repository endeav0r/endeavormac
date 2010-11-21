#include "vm.h"
#include "end.h"


/************************
* VM
*/

end_vm_t * end_vm_create ()
{


	end_vm_t * vm;
	
	vm = (end_vm_t *) malloc(sizeof(end_vm_t));
	vm->symbol_table_stack = NULL;
	vm->symbol_stack = NULL;
	// vm->result = NULL;
    vm->control_stack = NULL;
	
	return vm;
	
}



void end_vm_destroy (end_vm_t * vm)
{

	end_vm_symbol_table_stack_t * symbol_table_stack;
	
	symbol_table_stack = vm->symbol_table_stack;
	while (symbol_table_stack != NULL)
	{
		end_vm_symbol_table_stack_destroy(symbol_table_stack);
		symbol_table_stack = symbol_table_stack->next;
	}
	
	free(vm);
	
}



// doesn't copy, so don't free
/*
int end_vm_set_result (end_vm_t * vm, end_symbol_t * result)
{
	if (vm->result != NULL)
		end_symbol_destroy(vm->result);
	vm->result = result;
	return 0;
}
*/
		


/**************************
* symbol_table_stack
*/



void end_vm_symbol_table_stack_destroy (end_vm_symbol_table_stack_t * stack)
{

	if (stack == NULL)
		return;
	end_symbol_table_destroy(stack->symbols);
	end_vm_symbol_table_stack_destroy(stack->next);
	free(stack);
	
}



end_symbol_t * end_vm_symbol_get (end_vm_t * vm, char * name)
{
	
	end_vm_symbol_table_stack_t * stack;
	end_symbol_t * symbol = NULL;
	
	stack = vm->symbol_table_stack;
	
	while (stack != NULL)
	{
		symbol = end_symbol_table_get(stack->symbols, name);
		if (symbol != NULL)
			break;
		stack = stack->next;
	}
	
	return symbol;
	
}



int end_vm_symbol_table_stack_pop (end_vm_t * vm)
{

	end_vm_symbol_table_stack_t * stack;
	
	if (vm->symbol_table_stack == NULL)
		return -1;
		
	stack = vm->symbol_table_stack;
	vm->symbol_table_stack = stack->next;
	
	end_symbol_table_destroy(stack->symbols);
	free(stack);
	
	return 0;
	
}



int end_vm_symbol_table_stack_push (end_vm_t * vm)
{

	end_vm_symbol_table_stack_t * stack;
	
	stack = (end_vm_symbol_table_stack_t *) malloc(sizeof(end_vm_symbol_table_stack_t));
	
	stack->symbols = end_symbol_table_create();
	stack->next = vm->symbol_table_stack;
	vm->symbol_table_stack = stack;

	return 0;

}



int end_vm_symbol_set (end_vm_t * vm, end_symbol_t * symbol)
{
	end_vm_symbol_table_stack_t * symbol_table_stack;
	end_symbol_t * existing_symbol;

	// find out if this symbol already exists in a symbol_table stack
	symbol_table_stack = vm->symbol_table_stack;
	while (symbol_table_stack != NULL)
	{
		existing_symbol = end_symbol_table_get(symbol_table_stack->symbols, symbol->name);
		if (existing_symbol != NULL)
		{
			end_symbol_table_assign(symbol_table_stack->symbols, symbol);
			return 0;
		}
		symbol_table_stack = symbol_table_stack->next;
	}
	
	end_symbol_table_assign(vm->symbol_table_stack->symbols, symbol);
	
	return 0;
}



int end_vm_symbol_set_local (end_vm_t * vm, end_symbol_t * symbol)
{

	end_symbol_table_assign(vm->symbol_table_stack->symbols, symbol);
	
	return 0;
}
		



/**********************
* symbol_stack
*/



int end_vm_stack_push (end_vm_t * vm, end_symbol_t * symbol, int free)
{

	end_vm_symbol_stack_t * new;
	
    #ifdef VM_DEBUG
        printf("vm_stack_push %d\n", free);
    #endif
	new = malloc(sizeof(end_vm_symbol_stack_t));
	new->symbol = symbol;
	new->free = free;
	new->next = vm->symbol_stack;
	vm->symbol_stack = new;
	
	return 0;
	
}
	

end_symbol_t * end_vm_stack_get_symbol (end_vm_t * vm)
{
	if (vm->symbol_stack == NULL)
		return NULL;
	else
		return vm->symbol_stack->symbol;
}



end_vm_symbol_stack_t * end_vm_stack_pop (end_vm_t * vm)
{
	end_vm_symbol_stack_t * stack;
	
    #ifdef VM_DEBUG
        printf("vm_stack_pop\n");
    #endif
	if (vm->symbol_stack != NULL)
	{
		stack = vm->symbol_stack;
		vm->symbol_stack = vm->symbol_stack->next;
		return stack;
	}
	else
		return NULL;
	
}



void end_vm_symbol_stack_destroy (end_vm_symbol_stack_t * stack)
{
	if (stack == NULL)
		return;
	
	if (stack->free)
		end_symbol_destroy(stack->symbol);
	
	free(stack);
}



/**********************************
* control_stack
*/



int end_vm_control_stack_push (end_vm_t * vm, end_token_t * token)
{

    end_vm_control_stack_t * control_stack;
    
    control_stack = (end_vm_control_stack_t *) malloc(sizeof(end_vm_control_stack_t));

    control_stack->next = vm->control_stack;
    vm->control_stack = control_stack;
    
    control_stack->token = token;
    control_stack->autopop = 0;
    
    return 0;
}



int end_vm_control_stack_set_autopop (end_vm_t * vm, int autopop)
{
    vm->control_stack->autopop = autopop;
    return 0;
}



int end_vm_control_stack_set_retraddr (end_vm_t * vm, end_token_t * token)
{
    vm->control_stack->retraddr = token;
    return 0;
}



end_token_t * end_vm_control_stack_get (end_vm_t * vm)
{
    return vm->control_stack->token;
}



end_token_t * end_vm_control_stack_get_retraddr (end_vm_t * vm)
{
    return vm->control_stack->retraddr;
}



void end_vm_control_stack_pop (end_vm_t * vm)
{

    end_vm_control_stack_t * control_stack;
    
    if (vm->control_stack == NULL)
    {
        fprintf(stderr, "INTERNAL ERROR: tried to pop NULL control stack\n");
        exit(-1);
    }
    
    control_stack = vm->control_stack;
    vm->control_stack = vm->control_stack->next;
    free(control_stack);
    
    while (vm->control_stack != NULL)
    {
        if (vm->control_stack->autopop == 0)
            break;
        control_stack = vm->control_stack;
        vm->control_stack = vm->control_stack->next;
        free(control_stack);
    }
}