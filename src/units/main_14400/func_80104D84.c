#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    s16 delta;
    s16 index;
    void *fn;
} VtableEntry;

/* List vtable: slot 4 = element count, slot 7 = element at index. */
typedef struct {
    VtableEntry entries0[4];
    struct {
        s16 delta;
        s16 index;
        s32 (*fn)(void *list);
    } count;
    VtableEntry entries5[2];
    struct {
        s16 delta;
        s16 index;
        void *(*fn)(void *list, u32 index);
    } get;
} ListVtable;

typedef struct {
    s32 field_0;
    ListVtable *vtable;
} List;

/* Unit vtable slot 19: inventory list. */
typedef struct {
    u8 pad00[0x98];
    s16 inventory_delta;
    s16 inventory_index;
    List *(*inventory)(void *self);
} UnitVtable;

typedef struct {
    u32 pad0 : 21;
    u32 bit10 : 1;
    u32 pad1 : 10;
} Flags;

typedef struct {
    Pos pos;
    u8 pad08[0x1E - 0x8];
    u8 flags_1E;
    u8 pad1F;
    Flags flags_20;
    UnitVtable *vtable;
} Unit;

extern u8 D_80147620[];

extern s32 func_800F069C(void *obj);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800E3678(Unit *obj, Unit *attacker);
extern s32 func_80104C9C(void *object, Unit *context, s32 desired);
extern void func_80049AE8(s32, ...);
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern char *func_800A3B20(Unit *u);
extern void func_800CD304(List *list, u32 value);
extern s32 func_80111578(Item *item);
extern void func_800497F0(s32, ...);
extern s32 func_800A08D8(s32 mode, s32 key, s32 sel);
extern s32 func_800E0F40(Unit *obj);
extern u8 func_800C57CC(void *rng, s32 limit);
extern void *func_801142D0(Item *obj, u32 index);
extern void func_800D3650(void *arg);
extern s32 func_800CD278(ItemPot *list);
extern void func_80114268(Item *obj, s32 arg1);
extern s32 func_800AE9AC(Item *item, s32, s32);
extern s32 func_8010BCB0(Item *p, signed char delta);
extern void func_800D3698(s32, s32);
extern s32 func_800ADC90(void *obj, void *pos, void *origin);

static inline void copy_pos(Pos *to, Pos *from) {
    to->x = from->x;
    to->y = from->y;
}

/* The seal count is read sign-extended here (lb at 0x80105048); the field is unsigned
 * (func_8010BE60 reads it with lbu, as does the capacity test at 0x80105078). */
static inline s8 seal_count(Item *item) {
    return item->payload.seals.count0F;
}

/* Steal action: take a random stealable item from target's inventory. */
s32 func_80104D84(Unit *self, Unit *target) {
    struct {
        Pos pos;
        Flags flags;
    } local;
    s32 blocked = 0;
    s32 guarded;
    s32 name;
    s32 index;
    List *inventory;
    Item *item;

    if (target == 0 || !func_800F069C(self) || !(target->flags_1E & 0xC)
        || !target->vtable->inventory((u8 *)target + target->vtable->inventory_delta)) {
        blocked = 1;
    }
    if (blocked) {
        return 0;
    }
    name = func_80049CB4(0x57, self);
    func_800E3678(target, self);
    copy_pos(&local.pos, &self->pos);
    index = func_80104C9C(self, target, 0);
    if (index == 0 || (guarded = target->flags_20.bit10, local.flags = target->flags_20, guarded)) {
        func_80049AE8(0x14B, name);
        return 1;
    }
    index = func_80104C9C(self, target, (u8)func_800C5844(D_80147620, 1, index));
    inventory = target->vtable->inventory((u8 *)target + target->vtable->inventory_delta);
    item = inventory->vtable->get.fn((u8 *)inventory + inventory->vtable->get.delta, index);
    func_80049AE8(0x147, name, func_800A3B20(self), func_800AE674(item));
    func_800CD304(inventory, index);
    switch (item->field00) {
    case 7:
        if (func_80111578(item)) {
            func_800497F0(0x148, name);
            func_800A08D8(1, name, 0);
        }
        break;
    case 9: {
        Item *pot = item;

        if ((u8)func_800E0F40(self) == 3) {
            ItemPot *contents = &item->payload.pot;
            s32 count = contents->list.methods->count24((u8 *)&contents->list + contents->list.methods->delta20);

            if (count != 0) {
                Item *inner = func_801142D0(item, func_800C57CC(D_80147620, (u8)(count - 1)));

                func_800D3650(inner);
                if (inner != 0) {
                    inner->field08->method0C((u8 *)inner + inner->field08->delta08, 3);
                }
                if (item->field01 == 0xAC) {
                    item->payload.pot.field2C = 0xFF;
                }
            }
        }
        if (func_800CD278(&pot->payload.pot) > 0) {
            func_800497F0(0x149, name);
            func_800A08D8(1, name, 0);
            func_80114268(pot, -1);
        }
        break;
    }
    case 3:
    case 4: {
        s32 value = func_800AE9AC(item, 0, 0);
        s32 bonus = seal_count(item);

        if (bonus > 0) {
            func_8010BE60(item, func_800C57CC(D_80147620, (u8)(bonus - 1)));
        }
        if ((s8)(item->payload.seals.capacity0E - item->payload.seals.count0F) > 0) {
            func_800497F0(0x14A, name);
            func_800A08D8(1, name, 0);
            func_8010BCB0(item, -1);
        }
        if (~item->owner05) {
            func_800D3698(item->owner05, value - func_800AE9AC(item, 0, 0));
        }
        break;
    }
    }
    func_800ADC90(item, &local.pos, &local.pos);
    return 1;
}
