#include "common.h"

typedef struct { s32 unk0; s32 unk4; const void *unk8; } Obj;
extern const s32 D_80153AA0[];
void func_800AC68C(Obj *);
void func_8011FB14(Obj *obj, s32 flags) {
    obj->unk8 = &D_80153AA0;
    if (flags & 1) {
        func_800AC68C(obj);
    }
}
