#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    /*switch (c->mcause) {
      default: ev.event = EVENT_ERROR; break;
    }*/
	ev.event=c->mcause;
   /*for(int i=0;i<32;i++){
	printf("%d ",c->gpr[i]);}
	printf("\nmepc=%d\n",c->mepc);
	printf("mstatus=%d\n",c->mstatus);
	printf("mcause=%d\n",c->mcause); */
    c = user_handler(ev, c);
    assert(c != NULL);
  }

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
  	Context init={{0},0,0x1800,(uint32_t)(entry-4)};
	init.gpr[10]=(uintptr_t)arg;
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
