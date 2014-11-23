#include "exec/helper.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "uct_t.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "uct_t.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "uct_t.h"
#undef DATA_BYTE

extern char suffix;

make_helper(jmp_b){
	uint8_t imm;
	imm = instr_fetch(eip+1,1);
	cpu.eip += imm;
	print_asm("jmp" " %x <main+0x%x>",cpu.eip+2,cpu.eip+2-0x100000);
	return 2;
}

make_helper(jmp_l){
	int imm;
	imm = instr_fetch(eip+1,4);
	cpu.eip += imm;
	print_asm("jmp" " %x <main+0x%x>",cpu.eip+5,cpu.eip+5-0x100000);
	return 5;
}

/*
make_helper(jmp_nr_l){
	ModR_M m;
	m.val = instr_fetch(eip+1,4);
	if(m.opcode == 4){
		cpu.eip = reg_l(m.R_M) - 2;
		print_asm("jmp" " *%%%s",regsl[m.R_M]);
		return 2;
	}
	else return 0;//inv
}
*/

make_helper(uct_nr_v){
	return (suffix == 'l' ? uct_nr_l(eip) : uct_nr_w(eip));
}

