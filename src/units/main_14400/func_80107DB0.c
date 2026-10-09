#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct Collection {
    void *type;
    void *vtable;
    u8 *entries;
    u8 count, capacity;
    u8 pad_0E[2];
    void *owner;
    u16 text_id;
    u8 pad_16[2];
} Collection;

typedef struct Object {
    u8 pad_00[0x24];
    const void *vtable_24;
    u8 pad_28[0x8C - 0x28];
    Collection *collection_8C;
    u8 pad_90[0xA];
    u16 flags_9A;
    u8 pad_9C[4];
    u8 entries_A0[3];
    u8 pad_A3;
    Collection collection_A4;
    s32 field_BC;
    s32 field_C0;
    s32 field_C4;
} Object;

extern Object *func_800EFC70(Object *obj, s32 arg1, u8 arg2);
extern void *func_800CEC90(void *obj, void *owner, void *entries, u8 capacity, u16 text_id);
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern const unsigned char D_8015C450[192];

/* Kind 0x54 actor: base actor plus an embedded 3-entry collection. */
Object *func_80107DB0(Object *object, u8 variant)
{
    func_800EFC70(object, 0x54, variant);
    {
        Collection *list = &object->collection_A4;

        object->vtable_24 = D_8015C450;
        func_800CEC90(list, object, object->entries_A0, 3, 0);
        object->field_C0 = 1;
        object->field_C4 = 0;
        object->collection_8C = &object->collection_A4;
        object->flags_9A |= 1;
    }
    object->field_BC = D_80142F18.mode == 0x4D;
    return object;
}
