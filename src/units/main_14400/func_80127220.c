#include "common.h"
typedef struct { unsigned char pad_00[8]; const void *vtable_08; unsigned char pad_0C[4]; } Obj;
extern void *func_800AC5B4(s32 size, s32 alternate);
extern Obj *func_801128F0(Obj *obj, s32 kind);
extern const unsigned char D_801605B0[72];
Obj *func_80127220(void) {
    Obj *obj = func_800AC5B4(0x10, 0);
    func_801128F0(obj, 0xEB);
    obj->vtable_08 = D_801605B0;
    return obj;
}
