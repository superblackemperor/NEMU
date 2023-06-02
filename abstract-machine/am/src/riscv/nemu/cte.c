#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

#define IRQ_TIMER 0x80000007  // for riscv32
#define KERNEL 0
#define USER   1
static Context* (*user_handler)(Event, Context*) = NULL;
void __am_get_cur_as(Context *c);
void __am_switch(Context *c);

static inline void re_entry_cte(){
	asm volatile("li a1 ,0;csrw mscratch, a1");
}

Context* __am_irq_handle(Context *c) {
__am_get_cur_as(c);//get satp
if(c->gpr[2]<=0x80000000)//用户栈
   c->np=USER;
else c->np=KERNEL;

re_entry_cte();

  if (user_handler) {
    Event ev = {0};
    /*switch (c->mcause) {
      default: ev.event = EVENT_ERROR; break;
    }*/
	//ev.event=c->mcause;
	int NO;
	if(c->mcause>=0&&c->mcause<=19)
	NO=EVENT_SYSCALL;
	else if(c->mcause==-1)
	NO=EVENT_YIELD;
	else if(c->mcause==IRQ_TIMER)
	NO=EVENT_IRQ_TIMER;
	else{
	printf("error NO mcause =%d\n",c->mcause);
	NO=EVENT_ERROR;}
	ev.event=NO;
   /*for(int i=0;i<32;i++){
	printf("%d ",c->gpr[i]);}
	printf("\nmepc=%d\n",c->mepc);
	printf("mstatus=%d\n",c->mstatus);
	printf("mcause=%d\n",c->mcause); */
    c = user_handler(ev, c);
    assert(c != NULL);
  }
	//__am_switch(c);
	return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));
  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {//构造上下文
  	Context init={{0},0,0x1880,(uint32_t)(entry-4),NULL,KERNEL};
	init.gpr[10]=(uintptr_t)arg;//a0
	init.gpr[2]=(uintptr_t)kstack.end;//sp
	memcpy(kstack.end-sizeof(Context),&init,sizeof(Context));
	return kstack.end-sizeof(Context);
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
