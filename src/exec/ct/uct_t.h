#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "memory.h"

make_helper(concat(uct_nr_,SUFFIX)){
	ModR_M m;
	m.val = instr_fetch(eip+1,1);//I think
	if(m.mod == 3){//call *
	  swaddr_t eip_temp = cpu.eip;
//	  cpu.eip = MEM_R(REG(m.R_M)) - 2;
	  cpu.eip = addr_trans(REG(m.R_M)) - 2;
	  cpu.eip = REG(m.R_M) - 2;
	  if(m.opcode == 2){
		  cpu.esp -= 4;
		  MEM_W(cpu.esp,eip_temp + 2);//I think
		  print_asm("call" " *%%%s",REG_NAME(m.R_M));
		  return 2;
	  }
	  else if(m.opcode == 4){
		  print_asm("jmp" " *%%%s",REG_NAME(m.R_M));
		  return 2;
	  }
	  else return 0;//inv
	}
	else{
		if(m.opcode == 2){
			cpu.esp -= 4;
			swaddr_t addr;
			int len = read_ModR_M(eip + 1,&addr);
			MEM_W(cpu.esp,cpu.eip + len + 1);//I think
			addr = MEM_R(addr);
//			addr = MEM_R(addr + 0x800000);
//			cpu.eip = MEM_R(addr) - len - 1;
			cpu.eip = addr - len - 1;

			print_asm("call" " *%s",ModR_M_asm);
			return len + 1;
		}
		else return 0;//inv
	}
}

#include "exec/template-end.h"
