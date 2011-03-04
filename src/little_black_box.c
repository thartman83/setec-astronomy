/*****************************************************************************/
/* little_black_box.c for Setec Astronomy                                    */
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
#include "key.h"
#include <string.h>
#include <stdlib.h>

int init_lbb(struct little_black_box * lbb, int crypt_mode)
{
	 int buffer_size;

	 /* set everything to their default values */
	 init_header(&(lbb->header));
	 lbb->md = NULL;
	 lbb->fd = NULL;
	 lbb->buffer = NULL;
	 lbb->data_len = 0;
	 lbb_ref_count++;
	 lbb->crypt_mode = crypt_mode;

	 /* setup a mcrypt descriptor */
	 lbb->md = mcrypt_module_open((char *)CRYPT_ALGO, NULL, 
																(char *)CRYPT_MODE, NULL);

	 if(lbb->md == NULL)
			return SA_CAN_NOT_OPEN_CRYPT_MODULE;

	 lbb->block_size = mcrypt_enc_get_block_size(lbb->md);

	 /* If we are crypting data the buffer needs to be the block size,
			if we are decrypting the buffer needs to be the MAX_PAIR_SIZE +
			the block size.  This is for the corner case where the first character 
			of a read block is the \n terminating a pair string */
	 buffer_size = (crypt_mode == SA_CRYPT_MODE ? lbb->block_size : 
									MAX_PAIR_SIZE + lbb->block_size);
	 lbb->buffer = calloc(1, buffer_size);

	 return SA_SUCCESS;
}

int open_new_lbb(struct little_black_box * lbb, const char * filename, 
								 const char * password)
{
	 int iv_size = 0;
	 FILE * tmp_fd = NULL;	 
	 int err = 0;

	 /* initialize the little black box to defaults */
	 err = init_lbb(lbb, SA_CRYPT_MODE);
	 if(err != SA_SUCCESS)
			return err;

	 /* Setup the new header */
	 iv_size = mcrypt_enc_get_iv_size(lbb->md);
	 if(iv_size < 0)
			return SA_INVALID_IV_SIZE;

	 init_iv(&(lbb->header), iv_size);
	 init_salt(&(lbb->header), 0);
	 
	 /* Check to see if the filename already exists, no clobbering */
	 tmp_fd = fopen(filename, "rb");
	 if(tmp_fd != NULL) {
			fclose(tmp_fd);
			return SA_FILE_EXISTS;
	 }

	 lbb->fd = fopen(filename, "wb");

	 write_header_ext(&(lbb->header), lbb->fd);

	 return open_lbb_ext(lbb, password);
}

int open_lbb(struct little_black_box * lbb, const char * filename,
						 const char * password)
{	 
	 int err = 0;

	 /* initialize the little black box to defaults */
	 err = init_lbb(lbb, SA_DECRYPT_MODE);
	 if(err != SA_SUCCESS)
			return err;
	 
	 /* open up the file and read the header */
	 lbb->fd = fopen(filename, "rb");
	 if(lbb->fd == NULL)
			return SA_FILE_NOT_FOUND;

	 err = read_header_ext(&(lbb->header), lbb->fd);
	 if(err != SA_SUCCESS)
			return SA_INVALID_HEADER;

	 return open_lbb_ext(lbb, password);
}

int open_lbb_ext(struct little_black_box * lbb, const char * password)
{
	 char * key = calloc(1, KEY_LEN);

	 hash_key(password, strlen(password), 1, key);
	 
	 if(mcrypt_generic_init(lbb->md, key, KEY_LEN, lbb->header.iv) < 0)
			return SA_CAN_NOT_INIT_CRYPT;

	 return SA_SUCCESS;
}

int close_lbb(struct little_black_box * lbb)
{
	 int err;
	 /* write anything that may be left in the buffer */
	 if(lbb->data_len != 0 && lbb->crypt_mode == SA_CRYPT_MODE) {
			err = write_lbb_buffer(lbb);
			if(err != SA_SUCCESS)
				 return err;
	 }

	 /* free up the header */
	 free_header(&(lbb->header));

	 /* deinit and close the mcrypt descriptor (if necessary) */
	 mcrypt_generic_deinit(lbb->md);

	 lbb_ref_count--;

	 /* if this the last little black box to be closed kill the module as well */
	 if(lbb_ref_count == 0)
			mcrypt_module_close(lbb->md);

	 /* zero out the buffer before freeing it */
	 if(lbb->buffer != NULL) {
			memset(lbb->buffer, 0, lbb->block_size);
			free(lbb->buffer);
	 }

	 fclose(lbb->fd);

	 return SA_SUCCESS;
}

