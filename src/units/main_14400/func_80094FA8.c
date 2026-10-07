#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 pad[6]; s8 f_6; } S;
void *func_800C9E10(void);
void func_80045250(void *a, s32 b);
void func_80094FA8(S *s) {
    s32 v = s->f_6;
    if (v == 2 || v == 0) func_80045250(func_800C9E10(), 1);
}
