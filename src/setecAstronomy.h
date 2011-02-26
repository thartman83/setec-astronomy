/*****************************************************************************/
/* setecAstronomy.h for Setec Astronomy                                      */
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
#ifndef SETECASTRONOMY_H_
#define SETECASTRONOMY_H_

static const int MAX_NAME_LEN = 50;
static const int MAX_PASS_LEN = 25;

struct name_pass_pair
{
	 char * name;
	 char * pass;
};

void init_name_pass_pair(struct name_pass_pair * pair);
void free_name_pass_pair(struct name_pass_pair * pair);

#endif//SETECASTRONOMY_H_
