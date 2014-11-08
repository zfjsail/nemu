#include "exec/helper.h"
#include "cpu/modrm.h"

make_helper(mis_imm8_l){
	ModR_M m;
	int temp;
	m.val = instr_fetch(eip+1,1);
	if(m.mod == 3){
		int fir = reg_l(m.R_M);
		int sec = (char)instr_fetch(eip+1+1,1);
	    if(m.opcode == 7){//cmp
		    temp = fir - sec;
            set_6F(sec,fir,temp,0);
			print_asm("cmp" " $0x%x,%%%s",sec,regsl[m.R_M]);
			return 1 + 1 + 1;
		}
    	else if(m.opcode == 5){//sub
		    temp = fir - sec;
            set_6F(sec,fir,temp,0);
			reg_l(m.R_M) = temp;
			print_asm("sub" " $0x%x,%%%s",sec,regsl[m.R_M]);	
			return 3;
		}
		else if(m.opcode == 4){//and
			temp = fir & sec;
			cpu.CF = 0;
			cpu.OF = 0;
			reg_l(m.R_M) = temp;
		    set_rF(temp);
			print_asm("and" " $0x%x,%%%s",sec,regsl[m.R_M]);
			return 3;
		}
		else if(m.opcode == 0){//add
			temp = fir + sec;
			reg_l(m.R_M) = temp;
			set_6F(sec,fir,temp,1);
			print_asm("add" " $0x%x,%%%s",sec,regsl[m.R_M]);
			return 3;
		}
		else return 0; //inv
	}


	else if(m.mod == 1){
		int bx,disp8,fir,sec,addr;
		int inc = 0;
		if(m.R_M != 4){
			bx = reg_l(m.R_M);
		    disp8 = (char)instr_fetch(eip+2,1);
			addr = bx + disp8;
		    fir = swaddr_read(addr,4);
		    sec = (char)instr_fetch(eip+3,1);
		}
		else{
			inc = 1;
		    ModR_M mm;
			int ss;
		    mm.val = instr_fetch(eip+2,1);
		    bx = reg_l(mm.R_M);
			disp8 = (char)instr_fetch(eip+3,1);
			int add_temp;
			if(mm.reg == 4)
				add_temp = 0;
			else{
				switch(mm.mod){
					case 0: ss = 1;break;
					case 1: ss = 2;break;
					case 2: ss = 4;break;
					case 3: ss = 8;break;
				}
				add_temp = reg_l(mm.reg) * ss;
			}
			addr = bx + add_temp + disp8;
			fir = swaddr_read(addr,4);
			sec = (char)instr_fetch(eip+4,1);
		}
		if(m.opcode == 0){//addl
			temp = fir + sec;
			swaddr_write(addr,4,temp);
			set_6F(sec,fir,temp,1);
			if(disp8 >= 0)
				print_asm("addl" " $0x%x,0x%x(%%%s)",sec,disp8,regsl[m.R_M]);
			else
				print_asm("addl" " $0x%x,-0x%x(%%%s)",sec,-disp8,regsl[m.R_M]);
			return 4 + inc;
		}
		else if(m.opcode == 7){//cmpl
			temp = fir - sec;
			set_6F(sec,fir,temp,0);
			if(disp8 >= 0)
				print_asm("cmpl" " $0x%x,0x%x(%%%s)",sec,disp8,regsl[m.R_M]);
			else
				print_asm("cmpl" " $0x%x,-0x%x(%%%s)",sec,-disp8,regsl[m.R_M]);
			return 4 + inc;
		}
		else return 0;//inv
	}
	else return 0;//inv
}

