def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}
def_EHelper(auipc){
  rtl_li(s,ddest,id_src1->imm+cpu.pc);
}
