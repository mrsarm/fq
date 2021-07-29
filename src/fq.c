/* fq.c

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
#include <string.h>
#include "fq.h"
#include "const.h"
#include "freqlist.h"


/*
 * Return fq_data struct initialized with
 * default values.
 */
fq_data *fq_data_init(void) {
    fq_data* data = (fq_data*) malloc(sizeof(fq_data));
    if (data) {
        data->verbose = FALSE;
        data->filename_in = NULL;
        data->fi = NULL;
        data->freql = NULL;
    }
    return data;
}


/*
 * Initialization of input data structure
 * from the given file name.
 * Open data->filename_in in "rb" mode, if it's
 * NULL, use stdin as data->fi file.
 * Return 0 if no errors, otherwise an error code.
 */
int fq_data_init_resources(fq_data *data) {
    if (data->filename_in && strcmp(data->filename_in, "-")) {
        /* Open the file in binary mode / read only */
        data->fi = fopen(data->filename_in, "rb");
        if (!data->fi) {
            return ERROR_FILE_NOT_FOUND;
        }
    } else {
        data->fi = stdin;
    }
    data->length_in = 0l;
    return 0;
}


/*
 * Initialization of input/output data structures
 * from the given file.
 */
void fq_data_init_resources_fi(fq_data *data, FILE *fi) {
    data->fi = fi;
    data->length_in = 0l;
}


/*
 * Initialize the freql struct of data
 * with the first symbol available in the
 * input stream. Return a pointer
 * to the struct created.
 */
int fq_data_init_freql(fq_data *data) {
    int first_symb = fgetc(data->fi);
    if (first_symb != EOF) {
        data->length_in++;
        data->freql = freqlist_create(first_symb);
    }
    data->freql->autosort = data->verbose;
    return data->freql;
}


/*
 * Free all input/output resources of the application.
 */
void fq_data_free_resources(fq_data *data) {
    if (data->fi) {
        fflush(data->fi);
        if (data->fi != stdin) fclose(data->fi);
    }
    if (data->freql) freqlist_free(data->freql);
    free(data);
}


/*
 * Count the frequencies.
 */
int fq_count(fq_data *data) {
    if (!data->freql) {
        if (!fq_data_init_freql(data)) {
            error_mem(fq_data_free_resources, data);
        }
    }
    node_freqlist *pnode = NULL;
    do {
        unsigned char symbol = fgetc(data->fi);   // A buffer is not used due
        if( feof(data->fi) ) {                    // that any modern OS
            break ;                               // use one when we use
        }                                         // the libc library for I/O
        pnode = freqlist_add(data->freql, symbol);
        if (!pnode) {
            return ERROR_MEM;
        }
        if(data->verbose) {
            freqlist_fprintf(stdout, NULL, data->freql, pnode);
            printf("\n\n");
        }
        data->length_in++;
    } while (TRUE);
    if (!data->freql->autosort) {
        freqlist_sort(data->freql);
    }
    return 0;
}

/*
 * Print an error message with error_code and from string in the stderr, and abort
 * the program after invoking the free_resources function.
 */
void error_unknown_code(int error_code, char *from,
                        void(free_resources)(fq_data*), fq_data* data) {
    if (free_resources) {
        free_resources(data);
    }
    fprintf(stderr, "Error: unknown error code [%d] from `%s'.\n", error_code, from);
    exit(ERROR_UNKNOWN);
}

/*
 * Print error_code in the stderr, and abort
 * the program after invoking the free_resources function.
 */
void fatal(int error_code,
           char *error_msg,
           void(free_resources)(fq_data*), fq_data* data) {
    if (free_resources) {
        free_resources(data);
    }
    fprintf(stderr, error_msg);
    exit(error_code);
}


/*
 * Print an insufficient memory error in the stderr, and abort
 * the program after invoking the free_resources function.
 */
void error_mem(void(free_resources)(fq_data*), fq_data* data) {
    fatal(ERROR_MEM, "Error: Insufficient memory.\n",
          free_resources, data);
}
