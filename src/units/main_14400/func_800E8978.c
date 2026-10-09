#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x18];
    s16 delta_18;
    s16 pad1A;
    s32 (*func_1C)(void *self, s32 kind);
} ItemVTable;

typedef struct {
    u8 f0;
    u8 f1;
    u8 flags2;
    u8 pad3[5];
    ItemVTable *vtable_8;
} Item;

typedef struct {
    u8 pad0[0x90];
    s16 delta_90;
    s16 pad92;
    s32 (*func_94)(void *self, s32 a1, s32 a2, u8 a3, s32 a4);
    s16 delta_98;
    s16 pad9A;
    void *(*func_9C)(void *self); /* the unit's item list */
} UnitVTable;

typedef struct {
    u8 pad0[0x24];
    UnitVTable *vtable_24;
} Unit;

/* Item-list iterator (func_800CEB20 stores the list at +4 and the direction at +8;
 * func_800CEBA0 stores the current entry at +0xC). */
typedef struct {
    s32 index;
    void *list;
    s32 reverse;
    Item *current;
} ListIter;

s32 func_800E4454(Unit *unit);
ListIter *func_800CEB20(ListIter *it, void *list);
s32 func_800CEBA0(ListIter *it);
Item *func_800CEC68(ListIter *it);

/* First item in the unit's list with flag 4 that answers query 2, or null. */
Item *func_800E8978(Unit *self)
{
    Unit *unit = self;
    ListIter it;
    Item *item;
    void *list;
    s32 found;
    s32 blocked;

    blocked = 0;
    if (unit->vtable_24->func_94((u8 *)unit + unit->vtable_24->delta_90, 2, 9, 0, 0)) {
        blocked = 1;
    } else if (func_800E4454(unit)) {
        blocked = 1;
    }
    if (blocked) {
        return 0;
    }
    list = unit->vtable_24->func_9C((u8 *)unit + unit->vtable_24->delta_98);
    if (list == 0) {
        return 0;
    }
    func_800CEB20(&it, list);
    while (func_800CEBA0(&it)) {
        item = func_800CEC68(&it);
        found = 0;
        if (item->flags2 & 4) {
            found = item->vtable_8->func_1C((u8 *)item + item->vtable_8->delta_18, 2) != 0;
        }
        if (found) {
            return item;
        }
    }
    return 0;
}
