#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
extern u8 D_80142F1B;

void func_800AA3C0(void) {
    u8 *flags = &D_80142F1B;

    *flags |= 0x40;
}
