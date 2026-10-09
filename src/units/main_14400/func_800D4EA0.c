#include "common.h"

typedef unsigned char u8;

/* Two 12-byte records (contents unknown); the read-only pointer table below is owned by this unit. */
extern u8 D_80147FA0[];
extern u8 D_80147FAC[];

void *const D_8015483C[2] = { D_80147FA0, D_80147FAC };

void *func_800D4EA0(u32 index)
{
    if (index >= 2) {
        return 0;
    }
    return D_8015483C[index];
}
