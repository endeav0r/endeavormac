#ifndef number_HEADER
#define number_HEADER


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct end_number_s
{
	int value;
	char string_value[16];
} end_number_t;


end_number_t * end_number_create  (char * value);
void           end_number_destroy (end_number_t * number);

int    end_number_assign (end_number_t * number, char * value);
char * end_number_get    (end_number_t * number);

// a = b OPERATION c
// a, b and c must all be instantiated
int end_number_add (end_number_t * a,
                    end_number_t * b,
					end_number_t * c);
					
int end_number_subtract (end_number_t * a,
                         end_number_t * b,
					     end_number_t * c);
                         
int end_number_modulus (end_number_t * a,
                        end_number_t * b,
                        end_number_t * c);
                        
int end_number_divide (end_number_t * a,
                       end_number_t * b,
                       end_number_t * c);
                        
int end_number_multiply (end_number_t * a,
                         end_number_t * b,
                         end_number_t * c);
						 
int end_number_compare (end_number_t * a, end_number_t * b);

#endif