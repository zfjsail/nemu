#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(call_i_l){
	int imm;
	imm = instr_fetch(eip+1,4);
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.eip+5);
	cpu.eip += imm;
	print_asm("call" " %x <...>",cpu.eip+5);
	return 5;
}

make_helper(ret){
	cpu.eip = swaddr_read(cpu.esp,4) - 1;
	cpu.esp += 4;
	print_asm("ret");
	return 1;
}
