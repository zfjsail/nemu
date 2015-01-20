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

void raise_intr(uint8_t NO);

make_helper(l_dt_v) {
	int len = 0;
	if(suffix == 'w') {
		len = l_dt_w(eip);
		cpu.gdtr.base &= 0xffffff;
	}
	return (suffix == 'l' ? l_dt_l(eip) : len);
}
	
make_helper(ljmp) {
	if(cpu.CR0.PE) {//VM is doubtful...
	    cpu.eip = swaddr_read(eip + 1,4) - 7;
		cpu.sreg[1] = swaddr_read(eip + 5,2);
		print_asm("ljmp" " $0x%x,$0x%x",cpu.sreg[1],cpu.eip + 7);
		return 7;
	}
	else return 0;//inv
}

make_helper(cld) {
	cpu.DF = 0;
	print_asm("cld");
	return 1;
}

make_helper(cli) {
	cpu.IF = 0;
	print_asm("cli");
	return 1;
}

make_helper(intx) {

	/* push %eflags */
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.eflags);
    /* push %cs */
	cpu.esp -= 4;
	swaddr_write(cpu.esp,2,cpu.sreg[1]);
    /* push %eip */
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,cpu.eip);

	cpu.IF = 0;
	cpu.TF = 0;
	cpu.NT = 0;

	uint8_t NO = instr_fetch(eip + 1, 1);
	print_asm("int" " $0x%x",(unsigned)NO);
	raise_intr(NO);

	return 2;
}

make_helper(iret) {
	cpu.eip = swaddr_read(cpu.esp,4);
	cpu.esp += 4;
	cpu.sreg[1] = swaddr_read(cpu.esp,2);//CS
	cpu.esp += 4;
	cpu.eflags = swaddr_read(cpu.esp,4);
	cpu.esp += 4;

	print_asm("iret");
	return 2;//really ? int $0x80 len
}
