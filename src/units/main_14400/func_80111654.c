#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad[0xC]; u8 xC; u8 xD; } S;
extern char D_801CA620[];
char *func_80048480(u16 id);
s32 func_8005EF08(char *dst, const char *fmt, ...);
char *func_80111654(S *s, s32 mode) {
    if (mode == 2) {
        func_8005EF08(D_801CA620, func_80048480(0x24A), s->xC);
    } else {
        u8 n = s->xD;
        if (n) {
            func_8005EF08(D_801CA620, func_80048480(0x24A), -n);
        } else {
            D_801CA620[0] = 0;
        }
    }
    return D_801CA620;
}
