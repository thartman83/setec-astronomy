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
#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>

/* Assumes that lbb is a allocated but uninitialized 
	 little_black_box struct */
int lbb_open(struct little_black_box * lbb, int crypt_mode, 
						 const char * filename, const char * password)
{	 
	 int err, buffer_size;

	 lbb->md = NULL;
	 lbb->fd = NULL;
	 lbb->buffer = NULL;
	 lbb->key = NULL;	 
	 lbb->data_len = 0;
	 init_header(&lbb->header);

	 if(crypt_mode != SA_CRYPT_MODE && crypt_mode != SA_DECRYPT_MODE)
			return SA_INVALID_CRYPT_MODE;
	 
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

	 lbb->crypt_mode = crypt_mode;
	 if(crypt_mode == SA_CRYPT_MODE)
			err = lbb_open_write(lbb, filename, password);
	 else
			err = lbb_open_read(lbb, filename, password);

	 if(err != SA_SUCCESS)
			return err;

	 lbb->key_len = KEY_LEN;
	 lbb->key = malloc(lbb->key_len);

	 err = PKCS5_PBKDF2_HMAC_SHA1(password, strlen(password), lbb->header.salt, 
																lbb->header.salt_len, lbb->header.hash_count,
																lbb->key_len, lbb->key);
	 if(err != 1)
			return SA_UNABLE_TO_PBKDF2;
	 
	 if(mcrypt_generic_init(lbb->md, lbb->key, lbb->key_len, lbb->header.iv) < 0)
			return SA_CAN_NOT_INIT_CRYPT;
	 
	 return SA_SUCCESS;
}

int lbb_open_read(struct little_black_box * lbb, const char * filename, 
									const char * password)
{
	 int err;
	 unsigned char * hash;
	 
	 lbb->fd = fopen(filename, "rb");
	 if(lbb->fd == NULL)
			return SA_FILE_NOT_FOUND;
	 
	 err = read_header_ext(&(lbb->header), lbb->fd);
	 if(err != SA_SUCCESS)
			return SA_INVALID_HEADER;

	 hash = malloc(lbb->header.hash_len);
	 err = PKCS5_PBKDF2_HMAC_SHA1(password, strlen(password), lbb->header.salt, 
																lbb->header.salt_len, lbb->header.hash_count*2,
																lbb->header.hash_len, hash);
	 
	 if(err != 1) {
			free(hash);
			return err;
	 }

	 if(memcmp(hash, lbb->header.hash, lbb->header.hash_len) != 0) {
			free(hash);
			return SA_WRONG_PASSWORD;
	 }

	 free(hash);

	 return SA_SUCCESS;
}

int lbb_open_write(struct little_black_box * lbb, const char * filename,
									 const char * password)
{
	 int iv_len;
	 FILE * tmp_fd;

	 tmp_fd = fopen(filename, "rb");
	 if(tmp_fd != NULL) {
			fclose(tmp_fd);
			lbb_close(lbb);
			return SA_FILE_EXISTS;
	 }
			
	 iv_len = mcrypt_enc_get_iv_size(lbb->md);
	 if(iv_len < 0) {
			lbb_close(lbb);
			return SA_INVALID_IV_SIZE;
	 }
	 
	 create_header(&lbb->header, iv_len, DEFAULT_SALT_LEN,
								 DEFAULT_HASH_COUNT, password, DEFAULT_HASH_LEN);

	 lbb->fd = fopen(filename, "wb");
	 if(lbb->fd == NULL)
			return SA_CAN_NOT_OPEN_FILE;
	 
	 write_header_ext(&(lbb->header), lbb->fd);

	 return SA_SUCCESS;
}

/* Helper function to manage reading and writing */
int lbb_open_rw(struct little_black_box * r_lbb, struct little_black_box *w_lbb,
								const char * filename, const char * password)
{
	 int err;	 
	 char * temp_password_file = NULL;

	 err = lbb_open(r_lbb, SA_DECRYPT_MODE, filename, password);
	 if(err != SA_SUCCESS)
			return err;

	 temp_password_file = gen_temp_filename(filename);
	 
	 err = lbb_open(w_lbb, SA_CRYPT_MODE, temp_password_file, password);
	 if(err != SA_SUCCESS)
			lbb_close(r_lbb);	 

	 free(temp_password_file);	 
	 return err;
}

