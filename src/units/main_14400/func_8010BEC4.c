#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

/* func_8010BC98 limits capacity to sixteen; func_8010BD00 appends at 0x10. */
typedef struct { u8 pad0; u8 f_1; u8 pad2[0xD]; u8 count; u8 items[16]; } S;
s32 func_8010BEC4(S *s, u8 c) {
    s32 n = s->f_1 == c;
    s32 i = s->count;
    while (--i != -1) {
        if (s->items[i] == c) n++;
    }
    return n;
}
