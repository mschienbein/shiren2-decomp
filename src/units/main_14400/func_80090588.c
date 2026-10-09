#include "common.h"
/* TXIM resource (func_800905DC): +0x1C is the loaded image data pointer. */
typedef struct { char pad[0x1C]; void *f1C; } A0;
/* Model object: +0x64 is the texture-image pointer of its +0x48 render effect (func_8006F7F8). */
typedef struct { unsigned char f0; char pad[0x63]; void *f64; } A2;
s32 func_80090588(A0 *a, void *b, A2 *c) {
    /* The apply hook passes an owner pointer; this target does not use b. */
    if (c->f0 == 0) {
        c->f64 = a->f1C;
    }
    return 0;
}
