/*****************************************************************************/
/* test_little_black_box.h for Setec Astronomy                               */
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
#ifndef TEST_LITTLE_BLACK_BOX_HH_
#define TEST_LITTLE_BLACK_BOX_HH_

static const char EXISTING_FILE[]= "../tests/data/file_exists";
static const char NON_EXISTING_FILE[] = "../tests/data/file_does_not_exists";
static const char NO_DATA_FILE[] = "../tests/data/no_data";
static const char TEST_PASSWORD[] = "foobarbaz";

int test_init_lbb();
int test_open_new_lbb();
int test_open_lbb();
int test_open_lbb_ext();
int test_close_lbb();
int test_write_lbb();
int test_write_lbb_buffer();
int test_read_next_pair();
int test_init_name_pass_pair();

#endif//TEST_LITTLE_BLACK_BOX_HH_
