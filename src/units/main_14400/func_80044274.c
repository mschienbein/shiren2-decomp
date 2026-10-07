#include "common.h"

extern char D_80149F80[];
void func_800436F0(void *);
typedef struct { char pad0[0x18]; void *unk18; } Obj;
void func_80044274(Obj *obj, s32 flags) {
    obj->unk18 = D_80149F80;
    if (flags & 1) {
        func_800436F0(obj);
    }
}
