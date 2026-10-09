#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x10];
    u16 field_10;
    u8 pad12[2];
    s32 field_14;
} Item;

typedef struct {
    u8 pad0[0x58];
    void *value58;
    u8 pad5C[0x89 - 0x5C];
    u8 field_89;
    u8 pad8A[0x9A - 0x8A];
    u16 flags9A;
} Monster;

s32 func_800F10F8(Monster *obj, void *arg1, s32 arg2, s32 arg3, s32 arg4);
void *func_800A6BA4(void *obj, s32 range, s32 ignore_terrain);
s32 func_800E0F40(Monster *obj);
s32 func_800A692C(Monster *obj, s32 kind);
s32 func_800A44F4(void *self, void *target);
void *func_8011D378(void);
void *func_8011D3D8(void);
void *func_80123630(void);
/* Returns 0 or 1; the callers test the negated flag as `result ^ 1`. */
s32 func_800AC670(Item *obj);
s32 func_80100340(Monster *self, Item *item, u16 mode);

/* Actor vtable slot 0xB4 override (D_8015B1E8+0xB4). Slot callers (func_801004F0 at
 * 0x80100528, func_800F27A4 at 0x800F2B28) pass the receiver and a target in a1; this
 * override never reads the target. */
s32 func_8010055C(Monster *m, void *slot_target)
{
    Item *item;
    s32 mode;
    void *value = m->value58;
    s32 hidden = m->flags9A & 0x40;

    switch (func_800F10F8(m, value, 0, hidden != 0, 0)) {
    case 1:
        return 0;
    case 2: {
        Item *target = func_800A6BA4(m, 10, 0);
        s32 skip = (u8)func_800E0F40(m) < 3 || func_800A692C(m, 0x12)
            || (target != 0 && func_800A44F4(m, target) == 1 && (m->flags9A & 0x40));

        if (skip) {
            return 1;
        }
        break;
    }
    }
    mode = 0;
    switch ((u8)func_800E0F40(m)) {
    case 1:
        item = func_8011D378();
        break;
    case 2:
        item = func_8011D3D8();
        break;
    default:
        item = func_80123630();
        if ((func_800AC670(item) ^ 1) != 0) {
            mode = 0x401;
            item->field_10 = m->field_89;
            item->field_14 = 0;
        }
        break;
    }
    if ((func_800AC670(item) ^ 1) != 0) {
        return func_80100340(m, item, mode);
    }
    return 0;
}
