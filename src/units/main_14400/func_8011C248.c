#include "common.h"
typedef struct { unsigned char pad_00[8]; void *vtable_08; } Obj;
extern unsigned char D_80153AA0[];
extern void func_800AC68C(void *object);
void func_8011C248(Obj *obj, s32 flags) {
    obj->vtable_08 = D_80153AA0;
    if (flags & 1) func_800AC68C(obj);
}
