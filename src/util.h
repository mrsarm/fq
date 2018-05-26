/* util.h

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


#ifndef __FQ_UTIL_H
#define __FQ_UTIL_H


#include <stdio.h>

#ifndef TRUE

#define TRUE							1
#define FALSE							0

#endif /* TRUE */

#define INPUT_FILENAME					"in"			/* Default input file name. */

#define ERROR_MEM						-2				/* Insufficient memory error. */
#define ERROR_PARAM						-3				/* Command line parameterization error. */
#define ERROR_FILE_NOT_FOUND			-5				/* The input file is not found or can not
														   be opened. */


/*
 * Returns the size of the file in bytes.
 */
long filesize(FILE *f);

/*
 * Print an error in the stderr, and aborts the program.
 * @msg: the message to be printed.
 * @error_code: the program will exit with this number error.
 */
void error(const char *msg, int error_code);


#endif /* __FQ_UTIL_H */
