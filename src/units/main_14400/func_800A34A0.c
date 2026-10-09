#include "common.h"
typedef struct { s32 x; s32 y; } Point800A34A0;
typedef struct { s32 left; s32 top; s32 right; s32 bottom; } Bounds800A34A0;
Point800A34A0 *func_800A34A0(Point800A34A0 *out, Bounds800A34A0 *bounds) {
    s32 right = bounds->right;
    s32 top = bounds->top;
    out->x = right;
    out->y = top;
    return out;
}
