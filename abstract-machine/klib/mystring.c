//#include"include/klib.h"
#include"stdint.h"
#include"stddef.h"
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
//  panic("Not implemented");
	size_t count=0;
	for(;*s!='\0';s++)
		count++;
	return count;
}

char *strcpy(char *dst, const char *src) {
 // panic("Not implemented");
	//不作安全检测,调用者保证空间安全
	int i=0;
	for(;src[i]!='\0';i++)
		dst[i]=src[i];
	dst[i]='\0';
	return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  //panic("Not implemented");
	int i=0;
	for(;i<n&&src[i]!='\0';i++)
	dst[i]=src[i];
	dst[i]='\0';
	return dst;
}

char *strcat(char *dst, const char *src) {
  //panic("Not implemented");
	//不作安全检测,调用者保证空间安全
	size_t dstlen=strlen(dst),srclen=strlen(src);
	int i=0;
	for(;i<srclen;i++)
	dst[dstlen+i]=src[i];
	dst[dstlen+i]='\0';
	return dst;
}

int strcmp(const char *s1, const char *s2) {
  //panic("Not implemented");
	for(int i=0;s1[i]!='\0'||s2[i]!='\0';i++){
		if(s1[i]>s2[i])
		return 1;
		if(s1[i]<s2[i])
		return -1;
	}
	return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  //panic("Not implemented");
	
	for(int i=0;i<n&&(s1[i]!='\0'||s2[i]!='\0');i++){
		if(s1[i]>s2[i])
		return 1;
		if(s1[i]<s2[i])	
		return -1;
	}
	return 0;
}

void *memset(void *s, int c, size_t n) {
  //panic("Not implemented");
	char tmp=c;
	char *ss=(char*)s;
	for(int i=0;i<n;i++)
	ss[i]=tmp;
	return s;
}
/*
void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  panic("Not implemented");
	
}
*/
int memcmp(const void *s1, const void *s2, size_t n) {
  //panic("Not implemented");
	char *ss1=(char*)s1,*ss2=(char*)s2;
	for(int i=0;i<n;i++){
	if(ss1[i]>ss2[i])
	return 1;
	if(ss1[i]<ss2[i])
	return -1;
	}
	return 0;
}
#endif
