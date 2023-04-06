#define _I32_MIN    (-2147483647 - 1) // minimum signed 32 bit value
#define _I32_MAX      2147483647 // maximum signed 32 bit value

//load
def_EHelper(lw) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 4);
}
def_EHelper(lbu){
	rtl_lm(s,ddest,dsrc1,id_src2->imm,1);
}

def_EHelper(lb){
	rtl_lms(s,ddest,dsrc1,id_src2->imm,1);
}
def_EHelper(lh){
	rtl_lms(s,ddest,dsrc1,id_src2->imm,2);
}
def_EHelper(lhu){
	rtl_lm(s,ddest,dsrc1,id_src2->imm,2);
}
//store
def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(sh){
	rtl_sm(s,ddest,dsrc1,id_src2->imm,2);
}

def_EHelper(sb){
	rtl_sm(s,ddest,dsrc1,id_src2->imm,1);
}
//下面是I的算术指令
def_EHelper(addi){
	rtl_addi(s,ddest,dsrc1,id_src2->simm);
//无符号数加有符号数结果是无符号数，如果立即数是负数，那会不会造成错误？？不过我认为编译器应该会把加上一个负数转化为减去一个正数，所以不用担心会加上一个负数
}
def_EHelper(sltiu){
	rtl_setrelopi(s,RELOP_LTU,ddest,dsrc1,id_src2->imm);//在if-else.c的测试中把他改成了有符号比较了
}
def_EHelper(srai){
	id_src2->imm&=0x0000001f; 
	//if((id_src2->imm&0x00000010)==0)
	rtl_srai(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(srli){
	//id_src2->imm&=0x0000001f;
	//if((id_src2->imm&0x00000010)==0)
	rtl_srli(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(andi){
	rtl_andi(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(xori){
	rtl_xori(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(ori){
	rtl_ori(s,ddest,dsrc1,id_src2->imm);
}
def_EHelper(slli){
	//if((id_src2->imm&0x00000010)==0)
	rtl_slli(s,ddest,dsrc1,id_src2->imm);
}
//下面是reg的算术指令
def_EHelper(add){
	/*int64_t tmp=(int64_t)(int32_t)*dsrc1-(int64_t)(int32_t)*dsrc2;
	if(tmp>_I32_MAX)
	*ddest=_I32_MAX; 
	else if(tmp<_I32_MIN)
	*ddest=_I32_MIN;
	else*/
	rtl_add(s,ddest,dsrc1,dsrc2);
}
def_EHelper(sub){
	//int64_t tmp=(int64_t)(int32_t)*dsrc1-(int64_t)(int32_t)*dsrc2;
	/*int64_t tmp=(int64_t)*dsrc1-(int64_t)*dsrc2; 
	if(tmp>_I32_MAX)
	*ddest=-1;//_I32_MAX;
	else if(tmp<_I32_MIN)
	*ddest=-1;//_I32_MIN;
	else*/
	rtl_sub(s,ddest,dsrc1,dsrc2);
	
}
def_EHelper(sltu){
	rtl_setrelop(s,RELOP_LTU,ddest,dsrc1,dsrc2);	
}
def_EHelper(xor){
	rtl_xor(s,ddest,dsrc1,dsrc2);
}
def_EHelper(or){
	rtl_or(s,ddest,dsrc1,dsrc2);
}
def_EHelper(sll){
	int imm=0x0000001f&*dsrc2;
	rtl_slli(s,ddest,dsrc1,imm);
}
def_EHelper(and){
	rtl_and(s,ddest,dsrc1,dsrc2);
}
def_EHelper(mul){
	rtl_mulu_lo(s,ddest,dsrc1,dsrc2);//无符号的,不用扩展
}
def_EHelper(mulh){
	rtl_muls_hi(s,ddest,dsrc1,dsrc2);
}
def_EHelper(mulhu){
	rtl_mulu_hi(s,ddest,dsrc1,dsrc2);
}
def_EHelper(remu){
	rtl_divu_r(s,ddest,dsrc1,dsrc2);
}
def_EHelper(div){
	rtl_divs_q(s,ddest,dsrc1,dsrc2);//需要符号扩展，但是同为32，就处理不处理都一样
}
def_EHelper(divu){
	rtl_divu_q(s,ddest,dsrc1,dsrc2);
}
def_EHelper(rem){
	rtl_divs_r(s,ddest,dsrc1,dsrc2);//需要符号位扩展，取模
}
def_EHelper(slt){
	rtl_setrelop(s,RELOP_LT,ddest,dsrc1,dsrc2);
}
def_EHelper(sra){
	rtl_sra(s,ddest,dsrc1,dsrc2);
}
def_EHelper(srl){
	rtl_srl(s,ddest,dsrc1,dsrc2);
}	
