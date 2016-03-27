#include "common.h"
#include "x86.h"
#include "memory.h"
#include <string.h>

#include <elf.h>

#ifdef HAS_DEVICE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

//static PTE uptable[PHY_MEM / PAGE_SIZE];

uint32_t loader() {
	Elf32_Ehdr *elf = (void *)0;;
	Elf32_Phdr *ph = (void *)(elf -> e_phoff);

#ifdef HAS_DEVICE
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
	elf = (void*)buf;
#else
	/* The ELF file is located at memory address 0 */
	elf = (void *)0x0;
#endif

	/* Load each program segment */
//	nemu_assert(0);
	int i;
//	uint32_t vaddr;
	for(i = 0; i < elf -> e_phnum; i++) {
		/* Scan the program header table, load each segment into memory */
		if(ph->p_type == PT_LOAD) {
			int j;
//			vaddr = ph -> p_vaddr - 0xc0100000 + 0x8048000;
//			uint32_t hwaddr = mm_malloc(vaddr,ph -> p_memsz);
			uint32_t hwaddr = mm_malloc(ph -> p_vaddr, ph -> p_memsz);

			for(j = 0; j < ph -> p_filesz; j++) 
				*((char *)(hwaddr + j)) = *((char *)(ph -> p_offset + j));

			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			 
			 
			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */
			int k;
			for(k = ph -> p_filesz; k < ph -> p_memsz; k++)
				*((char *)(hwaddr + k)) = (char)0;

			/* Record the prgram break for future use. */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
		}
		ph = (void *)((unsigned)ph + elf -> e_phentsize);
	}

	volatile uint32_t entry = elf->e_entry;
//	((void(*)(void)) entry)();

//	HIT_GOOD_TRAP;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif

	return entry;
}
