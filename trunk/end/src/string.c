#include "string.h"



end_string_t * end_string_create (char * value)
{

    end_string_t * string;
    
    string = (end_string_t *) malloc(sizeof(end_string_t));
    
    if (value == NULL)
        string->value = NULL;
    else
    {
        string->value = (char *) malloc(strlen(value) + 1);
        strcpy(string->value, value);
    }
    
    return string;
    
}



void end_string_destroy (end_string_t * string)
{

    if (string->value != NULL)
        free(string->value);
    
    free(string);
    
}



int end_string_assign (end_string_t * string, char * value)
{

    if (string->value != NULL)
        free(string->value);
     
    string->value = (char *) malloc(strlen(value) + 1);
    strcpy(string->value, value);
    
    return 0;
    
}



char * end_string_get (end_string_t * string)
{
    #ifdef STRING_DEBUG
        printf("end_string_get: %s\n", string->value);
    #endif
    return string->value;
}



int end_string_add (end_string_t * a, end_string_t * b, end_string_t * c)
{

    if (a->value != NULL)
        free(a->value);
    
    a->value = malloc(strlen(b->value) + strlen(c->value) + 1);
    strcpy(a->value, b->value);
    strcat(a->value, c->value);
    
    return 0;
    
}



int end_string_compare (end_string_t * a, end_string_t * b)
{
    if (a->value == NULL)
    {
        if (b->value == NULL)
            return 0;
        return 1;
    }
    if (b->value == NULL)
        return -1;
    return strcmp(a->value, b->value);
}



#ifdef STRING_DEBUG
void end_string_debug (end_string_t * string)
{
    printf("end_string_debug\n");
    printf(" value ptr: %p\n value: %s\n", string->value, string->value);
}
#endif