#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct VEntry { s16 delta; s16 index; s32 (*func)(void *); } VEntry;
typedef struct { const void *primary; VEntry *vtable; u8 pad_08[0x10]; void *owner_18; } Part;
typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[3];
    s8 unk5;
    u8 pad6[6];
    Part unkC;
} Obj;
typedef struct { s32 index; void *collection; s32 position; void *current; } Iter;
extern u16 D_80157740[];
extern u8 D_80157788[];
u8 func_800AE98C(Obj *);
s32 func_800AC584(u16);
void *func_8011422C(Obj *);
Iter *func_800CEB20(Iter *, void *);
s32 func_800CEBA0(Iter *);
Obj *func_800CEC68(Iter *);
s32 func_800AE9AC(Obj *, s32, s32);

/* unk5 is the object's category; -1 means it has none. */
static __inline__ s8 category(Obj *obj) {
    return obj->unk5;
}
static __inline__ s32 hasCategory(Obj *obj) {
    return ~obj->unk5;
}
static __inline__ s32 isTarget(s32 mode, Obj *obj) {
    s32 hit = 0;
    if (mode == 1 || (mode == 2 && hasCategory(obj)) || (mode == 3 && category(obj) == 0) ||
        (mode == 4 && category(obj) == 1) || (mode == 5 && category(obj) == 2)) {
        hit = 1;
    }
    return hit;
}
/* Value (price) of a kind-9 container item: its own value plus that of the
 * contained items selected by mode; the sole caller, func_800AE9AC, keeps it in
 * its signed price.  The arithmetic is unsigned (the original divides with multu). */
s32 func_80114800(Obj *self, s32 mode) {
    u8 kind = func_800AE98C(self);
    u32 base = func_800AC584(D_80157740[kind]);
    u32 total;
    Iter it;
    Obj *obj;
    Part *part;

    part = &self->unkC;
    total = base + base * D_80157788[kind] * part->vtable[2].func((u8 *)part + part->vtable[2].delta) / 100;
    if (self->unk1 == 0xAB) {
        if (mode < 2 || (mode == 2 && hasCategory(self)) || (mode == 3 && category(self) == 0) ||
            (mode == 4 && category(self) == 1) || (mode == 5 && category(self) == 2)) {
            return base;
        }
        return 0;
    }
    if (mode >= 2) {
        if (!hasCategory(self) || (mode == 3 && category(self) != 0) ||
            (mode == 4 && category(self) != 1) || (mode == 5 && category(self) != 2)) {
            total = 0;
        }
    }
    if (mode == 0) {
        return total;
    }
    func_800CEB20(&it, func_8011422C(self));
    while (func_800CEBA0(&it)) {
        obj = func_800CEC68(&it);
        if (isTarget(mode, obj)) {
            total += func_800AE9AC(obj, 0, 0);
        }
    }
    return total;
}
