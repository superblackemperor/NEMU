#include <fs.h>
#include <common.h>
#include <proc.h>
#define FT file_table[fd]
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);


typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB,FD_DISP,FD_DEV};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", -1, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", -1, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", -1, 0, invalid_read, serial_write},
  [FD_FB]={"/dev/fb",-1,0,invalid_read, fb_write},
  [FD_DISP]={"/proc/dispinfo",-1,0,dispinfo_read,invalid_write},
  [FD_DEV]  =  {"/dev/events",-1,0,events_read,invalid_write},
#include "files.h"
};
extern AM_GPU_CONFIG_T gpu;
void init_fs() {
  //  initialize the size of /dev/fb
  file_table[FD_FB].size=gpu.width*gpu.height*4;
}

int fs_open(const char *pathname, int flags, int mode){
//	printf("filelen%d\n",ARRLEN(file_table));
	for(int i=3;i<ARRLEN(file_table);i++){
		if(strcmp(file_table[i].name,pathname)==0){
		return i;
	  }
	}
	return -1;
	//panic("filename error");
}
size_t fs_read(int fd, void *buf, size_t len){
	if(FT.open_offset+len>=FT.size){
	len=FT.size-1-FT.open_offset;
  //printf("fs_read->open_offset:%d,size:%d\n",FT.open_offset+len,FT.size);
	}
if(FT.read)
  return FT.read(buf,FT.open_offset,len);
else 
ramdisk_read(buf,FT.disk_offset+FT.open_offset,len);
 FT.open_offset+=len;
 return len;
}
size_t fs_write(int fd, const void *buf, size_t len){
	if(FT.open_offset+len>=FT.size){
	len=FT.size-1-FT.open_offset;
  //printf("fs_read->open_offset:%d,size:%d\n",FT.open_offset+len,FT.size);
	}
if(FT.write)
FT.write(buf,FT.open_offset,len);
else
ramdisk_write(buf,FT.disk_offset+FT.open_offset,len);
FT.open_offset+=len;
return len;
 
}
size_t fs_lseek(int fd, size_t offset, int whence){
	switch(whence){
	case SEEK_SET:
	FT.open_offset=offset;
	break;
	case SEEK_CUR:
	FT.open_offset+=offset;
	break;
	case SEEK_END:
	FT.open_offset=FT.size-1+offset;
	break;
	default:panic("lseek whence error");
	}
	if(!(FT.open_offset>=0&&
FT.open_offset<FT.size)){
	printf("open_offset:%d,size:%d\n",FT.open_offset,FT.size);
	assert(0);}
	return FT.open_offset; 
}

int fs_close(int fd){
	FT.open_offset=0;
	return 0;
}









