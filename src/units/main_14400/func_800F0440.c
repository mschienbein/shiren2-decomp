#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct Sub800F0440 Sub800F0440;
typedef struct {
    s16 delta;
    s16 index;
    s32 (*fn)(Sub800F0440 *self);
} VtblEntry800F0440;
typedef struct {
    u8 pad0[0x20];
    VtblEntry800F0440 isBusy;
} Vtbl800F0440;
struct Sub800F0440 {
    u8 pad0[4];
    Vtbl800F0440 *vtbl;
};
typedef struct {
    u8 pad0[0xC];
    Sub800F0440 sub;
} Item800F0440;
typedef struct {
    u8 pad0[0x9A];
    u16 flags;
    u8 field_9C;
} Obj800F0440;

Item800F0440 *func_800F0314(Obj800F0440 *self);
Obj800F0440 *func_801217DC(Item800F0440 *item);
/* Produces explicit success/failure (0x80121C4C, 0x80121D58, 0x80121D64); this caller
 * intentionally discards it and returns its own 1. */
s32 func_80121B8C(Item800F0440 *item);

s32 func_800F0440(Obj800F0440 *self) {
    s32 ready;
    s32 owned;
    Item800F0440 *item;
    Sub800F0440 *sub;

    ready = 0;
    if (self->flags & 0x40) {
        ready = self->field_9C != 0xFF;
    }
    if (ready) {
        item = func_800F0314(self);
        owned = 0;
        if (item != 0) {
            sub = &item->sub;
            if (sub->vtbl->isBusy.fn((Sub800F0440 *)((u8 *)sub + sub->vtbl->isBusy.delta)) == 0) {
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
