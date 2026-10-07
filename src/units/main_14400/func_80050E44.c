#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x, y, z; } Vec3i;
typedef struct { u8 pad0[0x12]; u16 flags; } Actor;
typedef struct { s32 x; s32 y; } Cell;
void *func_80085938(s32 id, s32 track, Vec3i pos, s32 arg4, s32 arg5, u16 flags, s32 arg7);
void func_80050E44(s32 arg0, Cell *cell) {
    Vec3i pos;
    Actor *actor;
    pos.x = (cell->y << 5) + 0x10;
    pos.y = (cell->x << 5) + 0x10;
    pos.z = 0;
    actor = func_80085938(arg0, -1, pos, 0, 2, 0, 0);
    actor->flags |= 4;
}
