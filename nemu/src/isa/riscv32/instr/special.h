def_EHelper(inv) {
  rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(nemu_trap) {
  rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}

#define a7 cpu.gpr[17]._32

#define CHECK_CSR rtlreg_t *csr=0;\
		switch(id_src2->simm){\
		case 0x305:\
		csr=&cpu.mtvec;\
		break;\
		case 0x300:\
		csr=&cpu.mstatus;break;\
		case 0x342:\
		csr=&cpu.mcause;break;\
		case 0x341:\
		csr=&cpu.mepc;break;\
		case 0x180:\
		csr=&cpu.satp;break;\
		case 0x340:\
		csr=&cpu.mscratch;break;\
		default:assert(0);\
		break;\
};

def_EHelper(csrrw){
  //judge csr register accroding to imm
  /*rtlreg_t *csr=0;
  switch(id_src2->simm){
	case 0x305:
	csr=&cpu.mtvec;
	break;
	default:assert(0);
	break;
};*/
	CHECK_CSR;
	rtlreg_t tmp_ddest=*ddest;
	rtl_li(s,ddest,*csr);
	if(dsrc1==ddest)
	rtl_mv(s,csr,&tmp_ddest);
	else
	rtl_mv(s,csr,dsrc1);
	
}
def_EHelper(csrrs){
 CHECK_CSR;
 rtl_li(s,ddest,*csr);
 rtl_or(s,csr,csr,dsrc1);
}
def_EHelper(ecall){
	/*int NO;
	if(a7>=0&&a7<=19)
	NO=EVENT_SYSCALL;
	else if(a7==-1)
	NO=EVENT_YIELD;
	else{
	printf("error NO a7=%u\n",a7);
	NO=EVENT_ERROR;
	}*/
  s->dnpc=isa_raise_intr(a7,cpu.pc);
  
}

def_EHelper(mret){
//	printf("mstatus:%x\n",cpu.mstatus);
	cpu.mstatus|=(cpu.mstatus&0x80)>>4;
	cpu.mstatus|=0x80;
//	printf("mepc:%x,mstatus:%x\n",cpu.mepc,cpu.mstatus);
	s->dnpc=cpu.mepc+4;
}

