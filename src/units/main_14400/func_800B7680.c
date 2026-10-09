#include "common.h"
typedef unsigned char u8;
typedef short s16;
typedef struct { s16 delta, index; void (*call)(void *, s32); } Entry;
typedef struct { u8 pad00[8]; Entry destroy08; } Vtable;
typedef struct { u8 pad00[0x30]; Vtable *vtable; } Component;
typedef struct { u8 pad00[0x84]; Component component84; } Unit;
typedef struct { s32 x, y; } Position;
extern u8 D_80147490;
extern s32 func_800A8AA0(s32, s32);
extern Unit *func_800A85A0(u8, u8);
extern s32 func_800A8974(void *);
extern void func_800EE718(Unit *, s32);
extern void func_800A58FC(void *, Position *);
void func_800B7680(Position *position, s32 id)
{
    s32 bit = id - 24;
    Unit *unit;
    u8 *component;
    s32 absent = ((D_80147490 >> bit) & 1) ^ 1;
    if (absent) {
        return;
    }
    if (func_800A8AA0(id, 0) != 0) {
        return;
    }
    unit = func_800A85A0(id, 1);
    if (func_800A8974(unit) != 0) {
        return;
    }
    func_800EE718(unit, 1);
    /* Keep the -0x84 virtual adjustment inside the complete unit, not its component subobject. */
    component = (u8 *)unit + 0x84;
    unit->component84.vtable->destroy08.call(
        component + unit->component84.vtable->destroy08.delta, 1);
    if (position->y != 0) {
        func_800A58FC(unit, position);
    }
    D_80147490 &= ~(1 << bit);
}
