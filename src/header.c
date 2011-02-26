/*****************************************************************************/
/* header.c for Setec Astronomy                                              */
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
#include "header.h"
#include "errors.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void init_header(struct setec_astronomy_header * header)
{
	 header->salt_len = 0;
	 header->salt = NULL;
	 header->iv_len = 0;
	 header->iv = NULL;
	 header->hash_count = 0;
}

void free_header(struct setec_astronomy_header * header)
{	 
	 if(header->salt != NULL)
			free(header->salt);

	 if(header->iv != NULL)
			free(header->iv);
}

int read_header(struct setec_astronomy_header * header, const char * filename)
{
	 int read_length;
	 unsigned char len_buffer[INT_LEN];
	 FILE * fd = fopen(filename, "rb");

	 /* Check to make sure the file opened */
	 if(fd == NULL) {
			return SA_FILE_NOT_FOUND;
	 }

	 /* Read the length of the salt */
	 read_length = fread(len_buffer, sizeof(char), INT_LEN, fd);
	 if(read_length != INT_LEN) {
			fclose(fd);
			return SA_NO_DATA;
	 }
	 header->salt_len = int_from_2bytes(len_buffer);
	 
	 /* Allocate and read the salt */	 
	 header->salt = malloc(header->salt_len);
	 read_length = fread(header->salt, sizeof(char), header->salt_len, fd);
	 if(read_length != header->salt_len) {
			fclose(fd);
			return SA_NO_DATA;
	 }
	 
	 /* Read the initialization vector length */
	 read_length = fread(len_buffer, sizeof(char), INT_LEN, fd);
	 if(read_length != INT_LEN) {
			fclose(fd);
			return SA_NO_DATA;
	 }
	 header->iv_len = int_from_2bytes(len_buffer);

	 /* Allocate and read the initialization vector */
	 header->iv = malloc(header->iv_len);
	 read_length = fread(header->iv, sizeof(char), header->iv_len, fd);
	 if(read_length != header->iv_len) {
			fclose(fd);
			return SA_NO_DATA;
	 }

	 /* Read the hash count, the number of times to hash the salt + key */
	 read_length = fread(len_buffer, sizeof(char), INT_LEN, fd);
	 if(read_length != INT_LEN) {
			fclose(fd);
			return SA_NO_DATA;
	 }
	 header->hash_count = int_from_2bytes(len_buffer);

	 fclose(fd);

	 return SA_SUCCESS;
}

int write_header(struct setec_astronomy_header * header, const char * filename)
{
	 /* Length values in the structure are at maximum 2 bytes */
	 unsigned char buf[2];
	 FILE * fd;
	 int write_len;
	 
	 fd = fopen(filename, "wb");

	 /* check to make sure the file description opened up properly */
	 if(fd == NULL)
			return SA_CAN_NOT_OPEN_FILE;
	 
	 /* write the salt_len first */
	 int_to_2bytes(header->salt_len, buf);
	 write_len = fwrite(buf, 1, INT_LEN, fd);
	 if(write_len != INT_LEN) {
			fclose(fd);
			return SA_CAN_NOT_WRITE_FILE;
	 }
	 
	 /* write the salt */
	 write_len = fwrite(header->salt, 1, header->salt_len, fd);
	 if(write_len != header->salt_len) {
			fclose(fd);
			return SA_CAN_NOT_WRITE_FILE;
	 }

	 /* write the iv length */
	 int_to_2bytes(header->iv_len, buf);
	 write_len = fwrite(buf, 1, INT_LEN, fd);	 
	 if(write_len != INT_LEN) {
			fclose(fd);
			return SA_CAN_NOT_WRITE_FILE;
	 }

	 /* write the iv */
	 write_len = fwrite(header->iv, 1, header->iv_len, fd);
	 if(write_len != header->iv_len) {
			fclose(fd);
			return SA_CAN_NOT_WRITE_FILE;
	 }

	 /* write the hash count */
	 int_to_2bytes(header->hash_count, buf);
	 write_len = fwrite(buf, 1, INT_LEN, fd);
	 if(write_len != INT_LEN) {
			fclose(fd);
			return SA_CAN_NOT_WRITE_FILE;
	 }

	 fclose(fd);

	 return 0;
}

void init_salt(struct setec_astronomy_header * header, int salt_len)
{
	 /* Allocate the salt */
	 header->salt_len = salt_len;
	 header->salt = malloc(salt_len);
	 
	 set_random(header->salt, header->salt_len);
}

void init_iv(struct setec_astronomy_header * header, int iv_len)
{
	 /* Allocate the IV */
	 header->iv_len = iv_len;
	 header->iv = malloc(iv_len);

	 set_random(header->iv, header->iv_len);
}
