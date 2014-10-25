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
	struct {
        int N0:8; int N1:6;
		int VM:1; int RF:1; int N2:1; int NT:1;
		int IOPL:2; int OF:1; int DF:1;
		int IF:1; int TF:1; int SF:1; int ZF:1;
		int N3:1; int AF:1; int N4:1; int PF:1;
		int N5:1; int CF:1;
	};
	};
} CPU_state;

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
void set_SF(unsigned r);
void set_CF(unsigned sec,unsigned fir,bool mark);
void set_AF(unsigned sec,unsigned fir,bool mark);
void set_OF(int sec,int fir,bool mark);

#endif
