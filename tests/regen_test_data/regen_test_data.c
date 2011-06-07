/*****************************************************************************/
/* regen_test_data.c for Setec Astronomy                                     */
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
#include "../../src/header.h"

#define TEST_DATA_DIR "../tests/data/"
const char FILE_EXISTS[] = TEST_DATA_DIR "file_exists";
const char GOOD_HEADER_DATA[] = TEST_DATA_DIR "good_header_data";
const char NO_DATA[] = TEST_DATA_DIR "no_data";
const char NOT_ENOUGH_DATA[] = TEST_DATA_DIR "not_enough_data";
const char DEFAULT_PASSWORD[] = "foobarbaz";

static const int DEFAULT_IV_LEN = 256;
static const int DEFAULT_SALT_LEN = 10;
static const int DEFAULT_HASH_COUNT = 2000;
static const int DEFAULT_HASH_LEN = 256;

void regen_file_exists()
{
	 struct setec_astronomy_header header;
	 remove(FILE_EXISTS);

	 create_header(&header, DEFAULT_IV_LEN, DEFAULT_SALT_LEN, DEFAULT_HASH_COUNT,
								 DEFAULT_PASSWORD, DEFAULT_HASH_LEN);

	 write_header(&header, FILE_EXISTS);
	 free_header(&header);
}

void regen_good_header_data()
{
	 struct setec_astronomy_header header;
	 remove(GOOD_HEADER_DATA);

	 create_header(&header, DEFAULT_IV_LEN, DEFAULT_SALT_LEN, DEFAULT_HASH_COUNT,
								 DEFAULT_PASSWORD, DEFAULT_HASH_LEN);

	 write_header(&header, GOOD_HEADER_DATA);
	 free_header(&header);
}

void regen_no_data()
{
	 FILE * fd;
	 remove(NO_DATA);

	 fd = fopen(NO_DATA, "wb");
	 fclose(fd);
}

void regen_not_enough_data()
{
	 FILE * fd;
	 remove(NOT_ENOUGH_DATA);

	 fd = fopen(NOT_ENOUGH_DATA, "wb");	 
	 fwrite("abcdefg",1,7,fd);
	 fclose(fd);
}

int main()
{
	 regen_file_exists();
	 regen_good_header_data();
	 regen_no_data();
	 regen_not_enough_data();
	 
	 return 0;
}
