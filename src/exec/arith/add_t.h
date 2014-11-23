#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"

make_helper(concat(add_rm2m_,SUFFIX)){
	ModR_M m;
	DATA_TYPE temp;
	DATA_TYPE add_1,add_2;
	m.val = instr_fetch(eip+1,1);
	add_2 = REG(m.reg);
	if(m.mod == 3){
		add_1 = REG(m.R_M);
		REG(m.R_M) = add_1 + add_2;
		set_6F(add_2,add_1,REG(m.R_M),1);
		print_asm("add" " %%%s,%%%s",REG_NAME(m.reg),REG_NAME(m.R_M));
		return 2;
	}
	else{
		swaddr_t addr;
		int len = read_ModR_M(eip + 1,&addr);
		add_1 = MEM_R(addr);
		temp = add_1 + add_2;
		MEM_W(addr,temp);
		set_6F(add_2,add_1,temp,1);

		print_asm("add" str(SUFFIX) " %%%s,%s",REG_NAME(m.reg),ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(add_rm2r_,SUFFIX)){
	ModR_M m;
	DATA_TYPE temp;
	DATA_TYPE add_1,add_2;
	m.val = instr_fetch(eip+1,1);
	add_1 = REG(m.reg);
	if(m.mod == 3){
		add_2 = REG(m.R_M);
		REG(m.reg) = add_1 + add_2;
		set_6F(add_2,add_1,REG(m.reg),1);
		print_asm("add" " %%%s,%%%s",REG_NAME(m.R_M),REG_NAME(m.reg));
		return 2;
	}
	else{
		swaddr_t addr;
		int len = read_ModR_M(eip + 1,&addr);
		add_2 = MEM_R(addr);
		temp = add_1 + add_2;
		REG(m.reg) = temp;
		set_6F(add_2,add_1,temp,1);

		print_asm("add" str(SUFFIX) " %s,%%%s",ModR_M_asm,REG_NAME(m.reg));
		return len + 1;
	}
}
#include "exec/template-end.h"
