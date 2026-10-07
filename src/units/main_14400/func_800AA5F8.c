#include "common.h"
extern unsigned char D_80142F0B;
extern unsigned char D_80147620[];
extern s32 func_800C587C(void *rng, unsigned char limit);
extern void *func_800A8640(s32 command, s32 flags);
void *func_800AA5F8(void) {
    void *result;
    if (func_800C587C(D_80147620, D_80142F0B) == 0)
        result = 0;
    else
        result = func_800A8640(0x5A, 1);
    return result;
}
