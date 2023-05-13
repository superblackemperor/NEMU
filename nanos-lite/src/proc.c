#include <proc.h>

#define MAX_NR_PROC 4

void naive_uload(PCB *pcb, const char *filename);

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%s' for the %dth time!\n", ((char*)arg), j);
 //   Log("hello world\n");
	j ++;
    yield();
  }
}
void context_kload(PCB*p,void (*entry)(void *),void*arg){
	Area kstack={p->stack,p->stack+STACK_SIZE};
	p->cp=kcontext(kstack,entry,arg);
	
}
uintptr_t loader(PCB *pcb, const char *filename);
void context_uload(PCB*p,const char*filename){
	Area kstack={p->stack,p->stack+STACK_SIZE};
	 // load program here
  Log("Initializing processes...");
  Log("loader %s",filename);
  //naive_uload(NULL,filename);
 void (*entry)(void *)=(void*)loader(NULL,filename); 
	p->cp=ucontext(NULL,kstack,entry);
}
#define UPCBptr (heap.end-sizeof(PCB))
void init_proc() {
	char*arg1="ARG1";
  context_kload(&pcb[0], hello_fun, arg1);//构造好pcd了
  context_uload((PCB*)UPCBptr, "/bin/bird");
	pcb[1].cp=((PCB*)UPCBptr)->cp;
   switch_boot_pcb();//初始化cur
   yield();//自陷操作进入进程调度
}

Context* schedule(Context *prev) {
  // save the context pointer
current->cp = prev;//第一次会让pcb_boot指向内核的上下文

// always select pcb[0] as the new process
current = (current == &pcb[0] ?  &pcb[1]: &pcb[0]);

// then return the new context
return current->cp;
}
