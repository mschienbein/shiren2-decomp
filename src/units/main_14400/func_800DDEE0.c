#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

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
    Pos pos;
    u8 pad8[0x20 - 0x8];
    UnitFlags flags;
} Unit;

/* Square iterator filled by func_800C5280 (0x18 bytes). */
typedef struct {
    u8 data[0x18];
} Iterator;

typedef struct {
    short delta;
    short index;
} VAdjust;

/* Collection table at +4: +0x24 count, +0x3C get, +0x64 add. */
typedef struct {
    u8 pad0[0x20];
    VAdjust countAdj;
    s32 (*count)(void *self);
    u8 pad28[0x38 - 0x28];
    VAdjust getAdj;
    void *(*get)(void *self, u32 index);
    u8 pad40[0x60 - 0x40];
    VAdjust addAdj;
    s32 (*add)(void *self, void *item, s32 verbose);
} ListVTable;

typedef struct {
    s32 field_0;
    ListVTable *vt;
} List;

typedef struct {
    u8 pad0[0xB0];
    List list;
} Obj;

extern Unit *D_801476B8;

void *func_800EB9FC(Unit *unit);
char *func_80048480(u16 id);
void func_800498E4(s32 id, ...);
s32 func_800A692C(Unit *unit, s32 kind);
void *func_800C5280(Iterator *it, Pos *origin, u16 radius);
s32 func_800C559C(Iterator *it);
Pos *func_800C532C(Pos *out, Iterator *it);
s32 func_800AD714(void *item, Pos *pos);
s32 func_800CD2BC(List *list, void *item);
s32 func_800AD8AC(void *item, Pos *pos);

static inline s32 flags_bit23(UnitFlags *flags)
{
    return flags->bit23;
}

static inline void copy_pos(Pos *out, Pos *in)
{
    out->x = in->x;
    out->y = in->y;
}

/* Scatter the list's items onto the free squares around the player. */
s32 func_800DDEE0(Obj *obj)
{
    Iterator it;
    Pos pos;
    UnitFlags flags;
    Unit *player = D_801476B8;
    Unit *unit;
    s32 blocked;
    s32 placed;
    u32 index;
    List *list;
    void *item;
    s32 accepted;
    s32 exhausted;

    if (func_800EB9FC(player)) {
        func_800498E4(0xB6, func_80048480(0x46A));
        return 0;
    }
    unit = D_801476B8;
    flags = unit->flags;
    blocked = !flags_bit23(&flags) && func_800A692C(unit, 0x12);
    if (blocked) {
        func_800498E4(0x113);
        return 0;
    }
    placed = 0;
    index = 0;
    copy_pos(&pos, &player->pos);
    func_800C5280(&it, &pos, 2);
    list = &obj->list;
    while (index < list->vt->count((u8 *)list + list->vt->countAdj.delta)) {
        item = list->vt->get((u8 *)list + list->vt->getAdj.delta, index);
        accepted = list->vt->add((u8 *)list + list->vt->addAdj.delta, item, 0) == 1;
        if (!accepted) {
            index++;
            continue;
        }
        while (func_800C559C(&it)) {
            func_800C532C(&pos, &it);
            if (func_800AD714(item, &pos)) {
                func_800CD2BC(&obj->list, item);
                func_800AD8AC(item, &pos);
                placed++;
                break;
            }
        }
        exhausted = func_800C559C(&it) != 1;
        if (exhausted) {
            break;
        }
    }
    if (placed > 0) {
        func_800498E4(0x7D, placed);
    } else {
        func_800498E4(0x7E, placed);
    }
    return 0;
}
