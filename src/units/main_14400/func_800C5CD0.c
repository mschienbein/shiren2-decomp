#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[0x10]; s32 x10; } S;
u32 func_800413C0(void);
void func_800C5CD0(S *s) { s->x10 = func_800413C0(); }
