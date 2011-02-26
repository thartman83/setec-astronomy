/*****************************************************************************/
/* setecAstronomy.c for Setec Astronomy                                      */
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
#include "setecAstronomy.h"
#include <stdlib.h>
#include <string.h>

void init_name_pass_pair(struct name_pass_pair * pair)
{
	 pair = (struct name_pass_pair *)malloc(sizeof(struct name_pass_pair));
	 pair->name = (char *)malloc(sizeof(char)*MAX_NAME_LEN);
	 pair->pass = (char *)malloc(sizeof(char)*MAX_PASS_LEN);
}

void free_name_pass_pair(struct name_pass_pair * pair)
{
	 /* Zero out the data before freeing it */
	 memset(pair->name, 0, sizeof(char)*MAX_NAME_LEN);
	 memset(pair->pass, 0, sizeof(char)*MAX_PASS_LEN);

	 free(pair->name);
	 free(pair->pass);
	 free(pair);
}
