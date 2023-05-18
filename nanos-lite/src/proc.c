#include <proc.h>

#define MAX_NR_PROC 4

void naive_uload(PCB *pcb, const char *filename);

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
uint32_t signindex=0;
void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
   Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", ((char*)arg), j);
 //   Log("hello world\n");
	j ++;
    yield();
  }
}
void context_kload(PCB*p,void (*entry)(void *),void*arg){
	Area kstack={p->stack,p->stack+STACK_SIZE};
	p->cp=kcontext(kstack,entry,arg);
	signindex++;
}
uintptr_t loader(PCB *pcb, const char *filename);
void context_uload(PCB*p,const char*filename,char *const argv[], char *const envp[]){
	uint32_t argvlen=0,envplen=0;
	char*strptr=NULL,**sstk=NULL;
//---------------sring area

	{char *stk=(char*)(p->stack+STACK_SIZE-1);
	*stk=0;
	for(int i=0;argv[i]!=NULL;i++){//get argv len
	uint32_t len=strlen(argv[i])+1;
	stk-=len;
	strcpy(stk,argv[i]);
	argvlen++;
	}
	for(int i=0;envp[i]!=NULL;i++){//get envp len,don not include "NULL"
	uint32_t len=strlen(envp[i])+1;
	stk-=len;
	strcpy(stk,envp[i]);
	envplen++;
	}
	strptr=stk;//string poniter
	*(--stk)=0;
	sstk=(char**)stk;
	}
//-------------------pointer area
	*(--sstk)=NULL;//envp
	for(int i=0;i<envplen;i++){
	sstk--;
	*sstk=strptr;
	strptr+=strlen(strptr)+1;
	}
	*(--sstk)=NULL;//argv
	for(int i=0;i<argvlen;i++){
	sstk--;
	*sstk=strptr;
	strptr+=strlen(strptr)+1;
	}
	sstk--;//argc
	uint32_t*a=(uint32_t*)sstk;
	*a=argvlen;
//-----------------------loader area

	 // load program here
	protect(&p->as);
  Log("Initializing processes...");
  Log("loader %s",filename);
  //naive_uload(NULL,filename);
 void (*entry)(void *)=(void*)loader(p,filename); 
	if(entry==NULL)return;
	Area kstack={p->as.area.end-8*PGSIZE,p->as.area.end};
	p->cp=ucontext(&p->as,kstack,entry);
}
//#define UPCBptr (heap.end-sizeof(PCB))
void init_proc() {
	char*arg1="ARG1";
  context_kload(&pcb[signindex], hello_fun, arg1);//构造好pcd了
	char*argv[]={"/bin/event-test",NULL};char*envp[]={"ef","gh",NULL};
  context_uload(&pcb[signindex], "/bin/nterm",argv,envp);
	
   switch_boot_pcb();//初始化cur
   yield();//自陷操作进入进程调度
}
void __am_switch_as(AddrSpace *as);
Context* schedule(Context *prev) {
  // save the context pointer
current->cp = prev;//第一次会让pcb_boot指向内核的上下文

// always select pcb[0] as the new process
current = (current == &pcb[0] ?  &pcb[1]: &pcb[0]);
__am_switch_as(&current->as);
// then return the new context
return current->cp;
}
