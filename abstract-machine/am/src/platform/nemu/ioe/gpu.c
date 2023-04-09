#include <am.h>
#include <nemu.h>
#include <klib.h>
#define SYNC_ADDR (VGACTL_ADDR + 4)
static uint32_t SCREEN_W=0;
static uint32_t SCREEN_H=0;

static uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
void __am_gpu_init() {
	SCREEN_H=inw(VGACTL_ADDR);
	SCREEN_W=inw(VGACTL_ADDR+2);
/*	printf("W:%d H:%d\n",SCREEN_W,SCREEN_H);
	fb_end=fb+(SCREEN_W*SCREEN_H);
   uint32_t st=100+50*SCREEN_W;
for(int j=0;j<50;j++)
 for(int i=0;i<100;i++)
fb[st+i+j*SCREEN_W] = 0xff;
  outl(SYNC_ADDR, 1);*/
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = SCREEN_W, .height = SCREEN_H,
    .vmemsz = SCREEN_W*SCREEN_H
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
// fb=(uint32_t *)(uintptr_t)FB_ADDR;	  
}
//uint32_t blocksize=ctl->w*ctl->h;
/*memcpy(fb,ctl->pixels,blocksize);
fb+=blocksize;*/
uint32_t start=ctl->x+ctl->y*SCREEN_W;
for(int j=0;j<ctl->h;j++)
	for(int i=0;i<ctl->w;i++)
fb[start+i+j*SCREEN_W]=*((uint32_t*)ctl->pixels+i+j*ctl->w);

/*for(int i=0;i<blocksize;i++)
	fb[start+i]=*((uint32_t*)ctl->pixels+i);	
*/
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
