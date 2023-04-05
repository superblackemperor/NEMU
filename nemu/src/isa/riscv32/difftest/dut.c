#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"
extern CPU_state cpu;
static bool check32reg(CPU_state *ref_r){
	for(int i=0;i<32;i++)
	if(cpu.gpr[i]._32!=ref_r->gpr[i]._32)
	return false;
	return true;
}

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
	if(ref_r->pc==pc&&check32reg(ref_r))
	return true;
  return false;
}

void isa_difftest_attach() {
}
