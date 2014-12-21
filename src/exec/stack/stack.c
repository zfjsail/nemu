#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(push_r_l){
	cpu.esp -= 4;
	ModR_M m;
	m.val = instr_fetch(eip,1);
//	assert(0);
	swaddr_write(cpu.esp,4,reg_l(m.R_M));
//	assert(0);
	print_asm("push" " %%%s",regsl[m.R_M]);
	return 1;
}

make_helper(pop_r_l){
	ModR_M m;
	m.val = instr_fetch(eip,1);
	reg_l(m.R_M) = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	print_asm("pop" " %%%s",regsl[m.R_M]);
	return 1;
}

make_helper(leave_l){
	cpu.esp = cpu.ebp;
	cpu.ebp = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	print_asm("leave");
	return 1;
}
