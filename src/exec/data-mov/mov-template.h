#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(mov_i2r_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	REG(reg_code) = imm;

	print_asm("mov" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(reg_code));
	return DATA_BYTE + 1;
}

make_helper(concat(mov_i2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		REG(m.R_M) = imm;
		print_asm("mov" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + DATA_BYTE + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		MEM_W(addr, imm);
		print_asm("mov" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + DATA_BYTE + 1;
	}
}

make_helper(concat(mov_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		REG(m.R_M) = REG(m.reg);
		print_asm("mov" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		MEM_W(addr, REG(m.reg));

		print_asm("mov" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
	
		return len + 1;
	}
}

make_helper(concat(mov_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		REG(m.reg) = REG(m.R_M);
		print_asm("mov" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		REG(m.reg) = MEM_R(addr);

		print_asm("mov" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
		return len + 1;
	}
}

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	if(addr > 0x40000000) current_sreg = 2;
	else current_sreg = 3;
	addr = segment_translate(addr,current_sreg);//lnaddr
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	if(addr > 0x40000000) current_sreg = 2;
	else current_sreg = 3;
	addr = segment_translate(addr,current_sreg);//lnaddr
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

make_helper(concat(mov_zb_,SUFFIX)){
	ModR_M mm;
	mm.val = instr_fetch(eip + 2,1);
	if(mm.mod == 3){
		REG(mm.reg) = (unsigned char)REG(mm.R_M);
		
		print_asm("movzb" str(SUFFIX) " %%%s,%%%s",REG_NAME(mm.R_M),REG_NAME(mm.reg));
		return 3;
	}
	else{
		swaddr_t addr;
		int len = read_ModR_M(eip + 2,&addr);
		REG(mm.reg) = MEM_R(addr);

		print_asm("movzb" str(SUFFIX) " %s,%%%s",ModR_M_asm,REG_NAME(mm.reg));
		return len + 2;
	}
}

make_helper(concat(mov_zw_,SUFFIX)){
	ModR_M mm;
	mm.val = instr_fetch(eip + 2,1);
	if(mm.mod == 3){
		REG(mm.reg) = (unsigned short)REG(mm.R_M);

		print_asm("movzw" str(SUFFIX) " %%%s,%%%s",REG_NAME(mm.R_M),REG_NAME(mm.reg));
		return 3;
	}
	else{
		swaddr_t addr;
		int len = read_ModR_M(eip + 2,&addr);
		REG(mm.reg) = (unsigned short)MEM_R(addr);

		print_asm("movzw" str(SUFFIX) " %s,%%%s",ModR_M_asm,REG_NAME(mm.reg));
		return len + 2;
	}
}

make_helper(concat(mov_cr2gr_,SUFFIX)) {
	ModR_M mm;
	mm.val = instr_fetch(eip + 2,1);
	if(mm.mod == 3) {
		if(mm.reg == 0) {
			REG(mm.R_M) = cpu.CR0.val;
			print_asm("mov" " %%cr0,%%%s",REG_NAME(mm.R_M));
			return 3;
		}
		else //inv
			return 0;
	}
	else return 0;//inv
}

make_helper(concat(mov_gr2cr_,SUFFIX)) {
	ModR_M mm;
	mm.val = instr_fetch(eip + 2,1);
	if(mm.mod == 3) {
		if(mm.reg == 0) {
			cpu.CR0.val = REG(mm.R_M);
			print_asm("mov" " %%%s,%%cr0",REG_NAME(mm.R_M));
			return 3;
		}
		else if(mm.reg == 3) {
			cpu.CR3.val = REG(mm.R_M);
			print_asm("mov" " %%%s,%%cr3",REG_NAME(mm.R_M));
			return 3;
		}
		else return 0;//inv
	}
	else return 0;//inv
}

make_helper(concat(mov_rm2sr_,SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1,1);
	seg_cache[m.reg].base = 0;
	seg_cache[m.reg].limit = 0xffffffff;
	if(m.mod == 3) {
		cpu.sreg[m.reg] = REG(m.R_M);
		print_asm("mov" " %%%s,%%%s",REG_NAME(m.R_M),regseg[m.reg]);
		return 2;
	}
	else {
		lnaddr_t addr;
		int len = read_ModR_M(eip + 1,&addr);
		cpu.sreg[m.reg] = MEM_R(addr);

		print_asm("mov" " %s,%%%s",ModR_M_asm,regseg[m.reg]);
		return len + 1;
	}
}

#include "exec/template-end.h"
