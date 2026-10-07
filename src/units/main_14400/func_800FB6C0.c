#include "common.h"

typedef struct {
    char pad0[0x24];
    void *unk24;
} Obj800FB6C0;

extern char D_8015A1F0[];
void func_800EFD28(Obj800FB6C0 *obj, s32 arg1);
void func_800A3918(Obj800FB6C0 *obj);

void func_800FB6C0(Obj800FB6C0 *obj, s32 flags) {
    obj->unk24 = D_8015A1F0;
    func_800EFD28(obj, 0);
    if (flags & 1) {
        func_800A3918(obj);
    }
}
