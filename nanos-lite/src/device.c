#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

AM_GPU_CONFIG_T gpu={};
static char dpinfo[50]={'\0'};
static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
 //yield();//自陷操作进入进程调度 
 for(int i=0;i<len;i++)
	putch(((char*)buf)[i]); 
 return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
//	yield();//自陷操作进入进程调度
	AM_INPUT_KEYBRD_T kev=io_read(AM_INPUT_KEYBRD);
	strcpy(buf,keyname[kev.keycode]);
	if(kev.keydown)
	 return len;
  return 0;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  //	yield();//自陷操作进入进程调度
	strncpy(buf,dpinfo,len);	
	return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
//	yield();//自陷操作进入进程调度
	int x,y;
	x=(offset%(4*gpu.width))/4;
	y=offset/(4*gpu.width);
	io_write(AM_GPU_FBDRAW,x,y,(void*)buf,len/4,1,true);
 return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
//gpu---------------------  
gpu=io_read(AM_GPU_CONFIG);
char tmp[20]={'\0'};
  strcpy(dpinfo,"WIDTH : ");
  int2strD(tmp,gpu.width);
  strcat(dpinfo,tmp);
  strcat(dpinfo,"\n");
  memset(tmp,0,sizeof(tmp));
  strcat(dpinfo,"HEIGHT : ");
  int2strD(tmp,gpu.height);
  strcat(dpinfo,tmp);
  //printf("dpinfo:%s\n",dpinfo);
//-----------------------
  
}
