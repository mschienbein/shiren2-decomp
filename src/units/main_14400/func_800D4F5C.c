#include "common.h"

extern char D_80154848[];
extern char D_80149DB8[];
void func_800D4F04(void *);
void func_800D8FA8(void *object);
typedef struct { char pad0[4]; void *unk4; } Obj;
void func_800D4F5C(Obj *obj, s32 flags) {
    obj->unk4 = D_80154848;
    func_800D4F04(obj);
    obj->unk4 = D_80149DB8;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
