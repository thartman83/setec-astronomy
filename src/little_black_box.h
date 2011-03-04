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

enum {MAX_NAME_LEN = 50, 
			MAX_PASS_LEN = 25, 
			MAX_PAIR_SIZE = 78}; // MAX_NAME_LEN + MAX_PASS_LEN + = + \n + \0

struct name_pass_pair
{
	 char name[MAX_NAME_LEN];
	 char pass[MAX_PASS_LEN];
};

enum { SA_CRYPT_MODE, SA_DECRYPT_MODE };
static int lbb_ref_count = 0;

struct little_black_box
{
	 struct setec_astronomy_header header;	 
	 MCRYPT md;
	 FILE * fd;
	 void * buffer;
	 int data_len;
	 int crypt_mode;
	 int block_size;
};

int init_lbb(struct little_black_box * lbb, int CRYPT_MODE);

int open_new_lbb(struct little_black_box * lbb, const char * filename, 
								 const char * password);
int open_lbb(struct little_black_box * lbb, const char * filename,
						 const char * password);
int open_lbb_ext(struct little_black_box * lbb, const char * password);
int close_lbb(struct little_black_box * lbb);
int write_lbb(struct little_black_box * lbb, void * buffer, int buffer_len);
int write_lbb_buffer(struct little_black_box * lbb);
int read_next_pair(struct little_black_box * lbb, struct name_pass_pair * pair);

int init_name_pass_pair(const char * pair_string, struct name_pass_pair * pair);

#endif//LITTLE_BLACK_BOX_H_
