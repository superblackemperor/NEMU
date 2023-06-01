#include <isa.h>
#include <utils.h>
word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  cpu.mepc=epc;
  cpu.mcause=NO;
  //if(cpu.mcause==IRQ_TIMER){		
  cpu.mstatus=(cpu.mstatus&~0x80)|((cpu.mstatus&0x8)<<4);//7bit is MPIE
  cpu.mstatus&=~0x8;
//}//先保存MIE,再置0
#ifdef CONFIG_ETRACE
	pushEtraceNode(NO);
#endif
  
  return cpu.mtvec;
}

word_t isa_query_intr() {
	if (cpu.INTR&&(cpu.mstatus&0x8)) {
    cpu.INTR = false;
    return IRQ_TIMER;
  }
  return INTR_EMPTY;
}


