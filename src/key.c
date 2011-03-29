/*****************************************************************************/
/* key.c for Setec Astronomy                                                 */
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
#include "key.h"
#include "errors.h"
#include <string.h>
#include <mhash.h>

int hash_key(char * password, int password_len, void ** key, int * key_len)
{
	 int err;

	 *key_len = mhash_get_keygen_max_key_size(SA_KEYGEN_ALGO);
	 
	 if(*key_len == 0)
			*key_len = DEFAULT_KEY_LEN;
	 
	 *key = calloc(1, *key_len);

	 err = mhash_keygen(SA_KEYGEN_ALGO, MHASH_MD5, 0, *key, *key_len, NULL, 0, 
											password, password_len);

	 if(err < 0) {
			free(*key);
			return SA_KEYGEN_FAILED;
	 }

	 return SA_SUCCESS;
}
