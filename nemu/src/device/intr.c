#include <isa.h>

void dev_raise_intr() {
	if (cpu.mstatus&0x8)
	cpu.INTR=true;
}
