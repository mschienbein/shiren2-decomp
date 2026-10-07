#include "common.h"

typedef struct {
    unsigned char pad0[4];
    void *vtable;
} Obj;

extern unsigned char D_80157FA8[];
extern void func_800D8FE8(Obj *obj);

void func_800DFC48(Obj *obj, s32 flags) {
    obj->vtable = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
