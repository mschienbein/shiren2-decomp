#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { s32 x, y; } Pair;
typedef struct { Pair current, first, last; } GridIter;
typedef struct Table Table;
typedef struct Item Item;
typedef struct Unit Unit;

/* Common item-list header (func_800CE620): the slots read the item table at +0,
 * the entry indices through +8 and the entry count at +0xE (func_800CE710,
 * func_800CE7A0 via vtable +0x24/+0x3C). */
typedef struct {
    Table *pool;
    const void *vtable;
    u8 *entries;
    u8 stateC, stateD;
    u8 count;
    u8 padF;
} ItemList;

/* Pot contents built by func_800CFF00 at Item+0xC: eight inline entries at
 * +0x10 and the owning pot at +0x18 (0x1C bytes). */
typedef struct {
    ItemList list;
    u8 inlineEntries[8];
    Item *owner;
} PotContents;

/* Unit kind 0x5A inventory built by func_800CEC90 at Unit+0x8C (0x18 bytes):
 * owner at +0x10, message id at +0x14; its ten entries live at Unit+0x80. */
typedef struct {
    ItemList list;
    Unit *owner;
    unsigned short textId;
    u8 pad16[2];
} UnitInventory;

typedef struct {
    u8 pad0[8];
    s16 destroy_delta, destroy_index;
    void (*destroy)(void *self, s32 flags);
} ItemVTable;

struct Item {
    u8 kind;
    u8 pad1[7];
    ItemVTable *vtable8;
    PotContents contents;
    u8 pad28[8];
};

struct Table {
    Item *items;
    u8 *occupied;
    s32 count;
};

typedef struct {
    u8 pad0[0x98];
    s16 contents_delta, contents_index;
    void *(*contents)(void *self);
} UnitVTable;

/* Partial unit view through the kind-0x5A inventory (object allocated 0xB0 bytes
 * by func_800F6F60; this function reads only the fields below). */
struct Unit {
    u8 pad0[0xA];
    u8 kind;
    u8 padB[0x13];
    u8 flags1E;
    u8 pad1F[5];
    UnitVTable *vtable24;
    u8 pad28[0x58];
    u8 entries80[0xC];
    UnitInventory contents8C;
};

extern const unsigned char D_8015488C[8];
typedef struct { Pair first, last; } Rectangle;
extern Rectangle D_801429D0;

Item *func_800AFD78(Table *table, u8 index);
Pair *func_800A3610(Pair *out, GridIter *it);
void *func_800B4D80(Pair *p);
s32 func_800CD090(void *container, void *element);
s32 func_800A8F6C(s32 *iter);
void *func_800A910C(s32 *iter);

/* ODD_C: The grid constructor retains the original temporary-copy ordering. */
static inline void init_grid(GridIter *it, Pair *tmp, s32 x, s32 y, s32 lastX, s32 lastY) {
    tmp->x = x;
    tmp->y = y;
    it->first = *tmp;
    it->current = it->first;
    tmp->x = lastX;
    tmp->y = lastY;
    it->last = *tmp;
}

/* Remove an item only after checking the map, item containers, and unit inventories.
 * Item vtable slot 1 uses the destructor (self, s32 flags) contract, for example
 * D_8015D518 + 0xC -> func_801114FC. Unit slot 19 returns its inventory pointer. */
/* Bit 2 of flags1E marks a unit whose inventory comes from vtable slot 19. */
static inline signed char is_player(Unit *unit) { return (unit->flags1E >> 2) & 1; }

void func_800B0438(Table *table) {
    s32 i;
    GridIter grid;
    Pair pos;
    s32 unitIter;

    for (i = table->count; ;) {
        Item *item;
        Item *owner;
        s32 found;
        s32 j;
        Unit *unitOwner;
        GridIter *cursor;
        i--;
        if (i == -1) {
            break;
        }
        if (!(table->occupied[i >> 3] & D_8015488C[i & 7])) {
            continue;
        }
        item = func_800AFD78(table, (u8)i);
        owner = item;
        if (item == 0) {
            continue;
        }
        found = 0;
        cursor = &grid;
        init_grid(cursor, &pos, D_801429D0.first.x, D_801429D0.first.y, D_801429D0.last.x, D_801429D0.last.y);
        for (;;) {
            s32 more = grid.current.x <= cursor->last.x;
            if (!more) {
                break;
            }
            func_800A3610(&pos, cursor);
            if (func_800B4D80(&pos) == owner) {
                found = 1;
                break;
            }
        }
        if (found) {
            continue;
        }
        for (j = table->count; ;) {
            Item *container;
            j--;
            if (j == -1) {
                break;
            }
            if (!(table->occupied[j >> 3] & D_8015488C[j & 7])) {
                continue;
            }
            container = func_800AFD78(table, (u8)j);
            if (container != owner && container->kind == 9 &&
                func_800CD090(&container->contents, owner) != -1) {
                owner = container;
                break;
            }
        }
        unitOwner = 0;
        unitIter = 0;
        while (func_800A8F6C(&unitIter)) {
            Unit *unit = func_800A910C(&unitIter);
            void *contents = 0;
            if (unit->flags1E & 0xC) {
                contents = unit->vtable24->contents((u8 *)unit + unit->vtable24->contents_delta);
            } else if (unit->kind == 0x5A) {
                contents = &unit->contents8C;
            }
            if (contents && func_800CD090(contents, owner) != -1) {
                /* ODD_C: the original classifies the owning unit (player, kind 0 or
                   kind >= 0x5A, other) but records it identically in every arm; GCC
                   cross-jumps the identical arms after reload, deleting the branches but
                   keeping the kind load the original also retains (wave-6 pattern 7). */
                if (is_player(unit) || unit->kind >= 0x5A || unit->kind == 0) {
                    unitOwner = unit;
                } else {
                    unitOwner = unit;
                }
                break;
            }
        }
        if (!unitOwner && item) {
            item->vtable8->destroy((u8 *)item + item->vtable8->destroy_delta, 3);
        }
    }
}
