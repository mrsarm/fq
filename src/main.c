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
#include <signal.h>
#include "const.h"
#include "freqlist.h"
#include "fq.h"


#define USAGE       "Usage: %s [-hv] [FILE] [-c NUM]\n" \
					"Print the frequency table to standard output.\n" \
					"\n" \
					"Options:\n" \
	 				"  -v		verbose mode, print frequency table\n" \
	  				"    		for each byte in the stream\n" \
					"  -c NUM	Max number of bytes to count\n" \
					"  -h		display this help and exit\n" \
					"\n" \
					"\"Frequency Counter\" project v1.0.1: fq <https://github.com/mrsarm/fq>\n"


/* Initialize the global variables with the command arguments */
fq_data* init_options(int argc, char *argv[]);

/* Ctrl+C handler */
void ctrlc_handler(int sig);

fq_data* data;

int main(int argc, char *argv[])
{
	data = init_options(argc, argv);			// Initialize data with command arguments
	signal(SIGINT, ctrlc_handler);				// Initialize Ctrl+C signal

	int r = fq_data_init_resources(data, NULL);	// Initialize resources (files)
	switch (r) {
		case ERROR_FILE_NOT_FOUND:
			fprintf(stderr, "%s error: The input file '%s' cannot be opened.\n",
					  argv[0], data->filename_in);
			  fq_data_free_resources(data);
			  exit(ERROR_FILE_NOT_FOUND);
		case ERROR_MEM:
			error_mem(fq_data_free_resources, data);
	}
	r = fq_count(data);							// Count the symbols
	switch (r) {
		case ERROR_MEM:
			error_mem(fq_data_free_resources, data);
	}

	freqlist_fprintf("> Final frequency table\n",	// Print the frequencies
					 data->freql, stdout);

	fq_data_free_resources(data);				// Close file and free memory

	return 0;
}


/* Initialize the global variables with command options */
fq_data* init_options(int argc, char *argv[])
{
	fq_data* data = fq_data_init();
	if (!data) error_mem(NULL, NULL);
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
    for (int index = optind; index < argc; index++) {
        data->filename_in = argv[index];
        break;
    }
	return data;
}


/* Ctrl+C handler */
void ctrlc_handler(int sig) {
	printf("\n");

	if (data && data->freql) {
		freqlist_fprintf("> Final frequency table\n",    // Print the frequencies
						 data->freql, stdout);
	}
	exit(0);
}

/*  End program.  */
