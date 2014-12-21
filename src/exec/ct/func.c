#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(call_i_l){
	int imm;
	imm = swaddr_read(eip+1,4);
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

make_helper(ret_iw) {//32
	uint16_t data_byte = instr_fetch(eip + 1, 2);
	cpu.eip = swaddr_read(cpu.esp,data_byte) - 3;
	cpu.esp += data_byte;
	print_asm("ret" " 0x%x",data_byte);
	return 3;
}
