#include "common.h"

typedef unsigned char u8;

extern char D_00194FC0[];
extern char D_2022CD0[];
extern u8 D_801C5230[];

void func_8006AC30(void *dst, void *romBase, void *segAddr, s32 stride, s32 first, s32 count);

/* Looks up the indexed segment pointer, then loads its 0x15 eight-byte records into D_801C5230. */
void *func_800ABEE4(u8 index)
{
    void *records;

    func_8006AC30(&records, D_00194FC0, D_2022CD0, 4, index, 1);
    func_8006AC30(D_801C5230, D_00194FC0, records, 8, 0, 0x15);
    return D_801C5230;
}
