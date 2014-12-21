#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "common.h"

#define HW_MEM_SIZE (128 * 1024 * 1024)

extern uint8_t *hw_mem;

#define hwa_to_va(p) ((void *)(hw_mem + (unsigned)p))
#define va_to_hwa(p) ((hwaddr_t)((void *)p - (void *)hw_mem))

typedef union PageDirectoryEntry {
	struct {
		uint32_t present : 1;
		uint32_t read_write : 1;
		uint32_t user_supervisor : 1;
		uint32_t page_write_through : 1;
		uint32_t page_cache_disable : 1;
		uint32_t accessed : 1;
		uint32_t pad0 : 6;
		uint32_t page_frame : 20;
	};
	uint32_t val;
} PDE;

typedef union PageTableEntry {
	struct {
		uint32_t present : 1;
		uint32_t read_write : 1;
		uint32_t user_supervisor : 1;
		uint32_t page_write_through : 1;
		uint32_t page_cache_disable : 1;
		uint32_t accessed : 1;
		uint32_t dirty : 1;
		uint32_t pad0 : 1;
		uint32_t global : 1;
		uint32_t pad1 : 3;
		uint32_t page_frame : 20;
	};
	uint32_t val;
} PTE;

typedef union page_addr {
	union {
        struct {
	        uint32_t offset : 12;
	        uint32_t page : 10;
	        uint32_t dir : 10;
        };
		struct {
			uint32_t off : 12;
			uint32_t ptag : 20;
		};
	};
	uint32_t val;
} PG_addr;

uint32_t swaddr_read(swaddr_t, size_t);
void swaddr_write(swaddr_t, size_t, uint32_t);

lnaddr_t segment_translate(swaddr_t addr,uint8_t current_sreg);
hwaddr_t addr_trans(swaddr_t addr);

#endif
