#include "common.h"

typedef unsigned char u8;

extern u8 D_80153AA0[];
typedef struct { s32 x0; s32 x4; void *x8; } Obj;
void func_800AC68C(Obj *obj);
void func_8011FC20(Obj *obj, s32 flags) {
    obj->x8 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
