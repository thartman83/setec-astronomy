/*****************************************************************************/
/* util.c for Setec Astronomy                                                */
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
#include "util.h"

#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>

void set_random(void * ptr, int len)
{
	 int x;
	 unsigned char * punned_ptr; /* ptr for type punning header->salt */

	 /* Seed the rng */
	 srand(time_seed());
	 punned_ptr = (unsigned char *)ptr;

	 for(x = 0; x < len; ++x)
			punned_ptr[x] = rand() % (UCHAR_MAX + 1);
}

int int_from_2bytes(unsigned char buffer[INT_LEN])
{
	 return (buffer[0] << 8) + buffer[1];
}

void int_to_2bytes(int val, unsigned char buf[INT_LEN])
{
	 buf[0] = (val >> 8) & UCHAR_MAX;
	 buf[1] = val & UCHAR_MAX;
}

/* The code below was found at: 
	 http://www.velocityreviews.com/forums/t597431-srand-time-null.html */
unsigned int time_seed ()
{
	 time_t timeval;/* Current time. */
	 unsigned char *ptr;/* Type punned pointed into timeval. */
	 unsigned int seed;/* Generated seed. */
	 size_t i;

	 timeval = time (NULL);
	 ptr = (unsigned char *) &timeval;

	 seed = 0;
	 for (i = 0; i < sizeof(timeval); i++)
			seed = seed * (UCHAR_MAX + 2u) + ptr[i];

	 return seed;
}
