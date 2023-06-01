#ifndef __ISA_H__
#define __ISA_H__

// Located at src/isa/$(GUEST_ISA)/include/isa-def.h
#include <isa-def.h>
#include <stdio.h>
typedef enum{
	EVENT_NULL = 0,
	EVENT_YIELD, EVENT_SYSCALL, EVENT_PAGEFAULT, EVENT_ERROR,
	EVENT_IRQ_TIMER, EVENT_IRQ_IODEV,
}event;
//exception trace
#ifdef CONFIG_ETRACE
typedef struct etrace{
	uint32_t pc;
	int e;
	struct etrace*next;
}etrace;
extern etrace*Ehead,*Eend;
void pushEtraceNode(event no);
#endif
// The macro `__GUEST_ISA__` is defined in $(CFLAGS).
// It will be expanded as "x86" or "mips32" ...
typedef concat(__GUEST_ISA__, _CPU_state) CPU_state;
typedef concat(__GUEST_ISA__, _ISADecodeInfo) ISADecodeInfo;

// monitor
extern char isa_logo[];
void init_isa();

// reg
extern CPU_state cpu;
void isa_reg_display();
word_t isa_reg_str2val(const char *name, bool *success);

// exec
struct Decode;
int isa_fetch_decode(struct Decode *s);

// memory
enum { MMU_DIRECT, MMU_TRANSLATE, MMU_FAIL, MMU_DYNAMIC };
enum { MEM_TYPE_IFETCH, MEM_TYPE_READ, MEM_TYPE_WRITE };
enum { MEM_RET_OK, MEM_RET_FAIL, MEM_RET_CROSS_PAGE };
#ifndef isa_mmu_check
int isa_mmu_check(vaddr_t vaddr, int len, int type);
#endif
paddr_t isa_mmu_translate(vaddr_t vaddr, int len, int type);

// interrupt/exception
vaddr_t isa_raise_intr(word_t NO, vaddr_t epc);
#define INTR_EMPTY ((word_t)-1)
#if defined(__ISA_NATIVE__)
#define IRQ_TIMER 32          // for x86
#elif defined(__ISA_X86__)
#define IRQ_TIMER 0           // for mips32
#elif defined(__ISA_RISCV32__)
#define IRQ_TIMER 0x80000007  // for riscv32
#elif defined(__ISA_RISCV64__)
#define IRQ_TIMER 0x8000000000000007  // for riscv64
#else
#define IRQ_TIMER 0x80000007  // for riscv32
#endif
word_t isa_query_intr();

// difftest
  // for dut
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc);
void isa_difftest_attach();

  // for ref
void isa_difftest_regcpy(void *dut, bool direction);
void isa_difftest_raise_intr(word_t NO);

#endif
