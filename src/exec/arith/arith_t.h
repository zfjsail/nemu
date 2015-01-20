#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"

make_helper(concat(op_no2rm_,SUFFIX)) {
	ModR_M m;
	DATA_TYPE temp, fir, sec;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		if(m.opcode == 0) {//test
			fir = REG(m.R_M);
			sec = MEM_R(eip + 2);
			temp = fir & sec;
			cpu.CF = 0;
			cpu.OF = 0;
			set_rF(temp);
			print_asm("test" " $0x%x,%%%s", sec, REG_NAME(m.R_M));
			return 2 + sizeof(DATA_TYPE);
		}
		else if(m.opcode == 2) {//not
			REG(m.R_M) = ~ REG(m.R_M);
			print_asm("not" " %%%s",REG_NAME(m.R_M));
			return 2;
		}
		else if(m.opcode == 3) {//neg
			if(REG(m.R_M)) cpu.CF = 1;
			else cpu.CF = 0;
			REG(m.R_M) = - REG(m.R_M);
			set_rF(REG(m.R_M));
			set_OF(-REG(m.R_M),0,0);
			print_asm("neg" " %%%s",REG_NAME(m.R_M));
			return 2;
		}
		else if(m.opcode == 5) {//imul
			if(sizeof(DATA_TYPE) == 4) {
				fir = REG(m.R_M);
				sec = REG(R_EAX);
				long long temp1 = (int)fir;
				long long temp2 = (int)sec;
				long long r = temp1 * temp2;
				REG(R_EAX) = r;
				REG(R_EDX) = r >> 32;
				uint32_t d = REG(R_EDX);
				if(d == 0 || d == 0xffffffff) {
					cpu.CF = 0;
					cpu.OF = 0;
				}
				print_asm("imul" " %%%s",REG_NAME(m.R_M));
				return 2;
			}
			else return 0;//inv
		}
		else return 0;//inv
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		if(m.opcode == 0) {//test
			sec = MEM_R(addr);
			fir = MEM_R(eip + len + 1);
			temp = fir & sec;
			cpu.CF = 0;
			cpu.OF = 0;
			set_rF(temp);
			print_asm("test" " $0x%x,%s", fir, ModR_M_asm);
			return 1 + len + sizeof(DATA_TYPE);
		}
		else if(m.opcode == 7) {//idiv
			sec = MEM_R(addr);
			if(sizeof(DATA_TYPE) == 4) {
				long long temp1 = ((long long)REG(R_EDX) << 32) + REG(R_EAX);
				temp = temp1 / sec;
				REG(R_EAX) = temp;
				REG(R_EDX) = temp1 % sec;
				print_asm("idiv" " %s",ModR_M_asm);
				return 1 + len;
			}
			else return 0;//inv
		}
		else return 0;//inv
	}
}

#include "exec/template-end.h"
