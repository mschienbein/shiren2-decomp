#include "common.h"

/* ROM bounds of two consecutive resources (linker symbols; only their PI device
 * offsets are used, as in func_8005F1D4). The end of the first resource and the
 * start of the second share the address 0x00157840 but were distinct linker
 * symbols: the original rematerializes the address for the second load
 * (0x8007F308) instead of reusing it from the first size computation, which GCC
 * CSE would do for one symbol. D_001575C0_end is proposed in config/symbol_aliases.ld. */
extern char D_001575C0[];
extern char D_001575C0_end[];
extern char D_00157840[];
extern char D_00157B80[];
/* Resource names. */
extern char D_8014D288[];
extern char D_8014D298[];
/* Loaded resources (canonical func_8007F33C.c owns them; proposed as void *). */
extern void *D_8013E8E8;
extern void *D_8013E8EC;

void *func_8006ABC4(char *name, u32 devAddr, s32 size);

void func_8007F2D8(void) {
    D_8013E8E8 = func_8006ABC4(D_8014D288, (u32)D_001575C0, (u32)D_001575C0_end - (u32)D_001575C0);
    D_8013E8EC = func_8006ABC4(D_8014D298, (u32)D_00157840, (u32)D_00157B80 - (u32)D_00157840);
}
