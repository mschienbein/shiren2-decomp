#include "common.h"

typedef struct {
    char pad0[4];
    void *unk4;
} Obj800DEF60;

extern char D_80157FA8[];
void func_800D8FE8(Obj800DEF60 *obj);

void func_800DEF60(Obj800DEF60 *obj, s32 flags) {
    obj->unk4 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
