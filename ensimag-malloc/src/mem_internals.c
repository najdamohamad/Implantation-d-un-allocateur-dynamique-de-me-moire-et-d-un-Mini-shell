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
    /* ecrire votre code ici */

    //get positions;
    unsigned long * taille_start =ptr;
    unsigned long * magique_start=taille_start+1;
    void* user_pointer=magique_start+1;
    unsigned long * magique_end=user_pointer+(size-4*sizeof(unsigned long));
    unsigned long * taille_end=magique_end+1;
    // printf("given size:%lu \n",size);
    // printf("ptr:%p \n",taille_start);
    // printf("taille_start:%p \n",taille_start);
    // printf("magique_start:%p \n",magique_start);
    // printf("user_pointer:%p \n",user_pointer);
    // printf("magique_end:%p \n",magique_end);
    // printf("taille_end:%p \n",taille_end);

    /*void * taille_start2 =ptr;
    void * magique_start2=taille_start2+sizeof(unsigned long);
    void* user_pointer2=magique_start2+sizeof(unsigned long);
    void * magique_end2=user_pointer2+size-32;
    void * taille_end2=magique_end2+sizeof(unsigned long);
    printf("taille_start2:%p \n",taille_start2);
    printf("magique_start2:%p \n",magique_start2);
    printf("user_pointer2:%p \n",user_pointer2);
    printf("magique_end2:%p \n",magique_end2);
    printf("taille_end2:%p \n",taille_end2);*/


    //get magique. IS parameter is ptr???
    //printf("giving (unsigned long):%lu \n",(unsigned long)ptr);
    unsigned long magique = knuth_mmix_one_round((unsigned long)ptr);

    // printf("Magic: %lu",magique);
    // getchar();

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

    // printf("Magic: %lu",magique);
    // getchar();

    //write taille and magique;
    *magique_start = magique;
    *magique_end =  magique;
    *taille_start =  size;
    *taille_end =  size;
    // printf("read magic_start:%lu \n",*magique_start);
    // printf("read magic_end:%lu \n",*magique_end);
    // printf("read taille_start:%lu \n",*taille_start);
    // printf("read taille_end:%lu \n",*taille_end);

    //return user pointer
    //printf("returning:%p \n",user_pointer);
    return user_pointer;
}

Alloc
mark_check_and_get_alloc(void *ptr)
{
    /* ecrire votre code ici */
    Alloc a = {};

    //get pointers
    unsigned long* magique_start = ptr-sizeof(unsigned long);
    unsigned long* taille_start = magique_start-1;
    // printf("ptr:%p \n",ptr);
    // printf("magique_start:%p \n",magique_start);
    // printf("taille_start:%p \n",taille_start);

    //get values
    unsigned long magique= *magique_start;
    unsigned long taille= *taille_start;
    // printf("magique:%lu \n",magique);
    // printf("taille_taille:%lu \n",taille);

    //assert magic number sam on both sides
    //TODO

    //get kind from last two bits
    int mask = 0b11;
    int type = magique & mask;
    // printf("type:%d \n",type);
    MemKind kind;
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

    //set values and return struct
    a.ptr=(void *) taille_start;
    a.kind=kind;
    a.size=taille;
    // printf("returning a.ptr:%p \n",a.ptr);
    // printf("returning a.kind:%d \n",a.kind);
    // printf("returning a.size:%lu \n",a.size);

    return a;
}


// void *mark_memarea_and_get_user_ptr(void *ptr, unsigned long size, MemKind k)
// {
//     uint64_t taille=(uint64_t)size;
//
//     uint64_t* ptr_taille1=ptr;
//     uint64_t* ptr_taille2=ptr+size-1;
//
//     uint64_t* ptr_magic1=ptr+1;
//     uint64_t* ptr_magic2=ptr+size-2;
//
//     *ptr_taille1=taille;
//     *ptr_taille2=taille;
//
//     uint64_t magic=(uint64_t)(knuth_mmix_one_round((unsigned long)ptr));
//
//     printf("Magic: %lu\n",(uint64_t)magic);
//
//     switch(k){
//         case SMALL_KIND:
//           magic=magic | 0b1;
//           magic=magic & ~(0b10);
//           break;
//
//         case MEDIUM_KIND:
//           magic=magic | 0b10;
//           magic=magic & ~(0b01);
//           break;
//
//         case LARGE_KIND:
//           magic= magic | 0b11;
//           break;
//     }
//
//     printf("Magic: %lu",magic);
//     getchar();
//
//
//     *ptr_magic1=magic;
//     *ptr_magic2=magic;
//
//     void* ptr_utilisateur=ptr+2;
//     return ptr_utilisateur;
// }
//
// Alloc
// mark_check_and_get_alloc(void *ptr)
// {
//     uint64_t* ptr_taille=ptr-2*sizeof(uint64_t);
//     uint64_t taille=*ptr_taille;
//
//     uint64_t* ptr_magic1=ptr-sizeof(uint64_t);
//     uint64_t* ptr_magic2=ptr+taille*sizeof(uint64_t);
//
//     uint64_t magic1=*ptr_magic1;
//     uint64_t magic2=*ptr_magic2;
//
//     assert(magic1==magic2);
//
//     uint64_t kind=magic1 & 0b11;
//
//     Alloc a;
//     a.ptr=ptr-2*sizeof(uint64_t);
//
//     if(kind==0b01){
//       a.kind=SMALL_KIND;
//     }
//     if(kind==0b10){
//       a.kind=MEDIUM_KIND;
//     }
//     if(kind==0b11){
//       a.kind=LARGE_KIND;
//     }
//
//     a.size=(unsigned long)taille;
//
//     return a;
// }


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
