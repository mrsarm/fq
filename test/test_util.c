/* test_util.c

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


#include <stdlib.h>
#include "test_util.h"
#include "const.h"


/**
 * Counts the buffer passed and returns the ``fq_data`` with the output data.
 */
fq_data* count_buff(const unsigned char* buff_in, unsigned int buff_in_length) {

	fq_data* data = fq_data_init();
	if (!data) {
		fprintf(stderr, "Error: Insufficient memory error.\n");
		exit(ERROR_MEM);
	}
	data->verbose = OUTPUT_VERBOSE;
	// Initialize with a memory stream
	fq_data_init_resources_fi(data, fmemopen(buff_in, buff_in_length, "rb"));

	int r = fq_count(data);
	switch (r) {
		case ERROR_MEM:
			error_mem(free_resources, data);
	}

	freqlist_fprintf("> Final list lfrec\n",
					 data->freql, stdout);
	if (data->verbose) printf("\n-------------------------------------\n\n");
	return data;
}


/*
 * Frees only the resources used in test mode.
 */
void free_resources(fq_data *data)
{
	if (data->freql) freqlist_free(data->freql);
	free(data);
}


/*
 * Checks if ``list`` is equals to ``expected_freqlist`` (second list is in
 * array format).
 */
int freqlist_check(const freqlist* plist, const unsigned int expected_freqlist[][2],
				   unsigned int expected_length) {

	if (plist->length !=  expected_length) {
		fprintf(stderr, "Different lengths. plist: %d - expected: %d\n", plist->length, expected_length);
		return FALSE;
	}
	node_freqlist*pnode = plist->list;
	int i = 0;
	while (pnode) {
		if (pnode->symb != expected_freqlist[i][0]
				|| pnode->freq != expected_freqlist[i][1]) {
			fprintf(stderr, "Different values in plist. Pos.: %d - Symbols: %d, %d. "
							"Frequencies: %lu, %u\n",
					i, (int)pnode->symb, expected_freqlist[i][0], pnode->freq, expected_freqlist[i][1]);
			return FALSE;
		}
		pnode = pnode->next;
		i++;
	}
	return TRUE;
}
