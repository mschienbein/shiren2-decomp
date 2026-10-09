#include "common.h"

typedef struct { s32 pad00[2]; const void *vtable08; s32 field0C; } Object;
extern Object *func_80117080(void *object, s32 kind);
extern const u32 D_801490F0[];

void *func_80127B80(void *ptr)
{
    Object *object = ptr;
    func_80117080(object, 0xEE);
    object->vtable08 = D_801490F0;
    object->field0C = 0;
    return object;
}
