/*****************************************************************************/
/* setec_astronomy.c for Setec Astronomy                                     */
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
#include "setec_astronomy.h"
#include "errors.h"
#include "util.h"
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

static const char DEFAULT_PASSWORD_FILE[]="~/.setec_astronomy";

static struct option long_options[] = 
{
	 {"add", no_argument, 0, 'a'},
	 {"delete", no_argument, 0, 'd'},
	 {"get", required_argument, 0, 'g'},
	 {"help",no_argument, 0, 'h'},
	 {"name", required_argument, 0, 'n'},
	 {"password", required_argument, 0, 'p'},
	 {"generate", no_argument, 0, 'g'},
	 {"file", required_argument, 0, 'f'},
	 {"init", no_argument, 0, 'i'},
	 {0,0,0,0}
};

int get_password(const char * password_file, const char * master_password,
								 const char * name, char * pass)
{
	 struct little_black_box lbb;
	 struct name_pass_pair pair;	 
	 int err;

	 err = open_lbb(&lbb, password_file, master_password);
	 if(err != SA_SUCCESS)
			return err;

	 while((err = read_next_pair(&lbb, &pair)) == SA_SUCCESS) {
			if(strncmp(pair.name, name, MAX_NAME_LEN) == 0) {
				 strncpy(pass, pair.pass, MAX_PASS_LEN);
				 break;
			}
	 }

	 close_lbb(&lbb);

	 return (err == SA_NO_MORE_PAIRS) ? SA_NAME_NOT_FOUND : err;
}

int add_entry(const char * password_file, const char * master_password, 
							const char * name, const char * password)
{
	 struct little_black_box lbb;
	 int err;

	 if( file_exists(password_file) ) {
			err = append_entry(password_file, master_password, name, password);
	 } else {
			err = open_new_lbb(&lbb, password_file, master_password);
			if( err == SA_SUCCESS ) {
				 err = write_lbb_name_pass(&lbb, name, password);
				 close_lbb(&lbb);
			}
	 }
	 
	 return err;
}

int append_entry(const char * password_file, const char * master_password,
								 const char * name, const char * password)
{
	 struct little_black_box r_lbb;
	 struct little_black_box w_lbb;
	 int err;
	 char * temp_password_file;

	 temp_password_file = gen_temp_filename(password_file);
	 if(rename(password_file, temp_password_file) != 0)
			return SA_COULD_NOT_RENAME;

	 err = open_lbb(&r_lbb, temp_password_file, master_password);
	 if(err != SA_SUCCESS) {
			rename(temp_password_file, password_file);
			free(temp_password_file);
			return err;
	 }
	 
	 err = open_new_lbb(&w_lbb, password_file, master_password);
	 if(err != SA_SUCCESS) {
			rename(temp_password_file, password_file);
			free(temp_password_file);
			close_lbb(&r_lbb);			
			return err;
	 }

	 err = copy_contents(&r_lbb, &w_lbb);
	 if(err != SA_SUCCESS) {
			rename(temp_password_file, password_file);
			free(temp_password_file);
			close_lbb(&w_lbb);
			close_lbb(&r_lbb);
	 }

	 err = write_lbb_name_pass(&w_lbb, name, password);	 
	 if(err != SA_SUCCESS) 
			rename(temp_password_file, password_file);
	 else
			remove(temp_password_file);
	 
	 free(temp_password_file);
	 close_lbb(&w_lbb);
	 close_lbb(&r_lbb);
	 	 
	 return err;
}
