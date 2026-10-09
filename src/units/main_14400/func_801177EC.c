#include "common.h"
typedef struct { s32 fields00[2]; const void *field08; } Object;
extern const s32 D_80153AA0[];
extern void func_800AC68C(Object *object);
void func_801177EC(Object *object, s32 flags) {
    flags &= 1;
    object->field08 = &D_80153AA0;
    if (flags) {
        func_800AC68C(object);
    }
}
