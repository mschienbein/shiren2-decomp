#include "common.h"

typedef unsigned char u8;
typedef struct Ent Ent;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    short delta;
    short index;
} VAdjust;

/* Owner vtable view: slots 12 (+0x60) and 13 (+0x68); both return 0/1. */
typedef struct {
    u8 pad0[0x60];
    VAdjust canRemove;
    s32 (*canRemoveFn)(void *self, Ent *ent, s32 flag);
    VAdjust canSwap;
    s32 (*canSwapFn)(void *self, Ent *out, Ent *in, s32 flag);
} OwnerVTable;

typedef struct {
    s32 field_0;
    OwnerVTable *vt;
} Owner;

/* Item entity: category byte, then item id. */
struct Ent {
    u8 kind;
    u8 id;
};

typedef struct {
    Owner *owner;
    Ent *ent;
} Request;

typedef struct {
    u8 pad0[0x8];
    Request give;
    Request take;
} Obj;

typedef struct {
    unsigned bits_31_24 : 8;
    unsigned bit23 : 1;
    unsigned bits_22_0 : 23;
} UnitFlags;

typedef struct {
    Pos pos;
    u8 dir;
    u8 pad9[0x20 - 0x9];
    UnitFlags flags;
} Unit;

extern Unit *D_801476B8;

static inline s32 flags_bit23(UnitFlags *flags)
{
    return flags->bit23;
}

s32 func_800A692C(Unit *unit, s32 kind);
void func_800498E4(s32 id, ...);
s32 func_80049CB4(s32 id, ...);
char *func_800AE674(void *obj);
/* Returns the removed item or null (0x800D02DC/0x800D02E0); the result is unused here. */
void *func_800D02AC(Request *link);
void func_800CD984(Owner *self, void *element, void *replacement);
s32 func_800CD538(void *self, Ent *obj);
Ent *func_800A6DC8(Unit *u);
void func_801269C4(Ent *item, u8 *dir);

/* Swap the two request entities between their owners; returns whether both have kind 0x10. */
s32 func_800DB4F0(Obj *obj)
{
    s32 failed = 0;
    Unit *player;
    UnitFlags flags;
    s32 blocked;
    Request *give;
    Ent *taken;
    Ent *given;
    Ent *held;

    if (!obj->take.owner->vt->canRemoveFn((u8 *)obj->take.owner + obj->take.owner->vt->canRemove.delta, obj->take.ent, 1)
        || !obj->give.owner->vt->canRemoveFn((u8 *)obj->give.owner + obj->give.owner->vt->canRemove.delta, obj->give.ent, 1)) {
        failed = 1;
    }
    if (failed) {
        return 0;
    }
    player = D_801476B8;
    flags = player->flags;
    blocked = !flags_bit23(&flags) && func_800A692C(player, 0x12);
    if (blocked) {
        func_800498E4(0x113);
        return 0;
    }
    give = &obj->give;
    taken = obj->take.ent;
    given = give->ent;
    if ((obj->take.owner->vt->canSwapFn((u8 *)obj->take.owner + obj->take.owner->vt->canSwap.delta, taken, given, 0) ^ 1) != 0) {
        return 1;
    }
    if ((obj->give.owner->vt->canSwapFn((u8 *)obj->give.owner + obj->give.owner->vt->canSwap.delta, given, taken, 1) ^ 1) != 0) {
        return 1;
    }
    func_800498E4(0x77, func_800AE674(given), func_800AE674(taken));
    func_800D02AC(give);
    func_800CD984(obj->take.owner, taken, given);
    func_800CD538(obj->give.owner, taken);
    held = func_800A6DC8(D_801476B8);
    if ((held == taken || held == given) && held->id == 0xE7) {
        Pos pos;
        Pos *p;

        func_801269C4(held, &D_801476B8->dir);
        p = &pos;
        p->x = D_801476B8->pos.x;
        p->y = D_801476B8->pos.y;
        func_80049CB4(0xD7, p);
    }
    return taken->kind == 0x10 && given->kind == 0x10;
}
