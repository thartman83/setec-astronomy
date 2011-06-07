/*****************************************************************************/
/* little_black_box.h for Setec Astronomy                                    */
/* Copyright (c) 2011 Thomas Hartman (rokstar83@gmail.com)                   */
/*                                                                           */
/* This program is free software; you can redistribute it and/or             */
/* modify it under the terms of the GNU General Public License               */
/* as published by the Free Software Foundation; either version 2            */
/* of the License, or the License, or (at your option) any later             */
/* version.                                                                  */
/*                                                                           */
/* This program is distributed in the hope that it will be useful,           */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             */
/* GNU General Public License for more details.                              */
/*****************************************************************************/
#ifndef LITTLE_BLACK_BOX_H_
#define LITTLE_BLACK_BOX_H_

#include "header.h"
#include <mcrypt.h>

static const char CRYPT_ALGO[] = "rijndael-256";
static const char CRYPT_MODE[] = "cbc";
static const int KEY_LEN = 32;
static const int DEFAULT_SALT_LEN = 32;
static const int DEFAULT_HASH_COUNT = 2000;
static const int DEFAULT_HASH_LEN = 256;

enum {MAX_NAME_LEN = 50, 
			MAX_PASS_LEN = 25, 
			MAX_PAIR_SIZE = 78}; // MAX_NAME_LEN + MAX_PASS_LEN + = + \n + \0

struct name_pass_pair
{
	 char name[MAX_NAME_LEN];
	 char pass[MAX_PASS_LEN];
};

enum { SA_CRYPT_MODE, SA_DECRYPT_MODE };

struct little_black_box
{
	 struct setec_astronomy_header header;
	 
	 /* mcrypt descriptor */
	 MCRYPT md;
	 
	 /* File pointer for writing or reading encrypted data */
	 FILE * fd;

	 /* this houses the data for either encrypting or decrypting 
			WARNING: this may contain sensitive data in clear text, zero out
			buffer before deleting */
	 void * buffer;
	 int data_len;

	 /* Crypt mode dependant structure members */
	 int crypt_mode;

	 /* encryption block size, avoids repeated calls to 
			mcrypt_enc_get_block_size() */
	 int block_size;
	 
	 /* Key information for decrypting*/
	 void * key;
	 int key_len;
};

int lbb_open(struct little_black_box * lbb, int crypt_mode, 
						 const char * filename, const char * password);
int lbb_open_read(struct little_black_box * lbb, const char * filename, 
									const char * password);
int lbb_open_write(struct little_black_box * lbb, const char * filename,
									 const char * password);
int lbb_close(struct little_black_box * lbb);
int lbb_write(struct little_black_box * lbb, void * buffer, int buffer_len);
int lbb_flush(struct little_black_box * lbb);
int lbb_write_pair(struct little_black_box * lbb, const struct name_pass_pair);
int lbb_write_name_pass(struct little_black_box * lbb, const char name[MAX_NAME_LEN],
												const char pass[MAX_PASS_LEN]);

int lbb_read_pair(struct little_black_box * lbb, struct name_pass_pair * pair);
int lbb_read_block(struct little_black_box * lbb);

int lbb_copy(struct little_black_box * r_lbb,
						 struct little_black_box * w_lbb);

int init_name_pass_pair(const char * pair_string, struct name_pass_pair * pair);

#endif//LITTLE_BLACK_BOX_H_
