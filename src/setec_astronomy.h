/*****************************************************************************/
/* setec_astronomy.h for Setec Astronomy                                     */
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
#ifndef SETEC_ASTRONOMY_H_
#define SETEC_ASTRONOMY_H_

int add_name_pass(const char * password_file, const char * master_password,
									const char * name, const char * password);

int append_name_pass(const char * password_file, const char * master_password,
										 const char * name, const char * password);

int del_name_pass(const char * password_file, const char * master_password,
									const char * name);

int get_pass_by_name(const char * password_file, const char * master_password,
										 const char * name, char * password);

int get_name_list(const char * password_file, const char * master_password,
									char *** name_list, int * num_names);

int import_name_pass(const char * password_file, const char * master_password,
										 const char * import_file);

#endif//SETEC_ASTRONOMY_H_
