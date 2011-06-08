/*****************************************************************************/
/* test_setec_astronomy.c for Setec Astronomy                                */
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../src/setec_astronomy.h"
#include "../../src/little_black_box.h"
#include "../../src/errors.h"
#include "../../src/setec_astronomy.h"
#include "../unit_test.h"
#include "test_setec_astronomy.h"

int main()
{	 
	 RUN_TEST(test_add_entry());
	 RUN_TEST(test_del_entry());
	 RUN_TEST(test_list_names());

	 return 0;
}

int test_add_entry()
{
	 char pass_buff[MAX_PASS_LEN];

	 remove(NON_EXISTING_FILE);

	 /* First test, new file single entry */
	 test_assert(add_name_pass(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME, 
														 TEST_PASS) == SA_SUCCESS);
	 test_assert(get_pass_by_name(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME, 
																pass_buff) == SA_SUCCESS);
	 test_assert(strncmp(TEST_PASS, pass_buff, MAX_PASS_LEN) == 0);  
	 
	 /* Second test, add a second entry to the file and check that both 
			entries are there */
	 test_assert(add_name_pass(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME2,
	 													 TEST_PASS2) == SA_SUCCESS);
	 test_assert(get_pass_by_name(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME2,
	 															pass_buff) == SA_SUCCESS);
	 test_assert(strncmp(TEST_PASS2, pass_buff, MAX_PASS_LEN) == 0);
	 test_assert(get_pass_by_name(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME, 
	 														pass_buff) == SA_SUCCESS);
	 test_assert(strncmp(TEST_PASS, pass_buff, MAX_PASS_LEN) == 0); 
	 
	 remove(NON_EXISTING_FILE);
	 
	 return UT_SUCCESS;
}

int test_del_entry()
{	 
	 char pass_buff[MAX_PASS_LEN];
	 
	 /* First test, new file single entry then delete */
	 remove(NON_EXISTING_FILE);
	 test_assert(add_name_pass(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME, 
														 TEST_PASS) == SA_SUCCESS);
	 test_assert(get_pass_by_name(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME, 
																pass_buff) == SA_SUCCESS);
	 test_assert(strncmp(TEST_PASS, pass_buff, MAX_PASS_LEN) == 0);

	 test_assert(del_name_pass(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME) ==
							 SA_SUCCESS);

	 test_assert(get_pass_by_name(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME,
																pass_buff) == SA_NAME_NOT_FOUND);

	 /* Second test, new file, multiple entries then delete */
	 remove(NON_EXISTING_FILE);
	 test_assert(add_name_pass(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME, 
														 TEST_PASS) == SA_SUCCESS);
	 test_assert(get_pass_by_name(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME, 
																pass_buff) == SA_SUCCESS);
	 test_assert(strncmp(TEST_PASS, pass_buff, MAX_PASS_LEN) == 0);
	 
	 test_assert(add_name_pass(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME2,
														 TEST_PASS2) == SA_SUCCESS);
	 test_assert(get_pass_by_name(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME2, 
																pass_buff) == SA_SUCCESS);
	 test_assert(strncmp(TEST_PASS2, pass_buff, MAX_PASS_LEN) == 0);

	 test_assert(del_name_pass(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME) ==
							 SA_SUCCESS);
	 test_assert(get_pass_by_name(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME,
									pass_buff) ==	 SA_NAME_NOT_FOUND);
	 test_assert(get_pass_by_name(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME2,
									pass_buff) == SA_SUCCESS);
	 test_assert(strncmp(TEST_PASS2, pass_buff, MAX_PASS_LEN) == 0);

	 remove(NON_EXISTING_FILE);
	 
	 return UT_SUCCESS;
}

int test_list_names()
{
	 char ** name_list;
	 int num_names;

	 remove(NON_EXISTING_FILE);
	 test_assert(add_name_pass(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME, 
														 TEST_PASS) == SA_SUCCESS);
	 test_assert(get_name_list(NON_EXISTING_FILE, TEST_PASSWORD, &name_list,
														 &num_names) == SA_SUCCESS);
	 test_assert(num_names == 1);
	 test_assert(strcmp(name_list[0], TEST_NAME) == 0);

	 free(name_list[0]);
	 free(name_list);

	 test_assert(add_name_pass(NON_EXISTING_FILE, TEST_PASSWORD, TEST_NAME2, 
														 TEST_PASS2) == SA_SUCCESS);

	 test_assert(get_name_list(NON_EXISTING_FILE, TEST_PASSWORD, &name_list,
														 &num_names) == SA_SUCCESS);
	 test_assert(num_names == 2);
	 test_assert(strcmp(name_list[0], TEST_NAME) == 0);
	 test_assert(strcmp(name_list[1], TEST_NAME2) == 0);

	 free(name_list[0]);
	 free(name_list[1]);
	 free(name_list);
	 remove(NON_EXISTING_FILE);
	 
	 return UT_SUCCESS;
}
