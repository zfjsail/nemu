#include "nemu.h"

#include <stdlib.h>
#include <time.h>

CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
	srand(time(0));
	uint32_t sample[8];
	int i;
	for(i = R_EAX; i <= R_EDI; i ++) {
		sample[i] = rand();
		reg_l(i) = sample[i];
		assert(reg_w(i) == (sample[i] & 0xffff));
	}

	assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
	assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
	assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
	assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
	assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
	assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
	assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
	assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

	assert(sample[R_EAX] == cpu.eax);
	assert(sample[R_ECX] == cpu.ecx);
	assert(sample[R_EDX] == cpu.edx);
	assert(sample[R_EBX] == cpu.ebx);
	assert(sample[R_ESP] == cpu.esp);
	assert(sample[R_EBP] == cpu.ebp);
	assert(sample[R_ESI] == cpu.esi);
	assert(sample[R_EDI] == cpu.edi);
}

void set_PF(int r){
	int count = 0, i;
	for(i = 0;i < 8;i++)
	{
		count += r % 2;
		r >>= 1;
	}
	if(count % 2 == 0)
		cpu.PF = 1;
	else
		cpu.PF = 0;
}

void set_SF(unsigned r){
	cpu.SF = r >> 31;
}

void set_ZF(int r){
	cpu.ZF = (r == 0) ? 1 : 0;
}

void set_CF(unsigned sec,unsigned fir,bool mark){
	if(mark){
		if(fir + sec >= fir)
			cpu.CF = 0;
		else
			cpu.CF = 1;
	}
	else{
		if(fir < sec) cpu.CF = 1;
		else cpu.CF = 0;
	}
}

void set_AF(unsigned sec,unsigned fir,bool mark){
	unsigned fl4 = fir & 0xf;
	unsigned sl4 = sec & 0xf;
	if(mark){
		unsigned temp = fl4 + sl4;
		if(temp >> 4) cpu.AF = 1;
		else cpu.AF = 0;
	}
	else{
		if(fl4 < sl4) cpu.AF = 1;
		else cpu.AF = 0;
	}
}

void set_OF(int sec,int fir,bool mark){
	if(!mark)
		sec = ~sec + 1;
	if(fir > 0 && sec > 0 && fir + sec < 0)
		cpu.OF = 1;
	else if(fir < 0 && sec < 0 && fir + sec > 0)
		cpu.OF = 1;
	else cpu.OF = 0;
}

void set_6F(int sec,int fir,int temp,bool mark){
	set_OF(sec,fir,mark);
	set_SF(temp);
    set_ZF(temp);
    set_AF(sec,fir,mark);
	set_PF(temp);
	set_CF(sec,fir,mark);
}

void set_rF(int r){//只传递结果
	set_SF(r);
	set_ZF(r);
	set_PF(r);
}
