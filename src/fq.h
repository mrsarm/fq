/* fq.h

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


#ifndef __FQ_H
#define __FQ_H


#include "freqlist.h"


/*
 * Contain the main information about
 * the encoding process: input file, frequency list, etc.
 */
typedef struct _fq_data {
    char *filename_in;          /* Input file name */
    FILE *fi;                   /* Input file manager */
    unsigned long length_in;    /* File size in bytes */
    freqlist *freql;            /* Frequency list of characters */
    int verbose;                /* If TRUE the verbose mode is activated */
} fq_data;



/*
 * Return fq_data struct initialized with
 * default values.
 */
fq_data* fq_data_init(void);



/*
 * Initialization of input data structure
 * from the given file name.
 * Open data->filename_in in "rb" mode, if it's
 * NULL, use stdin as data->fi file.
 * Return 0 if no errors, otherwise an error code.
 */
int fq_data_init_resources(fq_data *data);


/*
 * Initialization of input/output data structures
 * from the given file.
 */
void fq_data_init_resources_fi(fq_data *data, FILE *fi);


/*
 * Initialize the freql struct of data.
 */
freqlist *fq_data_init_freql(fq_data *data);


/*
 * Free all resources of the application.
 */
void fq_data_free_resources(fq_data *data);


/*
 * Count the frequencies.
 */
int fq_count(fq_data *data);


#endif /* __FQ_H */
