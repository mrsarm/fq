/* fq.h

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


#ifndef __FQ_H
#define __FQ_H


#include "freqlist.h"


/*
 * Contains the main information about
 * the encoding process: input file, buffers, etc.
 */
typedef struct _fq_data {
	char *filename_in;			/* Input file name. */
	unsigned char *buff_in;		/* Input buffer. */
	int buffer_size;			/* Input buffer size. */
	FILE *fi;					/* Input file manager. */
	unsigned long length_in;	/* File size in bytes, */
	unsigned long max;			/* Max characters to be read */
	freqlist *freql;			/* Frequency list of characters. */
	node_freqlist *pnode;		/* Temporary pointer to list node. */
	int verbose;				/* If TRUE the verbose mode is active. */
} fq_data;



/*
 * Return fq_data struct initialized with
 * default values.
 */
fq_data* fq_data_init(void);



/*
 * Initialization of input data structures.
 * Opens data->filename_in in "rb" mode, if it's
 * NULL, use stdin as data->fi file.
 * Returns `0` if no errors, or an error code.
 */
int fq_data_init_resources(fq_data *data, char *filename_in);


/*
 * Initialization of input/output data structures
 * from the given file.
 * Returns `0` if no errors, or an error code.
 */
int fq_data_init_resources_fi(fq_data *data, FILE *fi);


/*
 * Initializes the freql struct of data
 * with the first symbol available in the
 * input stream. Return a pointer
 * to the struct created.
 */
int fq_data_init_freql(fq_data *data);


/*
 * Free all resources of the application.
 */
void fq_data_free_resources(fq_data *data);


/**
 * Counts the frequencies.
 */
int fq_count(fq_data *data);


/*
 * Prints an insufficient memory error in the stderr, and aborts
 * the program after invoking the fq_data_free_resources function.
 */
void error_mem(void(free_resources)(fq_data*), fq_data* data);


#endif /* __FQ_H */
