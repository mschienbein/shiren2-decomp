#include "common.h"

typedef unsigned char u8;
extern void func_800D8F10(s32) __attribute__((noreturn));
extern u8 D_80142F28[];

void func_800AA5C0(s32 id) {
    id -= 0x18;
    if (id < 0) {
        func_800D8F10(0);
    }
    D_80142F28[0] |= id;
}
