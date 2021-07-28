/* freqlist.h

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


#ifndef _FREQLIST_H
#define _FREQLIST_H


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
    unsigned char symb;             /* Referenced symbol. */
    unsigned char pos;              /* Position in the list starting in 0
                                       the first element in the list
                                       (higher frequency) */
    unsigned long freq;             /* Occurrences of the character
                                       in the stream (frequency). */
    struct _node_freqlist *prev;    /* Pointer to the previous node in
                                       the list, with frequency greater
                                       or equal than this. */
    struct _node_freqlist *next;    /* Pointer to the next node in the list,
                                       with frequency less or equal
                                       than this. */
    int debug_last_jump;            /* Number of symbols jumped in freqlist
                                       last time symbol was found, e.g. if
                                       symbol was in pos 4 and them went to
                                       pos 1, the value will be 3 */
} node_freqlist;


/*
 * Struct with the node list of frequencies,
 * and an array with the frequency of each symbol.
 */
typedef struct _freqlist
{
    node_freqlist *list;            /* Pointer to the first node
                                       (with the highest frequency). */
    int autosort;                   /* TRUE if the list must be sorted
                                       each time a symbol is added */
    unsigned char freqs[256];       /* Frequencies of each symbol. */
    unsigned int length;            /* Numbers of different symbols
                                       in the list. */
    unsigned long size;             /* Numbers of symbols in the list. */
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
 * and rearranges if necessary. If the symbol is not present
 * in the list, it adds them, an sets the frequency of the node in 1.
 * Returns the node with the symbol.
 */
node_freqlist *freqlist_add(freqlist *l, unsigned char c);

/*
 * Prints the list of frequencies.
 * @f: the output stream, eg. the stdout.
 * @title: prints this message before the list (optional).
 * @freql: the frequency list.
 * @pnode: if not null the symbol is highlighted in
 *         the list (optional).
 */
void freqlist_fprintf(FILE *f, const char *title,
                      const freqlist *freql, node_freqlist *pnode);


/*
 * Sorts the frequencies in case aren't sort.
 * Returns the number of swaps made.
 */
int freqlist_sort(freqlist *l);


/*
 * Compares by frequency of the node,
 * or symbol if the frequencies are equals.
 */
int node_cmp(const node_freqlist *pnode1, const node_freqlist *pnode2);


#endif /* _FREQLIST_H */
