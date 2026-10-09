#include "common.h"

typedef unsigned char u8;

/* Resource name passed to the allocator. */
extern char D_8014D250[];
/* ROM bounds of the two packed assets (linker symbols; PI device offsets only).
 * The end of the first asset and the start of the second share 0x0030C830 but
 * were distinct linker symbols: the original rematerializes the address for the
 * second transfer (0x8007D530) instead of reusing it from the first size
 * computation, which GCC CSE would do for one symbol. D_00309CD0_end is proposed
 * in config/symbol_aliases.ld. */
extern char D_00309CD0[];
extern char D_00309CD0_end[];
extern char D_0030C830[];
extern char D_0030DD90[];

extern u8 D_8013DF70;
/* Owned by canonical func_8007D58C.c: loaded flag and the four buffers
 * (image/palette per asset) allocated here (proposed there as void *). */
extern s32 D_8013DF74;
extern void *D_8013DF78;
extern void *D_8013DF7C;
extern void *D_8013DF80;
extern void *D_8013DF84;

void *func_8006A8D8(char *name, u32 size);
void func_80054FA8(u32 arg0, s32 arg1, void *dst, s32 size, void *header);

s32 func_8007D480(void) {
    s32 result = 0;

    /* ODD_C: groups allocation and load; a failed allocation breaks out with result -1. Also
     * shapes scheduling: the if/else and goto-done forms are 256 vs 268 bytes, 63 words differ. */
    do {
        D_8013DF78 = func_8006A8D8(D_8014D250, 0xB1F8);
        D_8013DF7C = func_8006A8D8(D_8014D250, 0x200);
        D_8013DF80 = func_8006A8D8(D_8014D250, 0xB1F8);
        D_8013DF84 = func_8006A8D8(D_8014D250, 0x200);
        if (D_8013DF78 == 0 || D_8013DF7C == 0) {
            result = -1;
            break;
        }
        func_80054FA8((u32)D_00309CD0, (u32)D_00309CD0_end - (u32)D_00309CD0,
                      D_8013DF78, 0xB1F8, D_8013DF7C);
        func_80054FA8((u32)D_0030C830, (u32)D_0030DD90 - (u32)D_0030C830,
                      D_8013DF80, 0xB1F8, D_8013DF84);
        D_8013DF70 = 0;
        D_8013DF74 = 1;
    } while (0);
    return result;
}
