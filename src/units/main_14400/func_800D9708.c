#include "common.h"

typedef unsigned char u8;
typedef short s16;
typedef struct { u8 pad00[0x20]; s16 delta20; s16 slot22; s32 (*count24)(void *); } Vtable;
/* Complete 0x14-byte list header, including the count read by slot +0x24. */
typedef struct { s32 field00; Vtable *vtable04; void *records08; s32 field0C, count10; } List;
typedef struct { u8 field00; u8 field01; u8 pad02[0xAE]; List listB0; u8 padC4[3]; u8 fieldC7; } Object;
extern void func_800DDB64(Object *object, u8 *dst, s32 count);

s32 func_800D9708(Object *object, u8 *dst)
{
    List *list = &object->listB0;
    s32 count = list->vtable04->count24((u8 *)list + list->vtable04->delta20);
    *dst++ = object->field01;
    *dst++ = count + 2;
    *dst++ = object->fieldC7;
    func_800DDB64(object, dst, count);
    return count + 4;
}
