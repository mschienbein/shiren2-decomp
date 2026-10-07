#include "common.h"

extern s32 D_8015B2A8[];
extern void func_800EFD28(void *, s32);
extern void func_800A3918(void *);

void func_80100964(void **object, s32 flags) {
    object[0x24 / 4] = D_8015B2A8;
    func_800EFD28(object, 0);
    if (flags & 1) {
        func_800A3918(object);
    }
}
