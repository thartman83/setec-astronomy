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

void regen_file_exists()
{
	 struct setec_astronomy_header header;
	 remove(FILE_EXISTS);

	 init_header(&header);
	 header.salt_len = 256;
	 init_random_buffer(&(header.salt), header.salt_len);

	 header.hash_count = 256;
	 header.hash_len = 256;
	 init_random_buffer(&(header.hash), header.hash_len);

	 header.iv_len = 256;
	 init_random_buffer(&(header.iv), header.iv_len);

	 write_header(&header, FILE_EXISTS);
	 free_header(&header);
}

void regen_good_header_data()
{
	 struct setec_astronomy_header header;
	 remove(GOOD_HEADER_DATA);

	 init_header(&header);

	 header.salt_len = 256;
	 init_random_buffer(&(header.salt), header.salt_len);

	 header.hash_count = 256;
	 header.hash_len = 256;
	 init_random_buffer(&(header.hash), header.hash_len);

	 header.iv_len = 256;
	 init_random_buffer(&(header.iv), header.iv_len);

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
