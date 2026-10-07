#include "common.h"

extern char D_80149F80[];
typedef struct { char pad0[0xC]; s32 unkC; char pad10[8]; void *unk18; } Obj;
Obj *func_800CA030(Obj *obj) {
    obj->unk18 = D_80149F80;
    obj->unkC = 0;
    return obj;
}
