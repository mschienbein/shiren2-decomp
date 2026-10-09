#include "common.h"

typedef unsigned char u8;
typedef struct { void *field_00; void *field_04; } Entry;
typedef struct { s32 kind_00; void *vtable_04; Entry owner_08; Entry entry_10; } Object;
extern void *D_801476B8;
extern u8 D_801584A8[];
extern Object *func_800DA970(Object *object, s32 kind, void *owner);
extern Entry *func_800D0180(Entry *entry);
extern void func_800DAA58(u8 id, void *entry);

Object *func_800DB738(Object *object, u8 *id)
{
    Entry *entry;
    func_800DA970(object, 0x11, D_801476B8);
    entry = &object->entry_10;
    object->vtable_04 = D_801584A8;
    func_800D0180(entry);
    func_800DAA58(*id, entry);
    return object;
}
