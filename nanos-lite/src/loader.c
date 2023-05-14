#include <proc.h>
#include <elf.h>
#include <fs.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif


/*static uintptr_t loader(PCB *pcb, const char *filename) {
  //TODO();
//把img加载到地址为vaddr(83000000)的地方，vaddr是基本上就是am的堆区区域,用户程序使用系统的同一个栈,堆区使用klib的malloc申请

//先解析elf
	size_t off=0,base=0;
	Elf_Ehdr eh;
	ramdisk_read(&eh,off,sizeof(Elf_Ehdr));//读取elf头
	//base=eh.e_ehsize+eh.e_phentsize*eh.e_phnum+ eh.e_shentsize*eh.e_shnum;
	assert(*(uint32_t *)eh.e_ident == 0x464C457f);
	printf("EXPECT_TYPE:%d\n",eh.e_machine);
	assert(eh.e_machine==EXPECT_TYPE);
	//读取程序头表
	off+=eh.e_phoff;
	int segNum=eh.e_phnum;//程序表中的表项数目
	printf("base=%d,segNum=%d\n",base,segNum);	
	//uintptr_t ret_faddr=0;bool flag=false;
	Elf_Phdr ph;
for(int i=0;i<segNum;i++){
  ramdisk_read(&ph,off,sizeof(Elf_Phdr));
  off+=sizeof(Elf_Phdr);
if(ph.p_type==PT_LOAD){//判断type
printf("off=%d,vaddr=%d,fileSize=%d\n",
ph.p_offset,ph.p_vaddr,ph.p_filesz);
//把程序加载到合适区域
uint32_t j;
for(j=0;j<ph.p_filesz;j+=4){
    uint32_t instr;
    ramdisk_read(&instr,ph.p_offset+j,4);
    *(volatile uint32_t*)(ph.p_vaddr+j)=instr;
	//outl(ph.p_vaddr+j,instr);
    }
printf("ph.p_vaddr+j=%d,000=%d\n",ph.p_vaddr+j,ph.p_memsz-ph.p_filesz);
memset((void*)(ph.p_vaddr+j),0,ph.p_memsz-ph.p_filesz);
  }
}
//返回这个区域的首地址，naive_uload会使os像执行函数一样执行这个用户程序
  	
	return eh.e_entry;
}*/

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
uint32_t j;
for(j=0;j<ph.p_filesz;j+=4){
    uint32_t instr;
        fs_read(fd,&instr,sizeof(instr));
	*(volatile uint32_t*)(ph.p_vaddr+j)=instr;
    }
memset((void*)(ph.p_vaddr+j),0,ph.p_memsz-ph.p_filesz);
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

