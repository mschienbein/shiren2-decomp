#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x10];
    s16 delta_10;
    s16 pad12;
    s32 (*func_14)(void *self); /* nonzero: cursed / cannot be taken */
} ItemVTable;

typedef struct {
    u8 type;
    u8 kind;
    u8 flags2;         /* 0x02 */
    u8 pad3[5];
    ItemVTable *vtable_8;
} Item;

typedef struct {
    u8 pad0[0x20];
    s16 delta_20;
    s16 pad22;
    s32 (*func_24)(void *self);               /* item count */
    u8 pad28[0x38 - 0x28];
    s16 delta_38;
    s16 pad3A;
    void *(*func_3C)(void *self, u32 index);  /* item at index */
} ListVTable;

typedef struct {
    u8 pad0[4];
    ListVTable *vtable_4;
} ItemList;

typedef struct {
    u8 pad0[0x98];
    s16 delta_98;
    s16 pad9A;
    void *(*func_9C)(void *self); /* the unit's item list */
} UnitVTable;

typedef struct {
    u8 pad0[0x1E];
    u8 flags1E;        /* 0x1E */
    u8 pad1F[0x24 - 0x1F];
    UnitVTable *vtable_24;
    u8 pad28[0x72 - 0x28];
    u8 flags72;        /* 0x72 */
} Unit;

/* Item-list iterator. */
typedef struct {
    s32 index;
    ItemList *list;
    s32 direction;
    Item *current;
} ListIter;

extern u8 D_80147620[]; /* random number generator state */

s32 func_800A674C(Unit *self, Unit *other);
s32 func_800A7BE4(Unit *target, void *source);
void func_800E20F0(Unit *unit);
s32 func_800E0F40(Unit *unit);
ListIter *func_800CEB20(ListIter *it, void *list);
void func_800CEB54(ListIter *it);
s32 func_800CEBA0(ListIter *it);
Item *func_800CEC68(ListIter *it);
s32 func_800FA67C(Unit *self, Item *item);
u8 func_800C57CC(void *rng, s32 limit);
void func_800CD304(ItemList *list, u32 index);
Item *func_800F1568(Unit *unit, s32 force);
Item *func_800FA5FC(Unit *self);

static inline s32 can_take(Unit *self, Item *item, s32 level)
{
    s32 ok = 0;
    if (func_800FA67C(self, item)) {
        if (((u8)level == 3 && (item->flags2 & 4) &&
             item->vtable_8->func_14((u8 *)item + item->vtable_8->delta_10) == 0) ||
            ((u8)level < 3 && !(item->flags2 & 4))) {
            ok = 1;
        }
    }
    return ok;
}

static inline s32 iterator_index(ListIter *it)
{
    return it->index;
}

/* Steal one item from `target`; returns the item or null. */
Item *func_800FA298(Unit *self, Unit *target)
{
    ListIter it;
    ItemList *list;
    u8 pick;
    Item *item;
    s32 count;
    s32 level;
    s32 cursed;
    s32 busy;

    busy = func_800A674C(self, target) != 1;
    if (busy) {
        return 0;
    }
    func_800A7BE4(target, self);
    item = 0;
    if (target->flags72 & 1) {
        func_800E20F0(target);
    }
    if (target->flags1E & 0xC) {
        list = target->vtable_24->func_9C((u8 *)target + target->vtable_24->delta_98);
        if (list != 0) {
            count = list->vtable_4->func_24((u8 *)list + list->vtable_4->delta_20);
            if (count == 0) {
                return 0;
            }
            level = func_800E0F40(self);
            pick = 0;
            func_800CEB20(&it, list);
            while (func_800CEBA0(&it)) {
                item = func_800CEC68(&it);
                if (can_take(self, item, level)) {
                    pick++;
                }
            }
            if (pick == 0) {
                if ((u8)level < 3) {
                    return 0;
                }
                pick = func_800C57CC(D_80147620, (u8)(count - 1));
            } else {
                pick = func_800C57CC(D_80147620, (u8)(pick - 1));
                func_800CEB54(&it);
                while (func_800CEBA0(&it)) {
                    item = func_800CEC68(&it);
                    if (can_take(self, item, level)) {
                        if ((u8)--pick == 0xFF) {
                            pick = iterator_index(&it) + 1;
                            break;
                        }
                    }
                }
            }
            item = list->vtable_4->func_3C((u8 *)list + list->vtable_4->delta_38, pick);
            cursed = 0;
            if (item->flags2 & 4) {
                cursed = item->vtable_8->func_14((u8 *)item + item->vtable_8->delta_10) != 0;
            }
            if (cursed) {
                return 0;
            }
            func_800CD304(list, pick);
            return item;
        }
    } else if ((target->flags1E >> 4) & 1) {
        item = func_800F1568(target, 0);
        if (item != 0) {
            target->flags72 &= ~8;
            return item;
        }
    }
    if (target->flags72 & 8) {
        item = func_800FA5FC(self);
        if (item != 0) {
            target->flags72 &= ~8;
        }
    }
    return item;
}
