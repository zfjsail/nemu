#include "common.h"
#include "cpu/reg.h"
#include "memory.h"
#include <setjmp.h>

extern jmp_buf jbuf;

uint32_t dram_read(hwaddr_t addr, size_t len);
void dram_write(hwaddr_t addr, size_t len, uint32_t data);
void write_through(hwaddr_t addr,size_t len,uint32_t data);
uint32_t cache1_read(hwaddr_t addr,size_t len);
int TLB_read(uint32_t vp);
void TLB_write(uint32_t vp,uint32_t pp);

//extern struct SG_cache seg_cache[4];
//extern uint8_t current_sreg;

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return cache1_read(addr, len);
//	return dram_read(addr,len);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	write_through(addr, len, data);
//  dram_write(addr,len,data);
}

hwaddr_t page_translate(lnaddr_t addr) {
	PG_addr pgaddr;
	pgaddr.val = addr;
	int pf = TLB_read(pgaddr.ptag);
	if(pf < 0) {
	    hwaddr_t dir = cpu.CR3.page_directory_base << 12;
	    PDE *kpdir = (void *)dir;
	    PDE *pfir = (void *)((uint32_t)kpdir + 4 * pgaddr.dir);// *4?
	    PDE fir;
	    fir.val = dram_read((uint32_t)pfir,4);
	    if(!fir.present) {
			assert(0);
			printf("eip = 0x%x\n",cpu.eip);
		}
	    PTE *psec = (void *)((fir.page_frame << 12) + 4 * pgaddr.page);// *4?
	    PTE sec;
	    sec.val = dram_read((uint32_t)psec,4);
//	    printf("%x %x\n",(uint32_t)psec,sec.val);
	    if(!sec.present) assert(0);
	    pf = sec.page_frame;
		TLB_write(pgaddr.ptag,pf);
	}
	return (pf << 12) + pgaddr.offset;
}

uint32_t lnaddr_read(lnaddr_t addr,size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	hwaddr_t hwaddr;
	if(cpu.CR0.PE && cpu.CR0.PG) {
    	if((addr >> 12) != (addr + len - 1) >> 12) {//across boundary
			hwaddr = page_translate(addr);
			hwaddr_t cross = ((addr >> 12) + 1) << 12;
			hwaddr_t hwaddr_2 = page_translate(cross);
			uint32_t len_1 = ((~hwaddr) & 3) + 1;
			uint32_t len_2 = len - len_1;
			uint32_t d,u; //down & up
			if(len_1 == 3) {
				d = hwaddr_read(hwaddr - 1,4) >> 8;
				u = hwaddr_read(hwaddr_2,1);
				return ((u << 24) + d);
			}
			if(len_2 == 3) {
				d = hwaddr_read(hwaddr,1);
				u = hwaddr_read(hwaddr_2,4) & 0xffffff;
				return ((u << 8) + d);
			}
			d = hwaddr_read(hwaddr,len_1);
			u = hwaddr_read(hwaddr_2,len_2);
			return ((u << (len_1 * 8)) + d);
//	    	assert(0);
    	} 
	    else
		    hwaddr = page_translate(addr);
	}
	else hwaddr = addr;
    return hwaddr_read(hwaddr,len);
}

void lnaddr_write(lnaddr_t addr,size_t len,uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	hwaddr_t hwaddr;
	if(cpu.CR0.PE && cpu.CR0.PG) {
	    if((addr >> 12) != (addr + len - 1) >> 12) {//across boundary
		    assert(0);
	    }
	    else 
		    hwaddr = page_translate(addr);
	}
	else hwaddr = addr;
	return hwaddr_write(hwaddr,len,data);
}

lnaddr_t segment_translate(swaddr_t addr,uint8_t current_sreg) {
//	uint32_t off = cpu.gdtr.base + 8 * cpu.sr[current_sreg].index;
	return addr + seg_cache[current_sreg].base;
}

uint32_t swaddr_read(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	if(addr > 0x40000000) current_sreg = 2;
	else current_sreg = 3;
	lnaddr_t lnaddr;
	if(cpu.CR0.PE) lnaddr = segment_translate(addr,current_sreg);
	else lnaddr = addr;
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data) {
	assert(len == 1 || len == 2 || len == 4);
	if(addr > 0x40000000) current_sreg = 2;
	else current_sreg = 3;
	lnaddr_t lnaddr;
	if(cpu.CR0.PE) lnaddr = segment_translate(addr,current_sreg);
	else lnaddr = addr;
	lnaddr_write(lnaddr, len, data);
}

/*
static uint32_t hwaddr_read_instr(hwaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return cache1_read(addr, len);
}
*/

uint32_t instr_fetch(swaddr_t addr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
//	return hwaddr_read_instr(addr, len);
    lnaddr_t lnaddr;
	if(cpu.CR0.PE) lnaddr = segment_translate(addr,current_sreg);
	else lnaddr = addr;
	return lnaddr_read(lnaddr,len);
}

hwaddr_t addr_trans(swaddr_t addr) {
	lnaddr_t lnaddr;
	if(cpu.CR0.PE) lnaddr = segment_translate(addr,current_sreg);
	else lnaddr = addr;
	hwaddr_t hwaddr;
	if(cpu.CR0.PE && cpu.CR0.PG) 
		hwaddr = page_translate(lnaddr);
	else hwaddr = lnaddr;
	return hwaddr;
}

void raise_intr(uint8_t NO) {
	uint32_t in_idt = cpu.idtr.base;
	in_idt += 8 * NO;
	uint16_t segment = lnaddr_read(in_idt + 2,4);
	cpu.sreg[1] = segment;//CS
	uint32_t in_gdt = cpu.gdtr.base + (cpu.sr[1].INDEX << 3);
	uint32_t up = lnaddr_read(in_gdt + 4, 4);
	uint32_t low = lnaddr_read(in_gdt,4);
	uint32_t base = (up & 0xff000000) + ((up & 0xff) << 16) + (low >> 16);
	seg_cache[1].base = base;
	seg_cache[1].limit = 0xffffffff;
	uint16_t off_15_0 = lnaddr_read(in_idt,2);
	uint16_t off_31_16 = lnaddr_read(in_idt + 6, 2);
	uint32_t off = (off_31_16 << 16) + off_15_0;
	cpu.eip = base + off;
	
	longjmp(jbuf,1);
}
