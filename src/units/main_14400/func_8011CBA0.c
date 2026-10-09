#include "common.h"

typedef struct { unsigned char pad_00[8]; const void *vtable_08; } Object;
extern unsigned char D_80153AA0[];
extern void func_800AC68C(void *object);

void func_8011CBA0(Object *object, s32 flags)
{
    object->vtable_08 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(object);
    }
}
