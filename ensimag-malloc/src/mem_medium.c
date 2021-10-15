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

  Alloc a;
  a.size=size;
  a.ptr=ptr_user;
  a.kind=MEDIUM_KIND;

  return(a.ptr);
}



// void efree_medium(Alloc a) {
//     unsigned int indice=puiss2(a.size);
//
//     int pow=1;
//     for(int i=1;i<=indice;i++){
//       pow=pow*2;
//     }
//
//     void* adresse_budy=(void*)(((uint64_t)a.ptr)^((uint64_t)pow));
//     void* ptr_courant=arena.TZL[indice];
//     while(1){
//       if(ptr_courant==adresse_budy){
//
//         pow=1;
//         for(int i=1;i<=indice;i++){
//           pow=pow*2;
//         }
//
//         uint64_t** tmp=(uint64_t**)(adresse_budy-pow);
//         *tmp=NULL;
//         a.size=2*a.size;
//         indice++;
//
//         pow=1;
//         for(int i=1;i<=indice;i++){
//           pow=pow*2;
//         }
//
//         adresse_budy=(void*)(((uint64_t)a.ptr)^((uint64_t)pow));
//         ptr_courant=arena.TZL[indice];
//       }
//       else{
//         uint64_t** tmp=ptr_courant;
//         uint64_t* tmp_in=*tmp;
//         ptr_courant=(void*)tmp_in;
//         if(ptr_courant==NULL){
//           tmp=a.ptr;
//           tmp_in=arena.TZL[indice];
//           *tmp=tmp_in;
//           //*(a.ptr)=arena.TZL[indice];
//           arena.TZL[indice]=a.ptr;
//           break;
//         }
//       }
//     }
//
// }

void efree_medium(Alloc a) {

  printf("allocation:\n\tsize: %lu\n\tptr: %p",a.size,a.ptr);
  printf("\tkind: ");
  if(a.kind==SMALL_KIND){
    printf("SMALL_KIND\n");
  }
  if(a.kind==MEDIUM_KIND){
    printf("MEDIUM_KIND");
  }
  if(a.kind==LARGE_KIND){
    printf("LARGE_KIND");
  }

  unsigned int indice = two_pow_n(a.size);

  void* adresse_budy=(void*)(((uint64_t)a.ptr)^((uint64_t)(a.size)));

  while(arena.TZL[indice]==adresse_budy){
    arena.TZL[indice]=NULL;
    indice++;
  }
  arena.TZL[indice]=a.ptr;
}
