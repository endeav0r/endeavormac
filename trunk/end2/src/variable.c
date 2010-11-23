#include "variable.h"



variable_t * variable_create (void * data, int type) {
    variable_t * variable;
    
    variable = (variable_t *) malloc(sizeof(variable_t));
    
    switch (type) {
        case TYPE_NUMBER :
            variable->number = atoi((char *) data);
            break;
        case TYPE_STRING :
            variable->string = (char *) malloc(strlen(data) + 1);
            strcpy(variable->string, data);
            break;
        case TYPE_POINTER :
            variable->pointer = data;
            break;
    }
    
    variable->symbol = NULL;
    variable->type = type;
    
    return variable;
}


variable_t * variable_copy (variable_t * variable) {
    variable_t * copy;
    
    copy = (variable_t *) malloc(sizeof(variable_t));
    
    switch (variable->type) {
        case TYPE_NUMBER :
            copy->number = variable->number;
            break;
        case TYPE_POINTER :
            copy->pointer = variable->pointer;
            break;
        case TYPE_STRING :
            copy->string = (char *) malloc(strlen(variable->string) + 1);
            strcpy(copy->string, variable->string);
            break;
    }
    copy->symbol = variable->symbol;
    copy->type = variable->type;
    
    return copy;
}


void variable_destroy (variable_t * variable) {
    if (variable->type == TYPE_STRING)
        free(variable->string);
    free(variable);
}


void variable_set_symbol (variable_t * variable, symbol_t * symbol) {
    variable->symbol = symbol;
}


symbol_t * variable_get_symbol (variable_t * variable) {
    return variable->symbol;
}


void variable_assign (variable_t * a, variable_t * b) {
    
    if (a->type == TYPE_STRING)
        free(a->string);
        
    switch (b->type) {
        case TYPE_NUMBER :
            a->number = b->number;
            break;
        case TYPE_STRING :
            a->string = (char *) malloc(strlen(b->string) + 1);
            strcpy(a->string, b->string);
        case TYPE_POINTER :
            a->pointer = b->pointer;
            break;
    }
    a->type = b->type;
}


void variable_add (variable_t * a, variable_t * b) {
    if (a->type != b->type) {
        fprintf(stderr, "add between different types\n");
        exit(1);
    }
    
    switch (a->type) {
        case TYPE_NUMBER :
            a->number += b->number;
            break;
        case TYPE_STRING :
            a->string = realloc(a->string, strlen(a->string) + strlen(b->string) + 1);
            strcat(a->string, b->string);
            break;
        default :
            fprintf(stderr, "add on incompatible type\n");
            exit(1);
    }
}


void variable_subtract (variable_t * a, variable_t * b) {
    if (a->type != b->type) {
        fprintf(stderr, "subtract between different types\n");
        exit(1);
    }
    
    switch (a->type) {
        case TYPE_NUMBER :
            a->number -= b->number;
            break;
        default :
            fprintf(stderr, "add on incompatible type\n");
            exit(1);
    }
}


void variable_multiply (variable_t * a, variable_t * b) {
    if (a->type != b->type) {
        fprintf(stderr, "subtract between different types\n");
        exit(1);
    }
    
    switch (a->type) {
        case TYPE_NUMBER :
            a->number *= b->number;
            break;
        default :
            fprintf(stderr, "add on incompatible type\n");
            exit(1);
    }
}


void variable_modulus (variable_t * a, variable_t * b) {
    if (a->type != b->type) {
        fprintf(stderr, "subtract between different types\n");
        exit(1);
    }
    
    switch (a->type) {
        case TYPE_NUMBER :
            a->number %= b->number;
            break;
        default :
            fprintf(stderr, "add on incompatible type\n");
            exit(1);
    }
}


void variable_divide (variable_t * a, variable_t * b) {
    if (a->type != b->type) {
        fprintf(stderr, "subtract between different types\n");
        exit(1);
    }
    
    switch (a->type) {
        case TYPE_NUMBER :
            a->number /= b->number;
            break;
        default :
            fprintf(stderr, "add on incompatible type\n");
            exit(1);
    }
}


variable_t * variable_dereference (variable_t * variable) {
    if (variable->type != TYPE_POINTER) 
        return variable;
    return variable->pointer;
}


int variable_compare (variable_t * a, variable_t * b) {
    if (a->type != b->type) {
        fprintf(stderr, "comparison between incompatible types\n");
        exit(1);
    }
    
    switch (a->type) {
        case TYPE_NUMBER :
            return a->number - b->number;
        case TYPE_POINTER :
            return a->pointer - b->pointer;
        case TYPE_STRING :
            return strcmp(a->string, b->string);
        case TYPE_NONE :
            return 0;
    }
    
    exit(9000);
}
    


variable_t * variable_cast_string (variable_t * castee) {
    char buf[24];
    switch (castee->type) {
        case TYPE_NUMBER :
            snprintf(buf, 24, "%d", castee->number);
            return variable_create(buf, TYPE_STRING);
        case TYPE_NONE :
            snprintf(buf, 24, "NONE");
            return variable_create(buf, TYPE_STRING);
        case TYPE_POINTER :
            snprintf(buf, 24, "%p", castee->pointer);
            return variable_create(buf, TYPE_STRING);
        case TYPE_STRING :
            return variable_create(castee->string, TYPE_STRING);
    }
    return NULL;
}