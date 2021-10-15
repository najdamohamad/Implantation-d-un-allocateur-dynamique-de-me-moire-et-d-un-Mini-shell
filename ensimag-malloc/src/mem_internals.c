/******************************************************
 * Copyright Grégory Mounié 2018                      *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include <sys/mman.h>
#include <assert.h>
#include <stdint.h>
#include "mem.h"
#include "mem_internals.h"

unsigned long knuth_mmix_one_round(unsigned long in)
{
    return in * 6364136223846793005UL % 1442695040888963407UL;
}

void *mark_memarea_and_get_user_ptr(void *ptr, unsigned long size, MemKind k)
{

    unsigned long * taille_start =ptr;
    unsigned long * magique_start=taille_start+1;
    void* user_pointer=magique_start+1;
    unsigned long * magique_end=user_pointer+(size-4*sizeof(unsigned long));
    unsigned long * taille_end=magique_end+1;
    unsigned long magique = knuth_mmix_one_round((unsigned long)ptr);

    switch(k){
        case SMALL_KIND:
            //set last two bits to 00
            magique &= ~(1UL);magique &= ~(1UL<<1);
            break;
        case MEDIUM_KIND:
            //set last two bits to 01
            magique |= (1UL);magique &= ~(1UL<<1);
            break;
        case LARGE_KIND:
            //set last two bits to 10
            magique &= ~(1UL);magique |= (1UL<<1);
            break;
    }

    *magique_start = magique;
    *magique_end =  magique;
    *taille_start =  size;
    *taille_end =  size;
    return user_pointer;
}

Alloc
mark_check_and_get_alloc(void *ptr)
{
    Alloc a = {};

    unsigned long* magique_start = ptr-sizeof(unsigned long);
    unsigned long* taille_start = magique_start-1;
    unsigned long magique= *magique_start;
    unsigned long taille= *taille_start;

    int mask = 0b11;
    int type = magique & mask;
    MemKind kind;

    printf("magique: %lu\ntype:   %x\n",magique,type);


    switch(type){
        case 0:
            kind=SMALL_KIND;
            break;
        case 1:
            kind=MEDIUM_KIND;
            break;
        case 2:
            kind=LARGE_KIND;
            break;
    }

kind=MEDIUM_KIND; //attention

    a.ptr=(void *) taille_start;
    a.kind=kind;
    a.size=taille;

    return a;
}


unsigned long
mem_realloc_small() {
    assert(arena.chunkpool == 0);
    unsigned long size = (FIRST_ALLOC_SMALL << arena.small_next_exponant);
    arena.chunkpool = mmap(0,
			   size,
			   PROT_READ | PROT_WRITE | PROT_EXEC,
			   MAP_PRIVATE | MAP_ANONYMOUS,
			   -1,
			   0);
    if (arena.chunkpool == MAP_FAILED)
	handle_fatalError("small realloc");
    arena.small_next_exponant++;
    return size;
}

unsigned long
mem_realloc_medium() {
    uint32_t indice = FIRST_ALLOC_MEDIUM_EXPOSANT + arena.medium_next_exponant;
    assert(arena.TZL[indice] == 0);
    unsigned long size = (FIRST_ALLOC_MEDIUM << arena.medium_next_exponant);
    assert( size == (1 << indice));
    arena.TZL[indice] = mmap(0,
			     size*2, // twice the size to allign
			     PROT_READ | PROT_WRITE | PROT_EXEC,
			     MAP_PRIVATE | MAP_ANONYMOUS,
			     -1,
			     0);
    if (arena.TZL[indice] == MAP_FAILED)
	handle_fatalError("medium realloc");
    // align allocation to a multiple of the size
    // for buddy algo
    arena.TZL[indice] += (size - (((intptr_t)arena.TZL[indice]) % size));
    arena.medium_next_exponant++;
    return size; // lie on allocation size, but never free
}


// used for test in buddy algo
unsigned int
nb_TZL_entries() {
    int nb = 0;

    for(int i=0; i < TZL_SIZE; i++)
	if ( arena.TZL[i] )
	    nb ++;

    return nb;
}
