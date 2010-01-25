#ifndef error_H
#define error_H

#include <stdio.h>

int error_last;

#define NSRL_ALLOCATE_ERROR             101
#define NSRL_OPEN_FILE_ERROR            102
#define NSRL_READ_ERROR                 103
#define NSRL_EOF                        104
#define NSRL_LINE_BUFFER_OVERFLOW_ERROR 105
#define NSRL_GET_LINE_FAIL              106

void error_print_description (int description);


#endif
