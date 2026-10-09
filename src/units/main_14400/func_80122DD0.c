#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Unit +0x24 table: slot +0x14 reports whether the unit is unaffected. */
typedef struct {
    u8 pad0[0x10];
    short offset_10;
    short pad12;
    s32 (*fn_14)(void *self);
} UnitVTable;

typedef struct {
    s32 x;
    s32 y;
    u8 dir;             /* 0x08 */
    u8 pad09[0x13];
    u16 flags;          /* 0x1C */
    u8 pad1E[6];
    UnitVTable *vtable; /* 0x24 */
} Unit;

/* Event delivered to an item's +0x3C handler (type 0x13: wave the staff at a unit). */
typedef struct {
    s32 type;           /* 0x00 */
    Unit *user;         /* 0x04 */
    Unit *target;       /* 0x08 */
    u8 dir;             /* 0x0C */
    u8 pad0D[0xB];
    s32 arg18;          /* 0x18 */
    Unit *source;       /* 0x1C */
    u8 sourceDir;       /* 0x20 */
    u8 pad21[7];
} Event;

typedef struct {
    u8 pad0[8];
    short offset_08;
    short pad0A;
    void (*destroy)(void *self, s32 flags);         /* 0x0C */
    u8 pad10[0x28];
    short offset_38;
    short pad3A;
    s32 (*handle)(void *receiver, void *event);     /* 0x3C */
} ItemVTable;

typedef struct {
    u8 kind;            /* 0x00 */
    u8 pad1[3];
    u8 cost;            /* 0x04 */
    u8 pad5[3];
    ItemVTable *vtable; /* 0x08 */
} Item;

typedef struct { s32 w[4]; } Area;
typedef struct { s32 w[8]; } Iter;

extern s32 D_80140160[];
extern Unit *D_801476B8;
s32 func_800A99D0(void);
void func_80094DA0(void *arg0);
void func_80114268(void *obj, s32 amount);
void func_800D3650(void *item);
/* Returns its rectangle by value through the hidden result pointer. */
Area func_800B3080(void *pos);
Iter *func_800A9204(Iter *it, Area *area, void *pos);
s32 func_800A9284(Iter *it, s32 mask);
Unit *func_800A942C(Iter *it);
u16 func_800E08B0(void *unit);
s32 func_800A08D8(s32 mode, s32 key, s32 sel);

/* Units flagged with bit 0 cannot be targeted. */
static inline s32 is_untouchable(Unit *unit)
{
    return unit->flags & 1;
}

/*
 * Use a staff item: unless it is the wrong kind or use is blocked, pay its
 * cost from `self`, consume it, send event 0x13 to every targetable unit
 * around `user` (stopping if the player dies), then to the user itself, and
 * destroy the item.  Returns the item when it was not used, otherwise 0.
 */
Item *func_80122DD0(void *self, Unit *user, Item *item)
{
    s32 refused = 0;
    Iter iter;
    Area area;
    Event event;

    if (item->kind != 1 || func_800A99D0()) {
        refused = 1;
    }
    if (refused) {
        return item;
    }
    func_80094DA0(D_80140160);
    func_80114268(self, -item->cost);
    func_800D3650(item);
    area = func_800B3080(user);
    func_800A9204(&iter, &area, user);
    while (func_800A9284(&iter, 0x7C)) {
        Unit *unit = func_800A942C(&iter);
        Unit *target = unit;
        u8 dir;
        s32 ok = 0;

        if (func_800E08B0(unit)) {
            ok = !is_untouchable(unit);
        }
        if (!ok) {
            continue;
        }
        dir = (target->dir + 4) & 7;
        event.type = 0x13;
        event.user = user;
        event.target = target;
        event.arg18 = 0;
        event.source = user;
        event.sourceDir = dir;
        event.dir = dir;
        item->vtable->handle((char *)item + item->vtable->offset_38, &event);
        func_800A08D8(1, -1, 0);
        if (!func_800E08B0(D_801476B8)) {
            if (item) {
                item->vtable->destroy((char *)item + item->vtable->offset_08, 3);
            }
            return 0;
        }
    }
    {
        s32 ok = 0;
        if (!user->vtable->fn_14((char *)user + user->vtable->offset_10)) {
            ok = !is_untouchable(user);
        }
        if (ok) {
            event.type = 0x13;
            event.user = user;
            event.target = user;
            event.dir = user->dir;
            event.arg18 = 0;
            event.source = user;
            item->vtable->handle((char *)item + item->vtable->offset_38, &event);
            func_800A08D8(1, -1, 0);
        }
    }
    if (item) {
        item->vtable->destroy((char *)item + item->vtable->offset_08, 3);
    }
    return 0;
}
