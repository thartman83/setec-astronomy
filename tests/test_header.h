/*****************************************************************************/
/* test_header.h for Setec Astronomy                                         */
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
#ifndef TEST_HEADER_H_
#define TEST_HEADER_H_

#define TEST_DATA_DIR "../tests/data/"
static const char NO_DATA_TEST[] = TEST_DATA_DIR "no_data";
static const char NOT_ENOUGH_DATA_TEST[] = TEST_DATA_DIR "not_enough_data";
static const char GOOD_HEADER_TEST[] = TEST_DATA_DIR "good_header_data";

int test_read_header();

#endif//TEST_HEADER_H_
