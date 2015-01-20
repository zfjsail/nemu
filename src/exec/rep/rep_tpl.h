#include "exec/helper.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"
#include "exec/template-start.h"

void lnaddr_write(lnaddr_t addr,size_t len,uint32_t data);

make_helper(concat(rep_,SUFFIX)) {
	uint8_t op;
	op = instr_fetch(eip + 1, 1);
	if(op == 0xa4) {//rep mov 8
		int i = REG(R_ECX);
		while(i != 0) {
			lnaddr_t srcaddr = seg_cache[3].base + REG(R_ESI);//DS
			uint8_t value = swaddr_read(srcaddr,1);
			lnaddr_t desaddr = seg_cache[0].base + REG(R_EDI);//ES
			swaddr_write(desaddr,1,value);
			if(cpu.DF) {
				REG(R_ESI)--;
				REG(R_EDI)--;
			}
			else {
				REG(R_ESI)++;
				REG(R_EDI)++;
			}
			REG(R_ECX)--;
			i = REG(R_ECX);
		}

		print_asm("rep movsb" " %%%s:(%%%s),%%%s:(%%%s)",regseg[3],REG_NAME(R_ESI),regseg[0],REG_NAME(R_EDI));
		return 2;
	}
	else if(op == 0xa5) {//rep mov
		int i = REG(R_ECX);
		while(i != 0) {
			lnaddr_t srcaddr = seg_cache[3].base + REG(R_ESI);//DS
			DATA_TYPE value = MEM_R(srcaddr);
			lnaddr_t desaddr = seg_cache[0].base + REG(R_EDI);//ES
			MEM_W(desaddr,value);
			int off = sizeof(REG(R_EAX));
			if(cpu.DF) {
				REG(R_ESI) -= off;
				REG(R_EDI) -= off;
			}
			else {
				REG(R_ESI) += off;
				REG(R_EDI) += off;
			}
			REG(R_ECX)--;
			i = REG(R_ECX);
		}

		print_asm("rep movs" str(SUFFIX) " %%%s:(%%%s),%%%s:(%%%s)",regseg[3],REG_NAME(R_ESI),regseg[0],REG_NAME(R_EDI));
		return 2;
	}
	else if(op == 0xaa) {//rep stos 8
		int i = REG(R_ECX);
		while(i != 0) {
			hwaddr_t desaddr = seg_cache[0].base + REG(R_EDI);//ES
			MEM_W(desaddr,reg_b(R_EAX));
			if(cpu.DF) REG(R_EDI)--;
			else REG(R_EDI)++;
			REG(R_ECX)--;
			i = REG(R_ECX);
		}
		print_asm("rep stos" " %%%s,%%%s:(%%%s)",regsb[R_EAX],regseg[0],REG_NAME(R_EDI));
		return 2;
	}
	else if(op == 0xab) {//rep stos 16 32
		int i = REG(R_ECX);
		while(i != 0) {
			lnaddr_t desaddr = seg_cache[0].base + REG(R_EDI);//ES
			lnaddr_write(desaddr,sizeof(REG(R_EAX)),REG(R_EAX));
			int off = sizeof(REG(R_EAX));
			if(cpu.DF) REG(R_EDI) -= off;
			else REG(R_EDI) += off;
			REG(R_ECX)--;
			i = REG(R_ECX);
		}

		print_asm("rep stos" " %%%s,%%%s:(%%%s)",REG_NAME(R_EAX),regseg[0],REG_NAME(R_EDI));
		return 2;
	}
	else {assert(0); return 0;}//inv
}

#include "exec/template-end.h"
