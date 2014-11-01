#include "exec/helper.h"

make_helper(jmp_e_b){
	if(cpu.ZF){
	    int imm;
	    imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
//		print_asm("je" " 0x%x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	}
	return 2;
}

make_helper(jle_b){
	if(cpu.ZF || cpu.SF != cpu.OF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
//		print_asm("jle" " 0x%x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	}
	return 2;
}

make_helper(jl_b){
	if(cpu.SF != cpu.OF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
//		print_asm("jl" " 0x%x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	}
	return 2;
}

make_helper(jne_b){
	if(!cpu.ZF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
//		print_asm("jne" " 0x%x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	}
	return 2;
}
