/*****************************************************************************/
/* test_key.c for Setec Astronomy                                            */
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
#include "test_key.h"
#include "../../src/key.h"
#include "../unit_test.h"
#include "../../src/errors.h"
#include <string.h>
#include <stdlib.h>

int main()
{
	 RUN_TEST(test_hash_key());
	 return 0;
}

int test_hash_key()
{
	 void * key;
	 int key_len, err;
	 char pass[] = "The quick brown fox jumps over the lazy dog";
	 unsigned char verify_key[] = "\236\020}\235\067+\266\202k\330\035\065B\244\031\326>'I~0\264\207\370\313\036\273(th\177B";

	 err = hash_key(pass, strlen(pass), &key, &key_len);

	 test_assert(err == SA_SUCCESS);
	 test_assert(memcmp(key, verify_key, key_len) == 0);
	 
	 free(key);
	 
	 return UT_SUCCESS;
}

