#ifndef __SYSCALL_H__
#define __SYSCALL_H__

enum {
  SYS_exit,
  SYS_yield,
  SYS_open,
  SYS_read,
  SYS_write,
  SYS_kill,
  SYS_getpid,
  SYS_close,
  SYS_lseek,
  SYS_brk,
  SYS_fstat,
  SYS_time,
  SYS_signal,
  SYS_execve,
  SYS_fork,
  SYS_link,
  SYS_unlink,
  SYS_wait,
  SYS_times,
  SYS_gettimeofday
};
/*
void sys_exit(){
 halt(0);
}
void sys_yield(){
	yield();
}
int32_t sys_write(uint32_t fd,void*buf,size_t len){
	return fs_write(fd,buf,len);
}
int32_t sys_brk(uint32_t addr){
	return 0;
}
typedef long  __suseconds_t;  
typedef long  time_t;
struct timeval {
    time_t      tv_sec;   
    suseconds_t tv_usec; 
};
int sys_gettimeofday(struct timeval *timer){
	timer->tv_sec=io_read(AM_TIMER_UPTIME).us/1000000;
	timer->tv_usec=io_read(AM_TIMER_UPTIME).us%1000000;
return 0;
}*/
#endif
