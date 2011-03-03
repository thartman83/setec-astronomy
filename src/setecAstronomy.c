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
#include "errors.h"
#include "key.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * name_pass_pair_to_string(const struct name_pass_pair * pair)
{
	 int size = strlen(pair->name) + strlen(pair->pass) + 3;
	 char * retval = malloc(size);
	 snprintf(retval, size, "%s=%s", pair->name, pair->pass);

	 return retval;
}

void init_name_pass_pair(struct name_pass_pair * pair)
{
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
}

int get_name_pass_pair(const char * name, const char * password,
											 const char * filename, struct name_pass_pair * pair)
{
	 FILE * fd;
	 struct setec_astronomy_header header;
	 MCRYPT md;
	 int err;
	 

	 fd = fopen(filename, "rb");
	 if(fd == NULL)
			return SA_FILE_NOT_FOUND;

	 read_header_ext(&header, fd);

	 err = open_mcrypt(&header, password, &md);
	 if(err != SA_SUCCESS)
			return err;

	 while(

	 return SA_SUCCESS;
}

int add_name_pass_pair(const struct name_pass_pair * pair, 
											 const char * filename, const char * password)
{
	 FILE * r_fd, * w_fd;  /* file descriptors */
	 struct setec_astronomy_header r_header, w_header;  /* read and write file headers */
	 MCRYPT r_md = NULL, w_md = NULL;  /* read and write mcrypt descriptors */
	 int block_size, err, read_len, block_pos;
	 unsigned char * block_buffer; /* buffers for encrypting and decrypting */
	 char * temp_filename, * new_data;
	 int x;

	 /* Setup a new crypt descriptor */
	 err = open_new_mcrypt(&w_header, password, &w_md);
	 if(err != SA_SUCCESS)
			return err;

	 /* Setup write file descriptor */
	 temp_filename = malloc(strlen(filename) + 4);
	 snprintf(temp_filename, strlen(filename) + 4, "%s.%s", filename, "tmp");
	 w_fd = fopen(temp_filename, "wb");
	 write_header_ext(&w_header, w_fd);

	 /* Check to see if the file exists */
	 r_fd = fopen(filename, "rb");

	 /* initialize block buffer and size */
	 block_size = mcrypt_enc_get_block_size(w_md);
	 block_buffer = calloc(1, block_size);
	 read_len = 0;
	 
	 if(r_fd != NULL) {  /* file exists */

			/* Read the header of the original and setup a decrypting descriptor */
			read_header_ext(&r_header, r_fd);
			err = open_mcrypt(&r_header, password, &r_md);
			
			if(err != SA_SUCCESS)
				 return err;
			
			/* now read, decrypt, encrypt, write and zero loop */
			while((read_len = fread(block_buffer, 1, block_size, r_fd))) {
				 if(read_len < block_size) {
						break;
				 }
				 mdecrypt_generic(r_md, block_buffer, block_size);
				 encrypt_write_zero_buffer(w_md, w_fd, block_buffer, block_size);
			}
		
			/* close the in file descriptor and the decryptor */
			fclose(r_fd);
			mcrypt_generic_deinit(r_md);
			mcrypt_module_close(r_md);			
	 }

	 /* combine the new data into a single string */
	 new_data = name_pass_pair_to_string(pair);
	 block_pos = read_len;
	 for(x = 0; x < strlen(new_data); ++x, ++block_pos) {
			if(block_pos == block_size) {
				 /* we've filled a buffer, encrypt it, write it, and zero it out */
				 encrypt_write_zero_buffer(w_md, w_fd, block_buffer, block_size);

				 /* reset block_pos and continue */
				 block_pos = 0;
			}
			block_buffer[block_pos] = new_data[x];
	 }

	 /* pad the final block, encrypt, write, zero and free */
	 encrypt_write_zero_buffer(w_md, w_fd, block_buffer, block_size);

	 /* free up and close */
	 free(block_buffer);
	 free(temp_filename);
	 mcrypt_generic_deinit(w_md);
	 mcrypt_module_close(w_md);
	 fclose(w_fd);

	 return SA_SUCCESS;
}

int open_new_mcrypt(struct setec_astronomy_header * header, 
										const char * password, MCRYPT * md)
{
	 int iv_size;

	 /* First setup the encryption descriptor */
	 *md = mcrypt_module_open((char*)CRYPT_ALGO, NULL, (char*)CRYPT_MODE, NULL);
	 if(*md == NULL)
			return SA_CAN_NOT_OPEN_CRYPT_MODULE;

	 /* Next setup the new header and the new key */
	 init_header(header);
	 
	 iv_size = mcrypt_enc_get_iv_size(*md);
	 if(iv_size <= 0)
			return SA_INVALID_IV_SIZE;

	 init_iv(header, iv_size);
	 init_salt(header, 0);
	 
	 return open_mcrypt(header, password, md);
}

int open_mcrypt(const struct setec_astronomy_header * header, 
								const char * password, MCRYPT *md)
{
	 char * key = calloc(1, KEY_LEN);
	 hash_key(password, strlen(password), 1, key);
	 
	 /* Initialize mcrypt with the key and iv */
	 if(mcrypt_generic_init(*md, key, KEY_LEN, header->iv) < 0)
			return SA_CAN_NOT_INIT_CRYPT;
	 else
			return SA_SUCCESS;
}

int encrypt_write_zero_buffer(MCRYPT md, FILE * fd, void * buffer, int buf_len)
{
	 mcrypt_generic(md, buffer, buf_len);
	 fwrite(buffer, 1, buf_len, fd);
	 memset(buffer, 0, buf_len);

	 return SA_SUCCESS;
}

int read_next_pair(MCRYPT md, FILE * fd, char * buffer, int * buf_len,
									 struct name_pass_pair * pair)
{
	 
	 
	 return SA_SUCCESS;
}
