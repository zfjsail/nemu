#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(aos_imm8_l){//add or sub
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 3){
		int fir = reg_l(m.R_M);
		int sec = instr_fetch(eip+1+1,1);
		temp = fir - sec;
        set_6F(sec,fir,temp,0);
	    if(m.opcode == 7){//cmp
			print_asm("cmp" " $0x%x,%%%s",sec,regsl[m.R_M]);
			return 1 + 1 + 1;
		}
    	else if(m.opcode == 5){//sub
			reg_l(m.R_M) = temp;
			print_asm("sub" " $0x%x,%%%s",sec,regsl[m.R_M]);	
			return 3;
		}
		else return 0; //inv
	}
	else if(m.mod == 1){
		int bx = reg_l(m.R_M);
		int disp8 = (char)instr_fetch(eip+2,1);
		int fir = swaddr_read(bx+disp8,4);
		int sec = instr_fetch(eip+3,1);
		if(m.opcode == 0){
			temp = fir + sec;
			swaddr_write(bx+disp8,4,temp);
			set_6F(sec,fir,temp,1);
			if(disp8 >= 0)
				print_asm("addl" " $0x%x,0x%x(%%%s)",sec,disp8,regsl[m.R_M]);
			else
				print_asm("addl" " $0x%x,-0x%x(%%%s)",sec,-disp8,regsl[m.R_M]);
			return 4;
		}
		else if(m.opcode == 7){//cmpl
			temp = fir - sec;
			set_6F(sec,fir,temp,0);
			if(disp8 >= 0)
				print_asm("cmpl" " $0x%x,0x%x(%%%s)",sec,disp8,regsl[m.R_M]);
			else
				print_asm("cmpl" " $0x%x,-0x%x(%%%s)",sec,-disp8,regsl[m.R_M]);
			return 4;
		}
		else return 0;//inv
	}
	else return 0;//inv
}

make_helper(cmp_i2m_l){
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 1){
		int bx = reg_l(m.R_M);
		int disp8 = (char)instr_fetch(eip+2,1);
		int fir = swaddr_read(bx+disp8,4);
		int sec = instr_fetch(eip+3,4);
		if(m.opcode == 7){
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
	else return 0;//inv
}

make_helper(cmp_m2r_l){
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 1){
		int bx = reg_l(m.R_M);
		int disp8 = (char)instr_fetch(eip+2,1);
		int sec = swaddr_read(bx+disp8,4);
		int fir = reg_l(m.reg);
		temp = fir - sec;
		set_6F(sec,fir,temp,0);
		if(disp8 >= 0)
			print_asm("cmp" " 0x%x(%%%s),%%%s",disp8,regsl[m.R_M],regsl[m.reg]);
		else
			print_asm("cmp" " -0x%x(%%%s),%%%s",-disp8,regsl[m.R_M],regsl[m.reg]);
		return 3;
	}
	else return 0;//inv
}
