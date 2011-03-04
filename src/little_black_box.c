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
#include <string.h>

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

	 init_iv(header, iv_size);
	 init_salt(header, 0);
	 
	 /* Check to see if the filename already exists, no clobbering */
	 tmp_fd = fopen(filename, "rb");
	 if(tmp_fd != NULL) {
			fclose(tmp_fd);
			return SA_FILE_EXISTS;
	 }

	 lbb->fd = fopen(filename, "wb");

	 return open_lbb_ext(lbb, password);
}

int open_lbb(struct little_black_box * lbb, const char * filename,
						 const char * password)
{
	 int iv_size;	 
	 int err = 0;

	 /* initialize the little black box to defaults */
	 err = init_lbb(lbb, SA_DECRYPT_MODE);
	 if(err != SA_SUCCESS)
			return err;
	 
	 /* open up the file and read the header */
	 lbb->fd = fopen(filename, "rb");
	 if(fd == NULL)
			return SA_FILE_NOT_FOUND;

	 read_header_ext(&(lbb->header), lbb->fd);

	 return open_lbb_ext(lbb, password);
}

int open_lbb_ext(struct little_black_box * lbb, const char * password)
{
	 char * key = calloc(1, KEY_LEN);

	 hash_key(password, strlen(password), 1, key);
	 
	 if(mcrypt_generic_init(lbb->md, key, KEY_LEN, lbb->header->iv) < 0)
			return SA_CAN_NOT_INIT_CRYPT;

	 /* initialize the buffer to defaults */
	 lbb->buffer = NULL;
	 lbb->data_len = 0;

	 return SA_SUCCESS;
}

int init_lbb(struct little_black_box * lbb, int crypt_mode)
{
	 /* set everything to their default values */
	 init_header(&(lbb->header));
	 lbb->md = NULL;
	 lbb->fd = NULL;

	 /* If we are crypting data the buffer needs to be the block size,
			if we are decrypting the buffer needs to be the MAX_PAIR_SIZE +
			the block size.  This is for the corner case where the first character 
			of a read block is the \n terminating a pair string */
	 lbb->buffer = calloc(1, (crypt_mode == SA_CRYPT_MODE ? BLOCK_SIZE : 
														MAX_PAIR_SIZE + BLOCK_SIZE));
	 lbb->data_len = 0;
	 lbb->ref_count++;
	 lbb->crypt_mode = crypt_mode;

	 /* setup a mcrypt descriptor */
	 if(lbb->ref_count == 1)
			lbb->md = mcrypt_module_open((char *)CRYPT_ALGO, NULL, 
																	 (char *)CRYPT_MODE, NULL);

	 if(lbb->md == NULL)
			return SA_CAN_NOT_OPEN_CRYPT_MODULE;
	 else
			return SA_SUCCESS;
}

