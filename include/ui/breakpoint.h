#ifndef __UI_BREAKPOINT_H__
#define __UI_BREAKPOINT_H__

#include "common.h"

#define INT3_CODE 0xcc

typedef struct breakpoint {
	int NO;
	bool b_or_w;
	char e[50];
	int last_value;
	unsigned addr;
	unsigned short sav_istr;
	bool enb;
	bool isUsed;
	struct breakpoint *next;

	/* TODO: Add more members if necessary */


} BP;

/*要声明自己写的一些函数（在breakpoint.c中用）*/
void set_b(unsigned x);
BP* new_bp();
void free_bp(unsigned n);
void free_b(int num);
void print_bp(bool type);
BP* ret_head();

void set_w(char* e);
bool isChange();

#endif
