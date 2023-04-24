#include <debug.h>
#include <fs.h>
#include <common.h>
#include "syscall.h"


void sys_exit(){
 halt(0);
}
void sys_yield(){
	yield();
}
int32_t sys_open(const char *pathname, int flags, int mode){
	return fs_open(pathname,flags,mode);
}
int32_t sys_write(uint32_t fd,void*buf,size_t len){
	return fs_write(fd,buf,len);
}
int32_t sys_brk(uint32_t addr){
	return 0;
}
size_t sys_read(int fd, void *buf, size_t len){
	return fs_read(fd,buf,len);
}
size_t sys_lseek(int fd, size_t offset, int whence){
	return fs_lseek(fd,offset,whence);
}
int sys_close(int fd){
	return fs_close(fd);
}
typedef  long  suseconds_t;  
typedef  long  time_t;
struct timeval {
    time_t      tv_sec;     /* 秒数 */
    suseconds_t tv_usec;    /* 微秒数 */
};
int sys_gettimeofday(struct timeval *timer){
	timer->tv_sec=io_read(AM_TIMER_UPTIME).us/1000000;
	timer->tv_usec=io_read(AM_TIMER_UPTIME).us%1000000;
return 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case SYS_exit:
	sys_exit();c->GPRx=0;	
	Log("EXIT\n");
	break;
    case SYS_yield:
	sys_yield();c->GPRx=0;
	break;
    case SYS_open:
	c->GPRx=sys_open((const char*)a[1],a[2],a[3]);
	break;
    case SYS_write:
	c->GPRx=sys_write(a[1],(void*)a[2],a[3]);
	Log("WRITE\n");
	break;
    case SYS_brk:
	c->GPRx=sys_brk(a[1]);
	Log("BRK\n");
	break;
    case SYS_read:
	c->GPRx=sys_read(a[1],(void*)a[2],a[3]);
	break;
    case SYS_close:
	c->GPRx=sys_close(a[1]);
	break;
    case SYS_lseek:
	c->GPRx=sys_lseek(a[1],a[2],a[3]);
	break;
    case SYS_gettimeofday:
	c->GPRx=sys_gettimeofday((struct timeval*)a[1]);
	break;
    default: panic("Unhandled syscall ID = %d\n", a[0]);
  }

}
