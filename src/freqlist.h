/* freqlist.h

   Copyright (C) 2015-2018 Mariano Ruiz <mrsarm@gmail.com>
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


#ifndef __FQ_FREQLIST_H
#define __FQ_FREQLIST_H


#include <stdio.h>


/*
 * Node element of the frequency list.
 *
 *   prev * node ( >= freq )
 *        ▲
 *        |
 *       node
 *        |
 *        ▼
 *   next * node ( <= freq )
 *
 */
typedef struct _node_freqlist
{
   unsigned char symb;				/* Referenced symbol. */
   unsigned char pos;				/* Position in the list */
   unsigned long freq;				/* Occurrences of the character
									   in the stream (frequency). */
   struct _node_freqlist *prev;		/* Pointer to the previous node in
									   the list, with frequency greater
									   or equal than this. */
   struct _node_freqlist *next;		/* Pointer to the next node in the list,
									   with frequency less or equal
									   than this. */
} node_freqlist;


/*
 * Struct with the node list of frequencies,
 * and a array with the frequency of each symbol.
 */
typedef struct _freqlist
{
	node_freqlist *list;					/* Pointer to the first node
											   (with the highest frequency). */
	unsigned char freqs[256];				/* Frequency array of each symbol. */
	unsigned int length;					/* Numbers of different symbols
											   in the list. */
	unsigned long size;						/* Numbers of symbols in the list. */
} freqlist;


/*
 * Creates a list, and initializes the first node with the passed value.
 * Returns the list created.
 */
freqlist* freqlist_create(unsigned char c);

/*
 * Frees the memory of the list.
 */
void freqlist_free(freqlist* l);

/*
 * Returns the node with the symbol 'c', or return NULL if not
 * in the list.
 */
node_freqlist *freqlist_find(const freqlist* l, unsigned char c);

/*
 * Increases the frequency of the symbol 'c' in +1,
 * and rearrange if necessary. If the symbol is not present
 * in the list, add them, an set the frequency of the node in 1.
 * Returns the node with the symbol.
 */
node_freqlist *freqlist_add(freqlist *l, unsigned char c);

/*
 * Decreases the frequency of the symbol 'c', and rearrange if
 * necessary. The symbol is removed if has frequency=1
 * in the list.
 * Returns the node with the symbol, or NULL if the
 * symbol was removed.
 */
node_freqlist *freqlist_sub(freqlist *l, unsigned char c);

/*
 * Print the list of frequencies.
 * @msg: print this message before the list (optional).
 * @freql: the frequency list.
 * @f: the output stream, eg. the stdout.
 * @verbose: TRUE to print the freql.
 */
void freqlist_fprintf(const char *msg, const freqlist *freql, FILE *f);


/*
 * Compares by frequency of the node,
 * and symbol if previous comparison are equals.
 */
int node_cmp(const node_freqlist *pnode1, const node_freqlist *pnode2);


#endif /* __FQ_FREQLIST_H */
