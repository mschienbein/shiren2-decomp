#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

extern u8 D_80151350[];
typedef struct MenuCursor { s32 index; void *records; s32 unknown_08; void *vtable_0C; } MenuCursor;
extern MenuCursor D_80140100;
void func_800924EC(void){ u8 unused[16]; D_80140100.vtable_0C = D_80151350;}
