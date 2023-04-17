#include <isa.h>
#include "local-include/reg.h"


const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
	int i=0;
	for(;i<32;i++){
	printf(" <%s=%x> ",regs[i],cpu.gpr[i]._32);
	if((i+1)%8==0)
	printf("\n");
	}printf("\nPC = %xH mtvec = %xH mepc = %xH mcause = %x \
mstatus = %x\n",cpu.pc,cpu.mtvec,cpu.mepc,cpu.mcause,cpu.mstatus);
}

void isa_spiker_reg_display(CPU_state*ref_r) {                                                                                                                                      
 	int i=0;                                                                                                                                              
        for(;i<32;i++){                                                                                                                                       
        printf(" <%s=%x> ",regs[i],ref_r->gpr[i]._32);                                                                                                                if((i+1)%8==0)                                                                                                                                        
        printf("\n");                                                                                                                                         
        }printf("\nPC = %xH mtvec = %xH mepc = %xH mcause = %x \
mstatus = %x\n",ref_r->pc,ref_r->mtvec,ref_r->mepc,ref_r->mcause,ref_r->mstatus);
}

word_t isa_reg_str2val(const char *s, bool *success) {
  return 0;
}
