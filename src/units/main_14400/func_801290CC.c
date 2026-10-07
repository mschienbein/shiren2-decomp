#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

void *func_800AC5B4(s32 size, s32 b);
void *func_80129080(void *p);
void *func_801290CC(void) { return func_80129080(func_800AC5B4(0x14, 1)); }
