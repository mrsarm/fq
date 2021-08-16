/* util.c

   Copyright (C) 2015-2021 Mariano Ruiz <mrsarm@gmail.com>
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


#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "util.h"


/*
 * Print an error message with error_code and from string in the stderr, and abort
 * the program after invoking the free_resources function.
 */
void error_unknown_code(int error_code, char *from,
                        void(free_resources)(void*), void* data) {
    if (free_resources) free_resources(data);
    fprintf(stderr, "Error: unknown error code [%d] from `%s'.\n", error_code, from);
    exit(ERROR_UNKNOWN);
}

/*
 * Print an error message in the stderr that the file cannot be opened, and abort
 * the program after invoking the free_resources function.
 */
void error_cannot_open(int error_code, char *file_type, char *filename,
                       void(free_resources)(void*), void* data) {
    if (free_resources) free_resources(data);
    fprintf(stderr, "Error: The %s file `%s' cannot be opened.\n", file_type, filename);
    exit(error_code);
}

/*
 * Print error_code in the stderr, and abort
 * the program after invoking the free_resources function.
 */
void fatal(int error_code,
           char *error_msg,
           void(free_resources)(void*), void* data) {
    if (free_resources) free_resources(data);
    fprintf(stderr, "%s", error_msg);
    exit(error_code);
}


/*
 * Print an insufficient memory error in the stderr, and abort
 * the program after invoking the free_resources function.
 */
void error_mem(void(free_resources)(void*), void* data) {
    fatal(ERROR_MEM, "Error: Insufficient memory.\n",
          free_resources, data);
}
