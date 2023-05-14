#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args) {
  int argc=*args;
  args++;
  char**argv=(char**)args;
  char**envp=argv+argc+1;
  //char *empty[] =  {NULL };
  environ = envp;
  exit(main(argc, argv, envp));
  free(argv);
  assert(0);
}
