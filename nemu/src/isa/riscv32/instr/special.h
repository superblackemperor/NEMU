def_EHelper(inv) {
  rtl_hostcall(s, HOSTCALL_INV, NULL, NULL, NULL, 0);
}

def_EHelper(nemu_trap) {
  rtl_hostcall(s, HOSTCALL_EXIT, NULL, &gpr(10), NULL, 0); // gpr(10) is $a0
}

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
	rtl_li(s,ddest,*csr);
	rtl_mv(s,csr,dsrc1);
	
}
def_EHelper(csrrs){
 CHECK_CSR;
 rtl_li(s,ddest,*csr);
 rtl_or(s,csr,csr,dsrc1);
}
def_EHelper(ecall){
	
  s->dnpc=isa_raise_intr(EVENT_YIELD,cpu.pc);
  
}

def_EHelper(mret){
	s->dnpc=cpu.mepc+4;
}

