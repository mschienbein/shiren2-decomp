#include "common.h"

typedef struct {
    u32 a;
    u32 b;
} Pair;

s32 func_800A251C(Pair *x, Pair *y)
{
    s32 result = 0;

    if (x->a == y->a) {
        result = x->b == y->b;
    }
    return result;
}
