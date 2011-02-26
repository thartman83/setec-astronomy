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
			 SA_FILE_NOT_FOUND = -1,
			 SA_NO_DATA = -2,
			 SA_CAN_NOT_OPEN_FILE = -3,
			 SA_CAN_NOT_WRITE_FILE = -4};
			 
#endif//ERRORS_HH_
