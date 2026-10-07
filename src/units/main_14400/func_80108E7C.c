#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct { s32 x0; s32 x4; } Pos;
typedef struct { u8 value; } Dir;
typedef struct ShirenDirection { s8 value; } ShirenDirection;
typedef struct { s32 kind; void *sender; u8 pad8[8]; s32 x10; } Message;
typedef struct { u8 pad0[0x58]; s16 x58; s16 pad5A; s32 (*x5C)(void *, Message *); } VTable;
typedef struct { u8 pad0[0x24]; VTable *vtable; } Target;
typedef struct { u8 pad0[0x44]; Pos pos; u8 pad4C[4]; } Effect;
typedef struct {
    s32 x0;
    s32 x4;
    ShirenDirection dir;
    u8 pad9[0x4F];
    Target *target;
    u8 pad5C[0x2D];
    u8 x89;
} Unit;
extern u8 D_80147620[];
extern u32 D_8013960C;
s32 func_800E20CC(void *obj);
s32 func_80049CB4(s32 id, ...);
s32 func_800A67DC(Unit *unit, Target *target, s32 a, s32 b);
s32 func_800C587C(void *table, u8 kind);
s32 func_800E1CC4(Unit *unit, s32 mode);
Target *func_800A6BA4(Unit *unit, s32 a, s32 b);
void func_800A7204(Target *target, Unit *unit, ShirenDirection *dir, s32 a, s32 b, s32 c, s32 d, s32 e);
void *func_800C4360(void *self, void *owner, u16 value, s32 command, void *position, ShirenDirection direction, s32 limit, u16 flags, u8 mode);
void func_800C2D0C(Effect *effect);
s32 func_800B5300(void *pos, void *actor, u8 amount);
void *func_800A2594(void *out, void *from, Dir dir);
void *func_800A25D8(void *out, void *from, Dir dir, s32 scale);
/* Monster slot +0xB4 supplies a target; this override selects unit->target instead. */
s32 func_80108E7C(Unit *unit, void *supplied_target) {
    Target *target;
    Pos pos;
    Pos *posPtr;
    ShirenDirection dir;
    union {
        Effect effect;
        Message msg;
    } work; /* the message and the effect never live at the same time */
    Message *msgPtr;
    if (func_800E20CC(unit)) {
        func_80049CB4(0x1061, unit);
        return 1;
    }
    target = unit->target;
    posPtr = &pos;
    pos.x0 = unit->x0;
    posPtr->x4 = unit->x4;
    dir = unit->dir;
    if (func_800A67DC(unit, target, 0, 0)) {
        if (func_800C587C(D_80147620, unit->x89)) {
            func_80049CB4(0x106D, unit);
            if (func_800E1CC4(unit, 4)) {
                target = func_800A6BA4(unit, 0x63, 0);
            }
            if (target != 0) {
                func_800A7204(target, unit, &dir, 0x63, 10, 6, 0, 1);
            }
            return 1;
        } else {
            Pos center;
            Pos cell;
            Dir side;
            func_80049CB4(0x1061, unit);
            D_8013960C <<= 1;
            func_800C4360(&work.effect, unit, 0, 0x100, posPtr, dir, 5, 0x811, 0);
            func_800C2D0C(&work.effect);
            D_8013960C >>= 1;
            center.x0 = work.effect.pos.x0;
            center.x4 = work.effect.pos.x4;
            func_80049CB4(0x12D);
            func_800B5300(&center, 0, 10);
            func_80049CB4(0x129, 4);
            func_80049CB4(0x128, 0xAF);
            func_80049CB4(6);
            side.value = (dir.value + 2) & 7;
            func_800A2594(&cell, &center, side);
            func_800B5300(&cell, 0, 10);
            func_80049CB4(7);
            side.value = (dir.value - 2) & 7;
            func_800A2594(&cell, &center, side);
            func_800B5300(&cell, 0, 10);
            func_80049CB4(0x129, 4);
            func_80049CB4(6);
            side.value = (dir.value + 2) & 7;
            func_800A25D8(&cell, &center, side, 2);
            func_800B5300(&cell, 0, 10);
            func_80049CB4(7);
            side.value = (dir.value - 2) & 7;
            func_800A25D8(&cell, &center, side, 2);
            func_800B5300(&cell, 0, 10);
            return 1;
        }
    } else {
        func_80049CB4(0x62, unit);
        msgPtr = &work.msg;
        work.msg.kind = 11;
        work.msg.sender = unit;
        msgPtr->x10 = -1;
        target->vtable->x5C((u8 *)target + target->vtable->x58, msgPtr);
    }
    return 1;
}
