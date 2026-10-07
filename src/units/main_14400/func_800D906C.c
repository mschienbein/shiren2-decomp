#include "common.h"

typedef short s16;
typedef struct { s16 unk0; s16 pad2; void *unk4; } Obj;
extern s32 D_80157FA8;
Obj *func_800D906C(Obj *obj, s16 value) {
    obj->unk4 = &D_80157FA8;
    obj->unk0 = value;
    return obj;
}
