#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(xchg_a2r_,SUFFIX)){
	int reg_code = instr_fetch(eip,1) & 7;
	DATA_TYPE temp = REG(R_EAX);
	REG(R_EAX) = REG(reg_code);
	REG(reg_code) = temp;

	print_asm("xchg" str(SUFFIX) " %%%s,%%%s",REG_NAME(R_EAX),REG_NAME(reg_code));
	return 1;
}

#include "exec/template-end.h"
