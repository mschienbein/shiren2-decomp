#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct VTable VTable;
typedef struct Collection {
    void *type;
    VTable *vtable;
    u8 *entries;
    u8 count, capacity;
    u8 pad_0E[2];
    void *owner;
    u16 text_id;
    u8 pad_16[2];
} Collection;
typedef struct Object {
    u8 pad_00[0x24];
    VTable *vtable_24;
    u8 pad_28[0x61];
    u8 field_89;
    u8 pad_8A[2];
    Collection *collection_8C;
    u8 pad_90[0xA];
    u16 flags_9A;
    u8 pad_9C[2];
    u8 field_9E;
    u8 pad_9F;
    u8 entries_A0[3];
    u8 pad_A3;
    Collection collection_A4;
    s32 field_BC;
} Object;
typedef Object Obj800EFC70;
typedef Object Obj800FA1C4;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *, s32, u8);
extern void *func_800CEC90(void *, void *, void *, u8, u16);
extern void func_800FA1C4(Obj800FA1C4 *object);
extern VTable D_8015A070;

Object *func_800FA140(Object *object, u8 variant)
{
    func_800EFC70(object, 0x20, variant);
    object->vtable_24 = &D_8015A070;
    func_800CEC90(&object->collection_A4, object, object->entries_A0, 3, 0);
    object->collection_8C = &object->collection_A4;
    object->field_9E = object->field_89;
    object->field_BC = 0;
    object->flags_9A |= 8;
    func_800FA1C4(object);
    return object;
}