int write_lbb(struct little_black_box * lbb, void * buffer, int buffer_len)
{
	 unsigned char * tmp;
	 unsigned char * pun = (unsigned char *)buffer; /* type punned version of buffer */
	 int size;
	 int err;

	 if(lbb->crypt_mode != SA_CRYPT_MODE)
			return SA_WRONG_MODE;

	 while(buffer_len > 0) { /* loop until we've exhausted all of the data */
			/* figure out how much data can fit into the buffer */
			size = ((lbb->block_size - lbb->data_len) > buffer_len ? buffer_len : 
							(lbb->block_size - lbb->data_len));

			/* pun the buffer and advance the pointer to the point where it can take 
				 new data */
			tmp = (unsigned char *)lbb->buffer + lbb->data_len;

			/* copy the data to the buffer */
			memcpy(tmp, pun, size);
			
			/* Check to see if the buffer needs to be written and purged before 
				 the next pass, otherwise recalc the data length*/
			if((size + lbb->data_len) == lbb->block_size) {
				 err = write_lbb_buffer(lbb);
				 if(err != SA_SUCCESS)
						return err;
			}else
				 lbb->data_len += size;

			/* update the new buffer and buffer length */
			buffer_len -= size;
			if(buffer_len != 0)
				 pun += size;
	 }				 

	 return SA_SUCCESS;
}

int write_lbb_buffer(struct little_black_box * lbb)
{
	 int write_amt;
	 int err;

	 if(lbb->crypt_mode != SA_CRYPT_MODE)
			return SA_WRONG_MODE;
	 
	 err = mcrypt_generic(lbb->md, lbb->buffer, lbb->block_size);
	 if(err != 0)
			return SA_CAN_NOT_CRYPT;
	 
	 /* write the data */
	 write_amt = fwrite(lbb->buffer, 1, lbb->block_size, lbb->fd);
	 if(write_amt != lbb->block_size)	 
			return SA_CAN_NOT_WRITE_FILE;

	 /* zero out the data */
	 memset(lbb->buffer, 0, lbb->block_size);
	 lbb->data_len = 0;

	 return SA_SUCCESS;
}

int read_next_pair(struct little_black_box * lbb, struct name_pass_pair * pair)
{
	 char read_buffer[lbb->block_size];
	 char pair_string[MAX_PAIR_SIZE];
	 char tmp[MAX_PAIR_SIZE + lbb->block_size];
	 int found = 0;
	 int len, err;
	 char * ptr;

	 if(lbb->crypt_mode != SA_DECRYPT_MODE)
			return SA_WRONG_MODE;

	 while(!found) {
			/* Check to see if there is a pair in the buffer, do this first in case one already
				 exists */
			ptr = strchr((const char *)lbb->buffer, '\n');
			if(ptr != NULL) {
				 /* Found the end of a pair */
				 len = ptr - (char*)lbb->buffer;
				 found = 1;
			}else{ 
				 /* Did not find pair, check to see if the buffer is full before adding new data */
				 if(lbb->data_len >= MAX_PAIR_SIZE)
						return SA_PAIR_TOO_LONG;

				 err = fread(read_buffer, 1, lbb->block_size, lbb->fd);
				 /* setec astronomy files sizes should be divisible by block size after the header */
				 if(err != lbb->block_size) {
						if(err == 0 && lbb->data_len == 0) { /* no more pairs left in file */
							 return SA_NO_MORE_PAIRS;
						}else{
							 return SA_NOT_ENOUGH_DATA;
						}
				 }

				 /* Decrypt the new data */
				 err = mdecrypt_generic(lbb->md, read_buffer, lbb->block_size);
				 if(err != 0)
						return SA_CAN_NOT_DECRYPT;

				 /* add the new data to the buffer for the next pass */
				 strncpy((char *)lbb->buffer + lbb->data_len, read_buffer, lbb->block_size);
			}
	 }

	 /* Copy the pair out */
	 strncpy(pair_string, lbb->buffer, len);
	 pair_string[len] = '\0';

	 /* put the data into the pair */
	 init_name_pass_pair(pair_string, pair);
	 
	 /* Zero out the temp buffer to provide \0 termination when we are done */
	 memset(tmp, 0, MAX_PAIR_SIZE + lbb->block_size);

	 /* Copy the non-pair data to the beginning of the temporary buffer */
	 memcpy(tmp, lbb->buffer + (len + 1), MAX_PAIR_SIZE + lbb->block_size - len);

	 /* Copy the temporary buffer back to buffer */
	 memcpy(lbb->buffer, tmp, MAX_PAIR_SIZE + lbb->block_size);

	 /* New buffer should null terminated at the end of the data */
	 lbb->data_len = strlen(lbb->buffer);

	 return SA_SUCCESS;
}

int init_name_pass_pair(const char * pair_string, struct name_pass_pair * pair)
{
	 char * ptr = NULL;
	 int len;

	 ptr = strchr(pair_string, '=');

	 if(ptr == NULL)
			return SA_NOT_PAIR_FORMAT;

	 len = ptr - pair_string;
	 
	 strncpy(pair->name, pair_string, len);
	 pair->name[len] = '\0';

	 strcpy(pair->pass, pair_string + (len + 1));
	 
	 return SA_SUCCESS;
}
