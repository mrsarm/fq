/* test_datasets.h

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


#ifndef _FQ_TEST_DATASETS_H
#define _FQ_TEST_DATASETS_H


/*
 * ************
 *  DATA SET 1
 * ************
 */
#define BUFF_IN_LENGTH_1                10
#define EXPECTED_FREQLIST_LENGTH_1	    5
extern unsigned char buff_in_1[BUFF_IN_LENGTH_1];
extern unsigned int expected_freqlist_1[EXPECTED_FREQLIST_LENGTH_1][2];


/*
 * ************
 *  DATA SET 2
 * ************
 */
#define BUFF_IN_LENGTH_2                12
#define EXPECTED_FREQLIST_LENGTH_2	    8
extern unsigned char buff_in_2[BUFF_IN_LENGTH_2];
extern unsigned int expected_freqlist_2[EXPECTED_FREQLIST_LENGTH_2][2];


#endif  /* _FQ_TEST_DATASETS_H */
