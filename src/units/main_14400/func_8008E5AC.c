#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

extern char D_801C33E0[];
char *func_8008E5AC(char *buf, u32 code) {
    char *out = D_801C33E0;
    s32 i;
    s32 shift;
    u32 c;

    if (buf != 0) {
        out = buf;
    }
    for (i = 0, shift = 24; i < 4; i++, shift -= 8) {
        c = code >> shift;
        if ((u8)c < 0x20) {
            c = '.';
        }
        out[i] = c;
    }
    out[4] = 0;
    return out;
}
