/*****************************************************************************/
/* errors.h for Setec Astronomy                                              */
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
#ifndef ERRORS_HH_
#define ERRORS_HH_

enum { SA_SUCCESS = 0,
			 SA_NO_MORE_PAIRS = 1,
			 SA_FILE_NOT_FOUND = -1,
			 SA_NO_DATA = -2,
			 SA_CAN_NOT_OPEN_FILE = -3,
			 SA_CAN_NOT_WRITE_FILE = -4,
			 SA_CAN_NOT_OPEN_CRYPT_MODULE = -5,
			 SA_INVALID_IV_SIZE = -6,
			 SA_CAN_NOT_INIT_CRYPT = -7,
			 SA_FILE_EXISTS = -8,
			 SA_CAN_NOT_CRYPT = -9,
			 SA_WRONG_MODE = -10, 
			 SA_NOT_ENOUGH_DATA = -11,
			 SA_CAN_NOT_DECRYPT = -12,
			 SA_PAIR_TOO_LONG = -13,
			 SA_NOT_PAIR_FORMAT = -14,
			 SA_INVALID_HEADER = -15,
			 SA_INVALID_CRYPT_MODE = -16,
			 SA_KEY_TOO_SHORT = -17,
			 SA_KEYGEN_FAILED = -18,
			 SA_NAME_NOT_FOUND = -19,
			 SA_COULD_NOT_RENAME = -20,
			 SA_PASS_TOO_LONG = -21,
			 SA_NO_TERM_SET = -22,
			 SA_NO_TERM_GET = -23,
			 SA_NOT_IMPLEMENTED = -100};

void print_err_msg(int err);
			 
#endif//ERRORS_HH_
