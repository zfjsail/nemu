#include "exec/helper.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "mov-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "mov-template.h"
#undef DATA_BYTE


/* for instruction encoding overloading */

extern char suffix;

make_helper(mov_i2r_v) {
	return (suffix == 'l' ? mov_i2r_l(eip) : mov_i2r_w(eip));
}

make_helper(mov_i2rm_v) {
	return (suffix == 'l' ? mov_i2rm_l(eip) : mov_i2rm_w(eip));
}

make_helper(mov_r2rm_v) {
	return (suffix == 'l' ? mov_r2rm_l(eip) : mov_r2rm_w(eip));
}

make_helper(mov_rm2r_v) {
	return (suffix == 'l' ? mov_rm2r_l(eip) : mov_rm2r_w(eip));
}

make_helper(mov_a2moffs_v) {
	return (suffix == 'l' ? mov_a2moffs_l(eip) : mov_a2moffs_w(eip));
}

make_helper(mov_moffs2a_v) {
	return (suffix == 'l' ? mov_moffs2a_l(eip) : mov_moffs2a_w(eip));
}

make_helper(mov_s_bl){
	ModR_M mm;
	mm.val = instr_fetch(eip+2,1);
	if(mm.mod == 3){
		reg_l(mm.reg) = (char)reg_b(mm.R_M);
		print_asm("movsbl" " %%%s,%%%s",regsb[mm.R_M],regsl[mm.reg]);
		return 3;
	}
	else return 0;//inv
}

make_helper(mov_zb_v){
	return (suffix == 'l' ? mov_zb_l(eip) : mov_zb_w(eip));
}
/*
make_helper(mov_z_bl){
	ModR_M mm;
	mm.val = instr_fetch(eip+2,1);
	if(mm.mod == 1){
		int bx = reg_l(mm.R_M);
		int disp8 = (char)instr_fetch(eip+3,1);
		reg_l(mm.reg) = swaddr_read(bx+disp8,1);
		if(disp8 >= 0)
			print_asm("movzbl" " 0x%x(%%%s),%%%s",disp8,regsl[mm.R_M],regsl[mm.reg]);
		else
			print_asm("movzbl" " -0x%x(%%%s),%%%s",-disp8,regsl[mm.R_M],regsl[mm.reg]);
		return 4;
	}
	else if(mm.mod == 0){
		int bx = reg_l(mm.R_M);
		reg_l(mm.reg) = swaddr_read(bx,1);
		print_asm("movzbl" " (%%%s),%%%s",regsl[mm.R_M],regsl[mm.reg]);
		return 3;
	}
	else return 0;//inv
}
*/

make_helper(mov_zw_v){
	return mov_zw_l(eip);
}

make_helper(mov_rm2r_swv) {
	return mov_rm2r_swl(eip);
}

make_helper(mov_cr2gr_v) {
	return mov_cr2gr_l(eip);
}

make_helper(mov_gr2cr_v) {
	return mov_gr2cr_l(eip);
}

make_helper(mov_rm2sr_v) {
	return mov_rm2sr_w(eip);
}

make_helper(cmove_rm2r_v) {
	return (suffix == 'l' ? cmove_rm2r_l(eip) : cmove_rm2r_w(eip));
}

make_helper(cmova_rm2r_v) {
	return (suffix == 'l' ? cmova_rm2r_l(eip) : cmova_rm2r_w(eip));
}
