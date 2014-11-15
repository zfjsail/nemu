CC = gcc
CFLAGS = -m32 -O2 -Wall -Werror -fno-builtin
CFLAGS += -I./include

ASFLAGS = -m32

CSRC = $(shell find src/ -name "*.c")
CSRS = $(shell find src/ -name "*.S")
OBJS = $(CSRC:.c=.o) 
OBJSS = $(CSRS:.S=.o)

loader:$(OBJS) $(OBJSS)
	ld -o loader -e start -Ttext 0x100000 -m elf_i386 src/start.o $(OBJS)

clean:
	-rm -f loader $(OBJS) $(OBJSS)
