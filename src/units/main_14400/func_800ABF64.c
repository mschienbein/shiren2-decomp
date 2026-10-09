#include "common.h"

typedef unsigned char u8;
extern unsigned char D_00194FC0[];
extern unsigned char D_2023448[];
extern unsigned char D_801C52D8[21 * 8];
extern void func_8006AC30(void *dst, void *romBase, void *segAddr, s32 stride, s32 first, s32 count);
void *func_800ABF64(u8 index)
{
    void *address;
    func_8006AC30(&address, D_00194FC0, D_2023448, 4, index, 1);
    func_8006AC30(D_801C52D8, D_00194FC0, address, 8, 0, 21);
    return D_801C52D8;
}
