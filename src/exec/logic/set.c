#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(setne){
	ModR_M mm;
	mm.val = instr_fetch(eip+2,1);
	if(mm.mod == 3){
		if(!cpu.ZF)
			reg_b(mm.R_M) = 1;
		else
			reg_b(mm.R_M) = 0;
		print_asm("setne" " %%%s",regsb[mm.R_M]);
		return 3;
	}
	else return 0;//inv
}			