int lbb_close(struct little_black_box * lbb)
{
	 int err;

	 /* write anything that may be left in the buffer */
	 if(lbb->data_len != 0 && lbb->crypt_mode == SA_CRYPT_MODE) {
			err = lbb_flush(lbb);
			if(err != SA_SUCCESS)
				 return err;
	 }

	 /* free up the header */
	 free_header(&(lbb->header));

	 if(lbb->md != NULL) {
			/* deinit and close the mcrypt descriptor (if necessary) */
			mcrypt_generic_deinit(lbb->md);

			/* if this the last little black box to be closed kill the module as well */
			mcrypt_module_close(lbb->md);
			lbb->md = NULL;
	 }

	 /* zero out the buffer before freeing it */
	 if(lbb->buffer != NULL) {
			memset(lbb->buffer, 0, lbb->block_size);
			free(lbb->buffer);
			lbb->buffer = NULL;
	 }

	 if(lbb->fd != NULL) {
			fclose(lbb->fd);
			lbb->fd = NULL;
	 }

	 if(lbb->key != NULL) {
			memset(lbb->key, 0, lbb->key_len);
			free(lbb->key);
			lbb->key = NULL;
			lbb->key_len = 0;
	 }			

	 return SA_SUCCESS;
}

int lbb_close_rw(struct little_black_box * r_lbb, struct little_black_box * w_lbb,
								 const char * filename, int status)
{
	 char * temp_password_file = gen_temp_filename(filename);
	 if(status == 0)
			rename(temp_password_file, filename);
	 else
			remove(temp_password_file);
	 
	 free(temp_password_file);
	 lbb_close(r_lbb);
	 lbb_close(w_lbb);

	 return status;
}

int lbb_write(struct little_black_box * lbb, void * buffer, int buffer_len)
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
				 err = lbb_flush(lbb);
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

int lbb_flush(struct little_black_box * lbb)
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

int lbb_write_pair(struct little_black_box * lbb, const struct name_pass_pair pair)
{
	 return lbb_write_name_pass(lbb, pair.name, pair.pass);
}

int lbb_write_name_pass(struct little_black_box * lbb, 
												const char name[MAX_NAME_LEN], 
												const char pass[MAX_PASS_LEN])
{
	 char buffer[MAX_PAIR_SIZE];
	 snprintf(buffer, MAX_PAIR_SIZE, "%s=%s\n", name, pass);
	 return lbb_write(lbb, buffer, strlen(buffer));
}

int lbb_read_pair(struct little_black_box * lbb, struct name_pass_pair * pair)
{
	 char pair_string[MAX_PAIR_SIZE];
	 char * ptr;
	 int len, err, found = 0;

	 if(lbb->crypt_mode != SA_DECRYPT_MODE)
			return SA_WRONG_MODE;

	 while(!found) {
			ptr = strchr((const char *)lbb->buffer, '\n');
			if(ptr != NULL) {
				 len = ptr - (char*)lbb->buffer;
				 found = 1;
			}else{ 
				 err = lbb_read_block(lbb);
				 if(err != SA_SUCCESS)
						return err;
			}
	 }

	 strncpy(pair_string, lbb->buffer, len);
	 pair_string[len] = '\0';
	 init_name_pass_pair(pair_string, pair);

	 pop_data(lbb->buffer, MAX_PAIR_SIZE + lbb->block_size, len);
	 lbb->data_len = strlen(lbb->buffer);

	 return SA_SUCCESS;
}

int lbb_read_block(struct little_black_box * lbb)
{
	 char * read_buffer;
	 int retval = SA_SUCCESS;
	 int err;
	 
	 if(lbb->data_len >= MAX_PAIR_SIZE)
			return SA_PAIR_TOO_LONG;
	 
	 read_buffer = calloc(sizeof(char), lbb->block_size);

	 err = fread(read_buffer, 1, lbb->block_size, lbb->fd);
	 if(err != lbb->block_size) {
			retval = (err == 0 && lbb->data_len == 0) ? SA_NO_MORE_PAIRS : SA_NOT_ENOUGH_DATA;	 
	 } else {
			err = mdecrypt_generic(lbb->md, read_buffer, lbb->block_size);
			if(err != 0)
				 retval = SA_CAN_NOT_DECRYPT;
						
			strncpy((char *)lbb->buffer + lbb->data_len, read_buffer, lbb->block_size);			
	 }
	 
	 free(read_buffer);

	 return retval;
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

int lbb_copy(struct little_black_box * r_lbb,
						 struct little_black_box * w_lbb)
{
	 int err;
	 struct name_pass_pair pair;

	 while((err = lbb_read_pair(r_lbb, &pair)) == SA_SUCCESS)
			lbb_write_pair(w_lbb, pair);

	 return (err == SA_NO_MORE_PAIRS ? SA_SUCCESS : err);
}
