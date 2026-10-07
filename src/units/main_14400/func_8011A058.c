#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

void *func_800AC5B4(s32 size, s32 b);
void *func_8011A020(void *p);
void *func_8011A058(void) { return func_8011A020(func_800AC5B4(0x10, 0)); }
