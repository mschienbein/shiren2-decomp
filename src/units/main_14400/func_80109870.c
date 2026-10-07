#include "common.h"

typedef unsigned char u8;
extern s32 D_80159130[];
extern s32 D_8015C788[];
extern void *func_800EE3C0(void *obj, s32 kind, u8 mode);
extern s32 func_800A3934(void *);
extern void func_80109994(void *, s32);

void **func_80109870(void **object, u8 value) {
    func_800EE3C0(object, 0x19, value);
    object[0xB4 / 4] = D_80159130;
    object[0x24 / 4] = D_8015C788;
    if (!func_800A3934(object)) {
        func_80109994(object, value);
    }
    return object;
}
