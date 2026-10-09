#include "common.h"

typedef unsigned char u8;

/* 21 eight-byte records loaded from ROM into a BSS buffer. */
typedef struct {
    unsigned char data[8];
} Entry;

extern char D_00194FC0[];
extern char D_2023BA0[];
extern Entry D_801C5380[21];

void func_8006AC30(void *dst, void *romBase, void *segAddr, s32 stride, s32 first, s32 count);

void *func_800ABFE4(s32 index)
{
    void *table;

    func_8006AC30(&table, D_00194FC0, D_2023BA0, 4, (u8)index, 1);
    func_8006AC30(D_801C5380, D_00194FC0, table, 8, 0, 21);
    return D_801C5380;
}
