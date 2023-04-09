#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(auipc) f(jal) f(jalr) \
	f(beq) f(bne) f(bge) f(blt) f(bltu) f(bgeu)/*B jump*/ \
	f(lw) f(lbu) f(lh) f(lhu) f(lb)/*I load*/ \
	f(sw) f(sh) f(sb)/*S store*/  \
	f(addi) f(andi) f(sltiu) f(slti) f(srai) f(srli) f(xori) f(slli) f(ori)/*I arith_imm*/ \
        f(add) f(sub) f(sltu) f(xor) f(or) f(sll) f(slt) f(and) \
	f(mul) f(mulh) f(mulhu) f(div) f(rem) f(remu) f(sra) f(srl) f(divu)/*R arith_reg*/ \
	f(inv) f(nemu_trap)

def_all_EXEC_ID();
