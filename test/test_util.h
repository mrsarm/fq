/* test_util.h

   Copyright (C) 2015 Mariano Ruiz <mrsarm@gmail.com>
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


#ifndef _FQ_TEST_UTIL_H
#define _FQ_TEST_UTIL_H

#include <fq.h>


#define OUTPUT_VERBOSE				TRUE


/**
 * Counts the buffer passed and returns the ``fq_data`` with the output data.
 */
fq_data* count_buff(const unsigned char* buff_in, unsigned int buff_in_length);


/*
 * Frees only the resources used in test mode.
 */
void free_resources(fq_data *data);


/**
 * Check if both buffers are equals.
 */
int buff_check(const unsigned char *buff1, const unsigned char *buff2,
			   unsigned int length);


/*
 * Checks if ``list`` is equals to ``expected_freqlist`` (second list is in
 * array format).
 */
int freqlist_check(const freqlist* plist, const unsigned int expected_freqlist[][2],
				   unsigned int expected_length);

/*
 * Checks if ``list`` is equals to ``expected_freqlist`` (second list is in
 * array format), included H values.
 */
int freqlist_check_with_h(const freqlist* plist, const unsigned int expected_freqlist[][3],
						  unsigned int expected_length);


#endif  /* _FQ_TEST_UTIL_H */
