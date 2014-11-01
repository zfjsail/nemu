#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(or_r_l){
	ModR_M m;
	unsigned temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 3){
		temp = reg_l(m.reg) | reg_l(m.R_M);
		reg_l(m.R_M) = temp;
		cpu.OF = 0;
		cpu.CF = 0;
		set_rF(temp);
		print_asm("or" " %%%s,%%%s",regsl[m.reg],regsl[m.R_M]);
		return 2;
	}
	else return 0;//inv
}
