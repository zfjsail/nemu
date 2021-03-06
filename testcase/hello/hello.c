#include <stdio.h>
#include <sys/syscall.h>
#include <sys/stat.h>

#include "trap.h"

int __attribute__((__noinline__))
syscall(int id, ...) {
	int ret;
	int *args = &id;
	asm volatile("int $0x80": "=a"(ret) : "a"(args[0]), "b"(args[1]), "c"(args[2]), "d"(args[3]));
	return ret;
}

int read(int fd, char *buf, int len) {
	nemu_assert(0);
	return 0;
}

int write(int fd, char *buf, int len) {
	return syscall(SYS_write, fd, buf, len);
}

off_t lseek(int fd, off_t offset, int whence) {
	nemu_assert(0);
	return 0;
}

void *sbrk(int incr) {
	extern char end;
	static char *heap_end;
	char *prev_heap_end;

	if(heap_end == 0) {
		heap_end = &end;
	}
	prev_heap_end = heap_end;

	if(syscall(SYS_brk, heap_end + incr) == 0) {
		heap_end += incr;
	}

	return prev_heap_end;
}

int close(int fd) {
	nemu_assert(0);
	return 0;
}

int fstat(int fd, struct stat *buf) {
	return 0;
}

int isatty(int fd) {
	nemu_assert(0);
	return 0;
}

int main() {
	printf("Hello world!\n");
	printf("%s","Hello world!\n");
	printf("%d + %d = %d\n", 1, 1, 2);
	printf("\nPA4 —— to be continue\n\n");
//	printf("%s","Hello world!\n");

	return 0;
}

/*
 * const char str[] = "Hello,world!\n";

int main() {
	asm volatile ( "movl $4, %eax;"
			       "movl $1, %ebx;"
				   "movl $str, %ecx;"
				   "movl $14, %edx;"
				   "int $0x80");
	return 0;
}
*/
