/*****************************************************************************/
/* test_util.c for Setec Astronomy                                           */
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
#include "test_util.h"
#include "../../src/util.h"
#include "../unit_test.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	 RUN_TEST(test_int_from_2bytes());
	 RUN_TEST(test_int_to_2bytes());
	 RUN_TEST(test_create_temp_filename());

	 return 0;
}

int test_int_from_2bytes()
{
	 unsigned char buf[2];
	 int val;

	 buf[0] = 204;
	 buf[1] = 5;
	 
	 val = int_from_2bytes(buf);
	 test_assert(val == 52229);
	 
	 return 0;
}

int test_int_to_2bytes()
{
	 unsigned char buf[2];
	 int val;

	 val = 52229;
	 int_to_2bytes(val, buf);
	 
	 test_assert(buf[0] == 204 && buf[1] == 5);
	 
	 return 0;
}

int test_create_temp_filename()
{
	 char * temp_name;

	 temp_name = create_temp_filename("foobarbaz\0");
	 test_assert(strcmp(temp_name, "foobarbaz.tmp\0") == 0);

	 free(temp_name);
	 
	 return UT_SUCCESS;
}
