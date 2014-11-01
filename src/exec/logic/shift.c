#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(sft_rl_b){
	ModR_M m;
	m.val = instr_fetch(eip+1,1);
	int R_M = reg_l(m.R_M);
	uint8_t count = instr_fetch(eip+2,1);
	uint8_t temp = count;
	if(m.mod == 3){
		if(m.opcode == 7){
			while(count != 0){
				cpu.CF = R_M & 1;
				R_M >>= 1;
				count--;
			}
			reg_l(m.R_M) = R_M;
			set_rF(reg_l(m.R_M));
			print_asm("sar" " $0x%x,%%%s",temp,regsl[m.R_M]);
			return 3;
		}
		else if(m.opcode == 4){
			while(count != 0){
				cpu.CF = reg_l(m.R_M) >> 31;
				reg_l(m.R_M) <<= 1;
				count--;
			}
			set_rF(reg_l(m.R_M));
			print_asm("shl" " $0x%x,%%%s",temp,regsl[m.R_M]);
			return 3;
		}
		else return 0;//inv
	}
	return 0;//inv
}
