/*****************************************************************************/
/* util.h for Setec Astronomy                                                */
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
#ifndef UTIL_H_
#define UTIL_H_

static const int INT_LEN = 2;

void set_random(void * ptr, int len);
int int_from_2bytes(unsigned char buf[INT_LEN]);
void int_to_2bytes(int val, unsigned char buffer[INT_LEN]);
unsigned int time_seed();
void pop_data(void * buf, int buf_len, int data_len);
int file_exists(const char * filename);
char * gen_temp_filename(const char * filename);

#endif//UTIL_H_
