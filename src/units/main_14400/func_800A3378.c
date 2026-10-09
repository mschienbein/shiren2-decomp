#include "common.h"
typedef struct { s32 left; s32 top; s32 right; s32 bottom; } Rect800A3378;
extern Rect800A3378 *func_800A324C(Rect800A3378 *out, Rect800A3378 *a, Rect800A3378 *b);
static __inline__ s32 less_equal(s32 left, s32 right) {
    return left <= right;
}
static __inline__ s32 bounds_nonempty(Rect800A3378 *bounds) {
    return less_equal(bounds->top, bounds->bottom) || less_equal(bounds->left, bounds->right);
}
s32 func_800A3378(Rect800A3378 *a, Rect800A3378 *b) {
    Rect800A3378 bounds;
    func_800A324C(&bounds, a, b);
    return bounds_nonempty(&bounds);
}
