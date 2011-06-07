/*****************************************************************************/
/* test_header.c for Setec Astronomy                                         */
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
#include "test_header.h"
#include "../../src/header.h"
#include "../../src/errors.h"
#include "../unit_test.h"

#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>

const char DEFAULT_PASSWORD[] = "foobarbaz";

static const int DEFAULT_IV_LEN = 256;
static const int DEFAULT_SALT_LEN = 10;
static const int DEFAULT_HASH_COUNT = 2000;
static const int DEFAULT_HASH_LEN = 256;

int main()
{
	 RUN_TEST(test_read_header());
	 RUN_TEST(test_write_header());

	 return UT_SUCCESS;
}

int test_read_header()
{
	 struct setec_astronomy_header r_header;
	 unsigned char * hash;
	 
	 /* Start off with a good test */
	 init_header(&r_header);
	 
	 test_assert(read_header(&r_header, GOOD_HEADER_TEST) == SA_SUCCESS);

	 /* Check to make sure that the read and write headers are identical */
	 test_assert(r_header.salt_len == DEFAULT_SALT_LEN);
	 test_assert(r_header.hash_count == DEFAULT_HASH_COUNT);
	 test_assert(r_header.hash_len == DEFAULT_HASH_LEN);
	 test_assert(r_header.iv_len == DEFAULT_IV_LEN);
	 
	 hash = malloc(r_header.hash_len);
	 PKCS5_PBKDF2_HMAC_SHA1(DEFAULT_PASSWORD, strlen(DEFAULT_PASSWORD), 
													r_header.salt, r_header.salt_len, 
													r_header.hash_count*2, r_header.hash_len, hash);
	 test_assert(memcmp(r_header.hash, hash, r_header.hash_len) == 0);

	 free_header(&r_header);

	 /* Now try to read a header from a file that doesn't exist */
	 init_header(&r_header);
	 
	 test_assert(read_header(&r_header, "thisfiledoesntexists") == 
							 SA_FILE_NOT_FOUND);

	 free_header(&r_header);

	 /* Test against a file that exists but has no data */
	 init_header(&r_header);
	 test_assert(read_header(&r_header, NO_DATA_TEST) == SA_NO_DATA);
	 free_header(&r_header);

	 /* Test against a file that exists and has some but not enough data */
	 init_header(&r_header);
	 test_assert(read_header(&r_header, NOT_ENOUGH_DATA_TEST) == SA_NO_DATA);
	 free_header(&r_header);

	 return UT_SUCCESS;
}

int test_write_header()
{
	 struct setec_astronomy_header w_header, r_header;
	 char temp_file[] = TEST_DATA_DIR "temp";

	 
	 init_header(&w_header);
	 init_header(&r_header);

	 create_header(&w_header, DEFAULT_IV_LEN, DEFAULT_SALT_LEN, 
								 DEFAULT_HASH_COUNT, DEFAULT_PASSWORD, DEFAULT_HASH_LEN);
	 
	 test_assert(write_header(&w_header, temp_file) == SA_SUCCESS);	 
	 test_assert(read_header(&r_header, temp_file) == SA_SUCCESS);
	 
	 test_assert(w_header.salt_len == r_header.salt_len);
	 test_assert(w_header.hash_count == r_header.hash_count);
	 test_assert(w_header.hash_len == w_header.hash_len);
	 test_assert(w_header.iv_len == r_header.iv_len);
	 
	 test_assert(strncmp(w_header.salt, r_header.salt, w_header.salt_len) == 0);
	 test_assert(strncmp(w_header.hash, r_header.hash, w_header.hash_len) == 0);
	 test_assert(strncmp(w_header.iv, r_header.iv, w_header.iv_len) == 0);
	 test_assert(strncmp(w_header.hash, r_header.hash, w_header.hash_len) == 0);

	 free_header(&w_header);
	 free_header(&r_header);
	 
	 return 0;
}
