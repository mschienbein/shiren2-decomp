#include "common.h"
typedef struct { s32 x, y, z; } Vec3i;
typedef struct { s32 x0; s32 x4; } Pos;
typedef struct { char pad[0x12]; unsigned short x12; } Obj;
void *func_80085938(s32 id, s32 track, Vec3i pos, s32 arg4, s32 arg5, unsigned short flags, s32 arg7);
void func_80050F34(s32 a, Pos *pos, s32 c, s32 d) {
    Vec3i v;
    Obj *o;
    v.x = pos->x4 * 32 + 16;
    v.y = pos->x0 * 32 + 16;
    v.z = 0;
    o = func_80085938(a, -1, v, d, c, 0, 0);
    o->x12 |= 4;
}
