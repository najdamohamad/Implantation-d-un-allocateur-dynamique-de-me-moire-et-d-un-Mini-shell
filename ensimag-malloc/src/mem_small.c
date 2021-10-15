/******************************************************
 * Copyright Grégory Mounié 2018                      *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include <assert.h>
#include <stdint.h>
#include "mem.h"
#include "mem_internals.h"

void *
emalloc_small(unsigned long size)
{
  //assert(size<=64*sizeof(uint8_t));

  if(arena.chunkpool==NULL){

    printf("PLUS RIEN DANS LA LISTE DES CHUNKS\n");

    void** ptr_courant;
    uint64_t mem_realloc=(uint64_t)(mem_realloc_small());
    int nb_chunk=(int)(mem_realloc/96);
    for(int i=0;i<nb_chunk-1;i++){
      ptr_courant=arena.chunkpool+i*96*sizeof(uint8_t);
      *ptr_courant=arena.chunkpool+(i+1)*96*sizeof(uint8_t);
    }
    ptr_courant=arena.chunkpool+(nb_chunk-1)*96*sizeof(uint8_t);
    *ptr_courant=NULL;

  }
    void* ptr=arena.chunkpool;
    arena.chunkpool=/**arena.chunkpool*/arena.chunkpool+96*sizeof(uint8_t);
    void* mark=mark_memarea_and_get_user_ptr(ptr,CHUNKSIZE,SMALL_KIND);
    return(mark);
}


void efree_small(Alloc a) {
    void* precedent=arena.chunkpool;
    arena.chunkpool=a.ptr;
    *((uint64_t*)a.ptr)=*(uint64_t*)(precedent);
}
