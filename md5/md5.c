/*
	Implementation of MD5 from RFC 1321
	Not thoroughly tested
	Not very cleaned up
	Not optimized
	Works for small values only ~(less than 52 bits, possibly smaller)
*/


#include <stdlib.h>
#include <inttypes.h>



/*
	Step 4.2
	The 64-element table
*/
#define T_1 0xd76aa478
#define T_2 0xe8c7b756
#define T_3 0x242070db
#define T_4 0xc1bdceee
#define T_5 0xf57c0faf
#define T_6 0x4787c62a
#define T_7 0xa8304613
#define T_8 0xfd469501
#define T_9 0x698098d8
#define T_10 0x8b44f7af
#define T_11 0xffff5bb1
#define T_12 0x895cd7be
#define T_13 0x6b901122
#define T_14 0xfd987193
#define T_15 0xa679438e
#define T_16 0x49b40821
#define T_17 0xf61e2562
#define T_18 0xc040b340
#define T_19 0x265e5a51
#define T_20 0xe9b6c7aa
#define T_21 0xd62f105d
#define T_22 0x2441453
#define T_23 0xd8a1e681
#define T_24 0xe7d3fbc8
#define T_25 0x21e1cde6
#define T_26 0xc33707d6
#define T_27 0xf4d50d87
#define T_28 0x455a14ed
#define T_29 0xa9e3e905
#define T_30 0xfcefa3f8
#define T_31 0x676f02d9
#define T_32 0x8d2a4c8a
#define T_33 0xfffa3942
#define T_34 0x8771f681
#define T_35 0x6d9d6122
#define T_36 0xfde5380c
#define T_37 0xa4beea44
#define T_38 0x4bdecfa9
#define T_39 0xf6bb4b60
#define T_40 0xbebfbc70
#define T_41 0x289b7ec6
#define T_42 0xeaa127fa
#define T_43 0xd4ef3085
#define T_44 0x4881d05
#define T_45 0xd9d4d039
#define T_46 0xe6db99e5
#define T_47 0x1fa27cf8
#define T_48 0xc4ac5665
#define T_49 0xf4292244
#define T_50 0x432aff97
#define T_51 0xab9423a7
#define T_52 0xfc93a039
#define T_53 0x655b59c3
#define T_54 0x8f0ccc92
#define T_55 0xffeff47d
#define T_56 0x85845dd1
#define T_57 0x6fa87e4f
#define T_58 0xfe2ce6e0
#define T_59 0xa3014314
#define T_60 0x4e0811a1
#define T_61 0xf7537e82
#define T_62 0xbd3af235
#define T_63 0x2ad7d2bb
#define T_64 0xeb86d391



/* 
	Step 3
	Initialize a four word buffer
*/

uint32_t md5_A = 0x67452301;
uint32_t md5_B = 0xefcdab89;
uint32_t md5_C = 0x98badcfe;
uint32_t md5_D = 0x10325476;



typedef struct
{
	uint64_t length; // size of data in bytes (in little endian)
	uint64_t b; // original size of message in bits (in little endian)
	uint64_t A; // digest output
	uint64_t B; // digest output
	uint64_t C; // digest output
	uint64_t D; // digest output
	unsigned char * M; // where the original data is stored
} md5_context;



md5_context * md5_begin (char * data, int data_len)
{
	
	md5_context * context;
	
	context = malloc(sizeof(md5_context));
	if (context == NULL)
		return NULL;
	
	context->M = malloc(data_len);
	if (context->M == NULL)
		return NULL;
	
	context->length = data_len;
	context->b = data_len * 8;
	memcpy(context->M, data, data_len);
	context->A = md5_A;
	context->B = md5_B;
	context->C = md5_C;
	context->D = md5_D;
	
	return context;
	
}



uint32_t md5_rotl (uint32_t x, uint32_t s)
{
//(((x) << (n)) | ((x) >> (32-(n))))
	return (x << s) | (x >> (32 - s));
}


/* 
	Step 1
	Must pad data to be congruent to 448 bits, module 512 bits
	that's 56 % 64 bytes
	Padding is a 1 bit, followed by 0 bit0
*/
int md5_add_padding (md5_context * context)
{
	int i;
	int padding_needed;
	unsigned char * data_realloc;
	
	padding_needed = context->length % 64;
	padding_needed = 56 - context->length;
	if (padding_needed == 0)
		padding_needed = 64;
	else if (padding_needed < 0)
		padding_needed = 64 + padding_needed;
	
	data_realloc = realloc(context->M, context->length + padding_needed);
	if (data_realloc == NULL)
		return -1;
	context->M = data_realloc;
	
	context->M[context->length] = 0x80;
	memset(&(context->M[context->length + 1]), 0, padding_needed - 1);
	
	context->length += padding_needed;
	
	return 0;
}



