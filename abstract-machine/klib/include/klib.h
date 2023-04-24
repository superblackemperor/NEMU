#ifndef KLIB_H__
#define KLIB_H__

#include <am.h>
#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __NATIVE_USE_KLIB__


#define  PRINTF_BODY(DEAL)  do{\
	va_list ap;\
	int d;\
	char*s;\
	int count=0;\
	va_start(ap,fmt);\
	while(fmt[count]){\
		char buf[20]={'\0'}; \
		char *tmpcat=buf;\
		if(fmt[count]=='%'){\
		count++;\
		switch(fmt[count]){\
		case 'd':\
			d=va_arg(ap,int);\
		int2strD(tmpcat,d);\
			break;\
		case 's':\
			s=va_arg(ap,char*);\
			tmpcat=s;\
			break;\
		default:\
			return 0;\
		}\
	}else\
	tmpcat[0]=fmt[count];\
	DEAL\
	count++; \
	}\
	return 1;}while(0);

// string.h
void  *memset    (void *s, int c, size_t n);
void  *memcpy    (void *dst, const void *src, size_t n);
void  *memmove   (void *dst, const void *src, size_t n);
int    memcmp    (const void *s1, const void *s2, size_t n);
size_t strlen    (const char *s);
char  *strcat    (char *dst, const char *src);
char  *strcpy    (char *dst, const char *src);
char  *strncpy   (char *dst, const char *src, size_t n);
int    strcmp    (const char *s1, const char *s2);
int    strncmp   (const char *s1, const char *s2, size_t n);

// stdlib.h
void   srand     (unsigned int seed);
int    rand      (void);
void  *malloc    (size_t size);
void   free      (void *ptr);
int    abs       (int x);
int    atoi      (const char *nptr);

// stdio.h
int    printf    (const char *format, ...);
int    sprintf   (char *str, const char *format, ...);
int    snprintf  (char *str, size_t size, const char *format, ...);
int    vsprintf  (char *str, const char *format, va_list ap);
int    vsnprintf (char *str, size_t size, const char *format, va_list ap);
void int2strD(char*str,int32_t num);
// assert.h
#ifdef NDEBUG
  #define assert(ignore) ((void)0)
#else
  #define assert(cond) \
    do { \
      if (!(cond)) { \
        printf("Assertion fail at %s:%d\n", __FILE__, __LINE__); \
        halt(1); \
      } \
    } while (0)
#endif

#ifdef __cplusplus
}
#endif

#endif
