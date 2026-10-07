#include "common.h"
typedef struct { char pad0[0x24]; s32 unk24; char pad28[0x10]; s32 unk38; } Obj;
void func_8004898C(Obj *, s32);
void func_80098910(Obj *obj) {
    s32 mode = obj->unk24;
    if (mode < 2) {
        mode = 0;
    } else if (obj->unk38 == 0) {
        mode = 2;
    } else if (obj->unk38 == mode - 1) {
        mode = 1;
    } else {
        mode = 3;
    }
    func_8004898C(obj, mode);
}
