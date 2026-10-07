#include "common.h"
extern s32 D_80153AA0[];
extern void func_800AC68C(void *object);
typedef struct { unsigned char pad_00[8]; s32 *field_08; } Object;
void func_80124EA4(Object *object, s32 flags) {
    object->field_08 = D_80153AA0;
    if (flags & 1) func_800AC68C(object);
}
