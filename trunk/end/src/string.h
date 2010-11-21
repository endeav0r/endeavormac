#ifndef string_HEADER
#define string_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct end_string_s
{
    char * value;
} end_string_t;



end_string_t * end_string_create  (char * value);
void           end_string_destroy (end_string_t * string);

int    end_string_assign (end_string_t * string, char * value);
char * end_string_get (end_string_t * string);



int end_string_add (end_string_t * a, end_string_t * b, end_string_t * c);
int end_string_compare (end_string_t * a, end_string_t * b);

#ifdef STRING_DEBUG
void end_string_debug (end_string_t * string);
#endif

#endif