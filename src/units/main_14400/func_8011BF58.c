#include "common.h"

typedef struct { s32 field00[2]; const void *vtable08; } Object;
extern const u32 D_80153AA0[];
extern void func_800AC68C(void *object);

void func_8011BF58(Object *object, s32 flags)
{
    object->vtable08 = D_80153AA0;
    if (flags & 1)
        func_800AC68C(object);
}
