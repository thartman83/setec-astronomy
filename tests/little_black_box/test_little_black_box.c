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
#include <string.h>

int main(int argc, char ** argv)
{
	 RUN_TEST(test_lbb_open_close());
	 RUN_TEST(test_lbb_read_write());

	 return 0;
}

int test_lbb_open_close()
{
	 struct little_black_box lbb;
	 
	 /* Testing invalid crypt mode */
	 test_equals(lbb_open(&lbb, 3, EXISTING_FILE, TEST_PASSWORD), 
							 SA_INVALID_CRYPT_MODE);

	 test_equals(lbb_close(&lbb), SA_SUCCESS);

	 /* Open an existing file for encryptiong */
	 test_equals(lbb_open(&lbb, SA_CRYPT_MODE, EXISTING_FILE, TEST_PASSWORD), 
							 SA_FILE_EXISTS);
	 
	 test_equals(lbb_close(&lbb), SA_SUCCESS);

	 /* Test failure to create file */
	 test_equals(lbb_open(&lbb, SA_CRYPT_MODE, "/foo", TEST_PASSWORD),
							 SA_CAN_NOT_OPEN_FILE);

	 test_equals(lbb_close(&lbb), SA_SUCCESS);

	 /* Test for invalid file */
	 remove(NON_EXISTING_FILE);
	 test_equals(lbb_open(&lbb, SA_DECRYPT_MODE, NON_EXISTING_FILE, TEST_PASSWORD),
							SA_FILE_NOT_FOUND);

	 test_equals(lbb_close(&lbb), SA_SUCCESS);

	 /* Test for an invalid header file */
	 test_equals(lbb_open(&lbb, SA_DECRYPT_MODE, NO_DATA_FILE, TEST_PASSWORD),
							 SA_INVALID_HEADER);

	 test_equals(lbb_close(&lbb), SA_SUCCESS);

	 test_equals(lbb_open(&lbb, SA_DECRYPT_MODE, NOT_ENOUGH_DATA_FILE, 
												TEST_PASSWORD), SA_INVALID_HEADER);

	 test_equals(lbb_close(&lbb), SA_SUCCESS);

	 /* Test for invalid password */
	 test_equals(lbb_open(&lbb, SA_DECRYPT_MODE, GOOD_HEADER_FILE, "bazbarfoo"),
							 SA_WRONG_PASSWORD);
	 
	 test_equals(lbb_close(&lbb), SA_SUCCESS);

	 /* Test for a good decrypt open */
	 test_equals(lbb_open(&lbb, SA_DECRYPT_MODE, GOOD_HEADER_FILE, TEST_PASSWORD),
							 SA_SUCCESS);
	 
	 test_equals(lbb_close(&lbb), SA_SUCCESS);

	 /* Test for a good encrypt open */
	 test_equals(lbb_open(&lbb, SA_CRYPT_MODE, NON_EXISTING_FILE, TEST_PASSWORD), 
							 SA_SUCCESS);
	 test_equals(lbb_close(&lbb), SA_SUCCESS);

	 test_equals(remove(NON_EXISTING_FILE), 0);
	 
	 return UT_SUCCESS;
}

int test_lbb_read_write()
{
	 struct little_black_box lbb;
	 struct name_pass_pair pair;

	 test_equals(lbb_open(&lbb, SA_CRYPT_MODE, NON_EXISTING_FILE, TEST_PASSWORD),
							 SA_SUCCESS);

	 test_equals(lbb_write_name_pass(&lbb, "foo", "bar"), SA_SUCCESS);

	 test_equals(lbb_close(&lbb), SA_SUCCESS);

	 test_equals(lbb_open(&lbb, SA_DECRYPT_MODE, NON_EXISTING_FILE, TEST_PASSWORD),
							 SA_SUCCESS);
	 
	 test_equals(lbb_read_pair(&lbb, &pair), SA_SUCCESS);
	 
	 test_equals(strcmp(pair.name, "foo"), 0);
	 test_equals(strcmp(pair.pass, "bar"), 0);

	 test_equals(remove(NON_EXISTING_FILE), 0);
	 
	 lbb_close(&lbb);

	 return UT_SUCCESS;
}

int test_lbb_open_rw()
{
	 return UT_SUCCESS;
}
