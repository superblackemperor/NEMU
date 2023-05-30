#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>

#define VPN1(va) ((uint32_t)va>>22)
#define VPN2(va) (((uint32_t)va>>12) & 0x3ff)

extern CPU_state cpu;
int isa_mmu_check(vaddr_t vaddr, int len, int type){
	if((cpu.satp&0x80000000)==0)
	return MMU_DIRECT;
	else 
	return MMU_TRANSLATE;	
}

paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type) {
  	vaddr_t asp=(cpu.satp&0xfffff)<<12;
	asp+=VPN1(vaddr)*4;//指向二级页表的地址
	asp=paddr_read(asp,4);//二级页表的地址
	asp+=VPN2(vaddr)*4;//指向目标页表项
	asp=paddr_read(asp,4);
	if((asp&0x80000000)==0)printf("vaddr%x\n",vaddr);
	assert(asp&0x80000000);//用之前必为1
	return ((asp&0xfffff)<<12)+(vaddr&0xfff);
	return MEM_RET_FAIL;
}
