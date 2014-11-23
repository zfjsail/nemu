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
	        int i;
	        for(i = 0; i < ph -> p_filesz; i += 4)
				*((int *)(ph -> p_vaddr + i)) = *((int *)(ph_start + ph -> p_offset + i));
//		        *((char *)(ph -> p_vaddr + i)) = *((char *)(ph_start + ph -> p_offset + i));
//			for(j = 0; j < ph -> p_filesz;j += 4){
//				int temp = swaddr_read(ph_start + ph -> p_offset + j,4);
//				swaddr_write(ph -> p_vaddr + j,4,temp);
//			my_memcpy((void *)(ph -> p_vaddr),ph_start + ph -> p_offset,ph -> p_filesz);
//			for(j = 0; j < ph -> p_filesz - ph -> p_memsz; j += 4)
//				swaddr_write(ph -> p_vaddr + ph -> filesz,4,0);
//			memset((void *)(ph -> p_vaddr + ph -> p_filesz),0,ph -> p_filesz - ph -> p_memsz);
//			my_bzero((void *)(ph -> p_vaddr + ph -> p_filesz),ph -> p_filesz - ph -> p_memsz);
			for(i = 0; i < (ph -> p_filesz - ph -> p_memsz);i += 4)
//				*((char *)(ph -> p_vaddr + ph -> p_filesz + i)) = (char)0;
				*((int *)(ph -> p_vaddr + ph -> p_filesz + i)) = (int)0;
		}
		ph += ph -> p_filesz;
	}

    elf -> e_entry = 0x800000;
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