/*
	Step 2
	A 64-bit representation of the length of the message before padding
	bits is appended to the result of the previous step
	swap bytes on little endian
*/
int md5_append_length (md5_context * context)
{

	unsigned char * data_realloc;
	int i;
	unsigned char * original_size;
	
	data_realloc = realloc(context->M, context->length + 8);
	if (data_realloc == NULL)
		return -1;
	context->M = data_realloc;
	
	// adjust for endianness
	
	original_size = (unsigned char *) &(context->b);
	for (i = 0; i < 8; i++)
	{
		context->M[context->length + i] = original_size[i];//7 - i];
	}
	
	context->length += 8;
	
	return 1;
}



/*
	Step 4.1
	Define our four auxiliary functions
	F(X,Y,Z) = XY v not(X) Z 
	G(X,Y,Z) = XZ v Y not(Z) 
	H(X,Y,Z) = X xor Y xor Z 
	I(X,Y,Z) = Y xor (X v not(Z))
	Read more: http://www.faqs.org/rfcs/rfc1321.html#ixzz0hEDI5r7H
*/
uint32_t md5_F (uint32_t x, uint32_t y, uint32_t z)
{
	// (((x) & (y)) | ((~x) & (z)))
	return (x & y) | ((~x) & z);
}


uint32_t md5_G (uint32_t x, uint32_t y, uint32_t z)
{
	return (x & z) | (y & ~z);
}


uint32_t md5_H (uint32_t x, uint32_t y, uint32_t z)
{
	return (x ^ y ^ z);
}


uint32_t md5_I (uint32_t x, uint32_t y, uint32_t z)
{
	return (y ^ (x | ~z));
}



