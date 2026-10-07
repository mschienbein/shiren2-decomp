#include "common.h"

extern char D_80157FA8[];
void func_800D8FE8(void *);
typedef struct { char pad0[4]; void *unk4; } Obj;
void func_800DD554(Obj *obj, s32 flags) {
    obj->unk4 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
