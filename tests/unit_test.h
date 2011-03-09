/*****************************************************************************/
/* unit_test.h for Setec Astronomy                                           */
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
#ifndef UNIT_TEST_HH_
#define UNIT_TEST_HH_

#include <stdio.h>

enum {UT_SUCCESS = 0, UT_FAILED = -1, UT_NOT_IMPLEMENTED = -2};

#define test_assert( cond )																				\
	 if(!(cond)) {																									\
			printf("FAILED: %s " #cond " at %d in %s\n",								\
					   __FUNCTION__, __LINE__, __FILE__);			              \
      return UT_FAILED;																						\
   }

#define RUN_TEST(fn) {													\
	 int err = fn;																\
	 if(err == UT_SUCCESS)												\
			printf("Succeeded: %s\n", #fn);						\
	 else if(err == UT_NOT_IMPLEMENTED)						\
			printf("Not Implemented: %s\n", #fn);			\
	 else if(err == UT_FAILED)										\
			printf("Failed: %s\n", #fn);							\
	 }

#endif//UNIT_TEST_HH_
