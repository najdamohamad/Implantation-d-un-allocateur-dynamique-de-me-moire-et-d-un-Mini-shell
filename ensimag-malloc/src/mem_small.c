/******************************************************
 * Copyright Grégory Mounié 2018                      *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include <assert.h>
#include "mem.h"
#include "mem_internals.h"

void *
emalloc_small(unsigned long size)
{
  assert(size>64*sizeof(uint8_t));
  if(arena.chunkpool==NULL){
    unsigned long mem_realloc=mem_realloc_small();
    int nb_chunk=mem_realloc/96;
    for(int i=0;i<nb_chunk;i++){
      *(arena.chunkpool+i*96*sizeof(uint8_t))=arena.chunkpool+(i+1)*96*sizeof(uint8_t);
    }
    *(arena.chunkpool+nb_chunk*96*sizeof(uint8_t)=NULL;

  }
    void* ptr=arena.chunkpool;
    arena.chunkpool=arena.chunkpool+96*sizeof(uint8_t);
    void* mark=mark_memarea_and_get_user_ptr(ptr,CHUNKSIZE,SMALL_KIND);
    return(mark);
}

void efree_small(Alloc a) {
    /* ecrire votre code ici */
}
