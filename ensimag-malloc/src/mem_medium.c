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



unsigned long two_pow_n(int n){
  unsigned long pow=1;
    for(int i=1;i<=n;i++){
      pow=pow*2;
    }
  return pow;
}

void *
emalloc_medium(unsigned long size){
  size=size+4;
  unsigned int indice=puiss2(size);
  unsigned int courant=indice;

  while((courant<FIRST_ALLOC_MEDIUM_EXPOSANT + arena.medium_next_exponant) && (arena.TZL[courant]==NULL)){  //ou just <= ?
    courant++;
  }

  if((courant==FIRST_ALLOC_MEDIUM_EXPOSANT + arena.medium_next_exponant)
    && (arena.TZL[FIRST_ALLOC_MEDIUM_EXPOSANT + arena.medium_next_exponant]==NULL)){

      mem_realloc_medium();

  }

  while(courant!=indice){
    uint64_t* decoupe1=arena.TZL[courant];
    uint64_t* decoupe2=decoupe1+(two_pow_n(courant-1))/8;
    arena.TZL[courant]=(void*)(*decoupe1);
    arena.TZL[courant-1]=(void*)decoupe1;
    *decoupe1=(uint64_t)decoupe2;
    *decoupe2=0;
    courant--;
  }

  uint64_t* ptr_user=arena.TZL[courant];
  arena.TZL[courant]=(void*)(*ptr_user);

  void* mark=mark_memarea_and_get_user_ptr(ptr_user,size,MEDIUM_KIND);

  return(mark);
}




void efree_medium(Alloc a) {

  unsigned int indice = two_pow_n(a.size);

  if(arena.TZL[indice]==NULL){
    arena.TZL[indice]=a.ptr;
    uint64_t** tmp=a.ptr;
    *tmp= NULL;
  }

  else{
    uint64_t* ptr_chunk;
    uint64_t* ptr_prec_chunk;
    while(1){
      ptr_chunk=arena.TZL[indice];
      while(ptr_chunk!=(uint64_t*)(((uint64_t)a.ptr)^((uint64_t)(a.size))) && ptr_chunk!=NULL){
        ptr_prec_chunk=ptr_chunk;
        ptr_chunk=(uint64_t*)(*ptr_chunk);
      }

      if(ptr_chunk==NULL){

        uint64_t** tmp1=arena.TZL[indice];
        uint64_t** tmp2=a.ptr;

        arena.TZL[indice]=a.ptr;
        *tmp2=(uint64_t*)tmp1;

        break;
      }
      else{
        *ptr_prec_chunk=*ptr_chunk;
        if(((uint64_t)a.ptr)>((uint64_t)ptr_chunk)){
          a.ptr=ptr_chunk;
        }
        a.size=2*a.size;
      }
      indice++;
    }

  }

}
