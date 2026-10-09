#include "common.h"

typedef struct { char pad[0x24]; const void *unk24; } Obj;
extern const unsigned char D_8015A2B0[192];
void func_800EFD28(Obj *, s32);
void func_800A3918(Obj *);
void func_800FBCF4(Obj *obj, s32 flags) {
    obj->unk24 = D_8015A2B0;
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
