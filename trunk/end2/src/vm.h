#ifndef vm_HEADER
#define vm_HEADER

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct symbol_s symbol_t;

#include "lexer.h"
#include "variable.h"

#define OPCODE_NONE   0
#define OPCODE_ADD    1
#define OPCODE_PRINT  2
#define OPCODE_ASSIGN 3


typedef struct stack_s {
    variable_t * variable;
    struct stack_s * next;
} stack_t;


typedef struct opcodes_s {
    int opcode;
    struct opcodes_s * next;
} opcodes_t;


struct symbol_s {
    char * name;
    variable_t * value;
    struct symbol_s * left;
    struct symbol_s * right;
};


typedef struct frame_s {
    opcodes_t * opcodes;
    symbol_t * symbols;
    struct frame_s * next;
} frame_t;


typedef struct vm_s {
    stack_t * stack;
	frame_t * frame;
} vm_t;



vm_t *       vm_create  ();
void         vm_destroy (vm_t * vm);

void         vm_assign (vm_t * vm);
int          vm_execute (vm_t * vm);

void         vm_stack_push        (vm_t * vm, variable_t * variable);
variable_t * vm_stack_pop         (vm_t * vm);

void         vm_opcodes_push (vm_t * vm, int opcode);
int          vm_opcodes_pop  (vm_t * vm);

symbol_t *   symbol_create (char * name, variable_t * variable);
void         symbol_destroy (symbol_t * symbol);
symbol_t *   vm_symbol_get (vm_t * vm, char * name);
void         vm_symbol_insert (vm_t * vm, symbol_t * symbol);


void         vm_frame_push (vm_t * vm);
void         vm_frame_pop  (vm_t * vm);

#endif