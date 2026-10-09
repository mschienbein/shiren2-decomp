#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { s32 index; } Iter800B6F58;
typedef struct { s32 x, y; } Pos800B6F58;

/* Entity +0x24 table: slot +0x1C takes only the adjusted receiver. */
typedef struct {
    u8 pad_00[0x18];
    s16 delta_18;
    s16 index_1A;
    void (*reset_1C)(void *self);
} Vtbl800B6F58;

/* Sub-object at +0x84 whose second word selects the units handled here. */
typedef struct { s32 field_0, mode; } Member800B6F58;

typedef struct {
    Pos800B6F58 pos;
    u8 pad_08[0x1C];
    Vtbl800B6F58 *vtbl;
    u8 pad_28[0x5C];
    Member800B6F58 member;
} Unit800B6F58;

/* Whole 12-byte selection record; flags byte at +3. */
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

extern Unit800B6F58 *D_801476B8;
s32 func_800A8FC8(Iter800B6F58 *it, s32 kind);
void *func_800A910C(Iter800B6F58 *it);
s32 func_800A5D2C(void *object, Pos800B6F58 *pos, s32 distance);
void func_800A58FC(void *actor, Pos800B6F58 *pos);
s32 func_800A5B98(Unit800B6F58 *unit, Pos800B6F58 *out);

/* ODD_C: nullable conversion to the +0x84 sub-object (keeps the original null check). */
static inline Member800B6F58 *member_of(Unit800B6F58 *unit) {
    return unit ? &unit->member : 0;
}

/* The caller supplies a receiver that this function does not use. */
void func_800B6F58(Unit800B6F58 *unused) {
    Pos800B6F58 player, nearby;
    Iter800B6F58 iter;

    if ((D_80142F18.flags >> 2) & 1) return;
    iter.index = 0;
    while (func_800A8FC8(&iter, 8)) {
        Unit800B6F58 *unit = func_800A910C(&iter);
        /* ODD_C: separate receiver for the virtual call; the original keeps this copy in its
         * own register. */
        Unit800B6F58 *self = unit;
        Member800B6F58 *member = member_of(unit);
        Pos800B6F58 *target;

        if ((member->mode ^ 1) != 0) continue;
        target = &player;
        target->x = D_801476B8->pos.x;
        target->y = D_801476B8->pos.y;
        if (func_800A5D2C(unit, target, 10)) func_800A58FC(unit, target);
        else if (func_800A5B98(unit, &nearby)) func_800A58FC(unit, &nearby);
        else self->vtbl->reset_1C((u8 *)self + self->vtbl->delta_18);
    }
}
