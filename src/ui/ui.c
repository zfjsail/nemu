#include "ui/ui.h"
#include "ui/breakpoint.h"

#include "nemu.h"

#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int nemu_state = END;

bool suc = true;//表达式是否合法标志

void cpu_exec(uint32_t);
void restart();
int str_to_num(char* p);/*数字的字符串表示到十进制表示*/

/*数字的十六进制字符串到十进制字符表示*/
int shex_to_ndec(char* p);
/*一位16进制字符转换到十进制数字*/
int bit_htob(char ch);

/* We use the readline library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

/* This function will be called when you press <C-c>. And it will return to 
 * where you press <C-c>. If you are interesting in how it works, please
 * search for "Unix signal" in the Internet.
 */
static void control_C(int signum) {
	if(nemu_state == RUNNING) {
		nemu_state = INT;
	}
}

void init_signal() {
	/* Register a signal handler. */
	struct sigaction s;
	memset(&s, 0, sizeof(s));
	s.sa_handler = control_C;
	int ret = sigaction(SIGINT, &s, NULL);
	assert(ret == 0);
}

static void cmd_c() {
	if(nemu_state == END) {
		puts("The Program does not start. Use 'r' command to start the program.");
		return;
	}

	nemu_state = RUNNING;
	cpu_exec(-1);
	if(nemu_state != END) { nemu_state = STOP; }
}

static void cmd_r() {
	if(nemu_state != END) { 
		char c;
		while(1) {
			printf("The program is already running. Restart the program? (y or n)");
			fflush(stdout);
			scanf(" %c", &c);
			switch(c) {
				case 'y': goto restart_;
				case 'n': return;
				default: puts("Please answer y or n.");
			}
		}
	}

restart_:
	restart();
	nemu_state = STOP;
	cmd_c();
}

/*数字的字符串表示到十进制表示*/
int str_to_num(char* p)
{
	int l_str = strlen(p);
	unsigned int num = *p - 48;
	int i;
	for(i = l_str-1;i > 0;i--)
	{
		num *= 10;
		p++;
		num += *p - 48;
	}
	return num;
}

/*一位16进制字符转换到十进制数字*/
int bit_htob(char ch)
{
	if(ch <= 57)
		return (ch-48);
	else if(ch <= 70)
	    return (ch-55);
	else
	    return (ch-87);
}


/*数字的十六进制字符串到十进制字符表示*/
int shex_to_ndec(char* p)
{
	int l_shex = strlen(p);
	int result = bit_htob(*p);
	int i;
	for(i = l_shex-1;i > 0;i--)
	{
	    result *= 16;
        result += bit_htob(*(++p));
	}
	return result;
}

void main_loop() {
	char *cmd;
	while(1) {
		cmd = rl_gets();
		char *p = strtok(cmd, " ");

		if(p == NULL) { continue; }

		if(strcmp(p, "c") == 0) { cmd_c(); }
        else if(strcmp(p, "r") == 0) { cmd_r(); }
		else if(strcmp(p, "q") == 0) { return; }

		/*单步执行*/
		else if(strcmp(p,"si") == 0)
		{
			if(nemu_state == END) restart();
			nemu_state = RUNNING;
			p = strtok(NULL," ");
			if(p == NULL)
				cpu_exec(1);
			else
			{
				cpu_exec(str_to_num(p));
			}
			if(nemu_state != END) { nemu_state = STOP;}
		}

		/*cmd_info r 打印*/
		else if(strcmp(p,"info") == 0)
		{
			p = strtok(NULL," ");
			if(strcmp(p,"r") == 0){//寄存器
	        printf("eax    0x%08x        %10d\n",cpu.eax,cpu.eax);

	        printf("ecx    0x%08x        %10d\n",cpu.ecx,cpu.ecx);
	        printf("edx    0x%08x        %10d\n",cpu.edx,cpu.edx);
	        printf("ebx    0x%08x        %10d\n",cpu.ebx,cpu.ebx);
	        printf("esp    0x%08x        %10d\n",cpu.esp,cpu.esp);
	        printf("ebp    0x%08x        %10d\n",cpu.ebp,cpu.ebp);
	        printf("esi    0x%08x        %10d\n",cpu.esi,cpu.esi);
	        printf("edi    0x%08x        %10d\n",cpu.edi,cpu.edi);
	        printf("eip    0x%08x        %10d\n",cpu.eip,cpu.eip);
			printf("SF = %1x,OF = %1x\n",cpu.SF,cpu.OF);}
			else if(strcmp(p,"b") == 0)
				print_bp(true);
			else if(strcmp(p,"watch") == 0)
				print_bp(false);
		}

        /*x n(dec) addr  扫描内存*/
		else if(strcmp(p,"x") == 0)
		{
			p = strtok(NULL," ");
			int l_mem = atoi(p);
			p = p + strlen(p) + 1;
			int cur_mem = expr(p,&suc);
			if(suc)
			{
				int i;
				for(i = 0;i < l_mem;i++)
				{
					printf("0x%08x:\t\t0x%08x\n",cur_mem,swaddr_read(cur_mem,4));
					cur_mem += 4;
				}
			}
			else
				printf("Invalid memory address expression!\n");
		}

        /*设置断点*/
		else if(strcmp(p,"b") == 0)
		{
			for(;*p != '*';p++);
			int temp = expr(p+1,&suc);
			if(suc)
				set_b(temp);
			else
				printf("Invalid address expression!\n");
		}

		else if(strcmp(p,"w") == 0)
		{
			++p;
			for(;(*p) == ' ';p++);
		    set_w(p+2);
		}
		/*删除断点*/
		else if(strcmp(p,"d") == 0)
		{
			p = strtok(NULL," ");
        	if((*p) == '*' && (*(p+1)) == '0' && (*(p+2))== 'x')
			free_bp(shex_to_ndec(p+3));
			else
				free_b(str_to_num(p));
		}

		/*求表达式值*/
		else if(strcmp(p,"p") == 0)
		{
			p = p + 2;
			bool success = true;
			int result = expr(p,&success);
			if(success)
				printf("Result = %d\n",result);
			else
				printf("Error!\n");
		}
		else { printf("Unknown command '%s'\n", p); }
	}
}
