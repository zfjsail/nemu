#include "exec/helper.h"

make_helper(push_bp_l){
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.ebp);
//	print_asm("push" " %%ebp");
	return 1;
}

make_helper(push_bx_l){
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.ebx);
//	print_asm("push" " %%ebx");
	return 1;
}
