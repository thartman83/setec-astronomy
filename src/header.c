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

#include <stdlib.h>

void init_header(struct setec_astronomy_header * header)
{
	 header->salt_len = 0;
	 header->salt = NULL;
	 header->hash_count = 0;
	 header->hash_len = 0;
	 header->hash = NULL;
	 header->iv_len = 0;
	 header->iv = NULL;
}

void free_header(struct setec_astronomy_header * header)
{	 
	 header->salt_len = 0;
	 if(header->salt != NULL)
			free(header->salt);

	 if(header->hash != NULL)
			free(header->hash);

	 header->iv_len = 0;
	 if(header->iv != NULL)
			free(header->iv);
}

int read_header(struct setec_astronomy_header * header, const char * filename)
{
	 FILE * fd = fopen(filename, "rb");
	 int retval;

	 if(fd == NULL)
			return SA_FILE_NOT_FOUND;

	 retval = read_header_ext(header, fd);

	 fclose(fd);

	 return retval;
}

int read_header_ext(struct setec_astronomy_header * header, FILE * fd)
{
	 int err;
	 
	 err = read_len_data_pair(&header->salt_len, &header->salt, fd);
	 if(err != SA_SUCCESS)
			return err;
	 
	 err = read_int_len(&header->hash_count, fd);
	 if(err != SA_SUCCESS)
			return err;

	 err = read_len_data_pair(&header->hash_len, &header->hash, fd);
	 if(err != SA_SUCCESS)
			return err;
	 
	 err = read_len_data_pair(&header->iv_len, &header->iv, fd);
	 if(err != SA_SUCCESS)
			return err;
	 
	 return SA_SUCCESS;
}

/* Read INT_LEN bytes to determine data length, then read data,
	 returns SA_SUCCESS on success and SA_NO_DATA on failure.  

	 This function allocates space for data to be freed by caller.  
	 Recommended that data is set to NULL as preconditions, 
	 to avoid memory leaks.
	 This function assumes that fd is a file pointer open for reading.  */
int read_len_data_pair(int * len, void ** data, FILE * fd)
{
	 int err, read_len;

	 err = read_int_len(len, fd);
	 if(err != SA_SUCCESS)
			return err;

	 (*data) = malloc(*len);
	 read_len = fread(*data, sizeof(char), *len, fd);
	 if(read_len != *len)
			return SA_NO_DATA;

	 return SA_SUCCESS;
}

int read_int_len(int * len, FILE * fd)
{
	 int read_len;
	 unsigned char len_buffer[INT_LEN];

	 read_len = fread(len_buffer, sizeof(char), INT_LEN, fd);
	 if(read_len != INT_LEN)
			return SA_NO_DATA;

	 *len = int_from_2bytes(len_buffer);
	 return SA_SUCCESS;
}

int write_header(const struct setec_astronomy_header * header, 
								 const char * filename)
{
	 FILE * fd;
	 int retval;
	 
	 fd = fopen(filename, "wb");

	 if(fd == NULL)
			return SA_CAN_NOT_OPEN_FILE;
	 
	 retval = write_header_ext(header, fd);

	 fclose(fd);

	 return retval;
}

int write_header_ext(const struct setec_astronomy_header * header, FILE * fd)
{
	 int err;

	 err = write_len_data_pair(header->salt_len, header->salt, fd);
	 if(err != SA_SUCCESS)
			return err;

	 err = write_int_len(header->hash_count, fd);
	 if(err != SA_SUCCESS)
			return err;

	 err = write_len_data_pair(header->hash_len, header->hash, fd);
	 if(err != SA_SUCCESS)
			return err;

	 err = write_len_data_pair(header->iv_len, header->iv, fd);
	 if(err != SA_SUCCESS)
			return err;

	 return SA_SUCCESS;
}

int write_len_data_pair(int len, void * data, FILE * fd)
{
	 int err, write_len;

	 err = write_int_len(len, fd);
	 if(err != SA_SUCCESS)
			return err;

	 write_len = fwrite(data, 1, len, fd);
	 if(write_len != len)
			return SA_CAN_NOT_WRITE_FILE;		 

	 return SA_SUCCESS;
}

int write_int_len(int len, FILE * fd)
{
	 unsigned char buf[2];
	 int write_len;

	 int_to_2bytes(len, buf);
	 write_len = fwrite(buf, 1, INT_LEN, fd);
	 if(write_len != INT_LEN)
			return SA_CAN_NOT_WRITE_FILE;

	 return SA_SUCCESS;
}

int header_len(const struct setec_astronomy_header * header)
{
	 return (4 * INT_LEN) + header->salt_len + header->hash_len + header->iv_len;
}

void init_random_buffer(void ** buffer, int * buf_len, int len)
{
	 (*buf_len) = len;
	 *buffer = malloc(*buf_len);
	 set_random(*buffer, *buf_len);
}
