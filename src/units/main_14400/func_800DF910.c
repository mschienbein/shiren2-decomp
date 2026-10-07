#include "common.h"

typedef unsigned char u8;
extern u8 D_80142F1B;
u8 *func_800C5F60(void);
s32 func_80049CB4(s32, ...);

s32 func_800DF910(void *self /* receiver: unused; supplied by the vtable +0x14 call */)
{
    u8 *p = func_800C5F60();

    if (!((D_80142F1B >> 2) & 1)) {
        if (!(p[0x72] & 0x10)) {
            p[0x72] |= 0x10;
            func_80049CB4(0x1F, p);
        }
    }
    return 0;
}
