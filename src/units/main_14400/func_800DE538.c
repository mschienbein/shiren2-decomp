#include "common.h"
typedef unsigned char u8;
/* Derived action storage extends through the table-entry pointer at +0xC4. */
typedef struct { u8 pad_00[4]; void *vtable; u8 pad_08[0xBC]; void *entry_C4; } Object;
extern u8 D_80158988[], D_80143094[];
extern void *func_800DDAD0(void *, s32);
extern void *func_800AFD78(void *, u8);
extern void func_800DDC0C(void *, const void *, s32);
void *func_800DE538(Object *object, const u8 *data) {
    s32 size;
    u8 index;
    func_800DDAD0(object, 0x2A);
    object->vtable = D_80158988;
    size = *data++;
    index = *data++;
    size -= 2;
    object->entry_C4 = func_800AFD78(D_80143094, index);
    func_800DDC0C(object, data, size);
    return object;
}
