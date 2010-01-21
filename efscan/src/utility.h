#ifndef utility_H
#define utility_H

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

extern char itoa_result[64];

char int_to_hex (int i);

// result must not be freed
char * int64_t_itoa (int64_t n);
char * time_t_itoa (time_t t);
char * int_itoa (int i);

// result must be freed
char * base16_string (unsigned char * data, int data_len);

// result must be freed
char * sqlite_escape_string (char * s);

#endif
