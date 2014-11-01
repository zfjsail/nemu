#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(jmp_e_b){
	if(cpu.ZF){
	    int imm;
	    imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
//		print_asm("je" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	}
	return 2;
}

make_helper(jle_b){
	if(cpu.ZF || cpu.SF != cpu.OF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
//		print_asm("jle" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	}
	return 2;
}

make_helper(jl_b){
	if(cpu.SF != cpu.OF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
//		print_asm("jl" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	}
	return 2;
}

make_helper(jne_b){
	if(!cpu.ZF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
//		print_asm("jne" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	}
	return 2;
}

make_helper(op_plus_l){
	ModR_M m;
	m.val = instr_fetch(eip+1,1);
	if(m.val == 0x8e){
		if(cpu.ZF || cpu.SF != cpu.OF){
			int imm;
			imm = instr_fetch(eip+2,4);
			cpu.eip += imm;
//			print_asm("jle" " %x <main+0x%x>",cpu.eip+6,cpu.eip+6-0x100000);
		}
		return 6;
	}
	else if(m.val == 0xaf){
		ModR_M mm;
		mm.val = instr_fetch(eip+2,1);
		if(mm.mod == 3){
			long long fir = (int)reg_l(mm.reg);
			long long sec = (int)reg_l(mm.R_M);
			int temp = fir * sec;
			reg_l(mm.reg) = temp;
			if(fir * sec == temp){
				cpu.CF = 0;
				cpu.OF = 0;
			}
//			print_asm("imul" " %%%s,%%%s",regsl[mm.R_M],regsl[mm.reg]);
			return 3;
		}
		else return 0;//inv
	}
	else return 0;//inv
}

make_helper(jbe_b){
	if(cpu.CF || cpu.ZF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
//		print_asm("jbe" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	}
	return 2;
}
