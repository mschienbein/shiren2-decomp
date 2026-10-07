#include "common.h"

typedef struct {
    char pad0[0xC];
    void *vtable;
} Obj80092558;

extern char D_80151350[];
extern void func_800D8FA8(Obj80092558 *obj);

void func_80092558(Obj80092558 *obj, s32 flags) {
    obj->vtable = D_80151350;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
