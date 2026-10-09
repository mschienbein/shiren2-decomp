#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct VTable VTable;
/* Item-set storage is a byte-ID buffer, followed by its 0x18-byte owner list. */
typedef struct { void *pool; VTable *field_04; u8 *entries; u8 capacity; u8 limit; u8 count; u8 pad_0F; void *owner; u16 text_id; u16 pad_16; } List;
typedef struct Obj800EFC70 { u8 pad_00[0x24]; VTable *field_24; u8 pad_28[0x64]; List *field_8C; u8 pad_90[0x10]; u8 entries_A0[1]; u8 pad_A1[3]; List field_A4; } Obj800EFC70;
extern VTable D_80159F60;
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern void *func_800CEC90(void *obj, void *owner, void *entries, unsigned char capacity, unsigned short text_id);

Obj800EFC70 *func_800F94A0(Obj800EFC70 *object, u8 value) {
    func_800EFC70(object, 0x1F, value);
    object->field_24 = &D_80159F60;
    func_800CEC90(&object->field_A4, object, object->entries_A0, 1, 0);
    object->field_8C = &object->field_A4;
    return object;
}
