#ifndef variable_HEADER
#define variable_HEADER

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct variable_s variable_t;

#include "vm.h"

#define TYPE_NUMBER  1
#define TYPE_POINTER 2
#define TYPE_NONE    3
#define TYPE_STRING  4

struct variable_s {
    int type;
    char * string;
    int number;
    struct variable_s * pointer;
    symbol_t * symbol;
};


variable_t * variable_create      (void * data, int type);
variable_t * variable_copy        (variable_t * variable);
void         variable_destroy     (variable_t * variable);

void         variable_set_symbol  (variable_t * variable, symbol_t * symbol);
symbol_t *   variable_get_symbol  (variable_t * variable);
void         variable_assign      (variable_t * a, variable_t * b);

void         variable_add         (variable_t * a, variable_t * b);
void         variable_subtract    (variable_t * a, variable_t * b);
void         variable_multiply    (variable_t * a, variable_t * b);
void         variable_divide      (variable_t * a, variable_t * b);
void         variable_modulus     (variable_t * a, variable_t * b);

variable_t * variable_dereference (variable_t * variable);

int          variable_compare     (variable_t * a, variable_t * b);

variable_t * variable_cast_string (variable_t * variable);

#endif