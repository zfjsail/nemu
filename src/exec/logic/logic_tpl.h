#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"

make_helper(concat(and_r2i_,SUFFIX)) {
	DATA_TYPE fir,sec;
	fir = REG(R_EAX);
	sec = MEM_R(eip + 1);
	DATA_TYPE temp;
	REG(R_EAX) = fir & sec;
	temp = REG(R_EAX);
	cpu.CF = 0;
	cpu.OF = 0;
	set_rF(temp);
	print_asm("and" " $0x%x,%%%s",sec,REG_NAME(R_EAX));
	return 5;
}

make_helper(concat(or_r2i_,SUFFIX)) {
	DATA_TYPE fir,sec;
	fir = REG(R_EAX);
	sec = MEM_R(eip + 1);
	DATA_TYPE temp;
	REG(R_EAX) = fir | sec;
	temp = REG(R_EAX);
	cpu.CF = 0;
	cpu.OF = 0;
	set_rF(temp);
	print_asm("or" " $0x%x,%%%s",sec,REG_NAME(R_EAX));
	return 5;
}

make_helper(concat(test_rm2r_,SUFFIX)) {
	uint8_t fir,sec;
	ModR_M m;
	uint8_t temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 3) {
		if(m.opcode == 0) {
		    fir = REG(m.R_M);
		    sec = instr_fetch(eip+2,1);
			temp = fir & sec;
			cpu.CF = 0;
			cpu.OF = 0;
			set_rF(temp);
			print_asm("testb" " 0x%x,%%%s",sec,REG_NAME(m.R_M));
			return 3;
		}
		else {
			assert(0);
			return 0;//inv
		}
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1,&addr);
		fir = swaddr_read(addr,1);
		sec = instr_fetch(eip + len + 1,1);
		if(m.opcode == 0) {
			temp = fir & sec;
			cpu.CF = 0;
			cpu.OF = 0;
			set_rF(temp);
			print_asm("testb" " 0x%x,%s",sec,ModR_M_asm);
			return len + 2;
		}
		else {
			assert(0);
			return 0;//inv
		}
	}
}

#include "exec/template-end.h"
