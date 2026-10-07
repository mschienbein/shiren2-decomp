#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s32 x, y; } Vec2;
typedef struct { u8 value; } Dir;
extern void func_800A2758(void *pos, Dir dir);
extern u32 func_800B1C6C(void *pos);
void *func_800B5A18(void *out, void *pos, Dir dir, s32 count, u16 mask){
    s32 i = 0;
    for (;;) {
        u16 hit;
        if (i >= count) break;
        func_800A2758(pos, dir);
        hit = mask & func_800B1C6C(pos);
        if (hit) {
            Dir *dp = &dir;
            Dir back;
            back.value = (dp->value + 4) & 7;
            func_800A2758(pos, back);
            break;
        }
        i++;
    }
    ((Vec2 *)out)->x = ((Vec2 *)pos)->x;
    ((Vec2 *)out)->y = ((Vec2 *)pos)->y;
    return out;
}
