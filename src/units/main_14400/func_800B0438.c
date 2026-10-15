#include "common.h"
#include "item_records.h"
#include "map_position_iterator.h"

typedef unsigned char u8;
typedef short s16;
typedef struct Unit Unit;


/* Unit kind 0x5A inventory built by func_800CEC90 at Unit+0x8C (0x18 bytes):
 * owner at +0x10, message id at +0x14; its ten entries live at Unit+0x80. */
typedef struct {
    ItemList list;
    Unit *owner;
    unsigned short textId;
    u8 pad16[2];
} UnitInventory;

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
extern MapRect D_801429D0;

void *func_800B4D80(MapPos *p);
s32 func_800CD090(void *container, void *element);
s32 func_800A8F6C(s32 *iter);
void *func_800A910C(s32 *iter);

/* ODD_C: The grid constructor retains the original temporary-copy ordering. */
static inline void init_grid(MapPositionIterator *it, MapPos *tmp, s32 x, s32 y, s32 lastX, s32 lastY) {
    tmp->x = x;
    tmp->y = y;
    it->start = *tmp;
    it->cur = it->start;
    tmp->x = lastX;
    tmp->y = lastY;
    it->end = *tmp;
}

/* Remove an item only after checking the map, item containers, and unit inventories.
 * Item vtable slot 1 uses the destructor (self, s32 flags) contract, for example
 * D_8015D518 + 0xC -> func_801114FC. Unit slot 19 returns its inventory pointer. */
/* Bit 2 of flags1E marks a unit whose inventory comes from vtable slot 19. */
static inline signed char is_player(Unit *unit) { return (unit->flags1E >> 2) & 1; }

/* The only caller, func_800C701C, passes a second argument (li a1,1 in the jal delay slot);
 * this body never reads it. */
void func_800B0438(ItemPool *table, s32 flag) {
    s32 i;
    MapPositionIterator grid;
    MapPos pos;
    s32 unitIter;

    for (i = table->capacity; ;) {
        Item *item;
        Item *owner;
        s32 found;
        s32 j;
        Unit *unitOwner;
        MapPositionIterator *cursor;
        i--;
        if (i == -1) {
            break;
        }
        if (!(table->occupancy[i >> 3] & D_8015488C[i & 7])) {
            continue;
        }
        item = func_800AFD78(table, (u8)i);
        owner = item;
        if (item == 0) {
            continue;
        }
        found = 0;
        cursor = &grid;
        init_grid(cursor, &pos, D_801429D0.min.x, D_801429D0.min.y, D_801429D0.max.x, D_801429D0.max.y);
        for (;;) {
            s32 more = grid.cur.x <= cursor->end.x;
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
        for (j = table->capacity; ;) {
            Item *container;
            j--;
            if (j == -1) {
                break;
            }
            if (!(table->occupancy[j >> 3] & D_8015488C[j & 7])) {
                continue;
            }
            container = func_800AFD78(table, (u8)j);
            if (container != owner && container->field00 == 9 &&
                func_800CD090(&container->payload.pot, owner) != -1) {
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
            item->field08->method0C((u8 *)item + item->field08->delta08, 3);
        }
    }
}
