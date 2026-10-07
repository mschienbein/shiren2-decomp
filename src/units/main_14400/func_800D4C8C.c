#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x4];
    void *vtable_4;
} Obj_800D4C8C;

extern u8 D_80154828[];
extern u8 D_80149DB8[];
extern void func_800D4C34(Obj_800D4C8C *obj);
extern void func_800D8FA8(void *object);

void func_800D4C8C(Obj_800D4C8C *obj, s32 flags) {
    obj->vtable_4 = D_80154828;
    func_800D4C34(obj);
    obj->vtable_4 = D_80149DB8;
    if (flags & 1) {
        func_800D8FA8(obj);
    }
}
