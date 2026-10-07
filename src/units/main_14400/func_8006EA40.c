#include "common.h"

/* Partial byte/word effects view; complete object and historical API unknown. */
void func_8006EA40(unsigned char *arg0)
{
    unsigned char byte_1;

    if (arg0[0] != 0) {
        byte_1 = arg0[1];
        *(u32 *)(arg0 + 0x1C) = 0;
        arg0[1] = byte_1 ^ 1;
    }
}
