#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
extern s32 D_8015C880[];
extern s32 D_8015C8A0[];
extern void *func_800EE3C0(void *, s32, u8);
extern void *func_800CEC90(void *, void *, void *, u8, u16);
extern s32 func_800A3934(void *);
extern void func_8010A0F4(void *, s32);

void **func_80109F50(void **object, u8 value) {
    func_800EE3C0(object, 0x1A, value);
    object[0xB4 / 4] = D_8015C880;
    object[0x24 / 4] = D_8015C8A0;
    func_800CEC90(object + 0xC4 / 4, object, object + 0xC0 / 4, 4, 0x467);
    if (!func_800A3934(object)) {
        func_8010A0F4(object, value);
    }
    return object;
}
