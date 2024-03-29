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
#include "const.h"
#include "util.h"
#include "fq.h"
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
        data->length_in = 0l;
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
 * Initialize the freql struct of data.
 */
freqlist *fq_data_init_freql(fq_data *data) {
    data->freql = freqlist_create();
    if (data->freql) {
        data->freql->autosort = data->verbose;
    }
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
            error_mem((void*)fq_data_free_resources, data);
        }
    }
    do {
        unsigned char symbol = fgetc(data->fi);   // A buffer is not used due
        if( feof(data->fi) ) {                    // that any modern OS
            break ;                               // has one provided by
        }                                         // the libc library when I/O
        node_freqlist *pnode = freqlist_add(data->freql, symbol);
        if (!pnode) {
            return ERROR_MEM;
        }
        data->length_in++;
        if(data->verbose) {
            freqlist_fprintf(stdout, NULL, data->freql, pnode);
            printf("\n\n");
        }
    } while (TRUE);
    if (!data->freql->autosort) {
        freqlist_sort(data->freql);
    }
    return 0;
}
