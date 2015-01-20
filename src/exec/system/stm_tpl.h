#include "exec/helper.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"
#include "exec/template-start.h"

make_helper(concat(l_dt_,SUFFIX)) {
	ModR_M mm;
	mm.val = instr_fetch(eip+2,1);
	if(mm.reg == 2) {
		swaddr_t addr;
		int len = read_ModR_M(eip + 2,&addr);
		cpu.gdtr.limit = swaddr_read(addr, 2);
		addr += 2;
		cpu.gdtr.base = swaddr_read(addr,4);

		print_asm("lgdt" str(SUFFIX)" 0x%x",addr - 2);
		return len + 2;
	}
	else if(mm.reg == 3) {
		swaddr_t addr;
		int len = read_ModR_M(eip + 2,&addr);
		cpu.idtr.limit = swaddr_read(addr,2);
		addr += 2;
		cpu.idtr.base = swaddr_read(addr,4);

		print_asm("lidt" str(SUFFIX) " 0x%x",addr - 2);
		return len + 2;
	}
	else //inv
		return 0;
}

#include "exec/template-end.h"
