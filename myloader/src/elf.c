#include "trap.h"
#include <elf.h>

//void *my_memcpy(void *dest,const void *src,int n);
//void *my_bzero(void *s,int n);

void loader(){
	Elf32_Ehdr *elf = (void *)0;
	Elf32_Phdr *ph_start = (void *)(elf -> e_phoff),*ph = ph_start;
	int i;
	for(i = 0; i < elf -> e_phnum; i++){
		if(ph -> p_type == PT_LOAD){
	        int j;
	        for(j = 0; j < ph -> p_filesz; j++)
		        *((char *)(ph -> p_vaddr + j)) = *((char *)(ph -> p_offset + j));
			int k;
			for(k = 0; k < (ph -> p_memsz - ph -> p_filesz);k++)
				*((char *)(ph -> p_vaddr + ph -> p_filesz + k)) = (char)0;
		}
		ph = (void *)((unsigned)ph + elf -> e_phentsize);
	}
		
	((void(*)(void)) elf -> e_entry)();

	 HIT_GOOD_TRAP;
}

/*
void *my_memcpy(void *dest,const void *src,int n){
	int i;
	for(i = 0; i < n; i++)
		*((char *)(dest + i)) = *((char *)(src + i));
	return dest;
}

void *my_bzero(void *s,int n){
	int i;
	for(i = 0; i < n; i++)
		*((char *)(s + i)) = (char)0;
	return s;
}
*/
