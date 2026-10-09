#include "common.h"

/* ROM segment bounds (linker symbols). Only their numeric PI device offsets are
 * used; they are never dereferenced as CPU objects. */
extern char D_00157460[];
extern char D_001575C0[];
/* "GFX static" resource name. */
extern char D_8014C434[];
/* Loaded resource pointer, read back by func_8005F210/func_8005F7E0. */
extern void *D_8013B748;

void *func_8006ABC4(char *name, u32 devAddr, s32 size);

void func_8005F1D4(void) {
    s32 size = (u32)D_001575C0 - (u32)D_00157460;
    D_8013B748 = func_8006ABC4(D_8014C434, (u32)D_00157460, size);
}
