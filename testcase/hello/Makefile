CC     = gcc
LD     = ld
CFLAGS = -m32 -Wall -Werror -static -I..
LDFLAGS = -melf_i386 -e main 

SRCS = $(shell ls *.c)
OBJS = $(SRCS:.c=)
LIBS = /home/hh/NEMU/testcase/lib
CCLD_LIBS := -L$(LIBS)

all: $(OBJS)

$(OBJS): % : %.o
	$(LD) $(LDFLAGS) -o $@ $< $(CCLD_LIBS) -lc
	objdump -d $@ > $@.txt

clean:
	-rm $(OBJS) $(SRCS:.c=.o) $(SRCS:.c=.txt) $(SRCS:.c=.d) 2> /dev/null
