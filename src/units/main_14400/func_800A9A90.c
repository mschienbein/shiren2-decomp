#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern u8 D_80142F24[];
extern u16 D_801476C0;

void func_800A9A90(void)
{
    u8 *state = D_80142F24;

    if (state[0] == 0xB && state[1] < 3) {
        D_801476C0 = state[1] + 0xFA1;
    }
}
