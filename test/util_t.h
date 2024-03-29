/* test_util.h

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


#ifndef _FQ_UTIL_T_H
#define _FQ_UTIL_T_H

#include <fq.h>

#define ARRAY_SIZE(arr)             sizeof(arr) / sizeof((arr)[0])

#define OUTPUT_VERBOSE				FALSE


/*
 * Count the buffer passed and return the ``fq_data`` with the output data.
 */
fq_data* count_buff(unsigned char* buff_in, unsigned int buff_in_length, int verbose);


/*
 * Free only the resources used in test mode.
 */
void free_resources(fq_data *data);


/*
 * Check if plist is equals to expected_freqlist (second list is in
 * array format).
 */
int freqlist_check(const freqlist* plist, const unsigned int expected_freqlist[][2],
				   unsigned int expected_length);


#endif  /* _FQ_UTIL_T_H */
