#include "common.h"

typedef unsigned char u8;

/* PI device addresses of the save slots; not RAM object pointers. */
extern u32 D_8014A7EC[];
void func_80043C2C(u32 deviceAddress, void *destination, u32 length);
void func_80043D74(u32 deviceAddress, void *source, u32 length);

void func_80043900(s32 srcIndex, s32 dstIndex) {
    u32 src = D_8014A7EC[srcIndex];
    u32 dst = D_8014A7EC[dstIndex];
    s32 remaining = 0x2800;
    u8 buf[0x40];

    while (remaining >= 0x40) {
        func_80043C2C(src, buf, 0x40);
        src += 0x40;
        func_80043D74(dst, buf, 0x40);
        dst += 0x40;
        remaining -= 0x40;
    }
    if (remaining > 0) {
        func_80043C2C(src, buf, remaining);
        func_80043D74(dst, buf, remaining);
    }
}
