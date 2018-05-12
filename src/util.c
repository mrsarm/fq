/* util.c

   Copyright (C) 2015-2018 Mariano Ruiz <mrsarm@gmail.com>
   This file is part of the "Frequency Counter" project.

   This project is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the "Frequency Counter" project; if not, see
   <http://www.gnu.org/licenses/>.  */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"


/*
 * Returns the size of the file in bytes.
 */
long filesize(FILE *f)
{
	if (f==NULL) return 0;

	long int pos_now, len;
	pos_now=ftell(f);
	fseek(f, 0L, SEEK_END);
	len=ftell(f);
	fseek(f, pos_now, SEEK_SET);

	return len;
}


/*
 * Print parametrization help message in the stdout, and aborts the program.
 * @command_name: the name of the command that was invoked this tool.
 */
void help_param(const char *command_name)
{
	printf("Usage: %s [-hv] [input file] [-m MAX]\n", command_name);
	exit(0);
}


/*
 * Print a parametrization error in the stderr, and aborts the program.
 * @command_name: the name of the command that was invoked this tool.
 */
void error_param(const char *command_name)
{
	fprintf(stderr, "Error: Invalid options.\n"
			"Usage: %s [-hv] [input file] [-m MAX]\n", command_name);
	exit(ERROR_PARAM);
}

/*
 * Print an error in the stderr, and aborts the program.
 * @msg: the message to be printed.
 * @error_code: the program will exit with this number error.
 */
void error(const char *msg, int error_code)
{
	fprintf(stderr, msg);
	exit(error_code);
}
