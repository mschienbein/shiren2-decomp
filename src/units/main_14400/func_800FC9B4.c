#include "common.h"

typedef struct {
    unsigned char pad0[0x24];
    void *vtable;
} Obj;

extern unsigned char D_8015A530[];
extern void func_800EFD28(Obj *obj, s32 flags);
extern void func_800A3918(Obj *obj);

void func_800FC9B4(Obj *obj, s32 flags) {
    obj->vtable = D_8015A530;
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
