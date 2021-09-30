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
    uint64_t taille=(uint64_t)size;
    *ptr=taille;
    *(ptr+size-0x10)=taille;

    uint64_t magic=(uint64_t)(knuth_mmix_one_round((unsigned long)ptr);
    if(size<=96*sizeof(uint8_t)){
      magic=magic | 0x1;
      magic=magic & ~(0x10);
    }
    if((size>96*sizeof(uint8_t))&&(size<128*sizeof(uint8_t))){
      magic=magic | 0x10;
      magic=magic & ~(0x01);
    }
    if(size>=128*sizeof(uint8_t)){
      magic= magic | 0x11;
    }

    *(ptr+0x8)=magic;
    *(ptr+size-2*0x10)=magic;

    void* ptr_utilisateur=ptr+size;
    return ptr_utilisateur;
}

Alloc
mark_check_and_get_alloc(void *ptr)
{
    uint64_t taille=*(ptr-2*sizeof(uint64_t));
    uint64_t magic1=*(ptr-sizeof(uint64_t));
    uint64_t magic2=*(ptr+taille*sizeof(uint8_t));

    assert(magic1==magic2);

    uint64_t kind=magic & 0x111;

    Alloc a;
    a.ptr=ptr-2*sizeof(uint8_t);

    uint64_t kind=magic & 0x111;
    if(kind==0x01){
      a.kind=SMALL_KIND;
    }
    if(kind==0x10){
      a.kind=MEDIUM_KIND;
    }
    if(kind==0x11){
      a.kind=LARGE_KIND;
    }
    
    a.size=(unsigned long)taille;

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
