#include "number.h"



end_number_t * end_number_create (char * value)
{

	end_number_t * number;
	
	number = (end_number_t *) malloc(sizeof(end_number_t));
	
	end_number_assign(number, value);
	
	return number;

}



void end_number_destroy (end_number_t * number)
{
	free(number);
}



int end_number_assign (end_number_t * number, char * value)
{

	if (value == NULL)
		number->value = 0;
	else
		number->value = atoi(value);
	return 0;

}



char * end_number_get (end_number_t * number)
{

	int value;
	char * place;

	value = number->value;
	place = number->string_value;
	memset(number->string_value, 0, 16);

	snprintf(number->string_value, 16, "%d", number->value);
	
	return number->string_value;

}

int end_number_add (end_number_t * a,
                    end_number_t * b,
					end_number_t * c)
{
    #ifdef NUMBER_DEBUG
        printf("adding %d %d\n", b->value, c->value);
    #endif
	a->value = b->value + c->value;
	return 0;
}



int end_number_subtract (end_number_t * a,
                         end_number_t * b,
					     end_number_t * c)
{
	a->value = b->value - c->value;
	return 0;
}



int end_number_modulus (end_number_t * a, 
                        end_number_t * b,
                        end_number_t * c)
{
    a->value = b->value % c->value;
    return 0;
}



int end_number_divide (end_number_t * a,
                       end_number_t * b,
					   end_number_t * c)
{
	a->value = b->value / c->value;
	return 0;
}



int end_number_multiply (end_number_t * a,
                         end_number_t * b,
					     end_number_t * c)
{
	a->value = b->value * c->value;
	return 0;
}



int end_number_compare (end_number_t * a, end_number_t * b)
{
	if (a->value < b->value)
		return -1;
	else if (a->value > b->value)
		return 1;
	return 0;
}