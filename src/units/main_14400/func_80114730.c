#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[0x29]; u8 x29; } S;
void func_800B0B10(s32 id);
s32 func_800B0954(void);
u8 func_800B09A0(void *arg);
s32 func_80114730(S *s, void *arg) {
    if (s->x29) {
        func_800B0B10(s->x29);
    }
    if (func_800B0954()) {
        s->x29 = func_800B09A0(arg);
        return 1;
    }
    s->x29 = 0;
    return 0;
}
