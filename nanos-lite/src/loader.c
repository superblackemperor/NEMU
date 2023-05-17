#include <proc.h>
#include <elf.h>
#include <fs.h>
#include <memory.h>
#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif


uintptr_t loader(PCB *pcb, const char *filename) {
//把img加载到地址为vaddr(83000000)的地方，vaddr是基本上就是am的堆区区域,用户程序使用系统的同一个栈,堆区使用klib的malloc申请
	//先解析elf
	int fd=fs_open(filename,0,0);
	if(fd<0)return 0;
	size_t off=0;
	Elf_Ehdr eh;
	//ramdisk_read(&eh,off,sizeof(Elf_Ehdr));//读取elf头
	fs_read(fd,&eh,sizeof(Elf_Ehdr));
	assert(*(uint32_t *)eh.e_ident == 0x464C457f);
	assert(eh.e_machine==EXPECT_TYPE);
	//读取程序头表
	off+=eh.e_phoff;
	int segNum=eh.e_phnum;//程序表中的表项数目
	Elf_Phdr ph;
for(int i=0;i<segNum;i++){
  //ramdisk_read(&ph,off,sizeof(Elf_Phdr));
  fs_lseek(fd,off,SEEK_SET);
  fs_read(fd,&ph,sizeof(Elf_Phdr));
  off+=sizeof(Elf_Phdr);
if(ph.p_type==PT_LOAD){//判断type
//把程序加载到合适区域
fs_lseek(fd,ph.p_offset,SEEK_SET);
//------------VME verion
int pgnum=ph.p_memsz/PGSIZE;
if(ph.p_memsz%PGSIZE!=0)pgnum++;
void*paddr=new_page(pgnum);
void*va=(void*)ph.p_vaddr;//逐页映射
for(int i=0;i<pgnum;i++)
map(&pcb->as,va+i*PGSIZE,paddr+i*PGSIZE,0);

uint32_t j;
for(j=0;j<ph.p_filesz;j+=4){
    uint32_t instr;
        fs_read(fd,&instr,sizeof(instr));
	//*(volatile uint32_t*)(ph.p_vaddr+j)=instr;
    	*(volatile uint32_t*)(paddr+j)=instr;
	}
memset((paddr+j),0,ph.p_memsz-ph.p_filesz);
  }
}
//返回这个区域的首地址，naive_uload会使os像执行函数一样执行这个用户程序
fs_close(fd);  	
	return eh.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %d\n", entry);
  ((void(*)())entry) ();
}

