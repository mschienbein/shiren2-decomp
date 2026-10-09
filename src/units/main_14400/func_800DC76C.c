#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    unsigned bits_31_24 : 8;
    unsigned bit23 : 1;
    unsigned bits_22_0 : 23;
} UnitFlags;

typedef struct {
    s32 type;
    s32 field_04;
    void *object;
    s32 field_0c;
    s32 arg;
    s32 field_14;
} Event;

/* Entity table at +0x24: +0x5C dispatch. */
typedef struct {
    u8 pad0[0x58];
    short dispatchDelta;
    short dispatchIndex;
    s32 (*dispatch)(void *self, void *event);
} UnitVTable;

typedef struct {
    Pos pos;
    u8 dir;
    u8 pad9[0x20 - 0x9];
    UnitFlags flags;
    UnitVTable *vt;
} Unit;

/* Item table at +8: +0x1C query. */
typedef struct {
    u8 pad0[0x18];
    short queryDelta;
    short queryIndex;
    s32 (*query)(void *self, s32 kind);
} ItemVTable;

typedef struct {
    u8 kind;
    u8 pad1[7];
    ItemVTable *vt;
    u8 field_0c;
} Item;

typedef struct {
    void *owner;
    Item *item;
} Request;

typedef struct {
    u8 pad0[0x8];
    Request req;
} Obj;

extern SelectionRecord D_80142F18;
extern Unit *D_801476B8;

void func_800498E4(s32 id, ...);
s32 func_800A692C(Unit *unit, s32 kind);
s32 func_800CD7E0(void *list, Item *item, s32 notify);
s32 func_800E1CC4(Unit *unit, s32 kind);
void func_800A665C(Unit *unit, u8 *dir);
u32 func_800B1C6C(void *pos);
Pos *func_800A6CC0(Pos *out, Unit *unit);
u16 func_800AF1DC(Item *item, Unit *user, Unit *target, s32 *out);
Item *func_800D02F8(Request *req);

static inline s32 flags_bit23(UnitFlags *flags)
{
    return flags->bit23;
}

/* Tile attribute bits of a square; every attribute mask fits in 16 bits. */
static inline u16 tile_bits(Pos *pos, u16 mask)
{
    return func_800B1C6C(pos) & mask;
}

static inline s32 item_query(Item *item, s32 kind)
{
    return item->vt->query((u8 *)item + item->vt->queryDelta, kind);
}

s32 func_800DC76C(Obj *obj)
{
    Unit *player;
    UnitFlags flags;
    s32 blocked;
    Request *req;
    Item *item;
    Pos pos;
    Event event;
    u8 dir;
    s32 result;
    s32 slippery;

    if ((D_80142F18.flags >> 2) & 1) {
        return 1;
    }
    player = D_801476B8;
    flags = player->flags;
    blocked = !flags_bit23(&flags) && func_800A692C(player, 0x12);
    req = &obj->req;
    if (blocked) {
        func_800498E4(0x113);
        return 0;
    }
    if (!func_800CD7E0(obj->req.owner, req->item, 1)) {
        return 0;
    }
    item = req->item;
    if (item == 0) {
        return 0;
    }
    if (func_800E1CC4(D_801476B8, 4)) {
        dir = (D_801476B8->dir + 4) & 7;
        func_800A665C(D_801476B8, &dir);
    }
    slippery = 0;
    if (func_800B1C6C(D_801476B8) & 0x4000) {
        func_800A6CC0(&pos, D_801476B8);
        slippery = tile_bits(&pos, 0x4000) != 0;
    }
    if (slippery && !(func_800AF1DC(item, D_801476B8, D_801476B8, &result) & 0x20)) {
        s32 message;

        if (item_query(item, 0xC)) {
            message = 0x5D;
        } else if (item_query(item, 0xD)) {
            message = 0x5F;
        } else {
            message = 0x5E;
        }
        func_800498E4(message);
        return 1;
    }
    item = func_800D02F8(&obj->req);
    if (item->kind == 0x10) {
        item->field_0c |= 1;
    }
    event.type = 0xE;
    event.object = item;
    event.arg = 0;
    D_801476B8->vt->dispatch((u8 *)D_801476B8 + D_801476B8->vt->dispatchDelta, &event);
    return 0;
}
