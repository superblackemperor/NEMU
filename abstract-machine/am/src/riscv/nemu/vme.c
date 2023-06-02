#include <am.h>
#include <nemu.h>
#include <klib.h>


static AddrSpace kas = {};
static void* (*pgalloc_usr)(int) = NULL;
static void (*pgfree_usr)(void*) = NULL;
static int vme_enable = 0;

static Area segments[] = {      // Kernel memory mappings
  NEMU_PADDR_SPACE
};

#define USER_SPACE RANGE(0x40000000, 0x80000000)

static inline void set_satp(void *pdir) {
  uintptr_t mode = 1ul << (__riscv_xlen - 1);
  asm volatile("csrw satp, %0" : : "r"(mode | ((uintptr_t)pdir >> 12)));
}

static inline uintptr_t get_satp() {
  uintptr_t satp;
  asm volatile("csrr %0, satp" : "=r"(satp));
  return satp << 12;
}

bool vme_init(void* (*pgalloc_f)(int), void (*pgfree_f)(void*)) {
  pgalloc_usr = pgalloc_f;
  pgfree_usr = pgfree_f;

  kas.ptr = pgalloc_f(PGSIZE);

  int i;
  for (i = 0; i < LENGTH(segments); i ++) {//将内核空间恒等映射完，再开启pgtbwalk
    void *va = segments[i].start;
    for (; va < segments[i].end; va += PGSIZE) {
      map(&kas, va, va, 0);
    }
  }

  set_satp(kas.ptr);//<<--
  vme_enable = 1;

  return true;
}

void protect(AddrSpace *as) {
  PTE *updir = (PTE*)(pgalloc_usr(PGSIZE));
  as->ptr = updir;
  as->area = USER_SPACE;
  as->pgsize = PGSIZE;
  // map kernel space
  memcpy(updir, kas.ptr, PGSIZE);
}

void unprotect(AddrSpace *as) {
}

void __am_get_cur_as(Context *c) {
  c->pdir = (vme_enable ? (void *)get_satp() : NULL);
}

void __am_switch(Context *c) {
  if (vme_enable && c->pdir != NULL) {
    set_satp(c->pdir);
  }
}
void __am_switch_as(AddrSpace *as) {
  if (vme_enable && as->ptr != NULL) {
    set_satp(as->ptr);
  }
}
#define ASP1(name,va) name[VPN1(va)]
#define ASP2(name,va) name[VPN1(va)][VPN2(va)]
#define VPN1(va) ((uint32_t)va>>22)
#define VPN2(va) (((uint32_t)va>>12) & 0x3ff)
void map(AddrSpace *as, void *va, void *pa, int prot) {
	uint32_t**tmp=(uint32_t**)(as->ptr);
	if(tmp[VPN1(va)]==NULL){//页目录
		tmp[VPN1(va)]=pgalloc_usr(1);//申请一级页表项		
	}
	uint32_t pte=ASP2(tmp,va);
	if((pte&0x80000000)==0){//页表项
		ASP2(tmp,va)= 0x80000000 | ((uint32_t)pa>>12);
	}
	
}
#define USER 1
#define ustack as->area
Context *ucontext(AddrSpace *as, Area kstack, void *entry) {
	//int len=ustack.end-ustack.start;//map usr stack
  	int len=8*PGSIZE;
	void*pstack=pgalloc_usr(len);
	int pgnum=len/PGSIZE;
	if(len%PGSIZE!=0)pgnum++;
	void*va=ustack.end-8*PGSIZE;
	for(int i=0;i<pgnum;i++)
	map(as,va+i*PGSIZE,pstack+i*PGSIZE,0);
	Context init={{0},0,0x1880,(uint32_t)(entry-4),NULL,USER};//make kernel stack
	init.gpr[2]=(uintptr_t)ustack.end;
	memcpy(kstack.end-sizeof(Context),&init,sizeof(Context));
	return kstack.end-sizeof(Context);
}