/*
	Step 4.3
	Process each 16-word block
*/
void md5_process_block (md5_context * context)
{
	
	uint32_t X[16];
	uint32_t AA;
	uint32_t BB;
	uint32_t CC;
	uint32_t DD;
	
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
	
	int i;
	
	for (i = 0; i < 16; i++)
	{
		X[i] = (uint32_t) context->M[i*4] | 
		      (((uint32_t) (context->M[i*4 + 1])) << 8) |
			  (((uint32_t) (context->M[i*4 + 2])) << 16) |
			  (((uint32_t) (context->M[i*4 + 3])) << 24);
	}
	
	AA = context->A;
	a = AA;
	BB = context->B;
	b = BB;
	CC = context->C;
	c = CC;
	DD = context->D;
	d = DD;
	
	// round 1
	a = b + (md5_rotl(a + md5_F(b, c, d) + X[0]  + T_1, 7));
	d = a + (md5_rotl(d + md5_F(a, b, c) + X[1]  + T_2, 12));
	c = d + (md5_rotl(c + md5_F(d, a, b) + X[2]  + T_3, 17));
	b = c + (md5_rotl(b + md5_F(c, d, a) + X[3]  + T_4, 22));
	
	a = b + (md5_rotl((uint32_t) a + md5_F(b, c, d) + X[4]  + T_5, 7));
	d = a + (md5_rotl((uint32_t) d + md5_F(a, b, c) + X[5]  + T_6, 12));
	c = d + (md5_rotl((uint32_t) c + md5_F(d, a, b) + X[6]  + T_7, 17));
	b = c + (md5_rotl((uint32_t) b + md5_F(c, d, a) + X[7]  + T_8, 22));
	
	a = b + (md5_rotl((uint32_t) a + md5_F(b, c, d) + X[8]  + T_9, 7));
	d = a + (md5_rotl((uint32_t) d + md5_F(a, b, c) + X[9]  + T_10, 12));
	c = d + (md5_rotl((uint32_t) c + md5_F(d, a, b) + X[10] + T_11, 17));
	b = c + (md5_rotl((uint32_t) b + md5_F(c, d, a) + X[11] + T_12, 22));
	
	a = b + (md5_rotl((uint32_t) a + md5_F(b, c, d) + X[12] + T_13, 7));
	d = a + (md5_rotl((uint32_t) d + md5_F(a, b, c) + X[13] + T_14, 12));
	c = d + (md5_rotl((uint32_t) c + md5_F(d, a, b) + X[14] + T_15, 17));
	b = c + (md5_rotl((uint32_t) b + md5_F(c, d, a) + X[15] + T_16, 22));
	
	// round 2
	a = b + (md5_rotl((uint32_t) a + md5_G(b, c, d) + X[1]  + T_17, 5));
	d = a + (md5_rotl((uint32_t) d + md5_G(a, b, c) + X[6]  + T_18, 9));
	c = d + (md5_rotl((uint32_t) c + md5_G(d, a, b) + X[11] + T_19, 14));
	b = c + (md5_rotl((uint32_t) b + md5_G(c, d, a) + X[0]  + T_20, 20));
	
	a = b + (md5_rotl((uint32_t) a + md5_G(b, c, d) + X[5]  + T_21, 5));
	d = a + (md5_rotl((uint32_t) d + md5_G(a, b, c) + X[10] + T_22, 9));
	c = d + (md5_rotl((uint32_t) c + md5_G(d, a, b) + X[15] + T_23, 14));
	b = c + (md5_rotl((uint32_t) b + md5_G(c, d, a) + X[4]  + T_24, 20));
	
	a = b + (md5_rotl((uint32_t) a + md5_G(b, c, d) + X[9]  + T_25, 5));
	d = a + (md5_rotl((uint32_t) d + md5_G(a, b, c) + X[14] + T_26, 9));
	c = d + (md5_rotl((uint32_t) c + md5_G(d, a, b) + X[3]  + T_27, 14));
	b = c + (md5_rotl((uint32_t) b + md5_G(c, d, a) + X[8]  + T_28, 20));
	
	a = b + (md5_rotl((uint32_t) a + md5_G(b, c, d) + X[13] + T_29, 5));
	d = a + (md5_rotl((uint32_t) d + md5_G(a, b, c) + X[2]  + T_30, 9));
	c = d + (md5_rotl((uint32_t) c + md5_G(d, a, b) + X[7]  + T_31, 14));
	b = c + (md5_rotl((uint32_t) b + md5_G(c, d, a) + X[12] + T_32, 20));
	
	// round 3
	a = b + (md5_rotl((uint32_t) a + md5_H(b, c, d) + X[5]  + T_33, 4));
	d = a + (md5_rotl((uint32_t) d + md5_H(a, b, c) + X[8]  + T_34, 11));
	c = d + (md5_rotl((uint32_t) c + md5_H(d, a, b) + X[11] + T_35, 16));
	b = c + (md5_rotl((uint32_t) b + md5_H(c, d, a) + X[14] + T_36, 23));
	
	a = b + (md5_rotl((uint32_t) a + md5_H(b, c, d) + X[1]  + T_37, 4));
	d = a + (md5_rotl((uint32_t) d + md5_H(a, b, c) + X[4]  + T_38, 11));
	c = d + (md5_rotl((uint32_t) c + md5_H(d, a, b) + X[7]  + T_39, 16));
	b = c + (md5_rotl((uint32_t) b + md5_H(c, d, a) + X[10] + T_40, 23));
	
	a = b + (md5_rotl((uint32_t) a + md5_H(b, c, d) + X[13] + T_41, 4));
	d = a + (md5_rotl((uint32_t) d + md5_H(a, b, c) + X[0]  + T_42, 11));
	c = d + (md5_rotl((uint32_t) c + md5_H(d, a, b) + X[3]  + T_43, 16));
	b = c + (md5_rotl((uint32_t) b + md5_H(c, d, a) + X[6]  + T_44, 23));
	
	a = b + (md5_rotl((uint32_t) a + md5_H(b, c, d) + X[9]  + T_45, 4));
	d = a + (md5_rotl((uint32_t) d + md5_H(a, b, c) + X[12] + T_46, 11));
	c = d + (md5_rotl((uint32_t) c + md5_H(d, a, b) + X[15] + T_47, 16));
	b = c + (md5_rotl((uint32_t) b + md5_H(c, d, a) + X[12] + T_48, 23));
	
	// round 3
	a = b + (md5_rotl((uint32_t) a + md5_I(b, c, d) + X[0]  + T_49, 6));
	d = a + (md5_rotl((uint32_t) d + md5_I(a, b, c) + X[7]  + T_50, 10));
	c = d + (md5_rotl((uint32_t) c + md5_I(d, a, b) + X[14] + T_51, 15));
	b = c + (md5_rotl((uint32_t) b + md5_I(c, d, a) + X[5]  + T_52, 21));
	
	a = b + (md5_rotl((uint32_t) a + md5_I(b, c, d) + X[12] + T_53, 6));
	d = a + (md5_rotl((uint32_t) d + md5_I(a, b, c) + X[3]  + T_54, 10));
	c = d + (md5_rotl((uint32_t) c + md5_I(d, a, b) + X[10] + T_55, 15));
	b = c + (md5_rotl((uint32_t) b + md5_I(c, d, a) + X[1]  + T_56, 21));
	
	a = b + (md5_rotl((uint32_t) a + md5_I(b, c, d) + X[8]  + T_57, 6));
	d = a + (md5_rotl((uint32_t) d + md5_I(a, b, c) + X[15] + T_58, 10));
	c = d + (md5_rotl((uint32_t) c + md5_I(d, a, b) + X[6]  + T_59, 15));
	b = c + (md5_rotl((uint32_t) b + md5_I(c, d, a) + X[13] + T_60, 21));
	
	a = b + (md5_rotl((uint32_t) a + md5_I(b, c, d) + X[4]  + T_61, 6));
	d = a + (md5_rotl((uint32_t) d + md5_I(a, b, c) + X[11] + T_62, 10));
	c = d + (md5_rotl((uint32_t) c + md5_I(d, a, b) + X[2]  + T_63, 15));
	b = c + (md5_rotl((uint32_t) b + md5_I(c, d, a) + X[9]  + T_64, 21));
	
	a += AA;
	b += BB;
	c += CC;
	d += DD;
	
	context->A = a;
	context->B = b;
	context->C = c;
	context->D = d;
	
}
