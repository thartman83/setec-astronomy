/*****************************************************************************/
/* errors.c for Setec Astronomy                                              */
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
#include "errors.h"
#include <stdio.h>

void print_err_msg(int err)
{
	 switch(err) {
			case SA_FILE_NOT_FOUND:
				 printf("Could not find file.\n");
				 break;
			case SA_NO_DATA:
				 printf("No data to read.\n");
				 break;
			case SA_CAN_NOT_OPEN_FILE:
				 printf("Can not open file.\n");
				 break;
			case SA_CAN_NOT_WRITE_FILE:
				 printf("Can not write file.\n");
				 break;
			case SA_CAN_NOT_OPEN_CRYPT_MODULE:
				 printf("Can not open mcrypt module.\n");
				 break;
			case SA_INVALID_IV_SIZE:
				 printf("Invalid initialization vector.\n");
				 break;
			case SA_CAN_NOT_INIT_CRYPT:
				 printf("Could not initialize mcrypt.\n");
				 break;
			case SA_FILE_EXISTS:
				 printf("File already exists, will not clobber.\n");
				 break;
			case SA_CAN_NOT_CRYPT:
				 printf("Can not crypt data.\n");
				 break;
			case SA_WRONG_MODE:
				 printf("Function called on a little black box struct with "
								"wrong mode\n");
				 break;
			case SA_NOT_ENOUGH_DATA:
				 printf("Not enough data found in password file, possibly corrupted.\n");
				 break;
			case SA_CAN_NOT_DECRYPT:
				 printf("Could not decrypt data.\n");
				 break;
			case SA_PAIR_TOO_LONG:
				 printf("Name and password pair too long.\n");
				 break;
			case SA_NOT_PAIR_FORMAT:
				 printf("Pair data was not formatted properly.\n");
				 break;
			case SA_INVALID_HEADER:
				 printf("Invalid Setec Astronomy header found.\n");
				 break;
			case SA_INVALID_CRYPT_MODE:
				 printf("Invalid crypt mode selected.\n");
				 break;
			case SA_KEY_TOO_SHORT:
				 printf("Crypto key too short for crypt mode.\n");
				 break;
			case SA_KEYGEN_FAILED:
				 printf("Failed to generate key from password.\n");
				 break;
			case SA_NAME_NOT_FOUND:
				 printf("Could not find name within password file.\n");
				 break;
			case SA_COULD_NOT_RENAME:
				 printf("Could not rename temporary password file.\n");
				 break;
			case SA_PASS_TOO_LONG:
				 printf("Password too short.\n");
				 break;
			case SA_NO_TERM_SET:
				 printf("Could not set tty to no echo.\n");
				 break;
			case SA_NO_TERM_GET:
				 printf("Could not get tty info.\n");
				 break;
			case SA_UNABLE_TO_PBKDF2:
				 printf("Could not hash password using pbkdf2.\n");
				 break;
			case SA_WRONG_PASSWORD:
				 printf("Wrong password.\n");
				 break;
			case SA_NOT_IMPLEMENTED:
				 printf("INTERNAL ERROR: function not implemented.\n");
				 break;
	 }
}
