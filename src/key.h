/*****************************************************************************/
/* key.h for Setec Astronomy                                                 */
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
#ifndef KEY_H_
#define KEY_H_

#define SA_KEYGEN_ALGO KEYGEN_S2K_SALTED

static const int DEFAULT_KEY_LEN = 32;
static const int DEFAULT_SALT_LEN = 32;

int hash_key(unsigned char * password, int password_len, void * key, 
						 int * key_len);

#endif//KEY_H_
