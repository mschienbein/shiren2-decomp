#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct {
    u8 pad0[0x9A];
    u16 flags;
    u8 field_9C;
} Obj800F0440;

/* The held id-0xAC pot item (func_800F0314 tests id 0xAC; func_801216D0 builds id 0xAC
 * through the kind-9 constructor func_801140C0), or 0. */
Item *func_800F0314(Obj800F0440 *self);
Obj800F0440 *func_801217DC(Item *item);
/* Produces explicit success/failure (0x80121C4C, 0x80121D58, 0x80121D64); this caller
 * intentionally discards it and returns its own 1. */
s32 func_80121B8C(Item *item);

s32 func_800F0440(Obj800F0440 *self) {
    s32 ready;
    s32 owned;
    Item *item;
    ItemPot *pot;

    ready = 0;
    if (self->flags & 0x40) {
        ready = self->field_9C != 0xFF;
    }
    if (ready) {
        item = func_800F0314(self);
        owned = 0;
        if (item != 0) {
            pot = &item->payload.pot;
            /* An empty content list (list slot +0x20 count is 0). */
            if (pot->list.methods->count24((u8 *)&pot->list + pot->list.methods->delta20) == 0) {
                owned = func_801217DC(item) == self;
            }
        }
        if (owned) {
            func_80121B8C(item);
            return 1;
        }
    }
    return 0;
}
