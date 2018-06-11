/* freqlist.c

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


#include "const.h"
#include "freqlist.h"
#include <stdlib.h>


/*
 * Print the list of frequencies, only if verbose
 * is TRUE.
 * @msg: print this message before the list (optional).
 * @freql: the frequency list.
 * @f: the output stream, eg. the stdout.
 * @verbose: TRUE to print the freql.
 */
void freqlist_fprintf(const char *msg, const freqlist *freql, FILE *f)
{
	node_freqlist *pnode;
	if (msg)
		fprintf(f, msg);
	char * line = "Symb.: '%c' %2X   Freq.: %lu\tPos.: %2X\n";
	int i=0;
	for (pnode=freql->list; pnode; pnode=pnode->next, i++)
		fprintf(f, line,
				  (pnode->symb<0x7F && pnode->symb>=0x20)?pnode->symb:'.',
				   pnode->symb,
				   pnode->freq,
				   pnode->pos);
	fprintf(f, "Size: %lu - Number of symbols: %d\n",
			freql->size, freql->length);
}


/*
 * Creates a list, and initializes the first node with the passed value.
 * Returns the list created.
 */
freqlist* freqlist_create(unsigned char c)
{
	freqlist *l = (freqlist *)malloc(sizeof(freqlist));
	if (l) {
		l->list=(node_freqlist *)malloc(sizeof(node_freqlist));
		if (l->list) {
			l->list->symb=c;
			l->list->pos=(unsigned char)0;
			l->list->freq=1;
			l->list->next=l->list->prev=NULL;
		}
		for (int i=0; i<256; i++)
			l->freqs[i]=0;
		l->freqs[c]=1;
		l->length=1;
		l->size=1L;
		l->autosort = FALSE;
	}
	return l;
}

/*
 * Frees the memory of the list.
 */
void freqlist_free(freqlist* l)
{
	node_freqlist *pnode;
	pnode=l->list;
	while (pnode->next)
		pnode=pnode->next;
	while (pnode->prev) {
		pnode=pnode->prev;
		free(pnode->next);
	}
	free(pnode);
	free(l);
}


/*
 * Returns the node with the symbol 'c', or return NULL if not
 * in the list.
 */
node_freqlist *freqlist_find(const freqlist *l, unsigned char c)
{
	/* TODO Check if the node exist with l.freqs[c] > 0
	        to be more efficient */
	node_freqlist *pnode = l->list;
	while (pnode && c!=pnode->symb) pnode=pnode->next;
	if (pnode && c==pnode->symb) return pnode;
	return NULL;
}



/*
 * Increases the frequency of the symbol 'c' in +1,
 * and rearrange if necessary. If the symbol is not present
 * in the list, add them, an set the frequency of the node in 1.
 * Returns the node with the symbol.
 */
node_freqlist *freqlist_add(freqlist *l, unsigned char c)
{
	node_freqlist *pnode1, *pnode2;

	/* This function promotes the position of the symbol in the list
	   after increase your frequency, or be created. */
	node_freqlist *_freqlist_promote(freqlist *l, node_freqlist *pnode);

	pnode1=l->list;
	while (pnode1 && c!=pnode1->symb) {
		pnode2=pnode1;
		pnode1=pnode1->next;
	}

	/* After found the symbol in the list, increase your frequency
	   and calls the function that will promote. */
	if (pnode1 && c==pnode1->symb) {
		pnode1->freq++;
		l->freqs[c]++;
		l->size++;
		if (l->autosort) {
			pnode1 = _freqlist_promote(l, pnode1);
		}
		return pnode1;
	}

	/* If the element doesn't exist, is added to the end of the list,
	   with frequency=1, and is promoted if the list with the same
	   promotion algorithm. */
	pnode1=(node_freqlist *)malloc(sizeof(node_freqlist));
	if (pnode1) {
		l->freqs[c]=1;
		l->size++;
		l->length++;
		pnode1->symb=c;
		pnode1->pos=pnode2->pos+1;
		pnode1->freq=1;
		pnode1->prev=pnode2;
		pnode1->next=NULL;
		pnode2->next=pnode1;
		pnode1=_freqlist_promote(l, pnode1);
	} else {
		return NULL;
	}
	return pnode1;
}


