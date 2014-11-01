#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(idiv_m_l){
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 1){
		int bx = reg_l(m.R_M);
		int disp8 = (char)instr_fetch(eip+2,1);
		int sec = swaddr_read(bx+disp8,4);
		long long fir = ((long long)cpu.edx << 32) + cpu.eax;
		temp = fir / sec;
		cpu.eax = temp;
		cpu.edx = fir % sec;
//		if(disp8 >= 0)
//			print_asm("idiv" " 0x%x(%%%s)",disp8,regsl[m.R_M]);
//		else
//			print_asm("idiv" " -0x%x(%%%s)",-disp8,regsl[m.R_M]);
		return 3;
	}
	else return 0;//inv
}
