#include "common.h"
#include "cpu/reg.h"
#include "memory.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>

extern int var_to_addr(char *);

/*类型名字吧*/
enum {
	NOTYPE = 256, EQ, NUM, HEX, REG, LE, GE, NEQ, AND, OR, LS, RS,P,MN,VAR
	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
	int pcd;//优先级
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE, 50},			// white space
	{"\\+", '+', 10},				// plus
	{"==", EQ, 5},					// equal
	{"^0x[0-9|a-f|A-F]+", HEX, 50}, 
	{"[0-9][0-9]*", NUM, 50},       //dec num
	{"-",'-', 10},                  //sub
	{"\\*",'*', 20},                //mul
	{"/",'/', 20},                  //div
	{"\\(",'(',30},                 //left parentheses
	{"\\)",')',30},                 //right parentheses
	{"^\\$e[acdbsi][xip]",REG, 50},  
	{"%", '%', 20},                 
	{"<<",LS, 8},
	{">>",RS, 8},
	{"<=", LE, 7},
	{">=", GE, 7},
	{"<", '<', 7},                  
	{">", '>', 7},
	{"!=",NEQ, 5},
	{"&&",AND, 1},
	{"\\|\\|",OR,0},
	{"!",'!',25},
	{"&",'&',4},
	{"\\|",'|',2},
	{"\\^",'^',3},
	{"~",'~',25},
	{"\\*",P,25},
	{"-", MN,25},
	{"^[a-z|A-Z|_][a-z|A-Z|_|0-9]*",VAR,50}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			test(0, "regex compilation failed: %s\n%s\n", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
	int pcd;//优先级
} Token;

Token tokens[32];
int nr_token;//表达式中非空格字符数量

/*储存表达式字符串各个字符结构*/
static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);

				position += substr_len;


				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */
		

				switch(rules[i].token_type) {
					/*小心NOTYPE不记忆*/
					case NOTYPE: break;
					case '+':case '-':case '*':case '/':case EQ:case NUM:case '(':case ')':case HEX:case REG:case '%':case '<':case '>':case LE:case GE:case NEQ:case AND:case OR:case '!':case LS:case RS:case '&':case '|':case '^':case '~':case VAR:{
tokens[nr_token].type = rules[i].token_type;
tokens[nr_token].pcd = rules[i].pcd;
nr_token++;break;}//在这个分支计数合适
	/*先总体初始化，之后再修改‘*’和‘-’的问题*/
					default: assert(0);
				}
				if(rules[i].token_type == NUM || rules[i].token_type == HEX || rules[i].token_type == REG || rules[i].token_type == VAR)
				{
					/*数值需要记录字符串*/
					if(substr_len > 32) { assert(0);}
					else
						strncpy(tokens[nr_token-1].str,substr_start,substr_len);
				}


				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

bool isValid;//标志括号匹配是否使得表达式有效
bool check_parentheses(int p,int q)
{
	isValid = true;
	if(tokens[p].type != '(' || tokens[q].type != ')') 
	{
		/*一般情况括号是否匹配*/
		int count_l = 0,count_r = 0;
		int i;
		for(i = p;i <= q;i++)
		{
			if(tokens[i].type == '(')
				count_l++;
			else if(tokens[i].type == ')')
				count_r++;
			if(count_r > count_l)
			{
				printf("Invalid expression!\n");
				isValid = false;
				return false;
			}
		}
		if(count_l != count_r)
		{
			printf("Invalid expression!\n");
			isValid = false;
		}
		return false;
	}
	else
	{
		/*两边都是括号的情况*/
		int count_l = 1,count_r = 0;
		int i;
		for(i = 1;i < q-p;i++)
		{
			if(tokens[p+i].type == '(')
				count_l++;
			else if(tokens[p+i].type == ')')
				count_r++;
			if(count_l < count_r)
			{
				isValid = false;
				printf("Invalid expression!\n");
				return false;
			}
			else if(count_l == count_r)
				return false;
		}
		count_r++;
		if(count_l == count_r)
			return true;
		else
		{
			isValid = false;
			printf("Invalid expression!\n");
			return false;
		}
	}
}

