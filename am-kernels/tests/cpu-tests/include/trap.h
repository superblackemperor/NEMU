#ifndef __TRAP_H__
#define __TRAP_H__

#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#define ARRLEN(a) (sizeof(a)/sizeof(a[0]))

#define DEALARR_t(name,DEAL)  do{\
				for(int t=0;t<ARRLEN(name);t++)\
					{DEAL}\
					}while(0);

__attribute__((noinline))
void check(bool cond) {
  if (!cond) halt(1);
}

#endif
