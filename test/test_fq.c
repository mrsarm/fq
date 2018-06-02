/* test_fq.c

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


#include "cheat.h"
#include "fq.h"
#include "freqlist.h"
#include "test_util.h"

#define BUFF_LEN_1			10
#define EXPECTED_FQ_LEN_1	 5

#define BUFF_LEN_2			12
#define EXPECTED_FQ_LEN_2	 8

CHEAT_DECLARE(
	fq_data* data;

	/***************
	 *  DATA SET 1
	 ***************/
	unsigned char buff_1[BUFF_LEN_1] =					{ 4, 2, 3, 3, 6, 3, 4, 2, 6, 8 };
	unsigned int expected_fq_1[EXPECTED_FQ_LEN_1][2] =	{{3,3}, {2,2}, {4,2}, {6,2}, {8,1}};

	/***************
	 *  DATA SET 2
	 ***************/
	unsigned char buff_2[BUFF_LEN_2] =					{ 0, 5, 3, 2, 2, 8, 5, 0, 6, 18, 10, 0 };
	unsigned int expected_fq_2[EXPECTED_FQ_LEN_2][2] =	{{0,3}, {2,2}, {5,2}, {3,1}, {6,1}, {8,1}, {10,1}, {18,1}};
)

CHEAT_TEAR_DOWN(
	if (data) {
		free_resources(data);
	}
)


/* Test DATA SET 1 */
CHEAT_TEST(expected_buff_out_ok,
	data=count_buff(buff_1, BUFF_LEN_1);
	cheat_assert(  freqlist_check(data->freql, expected_fq_1, EXPECTED_FQ_LEN_1)  );
)

/* Test DATA SET 2 */
CHEAT_TEST(expected_buff_out_2_ok,
   data=count_buff(buff_2, BUFF_LEN_2);
   cheat_assert(  freqlist_check(data->freql, expected_fq_2, EXPECTED_FQ_LEN_2)  );
)
