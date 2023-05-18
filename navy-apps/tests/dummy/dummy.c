#include <stdint.h>

#ifdef __ISA_NATIVE__
#error can not support ISA=native
#endif

#define SYS_yield 1
extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);

int main() {
  for(int i=0;i<20;i++)
	 _syscall_(SYS_yield, 0, 0, 0);
	return 0;
}	
