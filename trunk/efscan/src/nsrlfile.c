#include <stdio.h>
#include <stdlib.h>

#include "utility.h"
#include "nsrl.h"
#include "config.h"
#include "sqlite3/sqlite3.h"


#define NSRLFILE_DEBUG 0
#define NSRLFILE_TRANSACTION_N 10000


const char CREATE_NSRLFILES_TABLE [] = "CREATE TABLE IF NOT EXISTS nsrlfiles\
                                        (sha1_hash TEXT,\
                                         md5_hash TEXT,\
                                         filename TEXT,\
                                         filesize INTEGER,\
                                         product_code INTEGER);";
                                         
                                         





int sqlite_execute_query (const char * query, sqlite3 * db)
{
	int result;
	sqlite3_stmt * statement;
	const char * statement_tail;
	
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
	
	if (sqlite_execute_query(CREATE_NSRLFILES_TABLE, db) != 0)
	{
		printf("Error creating files table\n");
		sqlite3_close(db);
		return NULL;
	}
	
	return db;
	
}



int nsrlfile_main (char * nsrlfilename, sqlite3 * db)
{

	nsrlfile * n;
	nsrlfile_record * record;
	char * tmp_sha1_string;
	char * tmp_md5_string;
	char * tmp_filename;
	
	char insert_string [8192];
	
	// only execute sqlite3 transaction every 10000 records so we aren't
	// overwhelmed by disk I/O
	int transaction_counter = 0;
	int total_transactions = 0;
	sqlite_execute_query("BEGIN", db);
	
	n = nsrlfile_open(nsrlfilename);
	record = nsrlfile_next(n);
	
	while (1)
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
		tmp_filename = sqlite_escape_string(record->filename);
		
		#ifdef ARCH_64
		snprintf(insert_string, 8192, "INSERT INTO nsrlfiles (sha1_hash, md5_hash,\
		                        filename, filesize, product_code) VALUES (\
		                        '%s', '%s', '%s', '%ld', '%d')", tmp_sha1_string,
		                        tmp_md5_string, tmp_filename, record->filesize,
		                        record->product_code);
		#else
		snprintf(insert_string, 8192, "INSERT INTO nsrlfiles (sha1_hash, md5_hash,\
		                        filename, filesize, product_code) VALUES (\
		                        '%s', '%s', '%s', '%lld', '%d')", tmp_sha1_string,
		                        tmp_md5_string, tmp_filename, record->filesize,
		                        record->product_code);
		#endif
		
		sqlite_execute_query(insert_string, db);
		
		transaction_counter++;
		if (transaction_counter >= NSRLFILE_TRANSACTION_N)
		{
			transaction_counter = 0;
			total_transactions++;
			sqlite_execute_query("COMMIT", db);
			sqlite_execute_query("BEGIN", db);
			printf("%d total transactions, %d * %d total inserts\n", total_transactions, total_transactions, NSRLFILE_TRANSACTION_N);
		}
		                     
		
		#if NSRLFILE_DEBUG == 1
		#ifdef ARCH_64
			printf("%s\n%s\n%s\n%ld\n%d\n\n", tmp_sha1_string, tmp_md5_string, record->filename, record->filesize, record->product_code);
		#else
			printf("%s\n%s\n%s\n%lld\n%d\n\n", tmp_sha1_string, tmp_md5_string, record->filename, record->filesize, record->product_code);
		#endif
		#endif
	
		free(tmp_sha1_string);
		free(tmp_md5_string);
		free(tmp_filename);
		
	}
	
	if (transaction_counter != 0)
		sqlite_execute_query("COMMIT", db);
	
	nsrlfile_close(n);
	
	return 1;
	
}



int main (int argc, char * argv[])
{
	
	int result;
	sqlite3 * db;

	if (argc < 3)
	{
		printf("Usage: %s <sqlite3_db> <nsrlfile>\n", argv[0]);
		return 0;
	}
	
	// open and init sqlite3 db
	db = sqlite3_db_init(argv[1]);
	
	sqlite_execute_query("PRAGMA count_changes=FALSE;", db);
	
	if (db != NULL)
	{
		result = nsrlfile_main (argv[2], db);
		
		if (result != 0)
			printf("nsrlfile_main result: %d\n", result);
	}
		
	sqlite3_close(db);
	
	return 1;
	
}
