#ifndef fileinfo_H
#define fileinfo_H

#include <inttypes.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

#include "polarssl/md5.h"
#include "polarssl/sha1.h"

#include "utility.h"


#define FILE_INFO_MD5_ERROR (1 << 0)
#define FILE_INFO_SHA1_ERROR (1 << 1)

#define FILE_TYPE_FILE 1
#define FILE_TYPE_DIRECTORY 2
#define FILE_TYPE_LINK 3
#define FILE_TYPE_IRREGULAR 4

typedef struct
{
	int user_id;
	int group_id;
	int type;
	int64_t size; // 2^63 bytes = 8192 petabytes
	time_t atime;
	time_t mtime;
	time_t ctime;
	unsigned char md5_sum[16];
	unsigned char sha1_sum[20];
	char name[1024]; // hope this is enough
	char path[4096]; // bounds checked so we should survive
} file_info_t;



int file_info (char * path, file_info_t * fi);
void file_info_debug_output (file_info_t * fi);


#endif
