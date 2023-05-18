#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int evfd;
static struct timeval timer={};
static int draw_w=0,draw_h=0;
uint32_t NDL_GetTicks() {
  if(gettimeofday(&timer,NULL)<0){
	printf("error gettime\n");
	exit(0);
  }
  return timer.tv_sec*1000+(uint32_t)(timer.tv_usec/1000);
//return 0;
}

int NDL_PollEvent(char *buf, int len) {
  return read(evfd,buf,len);

}
static int key_value(char*buf,char *key,char *value){
	int ret=0;
	char**tmp=&key;
	for(;*buf!='\0'&&*buf!='\n';buf++,ret++){
	//	printf("buf: %c\n",*buf);
		if(*buf==':'){
		  tmp=&value;continue;}
		if(*buf!=' '){
		**tmp=*buf;(*tmp)++;
	//	printf("%s\n",key);
		}
	}
	if(*buf=='\n')ret++;
		return ret;	
}
static int pow(int a,int n){
	int ret =1;
	for(int i=0;i<n;i++){
	ret*=a;
	}
	return ret;
}
static void strD2int(int*v,char*str){
	int len=strlen(str);
	*v=0;
	for(int i=len-1;i>=0;i--){
	*v+=(str[i]-48)*pow(10,len-1-i);
	}
}
int base=0;
void NDL_OpenCanvas(int *w, int *h) {
  /*if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }*/
	if(*w>screen_w||*h>screen_h){
	printf("over screen size\n");
	assert(0);
	}
	draw_w = *w; draw_h = *h;
	int x=(screen_w-draw_w)/2;
	int y=(screen_h-draw_h)/2;
	base=(x+y*screen_w)*4;
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
	lseek(fbdev,base+(x+y*screen_w)*4,SEEK_SET);
	int i=0;
	do{
	write(fbdev,pixels,w*4);//break;
	i++;
	if(i>=h)break;
	pixels+=w;
	lseek(fbdev,(screen_w-w)*4,SEEK_CUR);
	}while(1);
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  /*if (getenv("NWM_APP")) {
    evtdev = 3;
  }*/	printf("test\n");
//open keybord
	evfd=open("/dev/events",0,0);
//解析屏幕大小
//---------------------
  int fd=open("/proc/dispinfo",0,0);
  char tmp[50]={'\0'};
  read(fd,tmp,sizeof(tmp));
  //printf("%s\n",tmp);
  char key[20]={'\0'},value[20]={'\0'};
  int tmpoff=key_value(tmp,key,value);//:为分割点，\n或\0为一次操作的单位
	printf("%s,%s\n",key,value);
 if(strcmp("WIDTH",key)==0)
	strD2int(&screen_w,value);
  memset(key,0,sizeof(key));
  memset(value,0,sizeof(value));
   key_value(tmp+tmpoff,key,value);
	printf("%s,%s\n",key,value);
 if(strcmp("HEIGHT",key)==0)
	strD2int(&screen_h,value);
close(fd);
//-------------------------
fbdev=open("/dev/fb",0,0);

return 0;
}

void NDL_Quit() {
	close(evfd);
	close(fbdev);
}
