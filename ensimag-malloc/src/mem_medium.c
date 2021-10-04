/******************************************************
 * Copyright Grégory Mounié 2018                      *
 * This code is distributed under the GLPv3+ licence. *
 * Ce code est distribué sous la licence GPLv3+.      *
 ******************************************************/

#include <stdint.h>
#include <assert.h>
#include "mem.h"
#include "mem_internals.h"

unsigned int puiss2(unsigned long size) {
    unsigned int p=0;
    size = size -1; // allocation start in 0
    while(size) {  // get the largest bit
	p++;
	size >>= 1;
    }
    if (size > (1 << p))
	p++;
    return p;
}


void *
emalloc_medium(unsigned long size)
{
    assert(size < LARGEALLOC);
    assert(size > SMALLALLOC);
    unsigned int indice=puiss2(size);
    unsigned int courant=indice;
    while((arena.TZL[courant]==NULL) && (courant<TZL_SIZE)){
      courant++;
    }
    if(courant=TZL_SIZE){
      mem_realloc_medium();
    }
    while(courant!=indice){
      void* decoupe1=arena.TLZ[courant];
      void* decoupe2=arena.TLZ[courant]+pow(2,courant-1);
      arena.TLZ[courant]=*decoupe1;
      *decoupe1=decoupe2;
      *decoupe2=arena.TLZ[courant-1];
      arena.TLZ[courant-1]=decoupe1;
      courant--;
    }
    void* mem_alloue=arena.TLZ[indice];
    arena.TLZ[indice]=*(arena.TLZ[indice]);
    return (mem_alloue);
}



void efree_medium(Alloc a) {
    unsigned int indice=puiss2(size);
    void* adresse_budy=a.ptr^((uint64_t)*pow(2,indice));
    void* ptr_courant=arena.TLZ[indice];
    while(1){
      if(ptr_courant==adresse_budy){
        *(adresse_budy-pow(2,indice))=NULL;
        a.size=2*a.size;
        indice++;
        adresse_budy=a.ptr^((uint64_t)*pow(2,indice));
        ptr_courant=arena.TLZ[indice];
      }
      else{
        ptr_courant=*ptr_courant;
        if(ptr_courant==NULL){
          *(a.ptr)=arena.TLZ[indice];
          arena.TLZ[indice]=a.ptr;
          break;
        }
      }
    }

}
