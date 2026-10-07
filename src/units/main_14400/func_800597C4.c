#include "common.h"

typedef struct {
    u32 w[3];
} Triple;

extern Triple D_80165318;

void func_800597C4(Triple *out)
{
    *out = D_80165318;
}
