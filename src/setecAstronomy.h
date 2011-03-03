/*****************************************************************************/
/* setecAstronomy.h for Setec Astronomy                                      */
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
#ifndef SETECASTRONOMY_H_
#define SETECASTRONOMY_H_
#include <mcrypt.h>
#include <mutils/mhash_config.h>
#include <mutils/mhash.h>
#include "header.h"

static const int MAX_NAME_LEN = 50;
static const int MAX_PASS_LEN = 25;
static const int MAX_PAIR_SIZE = 78; // MAX_NAME_LEN + MAX_PASS_LEN + = + \n + \0
static const int KEY_LEN = 32;
static const char CRYPT_ALGO[] = "rijndael-256";
static const char CRYPT_MODE[] = "cbc";

struct name_pass_pair
{
	 char name[MAX_NAME_LEN];
	 char pass[MAX_PASS_LEN];
};

char * name_pass_pair_to_string(const struct name_pass_pair * pair);

void init_name_pass_pair(struct name_pass_pair * pair);
void free_name_pass_pair(struct name_pass_pair * pair);

int get_name_pass_pair(struct name_pass_pair * pair, const char * filename);
int add_name_pass_pair(const struct name_pass_pair * pair, 
											 const char * filename, const char * password);
int del_name_pass_pair(const struct name_pass_pair * pair,
											 const char * filename);

int open_new_mcrypt(struct setec_astronomy_header * header, 
										const char * password, MCRYPT * md);
int open_mcrypt(const struct setec_astronomy_header * header, 
								const char * password, MCRYPT * md);

int encrypt_write_zero_buffer(MCRYPT md, FILE * fd, void * buffer, int buf_len);

int read_next_pair(MCRYPT md, FILE * fd, void * buffer, int * buf_len, 
									 struct name_pass_pair * pair);

#endif//SETECASTRONOMY_H_
