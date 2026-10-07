#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

void *func_800AC5B4(s32 size, s32 mode);
void *func_8011B7F0(void *p);
void *func_8011B828(void) { return func_8011B7F0(func_800AC5B4(0x10, 0)); }
