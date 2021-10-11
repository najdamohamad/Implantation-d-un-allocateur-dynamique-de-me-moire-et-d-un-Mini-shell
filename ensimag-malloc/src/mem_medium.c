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
    // assert(size < LARGEALLOC);
    // assert(size > SMALLALLOC);
    unsigned int indice=puiss2(size);
    unsigned int courant=indice;
    while((arena.TZL[courant]==NULL) && (courant<TZL_SIZE)){
      courant++;
    }
    if((courant==TZL_SIZE) && (arena.TZL[courant]==NULL)){
      mem_realloc_medium();
      void* tmp_user=arena.TZL[indice];
      arena.TZL[indice]=NULL;
      return(tmp_user);
    }
    while(courant!=indice){
      uint64_t* decoupe1=arena.TZL[courant];

      int pow=1;
      for(int i=1;i<=courant-1;i++){
        pow=pow*2;
      }

      uint64_t* decoupe2=decoupe1+pow/16;
      arena.TZL[courant]=(void*)*decoupe2;
      *decoupe1=(uint64_t)decoupe2;
      *decoupe2=(uint64_t)arena.TZL[courant-1];//NULL
      arena.TZL[courant-1]=decoupe1;
      courant--;
    }
    //void* mem_alloue=arena.TZL[indice];

    //
    void** tmp_arena=(void**)(arena.TZL[indice]);
    void* tmp_in_arena=*tmp_arena;
    arena.TZL[indice]=(void*)(tmp_in_arena);
    //arena.TZL[indice]=*(arena.TZL[indice]);
    return ((void*)tmp_arena);
}



void efree_medium(Alloc a) {
    unsigned int indice=puiss2(a.size);

    int pow=1;
    for(int i=1;i<=indice;i++){
      pow=pow*2;
    }

    void* adresse_budy=(void*)(((uint64_t)a.ptr)^((uint64_t)pow));
    void* ptr_courant=arena.TZL[indice];
    while(1){
      if(ptr_courant==adresse_budy){

        pow=1;
        for(int i=1;i<=indice;i++){
          pow=pow*2;
        }

        uint64_t** tmp=(uint64_t**)(adresse_budy-pow);
        *tmp=NULL;
        a.size=2*a.size;
        indice++;

        pow=1;
        for(int i=1;i<=indice;i++){
          pow=pow*2;
        }

        adresse_budy=(void*)(((uint64_t)a.ptr)^((uint64_t)pow));
        ptr_courant=arena.TZL[indice];
      }
      else{
        uint64_t** tmp=ptr_courant;
        uint64_t* tmp_in=*tmp;
        ptr_courant=(void*)tmp_in;
        if(ptr_courant==NULL){
          tmp=a.ptr;
          tmp_in=arena.TZL[indice];
          *tmp=tmp_in;
          //*(a.ptr)=arena.TZL[indice];
          arena.TZL[indice]=a.ptr;
          break;
        }
      }
    }

}
