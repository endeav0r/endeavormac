#include <stdio.h>

#include "end.h"


void debug_symbol_node (end_symbol_node_t * symbol_node)
{

	printf("%02d %s\n", symbol_node->symbol->type, symbol_node->symbol->name);
	if (symbol_node->left != NULL)
	{
		printf("LEFT <-\n");
		debug_symbol_node(symbol_node->left);
	}
	if (symbol_node->right != NULL)
	{
		printf("RIGHT ->\n");
		debug_symbol_node(symbol_node->right);
	}
	printf("UP /\\\n");

}



void debug_symbol_table (end_symbol_table_t * symbol_table)
{
	debug_symbol_node(symbol_table->top);
}



void debug_vm (end_vm_t * vm)
{

	end_vm_symbol_table_stack_t * symbol_table_stack;
	end_vm_symbol_stack_t * symbol_stack;

	printf("debug_vm\n");
	printf("end_vm_symbol_table_stack\n");
	
	symbol_table_stack = vm->symbol_table_stack;
	while (symbol_table_stack != NULL)
	{
		printf("FRAME\n");
		debug_symbol_table(symbol_table_stack->symbols);
		symbol_table_stack = symbol_table_stack->next;
	}
	
	printf("end_vm_symbol_stack\n");
	symbol_stack = vm->symbol_stack;
	while (symbol_stack != NULL)
	{
		printf("%d %s\n", symbol_stack->symbol->type, symbol_stack->symbol->name);
		symbol_stack = symbol_stack->next;
	}
	
}



int main ()
{

	end_t * end;
	
	end = end_create();
	
	end_parse(end, "test = 17\ntest = test + 5\nprint test");
	
	debug_vm(end->vm);
	
	return 0;

}