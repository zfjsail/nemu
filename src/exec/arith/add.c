#include "exec/helper.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "add_t.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "add_t.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "add_t.h"
#undef DATA_BYTE

extern char suffix;

make_helper(add_rm2m_v){
	return (suffix == 'l' ? add_rm2m_l(eip) : add_rm2m_l(eip));
}
/*
make_helper(add_rm2m_l){
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
	else if(m.mod == 3){
		int add_1 = reg_l(m.R_M);
		reg_l(m.R_M) = add_1 + add_2;
		set_6F(add_2,add_1,reg_l(m.R_M),1);
		print_asm("add" " %%%s,%%%s",regsl[m.reg],regsl[m.R_M]);
		return 2;
	}
	else return 0;//inv
}
*/
make_helper(add_i2a_l){
	int add_2 = instr_fetch(eip+1,4);
	int add_1 = cpu.eax;
	int temp = add_1 + add_2;
	cpu.eax = temp;
	set_6F(add_2,add_1,temp,1);
	print_asm("add" " $0x%x,%%eax",add_2);
	return 5;
}

make_helper(adc_r2r_l){
	ModR_M m;
	m.val = instr_fetch(eip+1,1);
	int add_3 = 0;
	if(cpu.CF == -1)
		add_3 = 1;
	int add_2 = reg_l(m.reg) + add_3;
	if(m.mod == 3){
		int add_1 = reg_l(m.R_M);
		reg_l(m.R_M) = add_1 + add_2;
		set_6F(add_2,add_1,reg_l(m.R_M),1);
		print_asm("adc" " %%%s,%%%s",regsl[m.reg],regsl[m.R_M]);
		return 2;
	}
	else return 0;//inv
}

make_helper(add_rm2r_v){
	return (suffix == 'l' ? add_rm2r_l(eip) : add_rm2r_w(eip));
}

make_helper(inc_r_v){
	return (suffix == 'l' ? inc_r_l(eip) : inc_r_w(eip));
}
