def_EHelper(jal){
	/*if(id_src1->simm>=0)
	s->dnpc=cpu.pc+id_src1->imm;
	else*/
	s->dnpc=(int64_t)cpu.pc+(int64_t)id_src1->simm;
	//s->dnpc=cpu.pc-(id_src1->imm&(~0x80000000));
  	//rtl_li(s,ddest,s->dnpc+4);
	rtl_li(s,ddest,cpu.pc+4);
}

def_EHelper(jalr){
	s->dnpc=(int64_t)*dsrc1+(int64_t)id_src2->simm;
	rtl_li(s,ddest,cpu.pc+4);
}
//下面是B型指令，条件跳转
def_EHelper(beq){
	if(*dsrc1==*dsrc2)
	s->dnpc=(int)cpu.pc+id_dest->simm;
}
def_EHelper(bne){
     if(*dsrc1!=*dsrc2)
 	s->dnpc=(int)cpu.pc+id_dest->simm;
}
def_EHelper(bge){
	if((int32_t)*dsrc1>=(int32_t)*dsrc2)
	  s->dnpc=cpu.pc+id_dest->simm;
}
def_EHelper(blt){
	if((int32_t)*dsrc1<(int32_t)*dsrc2)
	s->dnpc=cpu.pc+id_dest->simm;
}
def_EHelper(bltu){
	if(*dsrc1<*dsrc2)
	s->dnpc=cpu.pc+id_dest->simm;
}
