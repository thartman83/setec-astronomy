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

int main()
{	 
	 RUN_TEST(test_read_header());

	 return 0;
}

int test_read_header()
{
	 struct setec_astronomy_header r_header;
	 
	 /* Start off with a good test */
	 init_header(&r_header);
	 
	 test_assert(read_header(&r_header, GOOD_HEADER_TEST) == SA_SUCCESS);

	 /* Check to make sure that the read and write headers are identical */
	 test_assert(r_header.salt_len == 1024);
	 test_assert(r_header.iv_len == 1024);
	 test_assert(r_header.hash_count == 42);

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

	 return 0;
}
