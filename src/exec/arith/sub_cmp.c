#include "exec/helper.h"
#include "cpu/modrm.h"

#define DATA_BYTE 1
#include "sub_cmp_tpl.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "sub_cmp_tpl.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "sub_cmp_tpl.h"
#undef DATA_BYTE

extern char suffix;

make_helper(cmp_add_l){
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 1){
		int bx = reg_l(m.R_M);
		int disp8 = (char)instr_fetch(eip+2,1);
		int fir = swaddr_read(bx+disp8,4);
		int sec = instr_fetch(eip+3,4);
		if(m.opcode == 7){//cmpl
			temp = fir - sec;
			set_6F(sec,fir,temp,0);
			if(disp8 >= 0)
				print_asm("cmpl" " $0x%x,0x%x(%%%s)",sec,disp8,regsl[m.R_M]);
			else
				print_asm("cmpl" " $0x%x,-0x%x(%%%s)",sec,-disp8,regsl[m.R_M]);
			return 7;
		}
		else return 0;//inv
	}
	else if(m.mod == 3){
		if(m.opcode == 0){//add
			int fir = reg_l(m.R_M);
			int sec = instr_fetch(eip+2,4);
			reg_l(m.R_M) = fir + sec;
			temp = reg_l(m.R_M);
			set_6F(sec,fir,temp,1);
			print_asm("add" " $0x%x,%%%s",sec,regsl[m.R_M]);
			return 6;
		}
		else return 0;//inv
	}
	else return 0;//inv
}

make_helper(cmp_m2r_l){
	ModR_M m;
	int temp;
	unsigned m_temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 1){
		int bx,disp8,fir,sec,addr,inc = 0;
		if(m.R_M != 4){
			bx = reg_l(m.R_M);
		    disp8 = (char)instr_fetch(eip+2,1);
		    sec = swaddr_read(bx+disp8,4);
		    fir = reg_l(m.reg);
			m_temp = m.R_M;
		}
		else{
			inc = 1;
			ModR_M mm;
			int ss = 0;
			mm.val = instr_fetch(eip+2,1);
			bx = reg_l(mm.R_M);
			disp8 = (char)instr_fetch(eip+3,1);
			if(mm.reg != 4)
				switch(mm.mod){
					case 0: ss = 1;break;
					case 1: ss = 2;break;
					case 2: ss = 4;break;
					case 3: ss = 8;break;
				}
			int add_temp = reg_l(mm.reg) * ss;
			addr = bx + add_temp + disp8;
			fir = reg_l(m.reg);
			sec = swaddr_read(addr,4);
			m_temp = mm.R_M;
		}
        temp = fir - sec;
		set_6F(sec,fir,temp,0);
		if(disp8 >= 0)
			print_asm("cmp" " 0x%x(%%%s),%%%s",disp8,regsl[m_temp],regsl[m.reg]);
		else
			print_asm("cmp" " -0x%x(%%%s),%%%s",-disp8,regsl[m_temp],regsl[m.reg]);
		return 3 + inc;
	}
	else return 0;//inv
}

/*
make_helper(cmp_r2r_l){
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 3){
		int fir = reg_l(m.R_M);
		int sec = reg_l(m.reg);
		temp = fir - sec;
		set_6F(sec,fir,temp,0);
		print_asm("cmp" " %%%s,%%%s",regsl[m.reg],regsl[m.R_M]);
		return 2;
	}
	else return 0;//inv
}
*/

make_helper(cmp_i2m_b){
	ModR_M m;
	char temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 1){
		int bx = reg_l(m.R_M);
		int disp8 = (char)instr_fetch(eip+2,1);
		char fir = swaddr_read(bx+disp8,1);
		char sec = instr_fetch(eip+3,1);
		if(m.opcode == 7){
			temp = fir - sec;
			set_6F(sec,fir,temp,0);
			if(disp8 >= 0)
				print_asm("cmpb" " $0x%x,0x%x(%%%s)",sec,disp8,regsl[m.R_M]);
			else
				print_asm("cmpb" " $0x%x,-0x%x(%%%s)",sec,-disp8,regsl[m.R_M]);
			return 4;
		}
		else return 0;//inv
	}
	else return 0;//inv
}
/*
make_helper(cmp_r2r_l){
	ModR_M m;
	char temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 3){
		char fir = reg_b(m.R_M);
		char sec = reg_b(m.reg);
		temp = fir - sec;
		set_6F(sec,fir,temp,0);
		print_asm("cmp" " %%%s,%%%s",regsb[m.reg],regsb[m.R_M]);
		return 2;
	}
	else return 0;//inv
}
*/

make_helper(cmp_r2r_v){
	return (suffix == 'l' ? cmp_r2r_l(eip) : cmp_r2r_w(eip));
}

make_helper(sub_r2r_l){
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 3){
		int fir = reg_l(m.R_M);
		int sec = reg_l(m.reg);
		temp = fir - sec;
		reg_l(m.R_M) = temp;
		set_6F(sec,fir,temp,0);
		print_asm("sub" " %%%s,%%%s",regsl[m.reg],regsl[m.R_M]);
		return 2;
	}
	else return 0;//inv
}

make_helper(sub_r2m_l){
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 1){
		int bx = reg_l(m.R_M);
		int disp8 = (char)instr_fetch(eip+2,1);
		int sec = swaddr_read(bx+disp8,4);
		int fir = reg_l(m.reg);
		temp = fir - sec;
		reg_l(m.reg) = temp;
		set_6F(sec,fir,temp,0);
		if(disp8 >= 0)
			print_asm("sub" " 0x%x(%%%s),%%%s",disp8,regsl[m.R_M],regsl[m.reg]);
		else
			print_asm("sub" " -0x%x(%%%s),%%%s",-disp8,regsl[m.R_M],regsl[m.reg]);
		return 3;
	}
	else return 0;//inv
}

make_helper(sbb_r2r_l){
	ModR_M m;
	m.val = instr_fetch(eip+1,1);
	int borrow = 0;
	if(cpu.CF)
		borrow = 1;
	if(m.mod == 3){
	    int sec = reg_l(m.reg) + borrow;
	    int fir = reg_l(m.R_M);
	    int temp = fir - sec;
	    reg_l(m.R_M) = temp;
	    set_6F(sec,fir,temp,0);
		print_asm("sbb" " %%%s,%%%s",regsl[m.reg],regsl[m.R_M]);
		return 2;
	}
	else return 0;//inv
}

make_helper(dec_r_v){
	return (suffix == 'l' ? dec_r_l(eip) : dec_r_w(eip));
}
