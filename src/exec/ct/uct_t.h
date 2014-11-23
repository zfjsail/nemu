#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(uct_nr_,SUFFIX)){
	ModR_M m;
	m.val = instr_fetch(eip+1,4);
	if(m.mod == 3){
	  cpu.eip = REG(m.R_M) - 2;
	  if(m.opcode == 2){
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
			swaddr_t addr;
			int len = read_ModR_M(eip + 1,&addr);
			cpu.eip = addr - len - 1;

			print_asm("call" " *%s",ModR_M_asm);
			return len + 1;
		}
		else return 0;//inv
	}
}

#include "exec/template-end.h"
