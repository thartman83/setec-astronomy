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
#include <string.h>
#include <openssl/sha.h>

void hash_key(const void * key, int key_len, int hash_count, void * hash)
{
	 SHA256_CTX ctx;
	 unsigned char md[SHA256_DIGEST_LENGTH];
	 unsigned char input[SHA256_DIGEST_LENGTH];
	 int x;

	 /* Prime the hash */
	 SHA256_Init(&ctx);
	 SHA256_Update(&ctx, key, key_len);
	 SHA256_Final(input, &ctx);

	 hash_count--;
	 /* Loop the hash hash_count - 1 times */	 
	 for(x = 0; x < hash_count; ++x) {
			SHA256_Init(&ctx);
			SHA256_Update(&ctx, input, SHA256_DIGEST_LENGTH);
			SHA256_Final(md, &ctx);
			strncpy((char *)input, (char *)md, SHA256_DIGEST_LENGTH);
	 }
		
	 strncpy((char *)hash, (char *)md, SHA256_DIGEST_LENGTH);
}
