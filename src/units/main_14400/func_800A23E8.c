#include "common.h"

typedef struct { s32 x; s32 y; } Vec800A23E8;

s32 func_800A2424(void *delta);

s32 func_800A23E8(Vec800A23E8 *origin, Vec800A23E8 *vec) {
    vec->x -= origin->x;
    vec->y -= origin->y;
    return func_800A2424(vec);
}
