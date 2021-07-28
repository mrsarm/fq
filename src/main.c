/* main.c

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
#include <getopt.h>
#include <ctype.h>
#include <signal.h>
#include "const.h"
#include "freqlist.h"
#include "fq.h"


#define USAGE   "Usage: %s [-hv] [FILE]\n" \
                "Print the frequency table of FILE to standard output.\n" \
                "\n" \
                "Options:\n" \
                "  -v       verbose mode, print the frequency table\n" \
                "           for each byte in the stream\n" \
                "  -h       display this help and exit\n" \
                "\n" \
                "With no FILE, or when FILE is -, read standard input.\n" \
                "\"Frequency Counter\" project v2.0.0-rc1: fq <https://github.com/mrsarm/fq>\n"


/* Initialize the global variables with the command arguments */
fq_data* init_options(int argc, char *argv[]);

/* Ctrl+C handler */
void ctrlc_handler(int sig);

fq_data* data;

int main(int argc, char *argv[])
{
    data = init_options(argc, argv);            // Initialize data with command the arguments
    signal(SIGINT, ctrlc_handler);              // Initialize Ctrl+C signal

    int r = fq_data_init_resources(data, NULL); // Initialize resources (files)
    switch (r) {
        case ERROR_FILE_NOT_FOUND:
            fprintf(stderr, "Error: The input file `%s' cannot be opened.\n", data->filename_in);
            fq_data_free_resources(data);
            exit(ERROR_FILE_NOT_FOUND);
        case ERROR_MEM:
            error_mem(fq_data_free_resources, data);
    }
    r = fq_count(data);                                  // Count the symbols
    switch (r) {
        case ERROR_MEM:
            error_mem(fq_data_free_resources, data);
    }

    freqlist_fprintf(                                    // Print the frequencies
            stdout, "> Final frequency table\n", data->freql, NULL);

    fq_data_free_resources(data);                        // Close file and free memory

    return 0;
}


/* Initialize the global variables with the command options */
fq_data* init_options(int argc, char *argv[])
{
    fq_data* data = fq_data_init();
    if (!data) error_mem(NULL, NULL);
    opterr = 0;
    int c;
    while ((c = getopt(argc, argv, "dcvh")) != -1) {
        switch (c) {
            case 'h':
                printf(USAGE, argv[0]);
                exit(0);
            case 'v':
                data->verbose = TRUE;
                break;
            case '?':
                if (isprint (optopt)) {
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
        if (!data->filename_in) {
            data->filename_in = argv[index];
        } else {
            fprintf(stderr, "%s error: extra operand `%s'\n", argv[0], argv[index]);
            fprintf(stderr, "Try '%s -h' for more information.\n", argv[0]);
            exit(ERROR_PARAM);
        }
    }
    return data;
}


/* Ctrl+C handler */
void ctrlc_handler(int sig) {
    printf("\n");
    if (data && data->freql) {
        if (!data->freql->autosort) {
            freqlist_sort(data->freql);
        }
        freqlist_fprintf(stdout, "> Final frequency table\n",   // Print the frequencies
                         data->freql, NULL);
    }
    exit(0);
}

/*  End program.  */
