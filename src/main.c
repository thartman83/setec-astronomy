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
#include "setecAstronomy.h"
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>

int main(int argc, char ** argv)
{
	 SHA256_CTX ctx;
	 int len = 7;
	 unsigned char input[SHA256_DIGEST_LENGTH] = "somekey\0";
	 unsigned char md[SHA256_DIGEST_LENGTH];
	 int x;

/* Prime the hash */
	 SHA256_Init(&ctx);
	 SHA256_Update(&ctx, input, len);
	 SHA256_Final(md, &ctx);

/* Loop the hash */
	 for(x = 0; x < 1024; ++x) {
			SHA256_Init(&ctx);
			SHA256_Update(&ctx, input, SHA256_DIGEST_LENGTH);
			SHA256_Final(md, &ctx);
			strncpy((char*)input, (char*)md, SHA256_DIGEST_LENGTH);
	 }

	 for(x = 0; x < SHA256_DIGEST_LENGTH; ++x) {
			printf("%c",md[x]);
	 }			

	 return 0;
}
