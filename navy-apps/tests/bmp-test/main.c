#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <NDL.h>
#include <BMP.h>

int main() {
  printf("bmp-test\n");
  NDL_Init(0);
  int a=300,b=300;
  NDL_OpenCanvas(&a, &b);
  int w, h;
  void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
  printf("w:%d,h:%d\n",w,h);
  assert(bmp);
    NDL_DrawRect(bmp, 0,0 , w, h);
  NDL_DrawRect(bmp, w,h , w, h);
  free(bmp);
  NDL_Quit();
  printf("Test ends! Spinning...\n");
  while (1);
  return 0;
}
