/* fq.c

   Copyright (C) 2015-2018 Mariano Ruiz <mrsarm@gmail.com>
   This file is part of the "Frequency Counter" project.

   This project is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */


#include <stdio.h>
#include <stdlib.h>
#include "fq.h"
#include "util.h"
#include "freqlist.h"


/*
 * Initialization of input/output data structures.
 * Returns `0` if no errors, or an error code.
 */
int fq_data_init_resources(fq_data *data)
{
	/* Open the file in binary mode / read only, and load in the buffer. */
	data->fi=fopen(data->filename_in, "rb");
	if (!data->fi) {
		return ERROR_FILE_NOT_FOUND;
	}
	data->length_in =filesize(data->fi);
	data->buff_in=(unsigned char *)malloc(data->length_in);
	if (!data->buff_in)
		return ERROR_MEM;
	fread(data->buff_in, data->length_in, 1, data->fi);
	return 0;
}


/*
 * Free all input/output resources of the application.
 */
void fq_data_free_resources(fq_data *data)
{
	if (data->fi) fclose(data->fi);
	if (data->buff_in) free(data->buff_in);
	if (data->freql) freqlist_free(data->freql);
	free(data);
}


/**
 * Counts the frequencies.
 */
int fq_count(fq_data *data) {
	int max;
	if (data->max==0) {
		max = data->length_in;
	} else {
		max = data->max;
	}
	for (int i=1; i<max; i++) {
		data->pnode=freqlist_add(data->freql, data->buff_in[i]);
		if (!data->pnode) {
			return ERROR_MEM;
		}
		if (data->verbose)
		if(data->verbose) {
			freqlist_fprintf(NULL, data->freql, stdout);
			printf("Symb.: '%c' %2X\n\n",
				   (data->buff_in[i]<0x7F && data->buff_in[i]>=0x20)?data->buff_in[i]:'.',
				   data->buff_in[i]);
		}
	}
	return 0;
}

/*
 * Print an insufficient memory error in the stderr, and aborts
 * the program after invoking the fq_data_free_resources function.
 */
void error_mem(void(free_resources)(fq_data*), fq_data* data)
{
	if (free_resources) {
		free_resources(data);
	}
	error("Error: Insufficient memory error.\n", ERROR_MEM);
}
