#include "utility.h"

char itoa_result[64];


char * sqlite_escape_string (char * s)
{
	int result_size;
	int result_i = 0;
	int s_i;
	char * result;
	char * result_realloc = NULL;
	
	result_size = strlen(s);
	result = malloc(result_size);
	if (result == NULL)
		return NULL;

	for (s_i = 0; s_i < strlen(s); s_i++)
	{
		
		if (result_i >= result_size - 4)
		{
			result_realloc = realloc(result, result_size + 1024);
			if (result_realloc == NULL)
			{
				free(result);
				return NULL;
			}
			result = result_realloc;
			result_size += 1024;
		}
		
		if (s[s_i] == '\'')
		{
			result[result_i++] = '\'';
			result[result_i++] = '\'';
		}
		else
		{
			result[result_i++] = s[s_i];
		}
		
	}
	
	result[result_i] = 0;
	
	return result;
}
			


char int_to_hex (int i)
{
	if ((i >= 0) && (i <= 9))
		return '0' + ((char) i);
	else if ((i >= 10) && (i < 16))
		return 'a' + (char) (i - 10);
	else
		return (char) 0;
}



int base16_char_to_int (unsigned char c)
{
	
	if ((c >= '0') && (c <= '9'))
		return (int) (c - '0');
	else if ((c >= 'a') && (c <= 'f'))
		return ((int) (c - 'a')) + 10;
	else if ((c >= 'A') && (c <= 'F'))
		return ((int) (c - 'A')) + 10;
	else
		return -1;
		
}
		


// function is not completely safe
int64_t int64_t_atoi (char * s)
{
	
	int i;
	int64_t multiplier = 1;
	int64_t result = 0;
	
	for (i = strlen(s) - 1; i >= 0; i--)
	{
		
		if ((s[i] >= '0') || (s[i] <= '9'))
		{
			result += ((int64_t) (s[i] - '0')) * multiplier;
			multiplier *= (int64_t) 10;
		}
		
	}

	if (s[0] == '-')
		result *= -1;
	
	return result;
	
}



char * base16_string (unsigned char * data, int data_len)
{
	
	int data_i;
	int result_i;
	
	char * result = malloc((data_len * 2) + 1);
	
	
	result_i = 0;
	for (data_i = 0; data_i < data_len; data_i++)
	{
		result[result_i++] = int_to_hex(((int) data[data_i]) / 16);
		result[result_i++] = int_to_hex(((int) data[data_i]) % 16);
	}
	result[result_i] = 0;
	
	return result;
	
}



char * time_t_itoa (time_t t)
{
	sprintf(itoa_result, "%d", (int) t);
	return itoa_result;
}



char * int_itoa (int i)
{
	sprintf(itoa_result, "%d", i);
	return itoa_result;
}


char * int64_t_itoa (int64_t n)
{
	#ifdef ARCH_64
		sprintf(itoa_result, "%ld", n);
	#else
		sprintf(itoa_result, "%lld", n);
	#endif
	return itoa_result;	
}
