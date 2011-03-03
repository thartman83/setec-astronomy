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

static const char CRYPT_ALGO[] = "rijndael-256";
static const char CRYPT_MODE[] = "cbc";
static const int KEY_LEN = 32;

enum { SA_CRYPT_MODE, SA_DECRYPT_MODE }

struct little_black_box
{
	 struct setec_astronomy_header header;	 
	 MCRYPT md;
	 FILE * fd;
	 void * buffer;
	 int buffer_len;
	 int crypt_mode;
	 static int ref_count = 0;
};

int open_new_lbb(struct little_black_box * lbb, const char * filename, 
								 const char * password);
int open_lbb(struct little_black_box * lbb, const char * filename,
						 const char * password);
int open_lbb_ext(struct little_black_box * lbb, const char * password);
int init_lbb(struct little_black_box * lbb);
int close_lbb(struct little_black_box * lbb);
int write_lbb(struct little_black_box * lbb, void * buffer, int buffer_len);
int read_next_pair(struct little_black_box * lbb, struct name_pass_pair * pair);

#endif//LITTLE_BLACK_BOX_H_
