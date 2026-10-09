#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Cell;
typedef struct { s32 x, y, z; } Vec3i;
typedef struct { Cell position; u8 pad_08; u8 field_09; u8 pad_0A[0x15]; u8 field_1F; } Unit;
typedef struct { u8 pad_00[0x12]; u16 field_12; u8 pad_14[0x10]; s32 field_24; } Actor;
extern u8 func_800A8C00(void *actor);
extern s32 func_80048EE0(Unit *);
extern s32 func_80046240(void);
extern s32 func_800625FC(s32 a, s32 b);
extern s32 func_80062554(s32 x, s32 y);
extern void *func_80085938(s32 id, s32 track, Vec3i pos, s32 arg4, s32 arg5, s32 flags, s32 arg7);

Actor *func_80050FA0(s32 id, Unit *unit, s32 mode, s32 flags) {
    Cell cell;
    Vec3i position;
    s32 track = (u8)func_800A8C00(unit);
    Actor *actor;
    s32 terrain;
    cell.x = unit->position.x;
    cell.y = unit->position.y;
    if (unit->field_1F == 0x17) {
        flags |= 0x4000;
    }
    if (func_80048EE0(unit)) {
        flags |= 0x20;
    }
    position.x = (cell.y << 5) + 0x10;
    position.y = (cell.x << 5) + 0x10;
    if (func_80046240() || ((terrain = func_800625FC(cell.y, cell.x)), (unit->field_09 & 0xF) != 1 && (terrain & 0x80) == 0)) {
        position.z = func_80062554(cell.y, cell.x);
    } else {
        position.z = -0x20;
    }
    actor = func_80085938(id, track, position, 0, mode, (u16)flags, 0);
    actor->field_24 = -1;
    actor->field_12 |= 4;
    return actor;
}
