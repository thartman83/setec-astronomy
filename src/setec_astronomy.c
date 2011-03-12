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

int add_entry(const char * password_file, const char * master_password, 
							const char * name, const char * password)
{
	 struct little_black_box r_lbb;
	 struct little_black_box w_lbb;
	 struct name_pass_pair pair;
	 char buffer[MAX_PAIR_SIZE];
	 char * new_password_file;
	 int err;

	 /* Open the existing file for decrypting */
	 err = open_lbb(&r_lbb, password_file, master_password);
	 if(err != SA_SUCCESS)
			return err;
	 	 
	 /* Open a new temporary file for encrypting the new data */
	 new_password_file = create_temp_filename(password_file);
	 err = open_new_lbb(&w_lbb, new_password_file, master_password);
	 if(err != SA_SUCCESS)
			return err;
	 
	 /* iterate through each name pass pair adding it to the new file */
	 while((err = read_next_pair(&r_lbb, &pair)) == SA_SUCCESS) {
			snprintf("%s=%s\n", MAX_PAIR_SIZE, pair.name, pair.pass);
			write_lbb(&w_lbb, buffer, strlen(buffer));
	 }

	 if(err != SA_NO_MORE_PAIRS)
			return err;

	 /* now add the new pair */
	 snprintf("%s=%s\n", MAX_PAIR_SIZE, name, password);
	 write_lbb(&w_lbb, buffer, strlen(buffer));

	 /* move the new file into position */
	 remove(password_file);
	 rename(new_password_file, password_file);

	 /* close up shop */
	 free(new_password_file);
	 close_lbb(&r_lbb);
	 close_lbb(&w_lbb);

	 return SA_SUCCESS;
}
