#ifndef __PROC_H__
#define __PROC_H__

#include <common.h>
#include <memory.h>

#define STACK_SIZE (8 * PGSIZE)

typedef union {
  uint8_t stack[STACK_SIZE] PG_ALIGN;
  struct {
    Context *cp;
    AddrSpace as;
    // we do not free memory, so use `max_brk' to determine when to call _map()
    uintptr_t max_brk;
  };
} PCB;

extern PCB *current;

extern PCB *bg_pcb;
extern PCB *Fx_pcb[];
#define Fx_pcb1 "F1"
#define Fx_pcb2 "F2"
#define Fx_pcb3 "F3"
#endif
