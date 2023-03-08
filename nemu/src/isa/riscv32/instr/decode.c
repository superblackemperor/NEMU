#include "../local-include/reg.h"
#include <cpu/ifetch.h>
#include <isa-all-instr.h>

def_all_THelper();

static uint32_t get_instr(Decode *s) {
  return s->isa.instr.val;
}

// decode operand helper
#define def_DopHelper(name) \
  void concat(decode_op_, name) (Decode *s, Operand *op, word_t val, bool flag)
static def_DopHelper(i) {
  op->imm = val;
}

static def_DopHelper(r) {
  bool is_write = flag;
  static word_t zero_null = 0;
  op->preg = (is_write && val == 0) ? &zero_null : &gpr(val);
}

static def_DHelper(I) {
  decode_op_r(s, id_src1, s->isa.instr.i.rs1, false);
  decode_op_i(s, id_src2, s->isa.instr.i.simm11_0, false);
  decode_op_r(s, id_dest, s->isa.instr.i.rd, true);
}

static def_DHelper(U) {
  decode_op_i(s, id_src1, s->isa.instr.u.imm31_12 << 12, true);
  decode_op_r(s, id_dest, s->isa.instr.u.rd, true);
}
#define INSTR_B(name,menber) (s->isa.instr.name.menber) 
static def_DHelper(J_jal){
	uint32_t jal_imm=0;
	jal_imm|=INSTR_B(jal,imm10_1);
	jal_imm|=INSTR_B(jal,imm11)<<10;
	jal_imm|=INSTR_B(jal,imm19_12)<<11;
	if(INSTR_B(jal,imm20)==1)
	{
	jal_imm|=0xfff80000;
	}
	jal_imm<<=1;

	decode_op_i(s,id_src1,jal_imm,true);
	decode_op_r(s,id_dest,INSTR_B(jal,rd),true);
}

static def_DHelper(R){
	decode_op_r(s,id_src1,INSTR_B(r,rs1),false);
	decode_op_r(s,id_src2,INSTR_B(r,rs2),false);
	decode_op_r(s,id_dest,INSTR_B(r,rd),true);
}

static def_DHelper(S) {
  decode_op_r(s, id_src1, s->isa.instr.s.rs1, false);
  sword_t simm = (s->isa.instr.s.simm11_5 << 5) | s->isa.instr.s.imm4_0;
  decode_op_i(s, id_src2, simm, false);
  decode_op_r(s, id_dest, s->isa.instr.s.rs2, false);
}

static def_DHelper(B){
 int32_t tmp=INSTR_B(b,imm4_1);
 	 tmp|=INSTR_B(b,imm10_5)<<4;
	 tmp|=INSTR_B(b,imm11)<<10;
	 if(INSTR_B(b,imm12)==1)
	{
	tmp|=0xfffff800;
	}
	tmp<<=1;
	decode_op_r(s,id_src1,INSTR_B(b,rs1),false);
	decode_op_r(s,id_src2,INSTR_B(b,rs2),false);
	decode_op_i(s,id_dest,tmp,false);
}

def_THelper(arith_reg){
  def_INSTR_TAB("0000000 ????? ????? 000 ????? ????? ??",add);
  def_INSTR_TAB("0100000 ????? ????? 000 ????? ????? ??",sub);
  def_INSTR_TAB("0000001 ????? ????? 000 ????? ????? ??",mul);
  def_INSTR_TAB("0000001 ????? ????? 100 ????? ????? ??",div);  
//  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??",add_sub_mul);
  def_INSTR_TAB("0000000 ????? ????? 011 ????? ????? ??",sltu);
  def_INSTR_TAB("0000000 ????? ????? 100 ????? ????? ??",xor); 
  def_INSTR_TAB("0000000 ????? ????? 110 ????? ????? ??",or);
  def_INSTR_TAB("0000000 ????? ????? 111 ????? ????? ??",and);
  def_INSTR_TAB("0000000 ????? ????? 001 ????? ????? ??", sll);  
return EXEC_ID_inv;
}

def_THelper(srai_srli){
 def_INSTR_TAB("0100000 ????? ????? ??? ????? ????? ??", srai);
 def_INSTR_TAB("0000000 ????? ????? ??? ????? ????? ??", srli);
 return EXEC_ID_inv;
}
def_THelper(arith_imm){
 def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", addi);
 def_INSTR_TAB("??????? ????? ????? 011 ????? ????? ??", sltiu);
 def_INSTR_TAB("??????? ????? ????? 101 ????? ????? ??", srai_srli);
 def_INSTR_TAB("??????? ????? ????? 111 ????? ????? ??", ani);
 def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", xori); 
return EXEC_ID_inv;
}
def_THelper(load) {
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", lw);
  def_INSTR_TAB("??????? ????? ????? 100 ????? ????? ??", lbu); 
 return EXEC_ID_inv;
}

def_THelper(store) {
  def_INSTR_TAB("??????? ????? ????? 010 ????? ????? ??", sw);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", sh);
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", sb);      
  return EXEC_ID_inv;
}
def_THelper(jump){
  def_INSTR_TAB("??????? ????? ????? 000 ????? ????? ??", beq);
  def_INSTR_TAB("??????? ????? ????? 001 ????? ????? ??", bne);
  def_INSTR_TAB("??????? ????? ????? 101 ????? ????? ??", bge); 
  return EXEC_ID_inv;
}
def_THelper(main) {
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01100 11", R     , arith_reg);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00100 11", I	  , arith_imm);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00000 11", I     , load);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01000 11", S     , store);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11000 11", B     , jump);
//单条指令
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 00101 11", U	  , auipc);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 01101 11", U     , lui);
  def_INSTR_IDTAB("??????? ????? ????? ??? ????? 11011 11", J_jal , jal);
  def_INSTR_IDTAB("??????? ????? ????? 000 ????? 11001 11", I     , jalr);
  def_INSTR_TAB  ("??????? ????? ????? ??? ????? 11010 11",         nemu_trap);
  return table_inv(s);
};

int isa_fetch_decode(Decode *s) {
  s->isa.instr.val = instr_fetch(&s->snpc, 4);
  int idx = table_main(s);
  return idx;
}