/* This function promotes the position of the symbol in the list
   after increase your frequency, or be created. */
node_freqlist *_freqlist_promote(freqlist *l, node_freqlist *pnode)
{
	unsigned char i;

	node_freqlist *pnode1=pnode,
	              *pnode2=pnode->prev,
	              *pnode3;

	/* While exist a prev node with less frequency than pnode
		   or have he same frequency and your ASCII code less ... */
	while ( pnode2 && node_cmp(pnode1, pnode2) > 0 ) {
		pnode2=pnode2->prev;
	}

	if (pnode2 && pnode2!=pnode1->prev) {
		/* Previous and next of pnode1 point together */
		(pnode1->prev)->next=pnode1->next;
		if (pnode1->next)
			(pnode1->next)->prev=pnode1->prev;
		/* pnode1 point at yours news prev and next */
		pnode1->prev=pnode2;
		pnode3=pnode1->next;
		pnode1->next=pnode2->next;
		/* The pnode1 news and prev point to pnode1 */
		pnode2->next=pnode1;
		(pnode1->next)->prev=pnode1;

		/* pnode1 is assigned to the position it has yours next */
		pnode1->pos=(pnode1->next)->pos;
		/* Increase in +1 the pos variable of all nodes after pnode1 */
		pnode2=pnode1;
		while (pnode2->next && pnode2->next!=pnode3) {
			pnode2=pnode2->next;
			pnode2->pos++;
		}
		return pnode1;
	}

	if (!pnode2 && l->list!=pnode1) {
		/* The symbol will be the first in the list */
		if (pnode1->next)
			(pnode1->next)->prev=pnode1->prev;

		(pnode1->prev)->next=pnode1->next;
		pnode1->prev=NULL;
		pnode1->next=l->list;
		pnode1->pos=0;
		l->list->prev=pnode1;
		/* Increase in +1 the pos variable of all nodes after pnode1 */
		l->list=pnode1;
		i=1;
		while (pnode1->next) {
			pnode1=pnode1->next;
			pnode1->pos=i++;
		}
		return l->list;
	}
	return pnode1;
}


/* This function dis-promotes the position of the symbol in the list
   after decrease your frequency. */
