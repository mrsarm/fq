/* main.c

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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "freqlist.h"
#include "fq.h"


/* Initialize the global variables with the command options */
fq_data* init_options(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	fq_data* data = init_options(argc, argv);

	int r = fq_data_init_resources(data);
	switch (r) {
		case ERROR_FILE_NOT_FOUND:
			fprintf(stderr, "%s error: The input file '%s' cannot be opened.\n",
					  argv[0], data->filename_in);
			  fq_data_free_resources(data);
			  exit(ERROR_FILE_NOT_FOUND);
		case ERROR_MEM:
			error_mem(fq_data_free_resources, data);
	}

	data->freql=freqlist_create(data->buff_in[0]);
	if (!data->freql) error_mem(fq_data_free_resources, data);

	r = fq_count(data);
	switch (r) {
		case ERROR_MEM:
			error_mem(fq_data_free_resources, data);
	}

	freqlist_fprintf("> Final list lfrec\n", data->freql, stdout);

	/* Close the file and free the memory buffers. */
	fq_data_free_resources(data);

	return 0;
}


/* Initialize the global variables with command options */
fq_data* init_options(int argc, char *argv[])
{
	fq_data* data = (fq_data*) malloc(sizeof(fq_data));
	/* Read the files names from the parameters,
	  else use the defaults options. */
	int is_params = FALSE;
	data->verbose = FALSE;
	data->max = 0l;
	if (argc>1 && *argv[1]=='-') {
		is_params = TRUE;
		if (strchr(argv[1], 'h')) {
			if (argc>2) {
				error_param(argv[0]);
			}
			help_param(argv[0]);
		}
		if (strchr(argv[1], 'v')) {
			data->verbose = TRUE;
		}
	}
	if (argc==1) {
		data->filename_in = INPUT_FILENAME;
	} else if (argc==2) {
		if (is_params) {
			data->filename_in = INPUT_FILENAME;
		} else {
			data->filename_in = argv[1];
		}
	} else if (argc==3) {
		if (is_params) {
			data->filename_in = argv[2];
		} else {
			error_param(argv[0]);
		}
	}  else if (argc==4) {
		if (is_params) {
			error_param(argv[0]);
		} else {
			data->filename_in = argv[1];
			if (strchr(argv[2], 'm')) {
				data->max = (unsigned long) atol(argv[3]);
			}
		}
	}   else if (argc==5) {
		if (!is_params) {
			error_param(argv[0]);
		} else {
			data->filename_in = argv[2];
			if (strchr(argv[3], 'm')) {
				data->max = (unsigned long) atol(argv[4]);
			}
		}
	} else {
		error_param(argv[0]);
	}
	return data;
}

/*  End program.  */
