#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>

#include "sqlite3/sqlite3.h"

#include "fileinfo.h"


#define CRAWL_ERROR_STAT_FAIL 1
#define CRAWL_ERROR_NOT_DIR 2
#define CRAWL_ERROR_PERMISSION 3
#define CRAWL_ERROR_TOO_MANY_LINKS 4
#define CRAWL_ERROR_NAME_TOO_LONG 5

#define CRAWL_ERROR_UNKNOWN 1000

#define SCAN_DEBUG 1
#define FILE_INFO_TO_STDOUT 0


const char CREATE_SCANS_TABLE[] = "CREATE TABLE IF NOT EXISTS scans (id INTEGER PRIMARY KEY AUTOINCREMENT, time INTEGER);";
const char CREATE_FILES_TABLE[] = "CREATE TABLE IF NOT EXISTS files\
                                   (scan_id INTEGER,\
                                    filename TEXT,\
                                    path TEXT,\
                                    user_id INTEGER,\
                                    group_id INTEGER,\
                                    type INTEGER,\
                                    size INTEGER,\
                                    access_time INTEGER,\
                                    modify_time INTEGER,\
                                    create_time INTEGER,\
                                    md5_sum TEXT,\
                                    sha1_sum TEXT);";
                                    
char query_tmp [8192];



int crawl_dir (char * path, int64_t scan_id, sqlite3 * db);
int sqlite_execute_query (const char * query, sqlite3 * db);
sqlite3 * sqlite3_db_init (char * db_filename);




int crawl_dir (char * path, int64_t scan_id, sqlite3 * db)
{
	
	int result;
	
	struct stat stat_buffer;
	struct dirent * dir_buffer;
	
	file_info_t fi;
	
	DIR * dir_handle;
	
	// NTFS filenames + paths can be upwards of 30,000 characters.
	// Windows cuts filenames off somewhere closer to 255-260 characters.
	// May this be somewhat of an absurdly overcautious medium.
	char tmp_filename[4096];
	char * tmp_s;
	
	#if SCAN_DEBUG == 1
		printf("SCAN_DEBUG 1\n");
	#endif
	
	result = stat((const char *) path, &stat_buffer);
	
	if (result != 0)
		return CRAWL_ERROR_STAT_FAIL;
	
	if (S_ISDIR(stat_buffer.st_mode) == 0)
		return CRAWL_ERROR_NOT_DIR;

	#if SCAN_DEBUG == 1
		printf("SCAN_DEBUG 2\n");
	#endif	
	
	dir_handle = opendir(path);
	if (dir_handle == NULL)
	{
		if (errno == EACCES)
			return CRAWL_ERROR_PERMISSION;
		/* MinGW not cooperating
		else if (errno == ELOOP)
			return CRAWL_ERROR_TOO_MANY_LINKS;
		*/
		else if (errno == ENAMETOOLONG)
			return CRAWL_ERROR_NAME_TOO_LONG;
		else 
			return CRAWL_ERROR_UNKNOWN;
	}
	
	#if SCAN_DEBUG == 1
		printf("SCAN_DEBUG 3\n");
	#endif	
	
	dir_buffer = readdir(dir_handle);
	
	while (dir_buffer != NULL)
	{
		
		if ((strcmp(dir_buffer->d_name, ".") != 0) && (strcmp(dir_buffer->d_name, "..") != 0))
		{
			strcpy(tmp_filename, path);
			strcat(tmp_filename, "/");
			strcat(tmp_filename, dir_buffer->d_name);
			
			#if SCAN_DEBUG == 1
				printf("SCAN_DEBUG 4 %s\n", tmp_filename);
			#endif
			
			result = file_info(tmp_filename, &fi);
			
			if (result != 0)
			{
				printf("Error reading %s\n", tmp_filename);
			}
			else
			{
				/*
				 * THIS NEEDS TO BECOME SPRINTF, BADLY
				 */
				query_tmp[0] = 0;
				strcat(query_tmp, "INSERT INTO files (scan_id, filename, path, user_id, group_id, type, size,\
													  access_time, modify_time, create_time, md5_sum, sha1_sum)\
													  VALUES ('");
				strcat(query_tmp, int64_t_itoa(scan_id));
				strcat(query_tmp, "', '");
				tmp_s = sqlite_escape_string(fi.name);
				if (tmp_s == NULL)
					printf("Error escaping %s\n", fi.name);
				else
				{
					strcat(query_tmp, tmp_s);
					free(tmp_s);
				}
				strcat(query_tmp, "', '");
				tmp_s = sqlite_escape_string(fi.path);
				if (tmp_s == NULL)
					printf("Error escaping %s\n", fi.path);
				else
				{
					strcat(query_tmp, tmp_s);
					free(tmp_s);
				}
				strcat(query_tmp, "', '");
				strcat(query_tmp, int_itoa(fi.user_id));
				strcat(query_tmp, "', '");
				strcat(query_tmp, int_itoa(fi.group_id));
				strcat(query_tmp, "', '");
				strcat(query_tmp, int_itoa(fi.type));
				strcat(query_tmp, "', '");
				strcat(query_tmp, int64_t_itoa(fi.size));
				strcat(query_tmp, "', '");
				strcat(query_tmp, time_t_itoa(fi.atime));
				strcat(query_tmp, "', '");
				strcat(query_tmp, time_t_itoa(fi.mtime));
				strcat(query_tmp, "', '");
				strcat(query_tmp, time_t_itoa(fi.ctime));
				strcat(query_tmp, "', '");
				tmp_s = base16_string(fi.md5_sum, 16);
				if (tmp_s == NULL)
					printf("Error base16_string md5_sum\n");
				else
				{
					strcat(query_tmp, tmp_s);
					free(tmp_s);
				}
				strcat(query_tmp, "', '");
				tmp_s = base16_string(fi.sha1_sum, 16);
				if (tmp_s == NULL)
					printf("Error base16_string sha1_sum\n");
				else
				{
					strcat(query_tmp, tmp_s);
					free(tmp_s);
				}
				strcat(query_tmp, "')");
				
				sqlite_execute_query(query_tmp, db);
				
				#if FILE_INFO_TO_STDOUT == 1
					file_info_debug_output(&fi);
				#endif

				if (fi.type == FILE_TYPE_DIRECTORY)
				{
					crawl_dir(fi.path, scan_id, db);
				}
			}
		}
		dir_buffer = readdir(dir_handle);
		
	}
	
	closedir(dir_handle);
	
	return 0;
	
}



