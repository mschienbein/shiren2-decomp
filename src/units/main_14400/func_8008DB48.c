#include "common.h"

typedef unsigned char u8;
typedef float f32;

/* 8-byte table entry: a float value and a one-byte attribute. */
typedef struct { f32 x0; u8 x4; u8 pad[3]; } Ent;
typedef struct { u8 pad[0x8]; u32 x8; Ent *xC; } Tbl;

extern Ent *func_80091450(u32 size);
extern f32 func_8008E178(void *stream);
extern u32 func_8008E0C4(void *stream, void *dst, u32 len);
extern void func_80091544(void *ptr);

/* Read t->x8 (float, byte) entries; returns the number of bytes consumed or -1. */
s32 func_8008DB48(void *ctx, Tbl *t) {
    s32 err = 0;
    s32 size = 0;
    u32 i;

    /* ODD_C: single-pass error block leaving the allocation/read sequence for the shared
     * error-size and buffer cleanup; it also shapes the prologue scheduling and the
     * original saved-register choice. */
    do {
        t->xC = func_80091450(t->x8 * 8);
        if (t->xC == 0) {
            err = -1;
            break;
        }
        for (i = 0; i < t->x8; i++) {
            t->xC[i].x0 = func_8008E178(ctx);
            size += 4;
            func_8008E0C4(ctx, &t->xC[i].x4, 1);
            size += 1;
        }
    } while (0);
    if (err) {
        size = -1;
        if (t->xC) {
            func_80091544(t->xC);
            t->xC = 0;
        }
    }
    return size;
}
