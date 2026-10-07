#include "common.h"
typedef unsigned char u8;
extern u8 D_8015A940[];
typedef struct { u8 pad[0x24]; void *vt; } Obj;
void func_800EFD28(Obj *o, s32 flags);
void func_800A3918(Obj *o);
void func_800FE1D4(Obj *o, s32 flags) {
    o->vt = D_8015A940;
    func_800EFD28(o, 0);
    if (flags & 1) func_800A3918(o);
}
