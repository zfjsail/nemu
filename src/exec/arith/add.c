#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(add_r2m_l){
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	int add_2 = reg_l(m.reg);
	if(m.mod == 1){
		int disp8 = (char)instr_fetch(eip+2,1);
		int R_M = reg_l(m.R_M);
		int add_1 = swaddr_read(R_M+disp8,4);
		temp = add_1 + add_2;
		swaddr_write(R_M+disp8,4,temp);
		set_6F(add_2,add_1,temp,1);
		if(disp8 >= 0)
			print_asm("add" " %%%s,0x%x(%%%s)",regsl[m.reg],disp8,regsl[m.R_M]);
		else
			print_asm("add" " %%%s,-0x%x(%%%s)",regsl[m.reg],-disp8,regsl[m.R_M]);
		return 3;
	}
	else return 0;//inv
}
