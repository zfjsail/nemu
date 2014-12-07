#include "exec/helper.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "stm_tpl.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "stm_tpl.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "stm_tpl.h"
#undef DATA_BYTE

extern char suffix;

make_helper(lgdt_v) {
	int len = 0;
	if(suffix == 'w') {
		len = lgdt_w(eip);
		cpu.gdtr.base &= 0xffffff;
	}
	return (suffix == 'l' ? lgdt_l(eip) : len);
}
	
make_helper(ljmp) {
	if(cpu.CR0.PE) {//VM is doubtful...
	    cpu.eip = swaddr_read(eip + 1,4) - 7;
		cpu.CS = swaddr_read(eip + 5,2);
		print_asm("ljmp" " $0x%x,$0x%x",cpu.CS,cpu.eip + 7);
		return 7;
	}
	else return 0;//inv
}
