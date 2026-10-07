#include "common.h"
typedef unsigned char u8; typedef unsigned short u16; typedef signed char s8; typedef short s16;
extern void *func_800AC5B4(s32, s32); extern void *func_8011BF90(void *);
void *func_8011BFD0(void){ return func_8011BF90(func_800AC5B4(0x18,0)); }
