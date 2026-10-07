#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xCC];
    u8 sub_CC[0x28];
    void *vtable_F4;
} Obj_800EE358;

extern u8 D_80154300[];
extern void func_800CE6A0(void *sub, s32 mode);
extern void func_800E016C(Obj_800EE358 *obj, s32 flags);
extern void func_800A3918(Obj_800EE358 *obj);

void func_800EE358(Obj_800EE358 *obj, s32 flags) {
    obj->vtable_F4 = D_80154300;
    func_800CE6A0(obj->sub_CC, 2);
    func_800E016C(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
