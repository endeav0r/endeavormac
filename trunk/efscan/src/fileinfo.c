#include "fileinfo.h"


int file_info (char * path, file_info_t * fi)
{
	
	int result;
	int errors = 0;
	int i;
	
	struct stat stat_buffer;
	
	// get stat info about file
	result = stat((const char *) path, &stat_buffer);
	
	if (result != 0)
		return -1;
		
	memset(fi->name, 0, 1024);
	memset(fi->path, 0, 4096);
	
	fi->user_id = (int) stat_buffer.st_uid;
	fi->group_id = (int) stat_buffer.st_gid;
	fi->size = (int64_t) stat_buffer.st_size;
	fi->atime = stat_buffer.st_atime;
	fi->mtime = stat_buffer.st_mtime;
	fi->ctime = stat_buffer.st_ctime;
	
	// check for type of file
	if (S_ISDIR(stat_buffer.st_mode) != 0)
	{
		fi->type = FILE_TYPE_DIRECTORY;
		strncpy(fi->path, path, 4095);
	}
	else
	{
		/* MinGW currently being retarded
		if (S_ISLNK(stat_buffer.st_mode) == 0)
		{
			fi->type = FILE_TYPE_LINK;
		}
		else*/ if (S_ISREG(stat_buffer.st_mode) != 0)
		{
			fi->type = FILE_TYPE_FILE;
			
			// get hash sums
			result = md5_file(path, fi->md5_sum);
			if (result > 0)
				errors |= FILE_INFO_MD5_ERROR;
			
			result = sha1_file(path, fi->sha1_sum);
			if (result > 0)
				errors |= FILE_INFO_SHA1_ERROR;
				
		}
		else
			fi->type = FILE_TYPE_IRREGULAR;
		// seperate filename from path by last '/'|'\\'
		for (i = strlen(path) - 1; i >= 0; i--)
		{
			if ((path[i] == '/') || (path[i] == '\\'))
			{
				memset(fi->name, strlen(path) - i + 1, 0);
				strncpy(fi->name, &path[i + 1], 1023);
				memset(fi->path, i + 1, 0);
				if (i > 4095)
					strncpy(fi->path, path, 4095);
				else
					strncpy(fi->path, path, i);
				break;
			}
		}
	}
	
	// if path was not set, then this is a file,
	// ie: "%s test.txt", argv[0]
	if (fi->path[0] == 0)
	{
		strncpy(fi->name, path, 1023);
	}
	
	return errors;

}



void file_info_debug_output (file_info_t * fi)
{

	char * tmp_s;

	printf("fi.user_id	: %d\n", fi->user_id);
	printf("fi.group_id	: %d\n", fi->group_id);
	printf("fi.size		: %d\n", (int) fi->size);
	printf("fi.type		: %d\n", (int) fi->type);
	printf("fi.atime	: %d\n", (int) fi->atime);
	printf("fi.mtime	: %d\n", (int) fi->mtime);
	printf("fi.ctime	: %d\n", (int) fi->ctime);
	tmp_s = base16_string(fi->md5_sum, 16);
	printf("fi.md5_sum	: %s\n", tmp_s);
	free(tmp_s);

	tmp_s = base16_string(fi->sha1_sum, 20);
	printf("fi.sha1_sum	: %s\n", tmp_s);
	free(tmp_s);

	printf("fi.name		: %s\n", fi->name);
	printf("fi.path		: %s\n", fi->path);
	
}

