#define SDL_malloc  malloc
#define SDL_free    free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"
#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
SDL_Surface* IMG_Load_RW(SDL_RWops *src, int freesrc) {
  assert(src->type == RW_TYPE_MEM);
  assert(freesrc == 0);

  return NULL;
}

SDL_Surface* IMG_Load(const char *filename) {
	printf("%s\n",filename);
	int fd=open(filename,0,0);
	int size=lseek(fd,0,SEEK_END)+1;
	printf("size:%d\n",size);
	lseek(fd,0,SEEK_SET);
	char*buf=malloc(size);
	assert(buf!=NULL);
	read(fd,buf,size);
	SDL_Surface*ret=STBIMG_LoadFromMemory(buf,size);
	free(buf);
	close(fd);
  return ret;
}

int IMG_isPNG(SDL_RWops *src) {
  return 0;
}

SDL_Surface* IMG_LoadJPG_RW(SDL_RWops *src) {
  return IMG_Load_RW(src, 0);
}

char *IMG_GetError() {
  return "Navy does not support IMG_GetError()";
}
