#include "common.h"

typedef unsigned char u8;

typedef struct { u8 b[8]; s32 x8; s32 xC; } Rec;
u32 func_8008E0C4(void *, void *, u32); s32 func_8008DF04(void *);
s32 func_8008F380(void *ctx, Rec *r) {
    func_8008E0C4(ctx, &r->b[0], 1);
    func_8008E0C4(ctx, &r->b[1], 1);
    func_8008E0C4(ctx, &r->b[2], 1);
    func_8008E0C4(ctx, &r->b[3], 1);
    func_8008E0C4(ctx, &r->b[4], 1);
    r->x8 = func_8008DF04(ctx);
    r->xC = func_8008DF04(ctx);
    return 0xD;
}
