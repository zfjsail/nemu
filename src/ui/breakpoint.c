#include "ui/breakpoint.h"
#include "ui/ui.h"

#include "nemu.h"

#define NR_BP 32

static BP bp_pool[NR_BP];
static BP *head, *free_;

void init_bp_pool() {
	int i;
	for(i = 0; i < NR_BP - 1; i ++) {
		bp_pool[i].NO = i;//NO+1代表断点编号
		bp_pool[i].enb = false;//enable
		bp_pool[i].isUsed = false;//断点是否被使用（若被归还到free链表中则未被使用）
		bp_pool[i].next = &bp_pool[i + 1];
	}
	bp_pool[i].NO = i;
	bp_pool[i].next = NULL;

	head = NULL;
	free_ = bp_pool;
}

/*设置断点*/
void set_b(unsigned x)
{
	BP* newb = new_bp();//从free中找到一个新的可使用的断点
	if(newb == NULL) { assert(0);}//free为空
	newb -> b_or_w = true;
	newb -> addr = x;//储存内存地址
	newb -> sav_istr = swaddr_read(x,1);//储存原指令
	swaddr_write(x,1,INT3_CODE);
	newb -> enb = true;//enable
	newb -> isUsed = true;
	printf("Breakpoint %d at 0x%x\n",(newb -> NO)+1,x);
}

BP* new_bp()
{
	BP* temp = free_;
	free_ = free_ -> next;
	temp -> next = head;
	head = temp;
	return head;
}

/*把某个内存地址的断点释放*/
void free_bp(unsigned n)
{
	BP* current = head,*last;
	while(current -> addr != n && current != NULL) 
	{
		last = current;
		current = current -> next;
	}
	if(current -> addr == n)
	{
		if(current == head)
			head = current -> next;
		else
			last -> next = current -> next;
		current -> next = free_;
		free_ = current;
		swaddr_write(n,1,current -> sav_istr);
		current -> isUsed = false;
	}
	else
		printf("There does't exist the breakpoint!\n");
}

/*把某个编号的断点释放*/
void free_b(int num)
{
	BP* current = head,*prev;
    while(current -> NO != num-1 && current != NULL)
   	{		
		prev = current;
		current = current -> next;
	}
	if(current && current -> b_or_w)
		free_bp(current -> addr);
	else if(current)
	{
		if(current == head)
			head = current -> next;
		else
			prev -> next = current -> next;
		current -> next = free_;
		free_ = current;
		current -> isUsed = false;
	}
	else
		printf("There doesn't exist the breakpoint!\n");
}

/*打印断点*/
void print_bp(bool type)
{
	BP* current;
	printf("Num\tType\t\tDisp Enb Address\n");
	int i;
	for(i = 0;i < NR_BP;i++)
	{
		current = &bp_pool[i];
		if(current -> isUsed == true)
		{
			if(current -> b_or_w && type)
			    printf("%d\tbreakpoint\tkeep ",(current -> NO)+1);       
			else if(!type && !current -> b_or_w)
				printf("%d\thw watchpoint\tkeep ",(current -> NO)+1);
			if(current -> enb == true)
	     		printf("y  ");
		    else
			    printf("n  ");
			if(current -> b_or_w && type)
	            printf("0x%x\n",current -> addr);
			else if(!type && !current -> b_or_w)
				printf("\t  %s\n",current -> e);
	    	current = current -> next;
		}
	}
}

/*返回head*/
BP* ret_head()
{
	return head;
}

void set_w(char *e)
{
	   BP* neww = new_bp();
	   if(neww == NULL) {assert(0);}
	   neww -> b_or_w = false;
	   strcpy(neww -> e,e);
	   bool success = true;
	   neww -> last_value = expr(e,&success);
	   if(success)
	   {
			neww -> enb = true;
			neww -> isUsed = true;
			printf("Hardware watchpoint %d:%s\n",(neww->NO)+1,e);
	   }
	   else
			printf("Invalid expression!\n");
}

bool isChange()
{
	int i;
	for(i = 0;i < NR_BP;i++)
	{
		if(bp_pool[i].isUsed && bp_pool[i].b_or_w == false)
		{
			bool success = true;
			int temp = expr(bp_pool[i].e,&success);
			if(temp != bp_pool[i].last_value)
			{
				printf("Hardware watchpoint %d: %s\n\n",bp_pool[i].NO+1,bp_pool[i].e);
				printf("Old value = %d\n",bp_pool[i].last_value);
				printf("New value = %d\n",temp);
			    bp_pool[i].last_value = temp;
				return false;
			}
		}
	}
	return true;
}

/* TODO: Implement the function of breakpoint */
