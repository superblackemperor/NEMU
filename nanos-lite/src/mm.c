#include <memory.h>
#include <proc.h>
static void *pf = NULL;

void* new_page(size_t nr_page) { 
	//return malloc(nr_page*4*1024);
	void *tmp=pf;
	pf+=nr_page*4*1024;
	return tmp;
}

#ifdef HAS_VME
static void* pg_alloc(int n) {
	int pgnum=n/PGSIZE;
	if(n%PGSIZE!=0)pgnum++;
	void *p=new_page(pgnum);  
	memset(p,0,pgnum*PGSIZE);
	return p;
}
#endif

void free_page(void *p) {
  panic("not implement yet");
}
bool ifmap(uint32_t addr){
	uint32_t**tmp=(uint32_t**)(current->as.ptr);
	if((ASP2(tmp,addr)&0x80000000)==0)
	return false;
	return true;	
}
/* The brk() system call handler. */
int mm_brk(uintptr_t brk) {

	if(current->max_brk==0){
	current->max_brk=brk;
	/*if(ifmap(brk)==false){
	void*pg=new_page(1);
	map(&current->as,(void*)brk,pg,0);
	}*/
	void*pg=new_page(1);
	map(&current->as,(void*)brk,pg,0);
	return 0;
	}	
	//非首次，brk must > current->max_brk
	void*va=(void*)(current->max_brk);//(void *)ROUNDUP(current->max_brk,PGSIZE);
	uint32_t len=brk-(uint32_t)va;
	int pgnum=len/PGSIZE;
	if(len%PGSIZE!=0)pgnum++;
	void*pg=new_page(pgnum);
	for(int i=0;i<pgnum;i++)
		map(&current->as,va+i*PGSIZE,pg+i*PGSIZE,0);
	current->max_brk=brk;
  return 0;
}

void init_mm() {
  pf = (void *)ROUNDUP(heap.start, PGSIZE);
  Log("free physical pages starting from %d", pf);

#ifdef HAS_VME
  vme_init(pg_alloc, free_page);
#endif
}
