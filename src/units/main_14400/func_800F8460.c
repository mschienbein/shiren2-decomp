#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

void *func_800A38FC(s32);
void *func_800F83F0(void *, u8);
void *func_800F8460(u8 arg0) { return func_800F83F0(func_800A38FC(0x80), arg0); }
