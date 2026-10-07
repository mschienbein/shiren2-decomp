#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

void *func_800AC5B4(s32, s32);
void *func_80125340(void *);
void *func_80125384(void) { return func_80125340(func_800AC5B4(0x10, 0)); }
