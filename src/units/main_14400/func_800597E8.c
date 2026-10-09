#include "common.h"

/* Whole three-float camera vector; its middle component is an angle. */
typedef struct { float x, y, z; } Triple_800597E8;

extern Triple_800597E8 D_80165330;

void func_800597E8(Triple_800597E8 *dst)
{
    *dst = D_80165330;
}
