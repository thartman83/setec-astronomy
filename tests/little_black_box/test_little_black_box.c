/*****************************************************************************/
/* test_little_black_box.c for Setec Astronomy                               */
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
#include "test_little_black_box.h"
#include "../unit_test.h"
#include "../../src/little_black_box.h"
#include "../../src/errors.h"
#include <stdio.h>

int main(int argc, char ** argv)
{
	 RUN_TEST(test_init_lbb());
	 RUN_TEST(test_open_new_lbb());
	 RUN_TEST(test_open_lbb());
	 RUN_TEST(test_open_lbb_ext());
	 RUN_TEST(test_close_lbb());
	 RUN_TEST(test_write_lbb());
	 RUN_TEST(test_write_lbb_buffer());
	 RUN_TEST(test_read_next_pair());
	 RUN_TEST(test_init_name_pass_pair());

	 return 0;
}

int test_init_lbb()
{
	 struct little_black_box lbb;
	 int err;
	 int block_size;

	 /* Bad test, checking invalid crypt modes */
	 err = init_lbb(&lbb, 4);
	 test_assert(err == SA_INVALID_CRYPT_MODE);
	 
	 err = init_lbb(&lbb, -10);
	 test_assert(err == SA_INVALID_CRYPT_MODE);

	 /* good tests, write mode */
	 err = init_lbb(&lbb, SA_CRYPT_MODE);
	 test_assert(err == SA_SUCCESS);

	 /* Now check that the various structure members are set properly */
	 test_assert(lbb.md != NULL);
	 test_assert(lbb.fd == NULL);
	 test_assert(get_lbb_ref_count() == 1);
	 test_assert(lbb.data_len == 0);
	 test_assert(lbb.crypt_mode == SA_CRYPT_MODE);
	 
	 block_size = mcrypt_enc_get_block_size(lbb.md);
	 test_assert(lbb.block_size == block_size);

	 /* close test */
	 close_lbb(&lbb);

	 /* good test, read mode */
	 err = init_lbb(&lbb, SA_DECRYPT_MODE);
	 test_assert(err == SA_SUCCESS);

	 test_assert(lbb.md != NULL);
	 test_assert(lbb.fd == NULL);
	 test_assert(get_lbb_ref_count() == 1);
	 test_assert(lbb.data_len == 0);
	 test_assert(lbb.crypt_mode == SA_DECRYPT_MODE);
	 
	 block_size = mcrypt_enc_get_block_size(lbb.md);
	 test_assert(lbb.block_size == block_size);
	 
	 close_lbb(&lbb);
	 
	 return UT_SUCCESS;
}

int test_open_new_lbb()
{
	 /* most of the work for this function is setting up the file descriptor
			for writing the encrypted data.  Everything else is handled by other
			functions */
	 struct little_black_box lbb;	

	 /* bad test, no clobber policy should make this fail */
	 test_assert(SA_FILE_EXISTS == open_new_lbb(&lbb, EXISTING_FILE,
																							TEST_PASSWORD));
	 
	 close_lbb(&lbb);

	 /* good test */
	 remove(NON_EXISTING_FILE);
	 test_assert(SA_SUCCESS == open_new_lbb(&lbb, NON_EXISTING_FILE,
																					TEST_PASSWORD));	 
	 close_lbb(&lbb);

	 /* delete the file that we just created for later tests */
	 remove(NON_EXISTING_FILE);

	 return UT_SUCCESS;
}

int test_open_lbb()
{
	 struct little_black_box lbb;
	 
	 /* bad test, trying to open a file that doesn't exist */
	 remove(NON_EXISTING_FILE);
	 test_assert(SA_FILE_NOT_FOUND == open_lbb(&lbb, NON_EXISTING_FILE,
																						 TEST_PASSWORD));

	 close_lbb(&lbb);
	 
	 /* test file that exists but has a bad header */
	 test_assert(SA_INVALID_HEADER == open_lbb(&lbb, NO_DATA_FILE, 
																						 TEST_PASSWORD));

	 close_lbb(&lbb);

	 /* good test, open existing properly formatted file */
	 test_assert(SA_SUCCESS == open_lbb(&lbb, EXISTING_FILE, TEST_PASSWORD));

	 close_lbb(&lbb);

	 return UT_SUCCESS;
}

int test_open_lbb_ext()
{
	 /* Is this really necessary? */
	 return UT_NOT_IMPLEMENTED;
}

int test_close_lbb()
{
	 struct little_black_box lbb;
	 
	 /* test a lbb in decrypt mode */
	 test_assert(SA_SUCCESS == open_lbb(&lbb, EXISTING_FILE, TEST_PASSWORD));
	 
	 /* check that the reference count is at 1 */
	 test_assert(get_lbb_ref_count() == 1);
	 
	 /* close the lbb and make sure the count is back down to 0 */
	 test_assert(close_lbb(&lbb) == SA_SUCCESS);
	 test_assert(get_lbb_ref_count() == 0);
	 	 
	 return UT_SUCCESS;
}

int test_write_lbb()
{
	 return UT_NOT_IMPLEMENTED;
}

int test_write_lbb_buffer()
{
	 return UT_NOT_IMPLEMENTED;
}

int test_read_next_pair()
{
	 return UT_NOT_IMPLEMENTED;
}

int test_init_name_pass_pair()
{
	 return UT_NOT_IMPLEMENTED;
}

