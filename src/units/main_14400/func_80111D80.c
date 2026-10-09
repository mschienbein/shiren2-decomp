#include "common.h"

typedef unsigned char u8;

/* Temporary effect object built on the stack by func_80111E08 (0x38 bytes). */
typedef struct {
    u8 pad[0x1C];
    void *vtbl;
    u8 pad20[0x18];
} Obj;

typedef struct { s32 x, y; } Vec2;

/* Partial unit view: map position at +0x0 and facing direction byte at +0x8. */
typedef struct {
    Vec2 pos;
    u8 dir;
} Unit;

extern s32 func_80111A20(void *, void *);
extern Obj *func_80111E08(Obj *o, void *unit, void *item, Vec2 *pos, u8 *dir);
extern void func_800C2D0C(Obj *);

/* Item table slot +0x48/+0x4C: use the item on a unit. */
void func_80111D80(void *item, Unit *unit) {
    Obj tmp;

    if (func_80111A20(item, unit)) {
        func_80111E08(&tmp, unit, item, &unit->pos, &unit->dir);
        func_800C2D0C(&tmp);
    }
}
