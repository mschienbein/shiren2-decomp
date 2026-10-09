#include "common.h"

typedef unsigned char u8;

extern s32 func_800A3AF0(u8 kind);
extern void func_8006AC30(void *dst, void *romBase, void *segAddr, s32 stride, s32 first, s32 count);
extern u8 D_00194FC0[];
extern u8 D_2025440[];
/* Static 8-byte record buffer (.bss 0x80160B20..0x80160B27). */
extern u8 D_80160B20[];

/* Loads the 4-byte table entry for kind and returns it when its level byte (+2)
 * reaches the requested level; otherwise returns null. */
void *func_80044ECC(u8 kind, u8 level)
{
    u8 *entry;

    if (func_800A3AF0(kind)) {
        func_8006AC30(D_80160B20, D_00194FC0, D_2025440, 4, kind - 29, 1);
        entry = D_80160B20;
        if (entry[2] >= level) {
            return entry;
        }
    }
    return 0;
}
