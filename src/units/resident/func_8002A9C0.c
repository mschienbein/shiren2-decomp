#include "common.h"

#define K1BASE 0xA0000000
#define STEP 0x100000

/* osGetMemSize */
u32 func_8002A9C0(void)
{
    volatile u32 *ptr;
    u32 size = 0x400000;
    u32 data0;
    u32 data1;

    while (size < 0x800000) {
        ptr = (volatile u32 *)(K1BASE + size);
        data0 = *ptr;
        data1 = ptr[STEP / 4 - 1];
        *ptr ^= ~0;
        ptr[STEP / 4 - 1] ^= ~0;
        if ((*ptr != (data0 ^ ~0)) || (ptr[STEP / 4 - 1] != (data1 ^ ~0))) {
            return size;
        }
        *ptr = data0;
        ptr[STEP / 4 - 1] = data1;
        size += STEP;
    }
    return size;
}
