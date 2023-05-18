#include <stdio.h>
#include <assert.h>
#include <fixedptc.h>
#include<malloc.h>
int main(int argc,char*argv[],char*envp[]) {
	//printf("argc=%d\nargv:\n",argc);
/*	for(int i=0;i<argc;i++){
	printf("%s\n",argv[i]);
	}
	printf("envp:\n");
	for(int i=0;envp[i]!=NULL;i++){
	printf("%s\n",envp[i]);
	}*/
	int*p=(int*)malloc(20*4);
	for(int i=0;i<20;i++)
	p[i]=i;
	for(int i=0;i<20;i++){
	printf("%dPASS!!!\n",p[i]);
	}
	free(p);
	p=(int*)malloc(20*4);
	for(int i=0;i<20;i++)
	p[i]=i;
	for(int i=0;i<20;i++){
	printf("%dPASS2!!!\n",p[i]);
	}
	free(p);
  return 0;
}
