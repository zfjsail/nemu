#include "ui/ui.h"//include目录下
#include "ui/breakpoint.h"

#include "nemu.h"

#include <setjmp.h>

#define LOADER_START 0x100000

int exec(swaddr_t);
void load_prog();
void init_dram();
void init_cache1();

char assembly[40];
jmp_buf jbuf;	/* Make it easy to perform exception handling */

extern uint8_t loader [];
extern uint32_t loader_len;

extern int quiet;

extern int isBreak;//触发断点的标志

void restart() {
	/* Perform some initialization to restart a program */
	load_prog();
	memcpy(hwa_to_va(LOADER_START), loader, loader_len);

	cpu.eip = LOADER_START;
	cpu.ebp = 0;
	cpu.esp = 0x8000000;

	cpu.N0 = 0;
	cpu.N1 = 0;
	cpu.VM = 0;
	cpu.RF = 0;
	cpu.N2 = 0;
	cpu.NT = 0;
	cpu.IOPL = 0;
	cpu.OF = 0;
	cpu.DF = 0;
	cpu.IF = 0;
	cpu.TF = 0;
	cpu.SF = 0;
	cpu.ZF = 0;
	cpu.N3 = 0;
	cpu.AF = 0;
	cpu.N4 = 0;
	cpu.PF = 0;
	cpu.N5 = 1;
	cpu.CF = 0;

	init_dram();
	init_cache1();
}

static void print_bin_instr(swaddr_t eip, int len) {
	int i;
	printf("%8x:   ", eip);
	for(i = 0; i < len; i ++) {
		printf("%02x ", swaddr_read(eip + i, 1));
	}
	printf("%*.s", 50 - (12 + 3 * len), "");
}

void cpu_exec(volatile uint32_t n) {
	volatile uint32_t n_temp = n;

	setjmp(jbuf);
	for(; n > 0; n --) {
		swaddr_t eip_temp = cpu.eip;
		BP* current;

		/*如果触发断点成功且eip-1恰好是int3指令*/
		if(isBreak == true && swaddr_read(cpu.eip-1,1) == INT3_CODE)
		{
			cpu.eip--;
			current = ret_head();//返回组织使用中断点的head指针
			while(current -> addr != cpu.eip)//找存放相应内存地址的断点结构
				current = current -> next;
			swaddr_write(cpu.eip,1,current -> sav_istr);//临时恢复原来的指令
			eip_temp--;
		}
		int instr_len = exec(cpu.eip);

		/*刚执行完恢复的指令要马上设置回断点，使其能反复触发*/
		if(isBreak == true && swaddr_read(eip_temp,1) != INT3_CODE)//刚触发断点但断点的位置刚被恢复为原指令
		{
		    swaddr_write(eip_temp,1,INT3_CODE);
			isBreak = false;//告别刚刚触发的那个断点，初始化isBreak，否则执行下一条正常指令也会进入这个判断
		}

		if(!isChange())
			isBreak = true;

		cpu.eip += instr_len;

		if(n_temp != -1 || (enable_debug && !quiet)) {
			print_bin_instr(eip_temp, instr_len);
			puts(assembly);
		}

		if(nemu_state == INT) {
			printf("\n\nUser interrupt\n");
			return;
		} 
	    else if(isBreak == true) { return; }//如果因触发断点而停止，回到主循环等待下一条指令
		else if(nemu_state == END) { return; }
	}
}
