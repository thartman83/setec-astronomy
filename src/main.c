/*****************************************************************************/
/* main.c for Setec Astronomy                                                */
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
#include "little_black_box.h"
#include <string.h>

int main(int argc, char ** argv)
{
	 struct little_black_box lbb;
	 char test_data[] = "test_name=test_password\n\0";

	 open_new_lbb(&lbb, "/home/thartman/new_setec_test", "foobarbaz");
	 write_lbb(&lbb, test_data, strlen(test_data));
	 close_lbb(&lbb);

	 return 0;
}
