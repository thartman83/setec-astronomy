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
#include <stdio.h>

int add_name_pass(const char * password_file, const char * master_password, 
									const char * name, const char * password)
{
	 struct little_black_box lbb;
	 int err;

	 if( file_exists(password_file) ) {
			err = append_name_pass(password_file, master_password, name, password);
	 } else {
			err = lbb_open(&lbb, SA_CRYPT_MODE, password_file, master_password);
			if( err == SA_SUCCESS ) {
				 err = lbb_write_name_pass(&lbb, name, password);			
			}
			lbb_close(&lbb);
	 }
	 
	 return err;
}

int append_name_pass(const char * password_file, const char * master_password,
										 const char * name, const char * password)
{
	 struct little_black_box r_lbb;
	 struct little_black_box w_lbb;
	 int err;

	 err = lbb_open_rw(&r_lbb, &w_lbb, password_file, master_password);
	 if(err != SA_SUCCESS) {
			lbb_close_rw(&r_lbb, &w_lbb, password_file, err);
			return err;
	 }

	 err = lbb_copy(&r_lbb, &w_lbb);
	 if(err != SA_SUCCESS) {
			lbb_close_rw(&r_lbb, &w_lbb, password_file, err);
			return err;
	 }

	 err = lbb_write_name_pass(&w_lbb, name, password);
	 return lbb_close_rw(&r_lbb, &w_lbb, password_file, err);
}

int del_name_pass(const char * password_file, const char * master_password,
									const char * name)
{
	 struct little_black_box r_lbb;
	 struct little_black_box w_lbb;
	 struct name_pass_pair pair;
	 int err;

	 err = lbb_open_rw(&r_lbb, &w_lbb, password_file, master_password);
	 if(err != SA_SUCCESS) {
			lbb_close_rw(&r_lbb, &w_lbb, password_file, err);
			return err;
	 }

	 while((err = lbb_read_pair(&r_lbb, &pair)) == SA_SUCCESS) {
			if(strncmp(pair.name, name, MAX_NAME_LEN) != 0) {
				 err = lbb_write_pair(&w_lbb, pair);
				 if(err != SA_SUCCESS)
						break;
			}
	 }

	 return lbb_close_rw(&r_lbb, &w_lbb, password_file, 
											 (err >= 0 ? SA_SUCCESS : err));
}

int get_pass_by_name(const char * password_file, const char * master_password,
										 const char * name, char * pass)
{
	 struct little_black_box lbb;
	 struct name_pass_pair pair;	 
	 int err;

	 err = lbb_open(&lbb, SA_DECRYPT_MODE, password_file, master_password);
	 if(err != SA_SUCCESS)
			return err;

	 while((err = lbb_read_pair(&lbb, &pair)) == SA_SUCCESS) {
			if(strncmp(pair.name, name, MAX_NAME_LEN) == 0) {
				 strncpy(pass, pair.pass, MAX_PASS_LEN);
				 break;
			}
	 }

	 lbb_close(&lbb);

	 return (err == SA_NO_MORE_PAIRS) ? SA_NAME_NOT_FOUND : err;
}

int get_name_list(const char * password_file, const char * master_password,
									char *** name_list, int * num_names)
{
	 struct little_black_box lbb;
	 struct name_pass_pair pair;
	 int x, err;

	 err = lbb_open(&lbb, SA_DECRYPT_MODE, password_file, master_password);
	 if(err != SA_SUCCESS)
			return err;

	 *num_names = 0;
	 while((err = lbb_read_pair(&lbb, &pair)) == SA_SUCCESS)
			(*num_names)++;
	 
	 lbb_close(&lbb);

	 if(err != SA_NO_MORE_PAIRS)
			return err;	 
	 
	 err = lbb_open(&lbb, SA_DECRYPT_MODE, password_file, master_password);
	 if(err != SA_SUCCESS) {
			lbb_close(&lbb);
			return err;
	 }

	 *name_list = (char**)malloc((*num_names)*sizeof(char*));
	 x = 0;
	 while((err = lbb_read_pair(&lbb, &pair)) == SA_SUCCESS) {
			(*name_list)[x] = malloc((strlen(pair.name) + 1) * sizeof(char));
			strcpy((*name_list)[x],pair.name);
			memset(pair.pass, 0, strlen(pair.pass));
			memset(pair.name, 0, strlen(pair.name));
			x++;
	 }

	 lbb_close(&lbb);
	 return (err != SA_NO_MORE_PAIRS ? err : SA_SUCCESS);
}

int import_name_pass(const char * password_file, const char * master_password,
										 const char * import_file)
{
	 struct little_black_box r_lbb, w_lbb;
	 struct name_pass_pair pair;
	 FILE * fd = NULL;
	 char * line = NULL;
	 int err = SA_SUCCESS, len = 0, read_len = 0;

	 err = lbb_open_rw(&r_lbb, &w_lbb, password_file, master_password);

	 if(err != SA_SUCCESS) {
			lbb_close_rw(&r_lbb, &w_lbb, password_file, err);
			return err;
	 }
	 
	 err = lbb_copy(&r_lbb, &w_lbb);

	 if(err != SA_SUCCESS) {
			lbb_close_rw(&r_lbb, &w_lbb, password_file, err);
			return err;
	 }
	 
	 fd = fopen(import_file, "r");
	 if(fd == NULL) {
			err = SA_CAN_NOT_OPEN_IMPORT_FILE;
			lbb_close_rw(&r_lbb, &w_lbb, password_file, err);
			return err;
	 }

	 while((read_len = getline(&line, (size_t *)&len, fd)) != -1) {
			/* chomp the newline */
			line[read_len-1] = '\0';
			err = init_name_pass_pair(line, read_len, &pair);
			if(err != SA_SUCCESS)
				 break;			

			err = lbb_write_pair(&w_lbb, pair);
			if(err != SA_SUCCESS)
				 break;				 
	 }
	 
	 free(line);
	 fclose(fd);
	 err = lbb_close_rw(&r_lbb, &w_lbb, password_file, err);
	 
	 return SA_SUCCESS;
}