int close_lbb(struct little_black_box * lbb)
{
	 /* write anything that may be left in the buffer */
	 if(lbb->data_len != 0)
			write_lbb_buffer(lbb);

	 /* free up the header */
	 free_header(lbb->header);

	 /* deinit and close the mcrypt descriptor (if necessary) */
	 mcrypt_generic_deinit(lbb->md);

	 if(lbb->ref_count == 1)
			mcrypt_close_module(lbb->md);
	 else
			lbb->ref_count--;				 

	 /* close the file descriptor */
	 fclose(fd);

	 /* zero out the buffer before freeing it */
	 if(lbb->buffer != NULL) {
			memset(lbb->buffer, 0, BLOCK_SIZE);
			free(lbb->buffer);
	 }

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
			size = ((BLOCK_SIZE - lbb->data_len) > buffer_len ? buffer_len : 
							(BLOCK_SIZE - lbb->data_len));

			/* pun the buffer and advance the pointer to the point where it can take 
				 new data */
			tmp = (unsigned char *)lbb->buffer + (lbb->data_len + 1);

			/* copy the data to the buffer */
			memcpy(tmp, pun, size);
			
			/* Check to see if the buffer needs to be written and purged before 
				 the next pass, otherwise recalc the data length*/
			if((size + lbb->data_len) == BLOCK_SIZE) {
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

	 if(lbb->crypt_mode != SA_CRYPT_MODE)
			return SA_WRONG_MODE;

	 /* encrypt the data */
	 if(mcrypt_generic(lbb->md, lbb->buffer, BLOCK_SIZE) != 0)
			return SA_CAN_NOT_CRYPT;
	 
	 /* write the data */
	 write_amt = fwrite(lbb->buffer, 1, BLOCK_SIZE, lbb->fd);
	 if(write_amt != BLOCK_SIZE)	 
			return SA_CAN_NOT_WRITE_FILE;

	 /* zero out the data */
	 memset(lbb->buffer, 0, BLOCK_SIZE);
	 lbb->data_len = 0;
}

int read_next_pair(struct little_black_box * lbb, struct name_pass_pair * pair)
{
	 unsigned char read_buffer[BLOCK_SIZE];
	 unsigned char pair_string[MAX_PAIR_SIZE];
	 unsigned char tmp[MAX_PAIR_SIZE];
	 int found = 0;
	 int len, err;
	 unsigned char * ptr;

	 if(lbb->crypt_mode != SA_DECRYPT_MODE)
			return SA_WRONG_MODE;

	 while(!found) {
			/* Check to see if there is a pair in the buffer, do this first in case one already
				 exists */
			ptr = strchr(lbb->buffer, '\n');
			if(ptr != NULL) {
				 /* Found the end of a pair */
				 len = lbb->buffer - ptr;
				 found = 1;
			}else{ 
				 /* Did not find pair, check to see if the buffer is full before adding new data */
				 if(lbb->buffer_len >= MAX_PAIR_SIZE)
						return SA_PAIR_TOO_LONG;

				 err = fread(read_buffer, 1, BLOCK_SIZE, lbb->fd);
				 /* setec astronomy files sizes should be divisible by block size after the header */
				 if(err != BLOCK_SIZE)		
						return SA_NOT_ENOUGH_DATA;

				 /* Decrypt the new data */
				 err = mdecrypt_generic(lbb->md, read_buffer, BLOCK_SIZE);
				 if(err != 0)
						return SA_CAN_NOT_DECRYPT;				 				 
			}
	 }






	 /* check to see if a pair already exists in the buffer */
	 ptr = strchr(lbb->buffer, '\n');
	 if(ptr != NULL) {
			/* a little pointer subtraction to find how long the string is */
			len = lbb->buffer - ptr;
			found = 1;
	 }

	 while(!found) {
			/* read a buffer block */
			err = mdecrypt_generic(lbb->md, read_buffer, BLOCK_SIZE);
			if(err != 0)
				 return SA_CAN_NOT_DECRYPT;
			
			/* search for the first '\n' which indicates the end of a pair */
			ptr = strchr(read_buffer, '\n');		 
			if(ptr != NULL) {				 
				 /* found the end of a pair append the data up to and including the '\n' 
						to the buffer */
				 len = read_buffer - ptr + 1;
				 strncpy((unsigned char *)lbb->buffer + (lbb->buffer_len + 1), read_buffer, len);
				 lbb->buffer_len = strlen((unsigned char *)lbb->buffer);

				 /* create pair structure from buffer */
				 
				 /* zero the buffer and append the rest of the data */
				 found = 1;
			}		 			
	 }

	 /* Copy the pair out */
	 strncpy(pair_string, lbb->buffer, len);
	 pair_string[len] = '\0';

	 /* put the data into the pair */
	 init_name_pass_pair(pair, pair_string, len);
	 
	 /* Move everthing but the pair over to a temp buffer */
	 memset(tmp, 0, MAX_PAIR_SIZE);
	 memcpy(tmp, buffer + len + 1, MAX_PAIR_SIZE - len);
	 memcpy(buffer, tmp, MAX_PAIR_SIZE);

	 return SA_SUCCESS;
}
