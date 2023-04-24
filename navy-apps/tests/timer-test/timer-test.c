#include <stdio.h>
//#include <sys/time.h>
#include <NDL.h>
int main(){
	uint32_t pre=0,cur=0;
 pre=NDL_GetTicks();
while(1){
	cur=NDL_GetTicks();
 if(cur-pre>500){
printf("hello,%u,%u\n",pre,cur);
pre=cur; 
 }
}
return 0;
}

/*int intval_time(struct timeval *ptimer,struct timeval*ppre_timer,suseconds_t intval){
 time_t s=ptimer->tv_sec-ppre_timer->tv_sec;
 if(s==0){ 
return  (uint32_t)(ptimer->tv_usec-ppre_timer->tv_usec)>=(uint32_t)intval;}
suseconds_t u=s*1000000;
 if((uint32_t)ptimer->tv_usec>(uint32_t)ppre_timer->tv_usec)
  u+= ptimer->tv_usec-ppre_timer->tv_usec;
else
  u-= ppre_timer->tv_usec-ptimer->tv_usec;
return (uint32_t)u>=(uint32_t)intval;

}

int main(){
	printf("timer-test\n");
  struct timeval pre_timer={},timer={};
  if(gettimeofday(&pre_timer,NULL)<0){
	printf("error gettime\n");
	return -1;
}
while(1){
 if(gettimeofday(&timer,NULL)<0)
	return -1;
 if(intval_time(&timer,&pre_timer,500000)){
//  printf("%u,%u\n",timer.tv_sec,pre_timer.tv_sec);
  printf("hello,%u\n",(uint32_t)(timer.tv_usec-pre_timer.tv_usec));
  pre_timer=timer;
}
}
return 0;
}*/