/*寻找dominant operator*/
int seek_op(int p,int q)
{
	int i,op = p;
	int count_l = 0,count_r = 0;
	for(i = p;i < q;i++)
	{
		if(tokens[i].type == '(')
			count_l++;
		else if(tokens[i].type == ')')
			count_r++;
		if(count_l == count_r && tokens[i].pcd <= tokens[op].pcd)
			op = i;
	}
	return op;
}

/*表达式求值函数*/
int eval(int p,int q)
{
	if(p > q)
	{
		printf("Invalid expression!\n");
		assert(0);
		return 0;
	}
	else if(p == q) { 
		if(tokens[p].type == NUM)
			return atoi(tokens[p].str);
		else if(tokens[p].type == HEX)
		{
			int temp;
			sscanf(tokens[p].str,"%x",&temp);
			return temp;
		}
		else if(tokens[p].type == REG)
		{
			switch(tokens[p].str[2])
			{
				case 'a': return cpu.eax;
				case 'c': return cpu.ecx;
				case 'i': return cpu.eip;
				case 'd':
						  {
		    switch(tokens[p].str[3])
			{
				case 'x': return cpu.edx;
				case 'i': return cpu.edi;
			}
						  }
			    case 'b': 
						  {
		    switch(tokens[p].str[3])
			{
				case 'x' :return cpu.ebx;
				case 'p' :return cpu.ebp;
			}
						  }
				case 's' :
						  {
			switch(tokens[p].str[3])
			{
				case 'p' :return cpu.esp;
				case 'i' :return cpu.esi;
			}
						  }
			    default : { assert(0); return 0;}
			}
		}
		else if(tokens[p].type == VAR)
			return var_to_addr(tokens[p].str);
		else 
		{   printf("Invalid expression!\n");
			assert(0);
			return 0;
		}
	}
	else if(check_parentheses(p,q) == true) { 
		return eval(p+1,q-1); }
	else{
		if(!isValid) { assert(0); return 0;}//出错处理
		int op = seek_op(p,q);
		int val1;
	    if(tokens[op].type != '!' && tokens[op].type != '~' && tokens[op].type != P && tokens[op].type != MN)
		    val1 = eval(p,op-1);
		else
        /*注意单目操作符左边不能像上面那样调用，否则可能出现p>q的情况*/
			val1 = 0;
		int val2 = eval(op+1,q);
		switch(tokens[op].type)
		{
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			case '%': return val1 % val2;
			case '<': return val1 < val2;
			case '>': return val1 > val2;
			case EQ : return val1 == val2;
			case LE : return val1 <= val2;
			case GE : return val1 >= val2;
			case NEQ: return val1 != val2;
			case AND: return val1 && val2;
			case OR:  return val1 || val2;
			case LS:  return val1 << val2;
			case RS:  return val1 >> val2;
			case '&': return val1 & val2;
			case '|': return val1 | val2;
			case '^': return val1 ^ val2;
			case '!': return !val2;
			case '~': return ~val2;
			case  P : return swaddr_read(val2,4);
			case MN : return -val2;
			default: { assert(0); return 0;}
		}
	}
}

/*主要的求值函数（从字符串到值）*/
uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Implement code to evaluate the expression. */
	int i;
	for(i = 0; i < nr_token; i++) {
		/*处理有双重含义的操作符*/
		     if(tokens[i].type == '*' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != HEX && tokens[i-1].type != REG && tokens[i-1].type != VAR && tokens[i-1].type != ')'))) {
					tokens[i].type = P;
					tokens[i].pcd = 25;
					    }
			 else if(tokens[i].type == '-' && (i == 0 || (tokens[i-1].type != NUM && tokens[i-1].type != HEX && tokens[i-1].type != REG && tokens[i-1].type != VAR && tokens[i-1].type != ')'))) {
				 tokens[i].type = MN;
				 tokens[i].pcd = 25;
	                     }
	}
	return eval(0,nr_token-1);
}

