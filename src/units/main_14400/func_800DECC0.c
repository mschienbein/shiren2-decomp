#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u32 high : 8; u32 masked : 1; u32 low : 23; } Flags;
typedef struct { u8 pad0[0x20]; Flags flags_20; } Player;

/* Item-collection vtable at +4 ({delta, index, pfn} entries). */
typedef struct {
    u8 pad0[0x20];
    s16 count_delta;
    s16 count_index;
    s32 (*count_24)(void *self);
    u8 pad28[0x10];
    s16 get_delta;
    s16 get_index;
    void *(*get_3C)(void *self, u32 index);
    u8 pad40[0x20];
    s16 add_delta;
    s16 add_index;
    s32 (*add_64)(void *self, void *item, s32 verbose);
} CollectionVTable;

typedef struct {
    s32 field_00;
    const CollectionVTable *vtable;
} Collection;

typedef struct {
    u8 pad0[0xB0];
    Collection items_B0;
} Object;

extern Player *D_801476B8;
void *func_800EB9FC(Player *player);
char *func_80048480(u16 id);
void func_800498E4(s32 id, ...);
s32 func_800A692C(void *object, s32 code);
s32 func_800AE9AC(void *item, s32 mode, s32 amount);
void func_80045A24(s32 id);
void func_800CD364(Collection *collection, void *item);

static inline s32 isMasked(Flags *flags) {
    return flags->masked;
}

static inline s32 canRemove(Collection *items, void *item) {
    s32 ok = 0;
    if (items->vtable->add_64((u8 *)items + items->vtable->add_delta, item, 0)) {
        ok = func_800AE9AC(item, 2, 0) == 0;
    }
    return ok;
}

s32 func_800DECC0(Object *object) {
    Flags flags;
    s32 blocked;
    s32 removed;
    u32 index;
    Collection *items;
    void *item;
    void *victim;

    if (func_800EB9FC(D_801476B8)) {
        func_800498E4(0xB6, func_80048480(0x46A));
        return 0;
    }
    blocked = 0;
    flags = D_801476B8->flags_20;
    if (!isMasked(&flags)) {
        blocked = func_800A692C(D_801476B8, 0x12) != 0;
    }
    if (blocked) {
        func_800498E4(0x113);
    } else {
        removed = 0;
        index = 0;
        items = &object->items_B0;
        while (index < items->vtable->count_24((u8 *)items + items->vtable->count_delta)) {
            item = items->vtable->get_3C((u8 *)items + items->vtable->get_delta, index);
            victim = item;
            if (canRemove(items, item)) {
                func_800CD364(items, victim);
                removed++;
            } else {
                index++;
            }
        }
        if (removed > 0) {
            func_800498E4(0x80, removed);
            func_80045A24(0xE);
        } else {
            func_800498E4(0x81);
        }
    }
    return 0;
}
