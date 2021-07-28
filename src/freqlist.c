/* freqlist.c

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


#include "const.h"
#include "freqlist.h"
#include <stdlib.h>


/*
 * Prints the list of frequencies.
 * @f: the output stream, eg. the stdout.
 * @title: prints this message before the list (optional).
 * @freql: the frequency list.
 * @pnode: if not null the symbol is highlighted in
 *         the list (optional).
 */
void freqlist_fprintf(FILE *f, const char *title,
                      const freqlist *freql, node_freqlist *pnode)
{
    if (title)
        fprintf(f, title);
    char *mark = "";
    char markup[16];
    if (pnode) {
        if (freql->freqs[pnode->symb]==1) {
            mark = " *";
        } else if (pnode->debug_last_jump == 0) {
            mark = " <";
        } else {    // +1..
            sprintf(markup, " ^ (%+d)", pnode->debug_last_jump);
            mark = markup;
        }
    }
    fprintf(f, "Symbol    Frequency   Pos\n");
    fprintf(f, "-------------------------\n");
    for (node_freqlist *pnode_i=freql->list; pnode_i; pnode_i=pnode_i->next)
        fprintf(f, "'%c' %02X    %9lu    %2X%s\n",
                (pnode_i->symb < 0x7F && pnode_i->symb >= 0x20) ? pnode_i->symb : '.',
                pnode_i->symb,
                pnode_i->freq,
                pnode_i->pos,
                pnode && pnode == pnode_i ? mark : "");
    fprintf(f, "-------------------------\n");
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
            l->list->debug_last_jump = 0;
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
 * and rearranges if necessary. If the symbol is not present
 * in the list, it adds them, an sets the frequency of the node in 1.
 * Returns the node with the symbol.
 */
node_freqlist *freqlist_add(freqlist *l, unsigned char c)
{
    node_freqlist *pnode1, *pnode_prev;

    /* This function promotes the position of the symbol in the list
       after increase its frequency, or being created. */
    void _freqlist_promote(freqlist *l, node_freqlist *pnode);

    pnode1=l->list;
    while (pnode1 && c!=pnode1->symb) {
        pnode_prev=pnode1;
        pnode1=pnode1->next;
    }

    /* After found the symbol in the list, increase its frequency
       and call the function that will promote the symbol. */
    if (pnode1 && c==pnode1->symb) {
        pnode1->freq++;
        l->freqs[c]++;
        l->size++;
        if (l->autosort) {
            _freqlist_promote(l, pnode1);
        }
        return pnode1;
    }

    /* If the element doesn't exist, it's added to the end of the list,
       with frequency=1, and is promoted in the list with the same
       promotion algorithm. */
    pnode1=(node_freqlist *)malloc(sizeof(node_freqlist));
    if (pnode1) {
        l->freqs[c]=1;
        l->size++;
        l->length++;
        pnode1->symb=c;
        pnode1->pos=pnode_prev->pos + 1;
        pnode1->freq=1;
        pnode1->prev=pnode_prev;
        pnode1->next=NULL;
        pnode1->debug_last_jump=0;
        pnode_prev->next=pnode1;
        _freqlist_promote(l, pnode1);
    }
    return pnode1;
}


/* Promotes the position of the symbol in the list
   (called after its frequency was increased, or after being created. */
void _freqlist_promote(freqlist *l, node_freqlist *pnode)
{
    unsigned char i;
    int steps = 0;      /* num of elements walked until reached new pos */

    node_freqlist *pnode_prev=pnode->prev,
                  *pnode_next;

    /* While exist a prev node with less frequency than pnode
       or have the same frequency but a lower ASCII code ... */
    while (pnode_prev && node_cmp(pnode, pnode_prev) > 0 ) {
        pnode_prev=pnode_prev->prev;
        steps++;
    }
    pnode->debug_last_jump = steps;

    if (pnode_prev && pnode_prev != pnode->prev) {
        /* Previous and next of pnode point together */
        (pnode->prev)->next=pnode->next;
        if (pnode->next)
            (pnode->next)->prev=pnode->prev;
        /* pnode points at its new prev and next */
        pnode->prev=pnode_prev;
        pnode_next=pnode->next;
        pnode->next=pnode_prev->next;
        /* The pnode new and prev point to pnode */
        pnode_prev->next=pnode;
        (pnode->next)->prev=pnode;

        /* pnode is assigned to the position it has its next */
        pnode->pos=(pnode->next)->pos;
        /* Increase in +1 the pos variable of all nodes after pnode */
        pnode_prev=pnode;
        while (pnode_prev->next && pnode_prev->next != pnode_next) {
            pnode_prev=pnode_prev->next;
            pnode_prev->pos++;
        }
        return;
    }

    if (!pnode_prev && l->list != pnode) {
        /* The symbol will be the first in the list */
        if (pnode->next)
            (pnode->next)->prev=pnode->prev;

        (pnode->prev)->next=pnode->next;
        pnode->prev=NULL;
        pnode->next=l->list;
        pnode->pos=0;
        l->list->prev=pnode;
        /* Increase in +1 the pos variable of all nodes after pnode */
        l->list=pnode;
        i=1;
        while (pnode->next) {
            pnode=pnode->next;
            pnode->pos=i++;
        }
    }
}


/* This function dis-promotes the position of the symbol in the list
   after decrease its frequency.
   Return 1 if element if removed from the list, otherwise 0 */
int _freqlist_dispromote(freqlist *l, node_freqlist *pnode)
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
        return 1;
    }

    pnode1=pnode;
    pnode2=pnode1->next;

    /* While exist a next node with more frequency than pnode
       or have same frequency and its ASCII code greater ... */
    while ( pnode2 && node_cmp(pnode1, pnode2) < 0 ) {
        pnode3=pnode2;
        pnode2=pnode2->next;
    }

    if (pnode2 && pnode2!=pnode1->next) {
        /* Previous and next of pnode1 point together */
        (pnode1->next)->prev=pnode1->prev;
        if (pnode1->prev)
            (pnode1->prev)->next=pnode1->next;
        /* pnode1 points at its news prev and next */
        pnode1->next=pnode2;
        pnode3=pnode1->prev;
        pnode1->prev=pnode2->prev;
        /* The new prev and next point to pnode1 */
        pnode2->prev=pnode1;
        (pnode1->prev)->next=pnode1;

        /* pnode1 is assigned to the position it has its prev */
        pnode1->pos=(pnode1->prev)->pos;
        /* Decrease in 1 the pos variable of all nodes before pnode1 */
        pnode2=pnode1;
        while (pnode2->prev && pnode2->prev!=pnode3) {
            pnode2=pnode2->prev;
            pnode2->pos--;
        }
    }

    /* If pnode2 doesn't exist, means that will be the last in the list,
       but if pnode3 is null, means that was already before this iteration */
    if (!pnode2 && pnode3) {
        /* Decrease in 1 the pos variable of all nodes before pnode1 */
        pnode2=pnode1;
        while (pnode2->next) {
            pnode2=pnode2->next;
            pnode2->pos--;
        }
        /* pnode3 points at the last of the list */
        pnode3->next=pnode1;
        /* Previous and next of pnode1 point together */
        (pnode1->next)->prev=pnode1->prev;
        if (pnode1->prev)
            (pnode1->prev)->next=pnode1->next;
        /* pnode1 points at its new prev */
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
    return 0;
}

/*
 * Swap position with the next element in the list.
 *
 *     pnode0          pnode0    (could be NULL)
 *     pnode1    |-->  *pnode
 *     *pnode  ---     pnode1
 *     pnode2          pnode2    (could be NULL)
 *
 * If pnode0 is NULL, then l->list = pnode (first element
 * in the list).
 *
 * Returns 1 if the element were swapped, else 0.
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
 * Sorts the frequencies in case aren't sort.
 * Returns the number of swaps made.
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
 * or symbol if the frequencies are equals.
 */
int node_cmp(const node_freqlist *pnode1, const node_freqlist *pnode2)
{
    if ( pnode1->freq > pnode2->freq
         || (pnode1->freq == pnode2->freq && pnode1->symb < pnode2->symb) ) {
        return 1;
    }
    return -1;
}
