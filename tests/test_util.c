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
#include "../src/util.h"
#include "unit_test.h"

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