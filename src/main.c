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
#include <getopt.h>
#include <string.h>

static const char DEFAULT_PASSWORD_FILE[]="~/.setec_astronomy";

// static struct char short_opts[] = "adg:hn:p:gf:";
static char short_opts[] = "adg:hn:p:";
static struct option long_options[] = 
{
	 {"add", no_argument, 0, 'a'},
	 {"delete", no_argument, 0, 'd'},
	 {"get", required_argument, 0, 'g'},
	 {"help", no_argument, 0, 'h'},
	 {"name", required_argument, 0, 'n'},
	 {"password", required_argument, 0, 'p'},
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
	 AT_ERR
};

struct Action
{
	 enum ACTION_TYPE action_type;
	 char name[MAX_NAME_LEN];
	 char pass[MAX_PASS_LEN];
};

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
			case AT_HELP:
				 print_help();
				 return 1;
			case AT_ERR:
				 printf("Invalid operation or argument passed.\n");
				 print_help();
				 return -1;
			default:
				 return -1;
	 }

	 return 0;
}

int main(int argc, char ** argv)
{
	 struct Action action;
	 int err;

	 action = parse_args(argc, argv);
	 err = analyze_args(action);

	 if(err != 0)
			return (err > 0 ? 0 : err);

	 switch(action.action_type) {
			case AT_ADD:
				 printf("Adding name=%s and password=%s to password file.", action.name, action.pass);
				 break;
			case AT_DEL:
				 printf("Deleting name=%s from password file.", action.name);
				 break;
			case AT_GET:
				 printf("Getting password for name=%s from password file.", action.name);
				 break;
			default:
				 return -1;
	 }

	 return 0;
}
