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
#include "errors.h"
#include <string.h>

int encrypt_test();
int decrypt_test();

int main(int argc, char ** argv)
{
	 if(argc < 2)
			return -1;

	 if(argv[1][0] == 'e')
			return encrypt_test();
	 else if(argv[1][0] == 'd')
			return decrypt_test();
	 else
			return -1;
}

int encrypt_test()
{	 
	 struct little_black_box lbb;
	 char test_data[] = "test_name=test_password\nsecond=second-pass\n\0";
	 int err;

	 err = open_new_lbb(&lbb, "/home/thartman/new_setec_test", "foobarbaz");
	 if(err != SA_SUCCESS) {
			printf("%d\n", err);
			return err;
	 }
			
	 err = write_lbb(&lbb, test_data, strlen(test_data));
	 if(err != SA_SUCCESS) {
			printf("%d\n", err);
			return err;
	 }

	 err = close_lbb(&lbb);
	 if(err != SA_SUCCESS) {
			printf("%d\n", err);
			return err;
	 }
	 
	 return SA_SUCCESS;
}

int decrypt_test()
{
	 struct little_black_box lbb;
	 struct name_pass_pair pair;
	 int err;
	 
	 err = open_lbb(&lbb, "/home/thartman/new_setec_test", "foobarbaz");
	 if(err != SA_SUCCESS) {
			printf("%d\n", err);
			return err;
	 }

	 err = read_next_pair(&lbb, &pair);
	 if(err != SA_SUCCESS) {
			printf("%d\n", err);
			return err;
	 }

	 printf("%s=%s\n", pair.name, pair.pass);
	 
	 err = read_next_pair(&lbb, &pair);
	 if(err != SA_SUCCESS) {
			printf("%d\n", err);
			return err;
	 }

	 printf("%s=%s\n", pair.name, pair.pass);

	 err = close_lbb(&lbb);
	 if(err != SA_SUCCESS) {
			printf("%d\n", err);
			return err;
	 }

	 return SA_SUCCESS;
}
