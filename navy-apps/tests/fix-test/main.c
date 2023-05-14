#include <stdio.h>
#include <assert.h>
#include <fixedptc.h>

int main(int argc,char*argv[],char*envp[]) {
	printf("argc=%d\nargv:\n",argc);
	for(int i=0;i<argc;i++){
	printf("%s\n",argv[i]);
	}
	printf("envp:\n");
	for(int i=0;envp[i]!=NULL;i++){
	printf("%s\n",envp[i]);
	}
	printf("PASS!!!\n");

  return 0;
}
