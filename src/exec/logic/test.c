#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(test_r_l){
	ModR_M m;
	unsigned temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 3){
		temp = reg_l(m.R_M) & reg_l(m.reg);
		cpu.CF = 0;
		cpu.OF = 0;
	
	    set_SF(temp);
	    set_ZF(temp);	
		set_PF(temp & 0xff);
//		print_asm("test" " %%%s,%%%s",regsl[m.reg],regsl[m.R_M]);
		return 2;
	}
	else
		return 0; //inv
}

