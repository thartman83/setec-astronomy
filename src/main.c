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
#include "setec_astronomy.h"
#include "little_black_box.h"
#include "errors.h"
#include <getopt.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char PASS_FILE_NAME[]=".setec_astronomy";

// static struct char short_opts[] = "adg:hn:p:gf:";
static char short_opts[] = "adghn:p:l";
static struct option long_options[] = 
{
	 {"add", no_argument, 0, 'a'},
	 {"delete", no_argument, 0, 'd'},
	 {"get", required_argument, 0, 'g'},
	 {"help", no_argument, 0, 'h'},
	 {"name", required_argument, 0, 'n'},
	 {"password", required_argument, 0, 'p'},
	 {"list", no_argument, 0, 'l'},
//	 {"generate", no_argument, 0, 'g'},
//	 {"password-file", required_argument, 0, 'f'},
	 {0,0,0,0}
};

enum ACTION_TYPE {
	 AT_NONE,
	 AT_ADD,
	 AT_DEL,
	 AT_GET,
	 AT_HELP,
	 AT_LIST,
	 AT_ERR
};

struct Action
{
	 enum ACTION_TYPE action_type;
	 char name[MAX_NAME_LEN];
	 char pass[MAX_PASS_LEN];
};

char * get_default_pass_file()
{
	 char * retval;
	 char * home_dir = getenv("HOME");
	 int len;

	 if(home_dir == NULL)
			return NULL;

	 len = strlen(home_dir) + strlen(PASS_FILE_NAME) + 2;
	 retval = malloc(sizeof(char) * len);
	 snprintf(retval, len, "%s/%s", home_dir, PASS_FILE_NAME);

	 return retval;
}

void print_help() 
{
	 printf("setec_astronomy:\n");
	 printf("\tActions:\n");
	 printf("\t\t-a: add an entry to the password file.\n");
	 printf("\t\t\tRequires a name (-n) and password (-n).\n");
	 printf("\t\t-d: delete an entry from the password file.\n");
	 printf("\t\t\tRequires a name (-n).\n");
	 printf("\t\t-g: get the password for an entry in the password file.\n");
	 printf("\t\t\tRequires a name (-n).\n");
	 printf("\t\t-l: list names in password file.\n");
	 printf("\tAction arguments:\n");
	 printf("\t\t-n: specifies the name of the entry within the password "
					"file.\n");
	 printf("\t\t-p: specifies the password for the entry within the password "
					"file.\n");
}

struct Action parse_args(int argc, char ** argv)
{
	 int opt, option_index;
	 struct Action act;

	 act.action_type = AT_NONE;
	 act.name[0] = '\0';
	 act.pass[0] = '\0';

	 while ((opt = getopt_long(argc, argv, short_opts, long_options, 
														 &option_index)) != -1) {
			switch(opt) {
				 case 'h':
						act.action_type = AT_HELP;
						return act;
				 case 'a':
						act.action_type = (act.action_type == AT_NONE ? AT_ADD : AT_ERR);
						break;
				 case 'd':
						act.action_type = (act.action_type == AT_NONE ? AT_DEL : AT_ERR);
						break;
				 case 'g':
						act.action_type = (act.action_type == AT_NONE ? AT_GET : AT_ERR);
						break;
				 case 'n':
						strncpy(act.name, optarg, MAX_NAME_LEN);
						break;
				 case 'p':
						strncpy(act.pass, optarg, MAX_PASS_LEN);
						break;
				 case 'l':
						act.action_type = (act.action_type == AT_NONE ? AT_LIST : AT_ERR);
						break;
				 default:
						act.action_type = AT_ERR;
						return act;
			}
	 }

	 return act;
}

int analyze_args(struct Action action)
{
	 switch(action.action_type) {
			case AT_ADD:
				 if(strcmp(action.name, "") == 0 || strcmp(action.pass, "") == 0) {
						printf("This operation requires both a name (-n) and password (-p).\n");
						return -1;
				 }
				 break;
			case AT_DEL:
			case AT_GET:
				 if(strcmp(action.name, "") == 0) {
						printf("This operation requires a name (-n) be given.\n");
						return -1;
				 }
				 break;
			case AT_LIST:
				 return 0;
			case AT_HELP:
				 return 1;
			case AT_ERR:
				 printf("Invalid operation or argument passed.\n");
				 return -1;
			default:
				 return -1;
	 }

	 return 0;
}

int get_password(char ** pass)
{
	 struct termios old, new;
	 size_t read_len;

	 if(tcgetattr(fileno(stdin), &old) != 0)
			return SA_NO_TERM_GET;

	 new = old;
	 new.c_lflag &= ~ECHO;

	 if(tcsetattr(fileno(stdin), TCSAFLUSH, &new) != 0)
			return SA_NO_TERM_SET;

	 printf("password:");

	 read_len = getline(pass, &read_len, stdin);
	 
	 (void) tcsetattr(fileno(stdin), TCSAFLUSH, &old);

	 printf("\n");

	 if(read_len > MAX_PASS_LEN) {
			printf("Password is too long.  Maximum password length is %d.", 
						 MAX_PASS_LEN);
			memset(*pass, '\0', MAX_PASS_LEN);
			free(*pass);
			return SA_PASS_TOO_LONG;
	 }

	 return SA_SUCCESS;
}

int main(int argc, char ** argv)
{
	 struct Action act;
	 int err;
	 char * master_password = NULL;
	 char pass[MAX_PASS_LEN];
	 char * pass_file;

	 act = parse_args(argc, argv);
	 err = analyze_args(act);

	 if(err != 0) {
			print_help();
			if(err < 0) {
				 print_err_msg(err);
				 return err;
			}else {
				 return 0;
			}
	 }

	 err = get_password(&master_password);
	 pass_file = get_default_pass_file();

	 switch(act.action_type) {
			case AT_ADD:
				 err = add_name_pass(pass_file, master_password, act.name,
														 act.pass);
				 break;
			case AT_DEL:
				 err = del_name_pass(pass_file, master_password, act.name);
				 break;
			case AT_GET:
				 err = get_pass_by_name(pass_file, master_password, act.name, 
																pass);
				 if( err == SA_SUCCESS )
						printf(" Password for %s : %s\n", act.name, pass);

				 memset(pass, '\0', MAX_PASS_LEN);
				 break;
			case AT_LIST:
			{
				 int num_names, x;
				 char ** name_list;
				 
				 num_names = 0;
				 name_list = 0;
				 err = get_name_list(pass_file, master_password, &name_list, &num_names);

				 printf("The password file contains the following entries:\n");

				 for(x = 0; x < num_names; ++x) {
						printf("\t%s\n", name_list[x]);
						free(name_list[x]);
				 }
				 free(name_list);
				 break;
			}						
			default:
				 err = -1;
	 }

	 memset(master_password, '\0', MAX_PASS_LEN);
	 free(master_password);
	 free(pass_file);

	 if(err < 0)
			print_err_msg(err);

	 return err;
}
