#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x4];
    void *vtable_4;
} Obj_800DD484;

extern u8 D_80157FA8[];
extern void func_800D8FE8(Obj_800DD484 *obj);

void func_800DD484(Obj_800DD484 *obj, s32 flags) {
    obj->vtable_4 = D_80157FA8;
    if (flags & 1) {
        func_800D8FE8(obj);
    }
}
