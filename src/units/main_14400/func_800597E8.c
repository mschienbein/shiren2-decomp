#include "common.h"

/* Twelve-byte record copied word by word; field meaning unknown. */
typedef struct {
    u32 word0;
    u32 word4;
    u32 word8;
} Triple_800597E8;

extern Triple_800597E8 D_80165330;

void func_800597E8(Triple_800597E8 *dst)
{
    *dst = D_80165330;
}
