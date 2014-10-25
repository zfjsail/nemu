#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(cmp_imm8_l){
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	if(m.opcode == 7){
		if(m.mod == 3){
			int fir = reg_l(m.R_M);
			int sec = instr_fetch(eip+1+1,1);
		    temp = fir - sec;
			set_OF(sec,fir,0);
			set_SF(temp);
			set_ZF(temp);
			set_AF(sec,fir,0);
			set_PF(temp);
			set_CF(sec,fir,0);
			print_asm("cmp" " $0x%x,%%%s",sec,regsl[m.R_M]);
			return 1 + 1 + 1;
		}
		else
			return 0; //inv
	}
	else return 0; //inv
}
			
