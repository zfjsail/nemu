#include "x86.h"

#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}

void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
		case 0: add_irq_handle(tf->ebx, (void*)tf->ecx); break;

		case SYS_brk: sys_brk(tf); break;

		/* TODO: Add more system calls. */

	    case 4: {
//write(tf -> ebx, (void *)tf -> ecx, tf -> edx); break;
                 if(tf -> ebx == 1 || tf -> ebx == 2)
					 asm volatile (".byte 0x82" :: "a"(2),"c"(tf -> ecx),"d"(tf -> edx));  
				     asm volatile ("movl %%eax, %0" : "=r"(tf -> eax)); break;
				}


		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}

