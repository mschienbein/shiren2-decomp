#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pair;
typedef struct { s32 x, y, z; } Vec3i;
typedef struct {
    Pair field_00;
    u8 pad_08;
    u8 field_09;
    u8 pad_0A[0x15];
    u8 field_1F;
} Unit;
typedef struct {
    u8 pad_00[0x12];
    u16 field_12;
    u8 pad_14[0x10];
    s32 field_24;
} Actor;
extern u8 func_800A8C00(void *actor);
extern s32 func_80048EE0(Unit *unit);
extern s32 func_80046240(void);
extern s32 func_80062554(s32 x, s32 y);
extern void *func_80085938(s32 id, s32 track, Vec3i pos, s32 arg4, s32 arg5, s32 flags, s32 arg7);

Actor *func_800510E0(s32 id, Unit *unit, s32 mode, s32 flags, s32 arg4) {
    Pair cell;
    Vec3i position;
    s32 track;
    Actor *actor;
    track = func_800A8C00(unit);
    cell.x = unit->field_00.x;
    cell.y = unit->field_00.y;
    if (unit->field_1F == 0x17) {
        flags |= 0x4000;
    }
    if (func_80048EE0(unit)) {
        flags |= 0x20;
    }
    position.x = (cell.y << 5) + 0x10;
    position.y = (cell.x << 5) + 0x10;
    if (func_80046240()) {
        position.z = func_80062554(cell.y, cell.x);
    } else {
        position.z = (unit->field_09 & 0xF) == 1 ? -0x20 : 0;
    }
    actor = func_80085938(id, track, position, 0, mode, (u16)flags, arg4);
    actor->field_24 = -1;
    actor->field_12 |= 4;
    return actor;
}
