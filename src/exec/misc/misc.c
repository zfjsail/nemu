#include "exec/helper.h"
#include "cpu/modrm.h"

#include "ui/ui.h"

#include "nemu.h"

make_helper(inv) {
	/* invalid opcode */

	uint8_t temp[8];
	((uint32_t *)temp)[0] = instr_fetch(eip, 4);
	((uint32_t *)temp)[1] = instr_fetch(eip + 4, 4);
	printf("invalid opcode(eip = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n", 
			eip, temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);

	assert(0);
}

bool isBreak = false;//触发断点的标志

make_helper(int3) {
	/* A breakpoint is hit! Do something here! */
	isBreak = true;//即将触发断点！
	printf("A breakpoint is hit!\n");

	return 1;//指令长度噢
}

make_helper(nemu_trap) {
	printf("nemu: HIT \33[1;31m%s\33[0m TRAP at eip = 0x%08x\n\n", (cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip);
	nemu_state = END;

	print_asm("nemu trap");
	return 1;
}

make_helper(lea_m2r_l){
	ModR_M m;
	m.val = instr_fetch(eip+1,1);
	int bx = reg_l(m.R_M);
	if(m.R_M != 4){
		if(m.mod == 1){
			int disp8 = (char)instr_fetch(eip+2,1);
		    reg_l(m.reg) = bx + disp8;
		    if(disp8 >= 0)
				print_asm("lea" " 0x%x(%%%s),%%%s",disp8,regsl[m.R_M],regsl[m.reg]);
		    else
			    print_asm("lea" " -0x%x(%%%s),%%%s",-disp8,regsl[m.R_M],regsl[m.reg]);
		    return 3;
	    }
	    else if(m.mod == 2){
		    int disp32 = instr_fetch(eip+2,4);
		    reg_l(m.reg) = bx + disp32;
		    if(disp32 >= 0)
			    print_asm("lea" " 0x%x(%%%s),%%%s",disp32,regsl[m.R_M],regsl[m.reg]);
		    else
			    print_asm("lea" " -0x%x(%%%s),%%%s",-disp32,regsl[m.R_M],regsl[m.reg]);
		    return 6;
	    }    
	    else return 0;//inv
	}
	else{
		ModR_M mm;
		int ss = 0;
		mm.val = instr_fetch(eip+2,1);
		if(m.mod == 0 && mm.R_M == 5){
			int bx = 0;
			int disp32= instr_fetch(eip+3,4);
	        int index = mm.reg;
			int add_temp;
			if(index == 4)
				add_temp = 0;
			else{
				switch(mm.mod){
					case 0: ss = 1;break;
					case 1: ss = 2;break;
					case 2: ss = 4;break;
					case 3: ss = 8;break;
				}
				add_temp = index * ss;
			}
			int addr = disp32 + bx + add_temp;
			reg_l(m.reg) = addr;
			if(disp32 >= 0)
				print_asm("lea" " 0x%x(,%%%s,%d),%%%s",disp32,regsl[mm.reg],ss,regsl[m.reg]);
			else
				print_asm("lea" " -0x%x(,%%%s,%d),%%%s",-disp32,regsl[mm.reg],ss,regsl[m.reg]);
			return 7;
		}
		else return 0;//inv
	}
}

make_helper(nop){
	return 1;
}
