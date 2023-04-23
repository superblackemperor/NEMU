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

void sys_exit(){
 halt(0);
}
void sys_yield(){
	yield();
}
int32_t sys_write(uint32_t fd,void*buf,size_t len){
	switch(fd){
	case 1:
	case 2:
	for(int i=0;i<len;i++)
	putch(((char*)buf)[i]);
	break;
	default:return -1;
	}
	return len;
}
int32_t sys_brk(uint32_t addr){
	return 0;
}
#endif
