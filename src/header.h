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

struct setec_astronomy_header
{
	 int salt_len;
	 void * salt;
	 int iv_len;
	 void * iv;
	 int hash_count;
};

void init_header(struct setec_astronomy_header * header);
void free_header(struct setec_astronomy_header * header);
int read_header(struct setec_astronomy_header * header, const char * filename);
int write_header(struct setec_astronomy_header * header, const char * filename);
int header_len(struct setec_astronomy_header * header);

void init_salt(struct setec_astronomy_header * header, int salt_len);
void init_iv(struct setec_astronomy_header * header, int iv_len);

#endif//HEADER_H_
