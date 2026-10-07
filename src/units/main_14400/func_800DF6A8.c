#include "common.h"

typedef struct { s32 unk0; void *unk4; } Obj;
extern s32 D_80157FA8;
void func_800D8FE8(Obj *);
void func_800DF6A8(Obj *obj, s32 flags) {
    obj->unk4 = &D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
