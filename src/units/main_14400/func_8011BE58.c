#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern void *func_800AC5B4(s32, s32);
extern void *func_8011BE20(void *);
void *func_8011BE58(void){ return func_8011BE20(func_800AC5B4(0x10, 0));}
