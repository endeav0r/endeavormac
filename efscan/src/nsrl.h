#ifndef nsrl_H
#define nsrl_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <inttypes.h>
#include "error.h"
#include "utility.h"

#define NSRL_BUFFER_SIZE 4096


typedef struct
{
	unsigned char sha1_hash[20];
	unsigned char md5_hash[16];
	char filename[261];
	int64_t filesize; // 2^63 = many petabytes
	int product_code;
} nsrlfile_record;



typedef struct
{
	FILE * fh;
	char buffer[NSRL_BUFFER_SIZE];
	int buffer_location;
	nsrlfile_record next_record;
} nsrlfile;



nsrlfile * nsrlfile_open (char * filename);
nsrlfile_record * nsrlfile_next (nsrlfile * n);
void nsrlfile_close (nsrlfile * n);
int nsrl_get_line (FILE * fh, char * buffer, int buffer_size, int * buffer_location, char * line_buffer, int line_buffer_size);

#endif
