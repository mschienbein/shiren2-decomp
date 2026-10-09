#include "common.h"
typedef struct { unsigned char fields_00[8]; void *vtable_08; } Object8011C99C;
extern unsigned char D_80153AA0[];
void func_800AC68C(void *object);
void func_8011C99C(Object8011C99C *object, s32 flags)
{
    object->vtable_08 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(object);
    }
}
