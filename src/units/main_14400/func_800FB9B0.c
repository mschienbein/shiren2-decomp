#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;

typedef struct {
    u8 kind;
    u8 pad1;
    u8 flags;
    u8 pad3[2];
    u8 slot;
} Item;

/* The iterator stores its collection and yielded item as genuine pointers. */
typedef struct { s32 index; void *collection; s32 reverse; Item *current; } Iter;

/* Container vtable (D_80154390 family): count at 0x24, item at 0x3C. */
typedef struct {
    u8 pad0[0x20];
    s16 count_delta;
    s16 count_index;
    s32 (*count)(void *self);
    u8 pad28[0x38 - 0x28];
    s16 item_delta;
    s16 item_index;
    Item *(*item)(void *self, u32 index);
} ItemSetVTable;

typedef struct {
    void *pool;
    ItemSetVTable *vtable;
} ItemSet;

/* Unit vtable: inventory at 0x9C, fallback action at 0xAC (func_800FB868 here). */
typedef struct {
    u8 pad0[0x98];
    s16 inventory_delta;
    s16 inventory_index;
    ItemSet *(*inventory)(void *self);
    u8 padA0[0xA8 - 0xA0];
    s16 fallback_delta;
    s16 fallback_index;
    s32 (*fallback)(void *self);
} UnitVTable;

typedef struct {
    u8 pad0[0x1E];
    u8 flags_1E;
    u8 pad1F[0x24 - 0x1F];
    UnitVTable *vtable;
} Unit;

extern void *D_801476B8;
extern unsigned char D_80147620[];

s32 func_800F069C(void *obj);
s32 func_800E0F40(Unit *obj);
s32 func_800F1040(Unit *u, Unit *o, s32 id);
s32 func_800A44F4(void *self, void *target);
s32 func_800FB7AC(Unit *self, Unit *target);
u8 func_800C57CC(void *rng, s32 limit);
extern s32 func_800AE9AC(Item *, s32, s32);
char *func_800AE674(void *obj);
void *func_8011422C(u8 *);
Iter *func_800CEB20(Iter *it, void *list);
extern s32 func_800CEBA0(Iter *);
extern void *func_800CEC68(Iter *);
void func_800D3650(void *arg);
void func_800CD468(void *list);
void func_8011541C(Item *obj);
void *func_800AC5F4(s32 size, Item *obj);
void *func_8011FEF0(void *o);
void func_800AE974(Item *p, s8 v);
extern void func_800D3698(s32, s32);
s32 func_80049CB4(s32 id, ...);
extern void func_80049AE8(s32, ...);
char *func_800A3B20(Unit *u);
extern void func_800497F0(s32, ...);
void func_800E3678(Unit *obj, Unit *attacker);
s32 func_800A08D8(s32 mode, s32 key, s32 sel);

s32 func_800FB9B0(Unit *self, Unit *target) {
    s32 ready;
    s32 result;

    ready = func_800F069C(self) == 1;
    if (!ready) {
        return self->vtable->fallback((u8 *)self + self->vtable->fallback_delta);
    }
    if ((u8)func_800E0F40(self) != 1) {
        result = func_800F1040(self, target, 0x54);
        if (result != 1) {
            Unit *victim;
            s32 blocked;
            ItemSet *items;
            s32 count;

            if (result == 2) {
                return 1;
            }
            if (func_800A44F4(target, D_801476B8) != 1) {
                return 0;
            }
            victim = target;
            switch ((u8)func_800E0F40(self)) {
            case 3:
                return func_800FB7AC(self, target);
            case 4:
                if (func_800FB7AC(self, target)) {
                    return 1;
                }
                break;
            }
            blocked = 0;
            if (!(victim->flags_1E & 0xC)
                || victim->vtable->inventory((u8 *)victim + victim->vtable->inventory_delta) == 0) {
                blocked = 1;
            }
            if (blocked) {
                return 0;
            }
            items = victim->vtable->inventory((u8 *)victim + victim->vtable->inventory_delta);
            count = items->vtable->count((u8 *)items + items->vtable->count_delta);
            if (count != 0) {
                Item *item;
                u8 slot;
                s32 before;
                char *item_name;
                s32 name;

                item = items->vtable->item((u8 *)items + items->vtable->item_delta,
                                           func_800C57CC(D_80147620, (u8)(count - 1)));
                if (item->flags & 4) {
                    return 0;
                }
                slot = item->slot;
                before = func_800AE9AC(item, 0, 0);
                item_name = func_800AE674(item);
                if (item->kind == 9) {
                    void *contents = func_8011422C((u8 *)item);
                    Iter it;

                    func_800CEB20(&it, contents);
                    while (func_800CEBA0(&it)) {
                        func_800D3650(func_800CEC68(&it));
                    }
                    func_800CD468(contents);
                    func_8011541C(item);
                }
                func_8011FEF0(func_800AC5F4(0xC, item));
                func_800AE974(item, (s8)slot);
                /* Slot byte 0xFF (-1) means unequipped. */
                if (~(s8)item->slot) {
                    func_800D3698((s8)slot, before - func_800AE9AC(item, 0, 0));
                }
                func_80049CB4(0x1131);
                func_80049CB4(6);
                name = func_80049CB4(0x54, self);
                func_80049CB4(7);
                func_80049AE8(0x126, name, func_800A3B20(self));
                func_80049CB4(6);
                func_80049CB4(0x23, target, 0, 0);
                func_80049CB4(7);
                func_800497F0(0x228, name, item_name, func_800AE674(item));
                func_800E3678(target, self);
                func_800A08D8(1, name, 0);
                return 1;
            }
        }
    }
    return 0;
}
