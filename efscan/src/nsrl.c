#include "nsrl.h"



nsrlfile * nsrlfile_open (char * filename)
{
	
	nsrlfile * n;
	
	n = malloc(sizeof(nsrlfile));
	if (n == NULL)
	{
		error_last = NSRL_ALLOCATE_ERROR;
		return NULL;
	}
	
	n->fh = fopen(filename, "r");
	if (n->fh == NULL)
	{
		error_last = NSRL_OPEN_FILE_ERROR;
		return NULL;
	}
	
	fseek(n->fh, 0, SEEK_SET);
	
	n->buffer_location = NSRL_BUFFER_SIZE;
	memset(n->buffer, 0, NSRL_BUFFER_SIZE);
	
	return n;
	
}



void nsrlfile_close (nsrlfile * n)
{
	fclose(n->fh);
	free (n);
}



/*
 * If the line_buffer_size is reached before a newline, any data that
 * was read into the line_buffer so far is lost
 */
int nsrl_get_line (FILE * fh, char * buffer, int buffer_size, int * buffer_location, char * line_buffer, int line_buffer_size)
{
	
	int fread_result;
	int line_buffer_i = 0;
	
	memset(line_buffer, 0, line_buffer_size);
	
	while (*buffer_location < buffer_size)
	{
		
		
		if ((buffer[*buffer_location] == '\n') || (buffer[*buffer_location] == (char) 0) || (buffer[*buffer_location] == '\r'))
		{
			line_buffer[line_buffer_i] = 0;
			(*buffer_location)++;
			return 0;
		}
		
		line_buffer[line_buffer_i++] = buffer[*buffer_location];
		
		if (line_buffer_i >= line_buffer_size - 1)
			return NSRL_LINE_BUFFER_OVERFLOW_ERROR;
			
		(*buffer_location)++;
		
	}
	
	// if we are here, we did not find a new line. we need to refill the
	// buffer and start over
	*buffer_location = 0;
	memset(buffer, 0, buffer_size);
	fread_result = fread(buffer, 1, buffer_size, fh);
	errno = 0;
	if ((fread_result <= 0) || (errno != 0))
		return NSRL_READ_ERROR;
	
	while (*buffer_location < buffer_size)
	{
		
		
		if ((buffer[*buffer_location] == '\n') || (buffer[*buffer_location] == (char) 0) || (buffer[*buffer_location] == '\r'))
		{
			line_buffer[line_buffer_i] = 0;
			(*buffer_location)++;
			return 0;
		}
		
		line_buffer[line_buffer_i++] = buffer[*buffer_location];
		
		if (line_buffer_i >= line_buffer_size - 1)
			return NSRL_LINE_BUFFER_OVERFLOW_ERROR;
			
		(*buffer_location)++;
		
	}
	
	return NSRL_GET_LINE_FAIL;
	
}



nsrlfile_record * nsrlfile_next (nsrlfile * n)
{
	
	int i;
	int j;
	char line_buffer[NSRL_BUFFER_SIZE];
	char column_buffer[1024];
	int column_buffer_i = 0;
	int column_i = 0;
	int result;
	
	// get line from nsrl file
	result = nsrl_get_line(n->fh, n->buffer, NSRL_BUFFER_SIZE, &(n->buffer_location), line_buffer, NSRL_BUFFER_SIZE);
	if (result != 0)
	{
		error_last = result;
		return NULL;
	}
	
	// start finding pieces
	memset(column_buffer, 0, 1024);
	for (i = 0; i < strlen(line_buffer); i++)
	{
		if ((line_buffer[i] != ',') || (line_buffer[i] == '\r') || (line_buffer[i] == '\n'))
			column_buffer[column_buffer_i++] = line_buffer[i];
		else
		{
			column_buffer[column_buffer_i] = 0;
			// sha 1 hash
			if (column_i == 0)
			{
				for (j = 0; j < 20; j++)
				{
					if (column_buffer[j] == (char) 0)
						break;
					n->next_record.sha1_hash[j] =  (unsigned char) base16_char_to_int (column_buffer[(j * 2) + 1]) * 16;
					n->next_record.sha1_hash[j] += (unsigned char) base16_char_to_int (column_buffer[(j * 2) + 2]);
				}
			}
			// md5 hash
			else if (column_i == 1)
			{
				for (j = 0; j < 16; j++)
				{
					if (column_buffer[j] == 0)
						break;
					n->next_record.md5_hash[j] =  (unsigned char) base16_char_to_int (column_buffer[(j * 2) + 1]) * 16;
					n->next_record.md5_hash[j] += (unsigned char) base16_char_to_int (column_buffer[(j * 2) + 2]);
				}
			}
			// 2 == crc
			// filename
			else if (column_i == 3)
			{
				strncpy(n->next_record.filename, &column_buffer[1], 260);
				// get rid of last "
				n->next_record.filename[strlen(n->next_record.filename) - 1] = 0;
			}
			// filesize
			else if (column_i == 4)
			{
				n->next_record.filesize = int64_t_atoi (column_buffer);
			}
			// product code
			else if (column_i == 5)
			{
				n->next_record.product_code = atoi(column_buffer);
				break;
			}
			
			memset(column_buffer, 0, 1024);
			column_buffer_i = 0;
			column_i++;
		}
	}
	
	return &(n->next_record);
	
}
	
	
