#include "common.h"

typedef unsigned char u8;

typedef struct Rect Rect;

/* Active area record (func_800D4838's object). */
typedef struct {
    Rect *area;
    s32 kind;
    s32 unk8;
    s32 unkC;
} Obj;

typedef struct Unit {
    u8 pad0[0x1E];
    u8 flags1E;
    u8 pad1F[0x7B];
    u8 flags9A;
} Unit;

extern Obj D_80143434;
extern Unit *D_801476B8;

Unit *func_800C5F60(void);
s32 func_800A8F6C(s32 *iter);
Unit *func_800A910C(s32 *iter);
s32 func_800A44F4(void *self, void *target);
Rect *func_800B1F90(void *pos);
s32 func_800B2048(Rect *area);
void func_800D4838(Obj *obj);

static inline Rect *current_area(void) {
    return D_80143434.area;
}

static inline s32 area_changed(void) {
    return D_80143434.unk8;
}

s32 func_800B60E0(void) {
    s32 inactive;
    Unit *self;
    Unit *unit;
    s32 iter;
    s32 *cursor;
    s32 eligible;

    inactive = 0;
    if (current_area() == 0 || area_changed() != 0) {
        inactive = 1;
    }
    if (inactive) {
        return 0;
    }
    self = func_800C5F60();
    iter = 0;
    cursor = &iter;
    while (func_800A8F6C(cursor)) {
        unit = func_800A910C(cursor);
        if (unit != self && func_800A44F4(D_801476B8, unit) != 1) {
            continue;
        }
        eligible = 0;
        if (!((unit->flags1E >> 4) & 1) || !(unit->flags9A & 1)) {
            eligible = 1;
        }
        if (eligible && func_800B2048(func_800B1F90(unit))) {
            func_800D4838(&D_80143434);
            return 1;
        }
    }
    return 0;
}
