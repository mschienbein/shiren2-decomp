#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 data[0x14];
} Entry14;

extern u8 D_8014344C;
extern Entry14 D_801431F0[];

Entry14 *func_800B1FD4(u8 index)
{
    if (index >= D_8014344C) {
        return 0;
    }
    return &D_801431F0[index];
}
