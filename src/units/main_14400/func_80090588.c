#include "common.h"
typedef struct { char pad[0x1C]; s32 f1C; } A0;
typedef struct { unsigned char f0; char pad[0x63]; s32 f64; } A2;
s32 func_80090588(A0 *a, void *b, A2 *c) {
    /* The apply hook passes an owner pointer; this target does not use b. */
    if (c->f0 == 0) {
        c->f64 = a->f1C;
    }
    return 0;
}
