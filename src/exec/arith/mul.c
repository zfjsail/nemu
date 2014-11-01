#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(imul_ibr2r_l){
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 3){
		long long fir = (int)reg_l(m.R_M);
		long long sec = (char)instr_fetch(eip+2,1);
		reg_l(m.reg) = fir * sec;
		temp = fir * sec;
		if(fir * sec == temp){
			cpu.CF = 0;
			cpu.OF = 0;
		}
		print_asm("imul" " $0x%x,%%%s,%%%s",(char)sec,regsl[m.R_M],regsl[m.reg]);
		return 3;
	}
	else return 0;//inv
}

make_helper(imul_r2r_l){
    ModR_M mm;
    mm.val = instr_fetch(eip+2,1);
    if(mm.mod == 3){
	    long long fir = (int)reg_l(mm.reg);
	    long long sec = (int)reg_l(mm.R_M);
	    int temp = fir * sec;
	    reg_l(mm.reg) = temp;
	    if(fir * sec == temp){
		    cpu.CF = 0;
		    cpu.OF = 0;
		}
	    print_asm("imul" " %%%s,%%%s",regsl[mm.R_M],regsl[mm.reg]);
	    return 3;
	}
    else return 0;//inv
}	
