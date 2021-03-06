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

#define test_equals(val1, val2) {      																	\
			int UT_EXPANDED_val1 = val1;																			\
			int UT_EXPANDED_val2 = val2;																			\
			if(UT_EXPANDED_val1 != UT_EXPANDED_val2) {												\
				 printf("FAILED: %s Expected '" #val1 "' to be %d but got %d"		\
								" instead at line %d in %s!\n",													\
								__FUNCTION__, UT_EXPANDED_val2, UT_EXPANDED_val1,				\
								__LINE__, __FILE__);																		\
				 return UT_FAILED;																							\
			}																																	\
	 }

#define RUN_TEST(fn) {													\
	 int err = fn;																\
	 if(err == UT_SUCCESS)												\
			printf("PASS: %s\n", #fn);								\
	 else if(err == UT_NOT_IMPLEMENTED)						\
			printf("SKIP: %s\n", #fn);								\
	 }

#endif//UNIT_TEST_HH_
