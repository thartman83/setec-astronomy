/*****************************************************************************/
/* header.h for Setec Astronomy                                              */
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
#ifndef HEADER_H_
#define HEADER_H_
#include <stdio.h>

struct setec_astronomy_header
{
	 int salt_len;
	 void * salt;

	 int hash_count;

	 int hash_len;
	 void * hash;

	 int iv_len;
	 void * iv;
};


void init_header(struct setec_astronomy_header * header);
void free_header(struct setec_astronomy_header * header);
void create_header(struct setec_astronomy_header * header, int iv_len, 
									 int salt_len, int hash_count, const char * password,
									 int hash_len);

int read_header(struct setec_astronomy_header * header, const char * filename);
int read_header_ext(struct setec_astronomy_header * header, FILE * fd);
int read_len_data_pair(int * len, void ** data, FILE * fd);
int read_int_len(int * len, FILE * fd);

int write_header(const struct setec_astronomy_header * header, const char * filename);
int write_header_ext(const struct setec_astronomy_header * header, FILE * fd);
int write_len_data_pair(int len, void * data, FILE * fd);
int write_int_len(int len, FILE * fd);

int header_len(const struct setec_astronomy_header * header);
void init_random_buffer(void ** buffer, int buf_len);

#endif//HEADER_H_