node_freqlist *_freqlist_dispromote(freqlist *l, node_freqlist *pnode)
{
	node_freqlist *pnode1, *pnode2, *pnode3=NULL;

	/* If the node has freq=0 is removed from the list */
	if (pnode->freq == 0) {
		if (pnode->next)
			(pnode->next)->prev=pnode->prev;
		if (pnode->prev)
			(pnode->prev)->next=pnode->next;
		pnode1=pnode->next;
		if (pnode==l->list) {
			l->list=pnode1;
		}
		free(pnode);
		while (pnode1) {
			pnode1->pos--;
			pnode1=pnode1->next;
		}
		l->length--;
		return NULL;
	}

	pnode1=pnode;
	pnode2=pnode1->next;

	/* While exist a next node with more frequency than pnode
	   or have same frequency and your ASCII code greater ... */
	while ( pnode2 && node_cmp(pnode1, pnode2) < 0 ) {
		pnode3=pnode2;
		pnode2=pnode2->next;
	}

	if (pnode2 && pnode2!=pnode1->next) {
		/* Previous and next of pnode1 point together */
		(pnode1->next)->prev=pnode1->prev;
		if (pnode1->prev)
			(pnode1->prev)->next=pnode1->next;
		/* pnode1 point at yours news prev and next */
		pnode1->next=pnode2;
		pnode3=pnode1->prev;
		pnode1->prev=pnode2->prev;
		/* The pnode1 news and prev point to pnode1 */
		pnode2->prev=pnode1;
		(pnode1->prev)->next=pnode1;

		/* pnode1 is assigned to the position it has yours prev */
		pnode1->pos=(pnode1->prev)->pos;
		/* Decrease in 1 the pos variable of all nodes before pnode1 */
		pnode2=pnode1;
		while (pnode2->prev && pnode2->prev!=pnode3) {
			pnode2=pnode2->prev;
			pnode2->pos--;
		}
	}

	/* If pnode2 doesn't exist, means that will be the last in the list,
	   but if pnode3 is null, means that already was before this iteration */
	if (!pnode2 && pnode3) {
		/* Decrease in 1 the pos variable of all nodes before pnode1 */
		pnode2=pnode1;
		while (pnode2->next) {
			pnode2=pnode2->next;
			pnode2->pos--;
		}
		/* pnode3 point at the last of the list */
		pnode3->next=pnode1;
		/* Previous and next of pnode1 point together */
		(pnode1->next)->prev=pnode1->prev;
		if (pnode1->prev)
			(pnode1->prev)->next=pnode1->next;
		/* pnode1 point at your new prev */
		pnode1->next=NULL;
		pnode1->prev=pnode3;
		pnode1->pos=pnode3->pos+1;

	}

	/* The symbol leaves the first position in the table, if it was */
	if (pnode1->prev && l->list==pnode1) {
		pnode2=pnode1->prev;
		while (pnode2->prev) {
			pnode2=pnode2->prev;
		}
		l->list=pnode2;
	}
	return pnode1;
}

/*
 * Swap position with the next element in the list.
 *
 *     pnode0          pnode0	(could be NULL)
 *     pnode1    |-->  *pnode
 *     *pnode  ---     pnode1
 *     pnode2          pnode2	(could be NULL)
 *
 * If pnode0 is NULL, then l->list = pnode (first element
 * in the list).
 *
 * Returns 1 if the element were swapped, otherwise 0
 */
int _freqlist_swap_with_prev(freqlist *l, node_freqlist * pnode) {
	if (pnode->prev) {
		node_freqlist * pnode1 = pnode->prev;
		node_freqlist * pnode0 = pnode1->prev;
		node_freqlist * pnode2 = pnode->next;
		if (pnode0) {
			pnode0->next = pnode;
		} else {
			l->list = pnode;
		}
		pnode->prev = pnode0;
		pnode->next = pnode1;
		pnode1->prev = pnode;
		pnode1->next = pnode2;
		if (pnode2) {
			pnode2->prev = pnode1;
		}
		unsigned char pnode_pos = pnode->pos;
		pnode->pos = pnode1->pos;
		pnode1->pos = pnode_pos;
		return 1;
	}
	return 0;
}


/*
 * Sorts the frequencies in case
 * aren't sort.
 * Returns the number swaps made.
 */
int freqlist_sort(freqlist *l) {
	int swaps = 0;
	if (!l->autosort) {
		// Gnome sort implementation
		node_freqlist * pnode = l->list;
		while (pnode) {
			if (pnode == l->list || node_cmp(pnode, pnode->prev) < 0) {
				pnode = pnode->next;
			} else {
				//node_freqlist * pnode_prev = pnode->prev;
				/* swap pnode and pnode->prev */
				_freqlist_swap_with_prev(l, pnode);
				swaps++;
			}
		}
	}
	return swaps;
}


/*
 * Compares by frequency of the node,
 * and symbol if the frequency are equals.
 */
int node_cmp(const node_freqlist *pnode1, const node_freqlist *pnode2)
{
	if ( pnode1->freq > pnode2->freq
		 || (pnode1->freq == pnode2->freq && pnode1->symb < pnode2->symb) ) {
		return 1;
	}
	return -1;
}
