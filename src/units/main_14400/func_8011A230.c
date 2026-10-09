#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    s32 x;
    s32 y;
} Pos;

typedef struct {
    u8 pad0;
    u8 kind;        /* 0x01 */
    u8 pad2[0xB];
    u8 flagsD;      /* 0x0D */
    u8 padE[3];
    u8 charges;     /* 0x11 */
} Item;

typedef struct {
    u8 pad0[0x98];
    s16 delta_98;
    s16 pad9A;
    void *(*func_9C)(void *self); /* the unit's item list */
} UnitVTable;

typedef struct {
    Pos pos;
    u8 pad8[0x1E - 0x8];
    u8 flags1E;
    u8 pad1F[0x24 - 0x1F];
    UnitVTable *vtable_24;
} Unit;

/* Item-list iterator (func_800CEB20 family): index, collection, direction, current element. */
typedef struct {
    s32 index;
    void *collection;
    s32 reverse;
    void *current;
} ListIter;

extern u8 D_80147620[]; /* random number generator state */
extern u8 D_801569C3;

s32 func_80049CB4(s32 id, ...);
s32 func_800C587C(void *rng, u8 limit);
ListIter *func_800CEB20(ListIter *it, void *list);
s32 func_800CEBA0(ListIter *it);
Item *func_800CEC68(ListIter *it);
void func_800ACD34(Item *obj);
void func_800498E4(s32 id, ...);
char *func_800AE674(Item *item);
s32 func_800ACEB4(Item *item);

static inline void copy_pos(Pos *dst, Pos *src)
{
    dst->x = src->x;
    dst->y = src->y;
}

/* Vtable slot +0x44 (self, unit, item): `self` is the adjusted receiver, unused here. */
void func_8011A230(void *self, Unit *unit, Item *item)
{
    Pos pos;
    ListIter it;
    char *name;
    s32 lost;
    s32 kind;
    s32 cursed;

    if (item == 0) {
        return;
    }
    copy_pos(&pos, &unit->pos);
    func_80049CB4(0x11D, &pos);
    func_80049CB4(0x12C);
    lost = 0;
    if ((unit->flags1E >> 2) & 1) {
        lost = func_800C587C(D_80147620, D_801569C3) != 0;
    }
    if (lost) {
        func_800CEB20(&it, unit->vtable_24->func_9C((u8 *)unit + unit->vtable_24->delta_98));
        while (func_800CEBA0(&it)) {
            func_800ACD34(func_800CEC68(&it));
        }
        func_800ACD34(item);
        func_800498E4(0xCD);
        return;
    }
    name = func_800AE674(item);
    kind = item->kind;
    cursed = 1;
    if (kind == 0x99) {
        cursed = item->charges != 0;
    } else if (kind == 0x98) {
        cursed = item->charges != 0;
    } else if (kind == 0xF2) {
        cursed = item->flagsD >> 7;
    }
    if (cursed && func_800ACEB4(item) == 2) {
        func_800498E4(0xCC, name);
        return;
    }
    func_800ACD34(item);
    func_800498E4(0xCB, name, func_800AE674(item));
}
