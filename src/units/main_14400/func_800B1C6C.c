#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Pos;
extern u16 D_80143450[0x36][0x4C];
static __inline__ s32 isOutside(Pos *pos) {
    s32 outside = 0;
    if (pos->y >= 0x4C || pos->x >= 0x36 || pos->y < 0 || pos->x < 0) {
        outside = 1;
    }
    return outside;
}
u32 func_800B1C6C(Pos *pos) {
    if (isOutside(pos)) {
        return 0xC020;
    }
    return D_80143450[pos->x][pos->y];
}
