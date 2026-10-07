#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

/* x1C is the stream's ROM (PI device) offset, not a RAM pointer. */
typedef struct { s32 x0; u8 pad[0x18]; u32 x1C; s32 x20; } S;
extern u8 D_80138B20[];
void func_8006AAF0(void *dst, u32 devAddr, s32 size);
void func_80044434(S *s) {
    if (s->x20 < 0 || s->x0 < s->x20 || s->x20 + 0x18 < s->x0) {
        s->x20 = s->x0 & ~1;
        func_8006AAF0(D_80138B20, s->x1C + s->x20, 0x20);
    }
}
