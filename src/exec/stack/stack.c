#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(push_r_l){
	cpu.esp -= 4;
	ModR_M m;
	m.val = instr_fetch(eip,1);
//	assert(0);
    uint8_t op_last = instr_fetch(eip, 1) & 7;	
	if(op_last == 4)
		swaddr_write(cpu.esp, 4, cpu.esp + 4);
	else
	    swaddr_write(cpu.esp,4,reg_l(m.R_M));
//	if(m.val == 5) 
//		printf("ebp = 0x%x\n",cpu.ebp);
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

make_helper(push_i_b) {
	cpu.esp -= 4;
	uint8_t temp = instr_fetch(eip + 1, 1);
	swaddr_write(cpu.esp,1,temp);
	print_asm("push" " $0x%x",temp);
	return 2;
}

make_helper(push_i_l) {
	cpu.esp -= 4;
	uint32_t temp = instr_fetch(eip + 1, 4);
	swaddr_write(cpu.esp,4,temp);
	print_asm("push" " $0x%x",temp);
	return 5;
}

make_helper(pusha) {

	uint32_t temp = cpu.esp;
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.eax);
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.ecx);
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.edx);
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.ebx);
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,temp);
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.ebp);
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.esi);
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.edi);

	print_asm("pusha");
	return 1;
}

make_helper(popa) {

	reg_l(R_EDI) = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	reg_l(R_ESI) = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	reg_l(R_EBP) = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	swaddr_read(cpu.esp,4);//throw
	cpu.esp += 4;
	reg_l(R_EBX) = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	reg_l(R_EDX) = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	reg_l(R_ECX) = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	reg_l(R_EAX) = swaddr_read(cpu.esp,4);
	cpu.esp += 4;

	print_asm("popa");
	return 1;
}

make_helper(leave_l){
	cpu.esp = cpu.ebp;
	cpu.ebp = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	print_asm("leave");
	return 1;
}
