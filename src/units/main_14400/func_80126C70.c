#include "common.h"

typedef union {
    s32 words[2];
    struct {
        unsigned char pad0[3];
        unsigned char x;
        unsigned char pad4[3];
        unsigned char y;
    } bytes;
} Pos;

extern void func_80116828(Pos *pos);

/* D_801604D0+0x44, the same slot as func_80126AC8: func_80115EB0 supplies seven
 * pointer arguments at 0x80116028-0x80116050 (a1 is its null-tested a1, a2 the
 * position it dereferences at 0x80115FB4). Only position is used here; self,
 * arg1, arg3, arg4, arg5 and arg6 are supplied by that call contract and unused. */
s32 func_80126C70(void *self, void *arg1, Pos *pos, void *arg3, void *arg4, void *arg5, void *arg6)
{
    func_80116828(pos);
    return 1;
}
