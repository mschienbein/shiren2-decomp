#include "common.h"

typedef struct {
    s32 x;
    s32 y;
} Pos;

/* Only the leading map position is read here. */
typedef struct {
    Pos pos;
} Unit;

extern s32 func_800E20CC(void *obj);
extern s32 func_800F17A8(Unit *object, Pos *position, s32 force);
extern s32 func_800F0EC4(Unit *obj);
extern void *func_800B4D80(Pos *p);
extern void func_800AD868(Pos *pos);
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_801017A4(Unit *self, void *target);
extern s32 func_800ADC90(void *object, Pos *position, void *origin);
extern void func_800498E4(s32 id, ...);

/* Strikes from the unit's own square: the square is filled in place and
 * handed by address to every probe and report.
 * ODD_C: the by-address square parameter is the one pointer every call
 * receives (the original keeps it in s0); with direct &square uses GCC
 * rematerialises sp+16 at each call instead. */
static inline s32 strike_from(Unit *unit, Pos *square) {
    s32 usable = 0;
    void *target;
    s32 blocked;

    square->x = unit->pos.x;
    square->y = unit->pos.y;
    if (func_800F17A8(unit, square, usable)) {
        usable = func_800F0EC4(unit) == 0;
    }
    if (usable) {
        target = func_800B4D80(square);
        func_800AD868(square);
        func_80049CB4(0x10E1, square);
        blocked = func_801017A4(unit, target) == 1;
        if (!blocked) {
            func_800ADC90(target, square, square);
            func_800498E4(0x144);
        }
        return 1;
    }
    return 0;
}

/* Monster +0xB4 action slot (D_80159440 family): s32 (self, target). The caller-supplied
 * target is unused; the victim is whatever stands on this unit's square. */
s32 func_80101A48(Unit *unit, void *target_unused) {
    Pos square;
    s32 active;

    active = func_800E20CC(unit) == 1;
    if (active) {
        return strike_from(unit, &square);
    }
    return 0;
}
