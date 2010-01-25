#include <stdio.h>
#include <stdlib.h>

#include "utility.h"
#include "nsrl.h"



int main (int argc, char * argv[])
{

	int i;
	nsrlfile * n;
	nsrlfile_record * record;
	char * tmp_sha1_string;
	char * tmp_md5_string;

	if (argc != 2)
	{
		printf ("usage: %s <nsrlfile>\n", argv[0]);
		return -1;
	}
	
	n = nsrlfile_open(argv[1]);
	record = nsrlfile_next(n);
	
	for(i = 0; i < 10000000; i++)
	{
		record = nsrlfile_next(n);
		if (record == NULL)
		{
			printf("null file record\n");
			error_print_description(error_last);
			break;
		}
		
		tmp_sha1_string = base16_string(record->sha1_hash, 20);
		tmp_md5_string = base16_string(record->md5_hash, 16);
		
		printf("%s\n%s\n%s\n%lld\n%d\n\n", tmp_sha1_string, tmp_md5_string, record->filename, record->filesize, record->product_code);
	
		free(tmp_sha1_string);
		free(tmp_md5_string);
		
	}
	
	nsrlfile_close(n);
	
	return 1;
	
}
