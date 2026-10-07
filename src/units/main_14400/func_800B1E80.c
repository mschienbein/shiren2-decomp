#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s32 x; s32 y; } Pos800B1E80;
typedef struct { u8 value; } Dir;
void *func_800A2594(void *out, void *from, Dir dir);
u32 func_800B1C6C(void *pos);
s32 func_800B1E80(Pos800B1E80 *pos) {
    s32 blocked = 0;
    s32 outside = 0;
    s32 i;
    if (pos->y >= 76 || pos->x >= 54 || pos->y < 0 || pos->x < 0) {
        outside = 1;
    }
    if (outside || !(func_800B1C6C(pos) & 0x1000)) {
        blocked = 1;
    }
    if (blocked) {
        return 0;
    }
    for (i = 0; ; i += 2) {
        Pos800B1E80 next;
        Dir dir;
        Dir *dp;
        if (i >= 7) {
            break;
        }
        dp = &dir;
        dp->value = i & 7;
        func_800A2594(&next, pos, dir);
        if (func_800B1C6C(&next) & 0x800) {
            return 1;
        }
    }
    return 0;
}
