#include "common.h"

typedef unsigned char u8;

typedef struct { void *header, *descriptor, *table_a, *table_b, *table_c; u8 metadata[4]; } Resource;
extern Resource D_80142B1C[21];
/* ROM base address symbol (DMA source), declared as the other users declare it. */
extern char D_00194FC0[];
void func_8006AC30(void *arg0, void *arg1, void *arg2, s32 arg3, s32 arg4, s32 arg5);
void func_800ABB50(void *arg0, u8 index, u8 arg2) {
    func_8006AC30(arg0, D_00194FC0, D_80142B1C[index].header, 0xC, arg2, 1);
}
