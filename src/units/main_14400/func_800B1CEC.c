#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    s32 x;
    s32 y;
} Point;

extern u16 D_80143450[54][76];

u32 func_800B1C6C(Point *pos);
s32 func_80049CB4(s32 id, ...);

/* Set or clear the 0x400 map flag at `pos` and post message 0xD7. */
void func_800B1CEC(s32 set, Point *pos)
{
    s32 outside = 0;
    u32 flags;

    if (pos->y >= 76 || pos->x >= 54 || pos->y < 0 || pos->x < 0) {
        outside = 1;
    }
    if (outside) {
        return;
    }
    flags = func_800B1C6C(pos);
    if (set) {
        if (!(flags & 0x400)) {
            D_80143450[pos->x][pos->y] |= 0x400;
            func_80049CB4(0xD7, pos);
        }
    } else {
        D_80143450[pos->x][pos->y] &= ~0x400;
        func_80049CB4(0xD7, pos);
    }
}
