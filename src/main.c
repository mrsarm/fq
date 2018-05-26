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
#include <getopt.h>
#include <ctype.h>
#include "util.h"
#include "freqlist.h"
#include "fq.h"


#define USAGE       "Usage: %s [-hv] [FILE] [-c MAX]\n" \
					"Print the frequency table to standard output.\n" \
					"\n" \
					"Options:\n" \
	 				"  -v		verbose mode, print frequency table\n" \
	  				"    		for each character in the stream\n" \
					"  -c NUM	Max number of bytes to count\n" \
					"  -h		display this help and exit\n" \
					"\n" \
					"\"Frequency Counter\" project v0.1b: fq <https://github.com/mrsarm/fq>\n"


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
	data->verbose = FALSE;
	data->max = 0l;
    opterr = 0;
	int c;
	while ((c = getopt(argc, argv, "hvc:")) != -1) {
        switch (c) {
            case 'v':
                data->verbose = TRUE;
                break;
            case 'c':
                data->max = (unsigned long) atol(optarg);
                break;
            case 'h':
				printf(USAGE, argv[0]);
				exit(0);
            case '?':
                if (optopt == 'c') {
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                } else if (isprint (optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
					fprintf(stderr, "Try '%s -h' for more information.\n", argv[0]);
                } else {
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                }
				exit(ERROR_PARAM);
        }
	}
    data->filename_in = NULL;
    for (int index = optind; index < argc; index++) {
        data->filename_in = argv[index];
        break;
    }
    if (data->filename_in == NULL) {
        data->filename_in = INPUT_FILENAME;
    }
	return data;
}

/*  End program.  */
