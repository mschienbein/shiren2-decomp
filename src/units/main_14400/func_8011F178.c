#include "common.h"

extern char D_80153AA0[];
void func_800AC68C(void *);
typedef struct { char pad0[8]; void *unk8; } Obj;
void func_8011F178(Obj *obj, s32 flags) {
    obj->unk8 = D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
