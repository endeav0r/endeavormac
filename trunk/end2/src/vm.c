#include "vm.h"


vm_t * vm_create () {
    vm_t * vm;
    
    vm = (vm_t *) malloc(sizeof(vm_t));
    vm->frame = NULL;
    vm->stack = NULL;
    vm_frame_push(vm);
    
    return vm;
}


void vm_destroy (vm_t * vm) {
    vm_frame_pop(vm);
    
    if (vm->stack != NULL)
        fprintf(stderr, "destroying vm with non-null stack\n");
    if (vm->frame != NULL)
        fprintf(stderr, "destroying vm with non null frame\n");
    free(vm);
}


void vm_assign (vm_t * vm) {
    variable_t * lhs;
    variable_t * rhs;
    symbol_t * symbol;
    
    rhs = vm_stack_pop(vm);
    lhs = vm_stack_pop(vm);
    if (lhs->symbol == NULL) {
        fprintf(stderr, "assignment attempted on non-symbol\n");
        exit(1);
    }
    symbol = lhs->symbol;
    variable_destroy(lhs);
    lhs = symbol->value;
    variable_assign(lhs, rhs);
    variable_destroy(rhs);
}


token_t * vm_execute (vm_t * vm) {
    int opcode;
    variable_t * a, * b;
    token_t * jmp_token = NULL;
    
    while ((opcode = vm_opcodes_pop(vm)) != OPCODE_HALT) {
        //printf("OPCODE %d\n", opcode);
        switch (opcode) {
        
            case OPCODE_ADD :
            case OPCODE_SUBTRACT :
            case OPCODE_MULTIPLY :
            case OPCODE_DIVIDE :
            case OPCODE_MODULUS :
                a = vm_stack_pop(vm);
                b = vm_stack_pop(vm);
                if (opcode == OPCODE_ADD)
                    variable_add(a, b);
                if (opcode == OPCODE_SUBTRACT)
                    variable_subtract(a, b);
                if (opcode == OPCODE_MULTIPLY)
                    variable_multiply(a, b);
                if (opcode == OPCODE_DIVIDE)
                    variable_divide(a, b);
                if (opcode == OPCODE_MODULUS)
                    variable_modulus(a, b);
                vm_stack_push(vm, a);
                variable_destroy(b);
                break;
                
            case OPCODE_PRINT :
                a = vm_stack_pop(vm);
                b = variable_cast_string(a);
                printf("%s\n", b->string);
                variable_destroy(a);
                variable_destroy(b);
                break;
                
            case OPCODE_EQ :
            case OPCODE_LT :
            case OPCODE_GT :
                a = vm_stack_pop(vm);
                b = vm_stack_pop(vm);
                if ((opcode == OPCODE_EQ) && (variable_compare(a, b) != 0))
                    jmp_token = vm_stack_pop(vm);
                else if ((opcode == OPCODE_LT) && (variable_compare(a, b) <= 0))
                    jmp_token = vm_stack_pop(vm);
                else if ((opcode == OPCODE_GT) && (variable_compare(a, b) >= 0))
                    jmp_token = vm_stack_pop(vm);
                else
                    vm_stack_pop(vm);
                variable_destroy(a);
                variable_destroy(b);
                break;
                
            case OPCODE_JMP :
                jmp_token = vm_stack_pop(vm);
                break;
            
            case OPCODE_ASSIGN :
                vm_assign(vm);
                break;
        }
            
    }
    return jmp_token;
}



void vm_stack_push (vm_t * vm, void * variable) {
	stack_t * stack;
    //printf("STACK PUSH\n");
    stack = (stack_t *) malloc(sizeof(stack_t));
    stack->data = variable;
    stack->next = vm->stack;
    vm->stack = stack;
    

}


void * vm_stack_pop (vm_t * vm) {
	stack_t * stack;
	void * data;
	//printf("STACK POP\n");
	
	stack = vm->stack;
	vm->stack = vm->stack->next;
	
	data = stack->data;
	free(stack);

	return data;
}


void vm_opcodes_push (vm_t * vm, int opcode) {
	opcodes_t * opcodes;
	
	opcodes = (opcodes_t *) malloc(sizeof(opcodes_t));
	opcodes->opcode = opcode;
	opcodes->next = vm->frame->opcodes;
	vm->frame->opcodes = opcodes;
}


int vm_opcodes_pop (vm_t * vm) {
	int opcode;
	opcodes_t * opcodes;
	
    if (vm->frame->opcodes == NULL)
        return OPCODE_HALT;
    
	opcodes = vm->frame->opcodes;
	vm->frame->opcodes = vm->frame->opcodes->next;
	
	opcode = opcodes->opcode;
	free(opcodes);
	
	return opcode;
}


symbol_t * symbol_create (char * name, variable_t * variable) {
    symbol_t * symbol;
    
    symbol = (symbol_t *) malloc(sizeof(symbol_t));
    symbol->name = (char *) malloc(strlen(name) + 1);
    strcpy(symbol->name, name);
    symbol->value = variable;
    symbol->left = NULL;
    symbol->right = NULL;
    
    return symbol;
}


void symbol_destroy (symbol_t * symbol) {
    if (symbol == NULL)
        return;
    symbol_destroy(symbol->left);
    symbol_destroy(symbol->right);
    variable_destroy(symbol->value);
    free(symbol->name);
    free(symbol);
}


symbol_t * vm_symbol_get (vm_t * vm, char * name) {
    symbol_t * node;
    frame_t * frame;
    
    frame = vm->frame;
    while (frame != NULL) {
        node = frame->symbols;
        while (node != NULL) {
            if (strcmp(name, node->name) < 0)
                node = node->left;
            else if (strcmp(name, node->name) > 0)
                node = node->right;
            else
                return node;
        }
        frame = frame->next;
    }
    
    return NULL;
}


void vm_symbol_insert (vm_t * vm, symbol_t * symbol) {
    symbol_t * node;
    
    if (vm->frame->symbols == NULL)
        vm->frame->symbols = symbol;
    else {
        node = vm->frame->symbols;
        while (node != NULL) {
            if (strcmp(symbol->name, node->name) < 0) {
                if (node->left == NULL) {
                    node->left = symbol;
                    break;
                }
                else
                    node = node->left;
            }
            else if (strcmp(symbol->name, node->name) > 0) {
                if (node->right == NULL) {
                    node->right = symbol;
                    break;
                }
                else
                    node = node->right;
            }
            else {
                fprintf(stderr, "INTERNAL ERROR: symbol_insert symbol already exists\n");
                exit(1);
            }
        }
    }
}
        


void vm_frame_push (vm_t * vm) {
    frame_t * frame;
    
    frame = (frame_t * ) malloc(sizeof(frame_t));
    frame->opcodes = NULL;
    frame->symbols = NULL;
    frame->next = vm->frame;
    vm->frame = frame;
}


void vm_frame_pop (vm_t * vm) {
    frame_t * frame;
    
    frame = vm->frame;
    vm->frame = vm->frame->next;
    
    if (frame->opcodes != NULL)
        fprintf(stderr, "destroying frame with non-null opcodes\n");
    
    symbol_destroy(frame->symbols);
    free(frame);
}