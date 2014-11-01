#include "exec/helper.h"

make_helper(jmp_b){
	uint8_t imm;
	imm = instr_fetch(eip+1,1);
	cpu.eip += imm;
//	print_asm("jmp" " 0x%x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	return 2;
}