int sqlite_execute_query (const char * query, sqlite3 * db)
{
	int result;
	sqlite3_stmt * statement;
	const char * statement_tail;
	
	// create scans table
	result = sqlite3_prepare(db,
	                         query,
	                         strlen(query),
	                         &statement,
	                         &statement_tail);
	if (result != SQLITE_OK)
	{
		printf("sqlite3_prepare error: %s\n%s\n", query, sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}
	result = sqlite3_step(statement);
	if (result == SQLITE_ERROR)
	{
		printf("sqlite3_step error: %s\n%s\n", query, sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}
	result = sqlite3_finalize(statement);
	if (result != SQLITE_OK)
	{
		printf("sqlite3_finalize error: %s\n%s\n", query, sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}
	
	return 0;
}



sqlite3 * sqlite3_db_init (char * db_filename)
{
	
	int result;
	sqlite3 * db;
	result = sqlite3_open(db_filename, &db);
	if (result != SQLITE_OK)
	{
		printf("sqlite3_open error: %s\n", sqlite3_errmsg(db));
		return NULL;
	}
	
	if (sqlite_execute_query(CREATE_SCANS_TABLE, db) != 0)
	{
		printf("Error creating files table\n");
		sqlite3_close(db);
		return NULL;
	}
	
	if (sqlite_execute_query(CREATE_FILES_TABLE, db) != 0)
	{
		printf("Error creating files table\n");
		sqlite3_close(db);
		return NULL;
	}
	
	return db;
	
}

		

int main (int argc, char * argv[])
{
	
	int result;
	sqlite3 * db;

	if (argc < 3)
	{
		printf("Usage: %s <sqlite3_db> <directory>\n", argv[0]);
		return 0;
	}
	
	// open and init sqlite3 db
	db = sqlite3_db_init(argv[1]);
	
	if (db != NULL)
	{
		result = crawl_dir (argv[2], (int64_t) 0, db);
		
		if (result != 0)
			printf("crawl_dir result: %d\n", result);
	}
		
	sqlite3_close(db);
	
	return 1;
	
}
	
	
