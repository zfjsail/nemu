#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(jmp_e_b){
	if(cpu.ZF){
	    int imm;
	    imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
	}
	print_asm("je" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	return 2;
}

make_helper(jle_b){
	if(cpu.ZF || cpu.SF != cpu.OF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
	}
	print_asm("jle" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	return 2;
}

make_helper(jl_b){
	if(cpu.SF != cpu.OF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
	}
	print_asm("jl" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	return 2;
}

make_helper(jne_b){
	if(!cpu.ZF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
	}
	print_asm("jne" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	return 2;
}

make_helper(jbe_b){
	if(cpu.CF || cpu.ZF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
	}
	print_asm("jbe" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	return 2;
}

make_helper(jge_b){
	if(cpu.SF == cpu.OF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
	}
	print_asm("jge" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	return 2;
}

make_helper(jg_b){
	if(!cpu.ZF && cpu.SF == cpu.OF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
	}
	print_asm("jg" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	return 2;
}

make_helper(ja_b){
	if(!cpu.CF && !cpu.ZF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
	}
	print_asm("ja" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	return 2;
}

make_helper(jb_b){
	if(cpu.CF){
		int imm;
		imm = (char)instr_fetch(eip+1,1);
		cpu.eip += imm;
	}
	print_asm("jb" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	return 2;
}

make_helper(jae_b) {
	int imm;
	imm = (char)instr_fetch(eip+1,1);
	int temp = cpu.eip + imm;
	if(!cpu.CF) 
		cpu.eip += imm;
	print_asm("jae" " %x ",temp + 2);
	return 2;
}

make_helper(js_b) {
	int imm = (char)instr_fetch(eip + 1, 1);
	int temp = cpu.eip + imm;
	if(cpu.SF)
		cpu.eip = temp;
	print_asm("js" " %x",temp + 2);
	return 2;
}

make_helper(jle_l){
	if(cpu.ZF || cpu.SF != cpu.OF){
		int imm;
		imm = instr_fetch(eip+2,4);
		cpu.eip += imm;
	}
	print_asm("jle" " %x <main+0x%x>",cpu.eip+6,cpu.eip+6-0x100000);
	return 6;
}

make_helper(jbe_l){
	if(cpu.CF || cpu.ZF){
		int imm;
		imm = instr_fetch(eip+2,4);
		cpu.eip += imm;
	}
	print_asm("jbe" " %x <main+0x%x>",cpu.eip+6,cpu.eip+6-0x100000);
	return 6;
}

make_helper(jl_l){
	if(cpu.SF != cpu.OF){
		int imm;
		imm = instr_fetch(eip+2,4);
		cpu.eip += imm;
	}
	print_asm("jl" " %x <main+0x%x>",cpu.eip+6,cpu.eip+6-0x100000);
	return 6;
}

make_helper(je_l){
	if(cpu.ZF){
		int imm;
		imm = instr_fetch(eip+2,4);
		cpu.eip += imm;
	}
	print_asm("je" " %x <main+0x%x>",cpu.eip+6,cpu.eip+6-0x100000);
	return 6;
}

make_helper(jg_l) {
	int imm;
	imm = instr_fetch(eip+2,4);
	int temp = cpu.eip + imm;
	if(!cpu.ZF && cpu.SF == cpu.OF) {
		cpu.eip += imm;
	}
	print_asm("jg" " %x ",temp + 6);
	return 6;
}

make_helper(js_l) {
	int imm;
	imm = instr_fetch(eip + 2, 4);
	int temp = cpu.eip + imm;
	if(cpu.SF)
		cpu.eip += imm;
	print_asm("js" " %x ",temp + 6);
	return 6;
}

make_helper(jne_l) {
	int imm;
	imm = instr_fetch(eip + 2, 4);
	int temp = cpu.eip + imm;
	if(!cpu.ZF)
		cpu.eip = temp;
	print_asm("jne" " %x", temp + 6);
	return 6;
}

make_helper(ja_l) {
	int imm = instr_fetch(eip + 2, 4);
	int temp = cpu.eip + imm;
	if(!cpu.CF && !cpu.ZF)
		cpu.eip = temp;
	print_asm("je" " %x", temp + 6);
	return 6;
}
