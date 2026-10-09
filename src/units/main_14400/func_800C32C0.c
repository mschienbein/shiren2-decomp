#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Point;
typedef struct { u8 value; } Dir;
typedef struct {
    u8 pad_00[0x18];
    signed short delta_18;
    u16 reserved_1A;
    s32 (*test_1C)(void *, s32);
} ItemVtable;
typedef struct { u8 pad_00[8]; ItemVtable *vtable_08; } Item;
typedef struct {
    u8 pad_00[0x1C];
    void *vtable_1C;
    void *owner_20;
    Item *item_24;
    s32 mode_28;
    Point position_2C;
    u16 flags_34;
    u8 pad_36[2];
    s32 count_38;
    u8 state_3C;
    u8 pad_3D[0x7B];
    s32 predicate_B8;
    u8 state_BC;
} Operation;
extern const u8 D_80153F70[];
extern void *func_800A2594(Point *out, void *position, Dir direction);
extern void *func_800C2CA0(u8 *self, void *position, void *direction, s32 limit, s32 mode);

Operation *func_800C32C0(Operation *operation, void *owner, Item *item, Point *position, Dir *direction, s32 limit, s32 mode, u16 flags)
{
    Point next;
    s32 effective_limit;
    s32 i;
    func_800A2594(&next, position, *direction);
    effective_limit = 0xFF;
    if (!(flags & 0x20)) {
        effective_limit = limit;
    }
    func_800C2CA0((u8 *)operation, &next, direction, effective_limit, 2);
    operation->vtable_1C = (void *)D_80153F70;
    /* Both counted loops are present in the original constructor. */
    for (i = 9; i != -1; --i) { }
    { s32 j; for (j = 9; j != -1; --j) { } }
    operation->owner_20 = owner;
    operation->item_24 = item;
    operation->mode_28 = mode;
    operation->position_2C = *position;
    operation->flags_34 = flags;
    if (flags & 0x20) {
        operation->flags_34 = flags | 0x10;
        operation->mode_28 = 0;
    }
    operation->count_38 = 0;
    operation->state_3C = 0;
    {
        ItemVtable *vtable = item->vtable_08;
        operation->predicate_B8 = vtable->test_1C((char *)item + vtable->delta_18, 0xD);
    }
    operation->state_BC = 0;
    return operation;
}
