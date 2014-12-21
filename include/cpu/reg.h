#ifndef __CPU_REG_H__
#define __CPU_REG_H__

#include "common.h"

/* TODO: Re-organize the 'CPU_state' structure to match
 * the GPR encoding scheme in i386 instruction format.
 * For example, if we access reg_w(R_BX) we will get the 'bx' register;
 * if we access reg_b(R_CH), we will get the 'ch' register.
 * Hint: Use 'union'.
 * For more details about the GPR encoding scheme, see i386 manual.
 */

typedef union {
	union {
		uint32_t _32;
		uint16_t _16;
		uint8_t _8[2];
	} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. 
	 * They match the register encoding scheme used in i386 instruction format.
	 * See i386 manual for more details.
	 */

    struct {
	uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    swaddr_t eip;
	union{
		struct {
			int CF : 1; int : 1; int PF : 1; int : 1;
			int AF : 1; int : 1; int ZF : 1; int SF : 1;
			int TF : 1; int IF : 1; int DF : 1; int OF : 1;
			int IOPL : 2; int NT : 1; int : 1; int RF : 1;
			int VM : 1; int : 6; int : 8;
		};
		int eflags;
	};
	struct {
		uint16_t limit;
		uint32_t base;
	} gdtr;
	union {
		struct {
			uint32_t PE : 1;
			uint32_t MP : 1;
			uint32_t EM : 1;
			uint32_t TS : 1;
			uint32_t ET : 1;
            uint32_t : 26;
			uint32_t PG : 1;
		};
		uint32_t val;
	} CR0;
	union {
		struct {
			uint32_t pad0 : 3;
			uint32_t page_write_through : 1;
			uint32_t page_cache_disable : 1;
			uint32_t pad1 : 7;
			uint32_t page_directory_base : 20;
		};
		uint32_t val;
	} CR3;
	union {
		struct {
			uint16_t RPL : 2;
			uint16_t TI : 1;
			uint16_t INDEX : 13;
		} sr[4];
/*
		struct {
			uint16_t ES;
			uint16_t CS;
			uint16_t SS;
			uint16_t DS;
*/
	    uint16_t sreg[4];
		};
	};
} CPU_state;

//uint16_t cs_limit;

struct SG_cache {
	uint32_t base;
	uint32_t limit;
	bool privilege;
} seg_cache[4];

uint8_t current_sreg;

extern CPU_state cpu;

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

#define reg_l(index) (cpu.gpr[index]._32)
#define reg_w(index) (cpu.gpr[index]._16)
#define reg_b(index) (cpu.gpr[index & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

void set_PF(int r);
void set_ZF(int r);
void set_SF(int r);
void set_CF(unsigned sec,unsigned fir,bool mark);
void set_AF(unsigned sec,unsigned fir,bool mark);
void set_OF(int sec,int fir,bool mark);

//mark = 0为减法
void set_6F(int sec,int fir,int temp,bool mark);//set OSZAPC F 
void set_rF(int r);//只传递结果

void init_CR0();

#endif
