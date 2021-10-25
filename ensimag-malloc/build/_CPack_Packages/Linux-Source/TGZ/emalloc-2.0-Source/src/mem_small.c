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

  //caas 1: la liste des chunk est vide
  if(arena.chunkpool==NULL){
    void** ptr_courant;

    //reallocation d'un zone memoire pour refaire une liste de chunk
    uint64_t mem_realloc=(uint64_t)(mem_realloc_small());

    //calcul du nombre de chunk possible de decouper dans la zone memoire reallouee
    int nb_chunk=(int)(mem_realloc/96);

    //linkage des differents chunks de la liste
    for(int i=0;i<nb_chunk-1;i++){
      ptr_courant=arena.chunkpool+i*96;
      printf("ptr : %p\n",ptr_courant);
      *ptr_courant=arena.chunkpool+(i+1)*96;
    }
    ptr_courant=arena.chunkpool+(nb_chunk-1)*96;
    *ptr_courant=NULL;

  }
  //cas courant (liste de chunk non vide): on prend le retourne le premier chunk de la liste
  //apres l'avoir marque et on lie le chunkpool aux 2eme chunk de la liste
    void* ptr=arena.chunkpool;
    arena.chunkpool=arena.chunkpool+96 ;
    void* mark=mark_memarea_and_get_user_ptr(ptr,CHUNKSIZE,SMALL_KIND);
    return(mark);
}


void efree_small(Alloc a) {
    //on insere le chunk libere en tete de liste des chunks
    void* precedent=arena.chunkpool;
    arena.chunkpool=a.ptr;
    *((uint64_t*)a.ptr)=*(uint64_t*)(precedent);
}
