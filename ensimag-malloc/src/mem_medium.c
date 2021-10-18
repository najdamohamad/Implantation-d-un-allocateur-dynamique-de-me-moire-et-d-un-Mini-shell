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

  //taille reelle a alouer: size + 4*8 octets de marquage
  size=size+4*8;

  unsigned int indice=puiss2(size);
  unsigned int courant=indice;

  //parcour de la liste jusqu'a etre dans un des cas suivants:
  //  1.on a parcouru tout le tableau arena.TZL sauf la dernière case
  //  2.on trouve une case case de memoire disponible que l'on va pouvoir (si necessaire) redecouper
  while((courant<FIRST_ALLOC_MEDIUM_EXPOSANT + arena.medium_next_exponant) && (arena.TZL[courant]==NULL)){
    courant++;
  }

  //suite au cas 1. precedent,
  //si en plus on a la derniere case (indice 17) vide alors on appel le realloc_medium
  if((courant==FIRST_ALLOC_MEDIUM_EXPOSANT + arena.medium_next_exponant)
    && (arena.TZL[FIRST_ALLOC_MEDIUM_EXPOSANT + arena.medium_next_exponant]==NULL)){

      mem_realloc_medium();

  }
  //a partir d'ici on a forcement une case libre que l'on peut redecouper


  //on redecoupe recursivement la zone memoire recuperer jusqu'a avoir un bout de la taille souhaitee
  //a chaque indice i on decoupe la case en 2 case de taille /2,
  //on laisse de ces cases à l'indice i-1
  //on utilise l'autre soit pour l'allouer, soit pour la redecouper a l'etape suivante
  while(courant!=indice){
    uint64_t* decoupe1=arena.TZL[courant];
    uint64_t* decoupe2=decoupe1+(two_pow_n(courant-1))/8;
    arena.TZL[courant]=(void*)(*decoupe1);
    arena.TZL[courant-1]=(void*)decoupe1;
    *decoupe1=(uint64_t)decoupe2;
    *decoupe2=0;
    courant--;
  }
  //a partir d'ici on a donc une case libre et de la taille souhaitee

  //on recupere la case en question
  uint64_t* ptr_user=arena.TZL[courant];
  arena.TZL[courant]=(void*)(*ptr_user);

  //on marque la case
  void* mark=mark_memarea_and_get_user_ptr(ptr_user,size,MEDIUM_KIND);

  return(mark);
}




void efree_medium(Alloc a) {

  unsigned int indice = puiss2(a.size);

  //cas 1. il n'y a aucun autre chunk de la meme taille que celui libere dans l'arena.TZL
  if(arena.TZL[indice]==NULL){
    arena.TZL[indice]=a.ptr;
    uint64_t** tmp=a.ptr;
    *tmp= NULL;
  }

  //cas 2. il existe d'autres chunk de la meme taille
  else{
    uint64_t* ptr_chunk;
    uint64_t* ptr_prec_chunk;

    //on verifie si le buddy de notre chunk libere exst dans la liste
    while(1){
      ptr_chunk=arena.TZL[indice];
      while(ptr_chunk!=(uint64_t*)(((uint64_t)a.ptr)^((uint64_t)(a.size))) && ptr_chunk!=NULL){
        ptr_prec_chunk=ptr_chunk;
        ptr_chunk=(uint64_t*)(*ptr_chunk);
      }

      //si il n'y est pas, on met le chunk dans la liste
      if(ptr_chunk==NULL){
        uint64_t** tmp1=arena.TZL[indice];
        uint64_t** tmp2=a.ptr;

        arena.TZL[indice]=a.ptr;
        *tmp2=(uint64_t*)tmp1;

        break;
      }

      //si il y est on le recupere, on reetablie le linkage de la liste et on augmente
      //les valeurs du chunk et de l'indice pour regarder si le budy de ce double chunk
      //est present dans la case suivante du tableau
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
