/* test_fq.c

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


#include "cheat.h"
#include "fq.h"
#include "freqlist.h"
#include "test_util.h"
#include "test_datasets.h"


CHEAT_DECLARE(
		fq_data* data;
)
CHEAT_TEAR_DOWN(
	if (data) {
		free_resources(data);
	}
)

/***************
 *  DATA SET 1
 ***************/
CHEAT_TEST(expected_buff_out_ok,
	data=count_buff(buff_in_1, BUFF_IN_LENGTH_1);
	cheat_assert(  freqlist_check(data->freql, expected_freqlist_1, EXPECTED_FREQLIST_LENGTH_1)  );
)

/***************
 *  DATA SET 2
 ***************/
CHEAT_TEST(expected_buff_out_2_ok,
           data=count_buff(buff_in_2, BUFF_IN_LENGTH_2);
                   cheat_assert(  freqlist_check(data->freql, expected_freqlist_2, EXPECTED_FREQLIST_LENGTH_2)  );
)
