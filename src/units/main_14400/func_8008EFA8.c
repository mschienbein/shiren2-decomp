#include "common.h"
typedef struct { char pad[0x18]; unsigned char f18; unsigned char f19; char pad2[2]; s32 f1C; char pad3[4]; s32 f24; s32 f28; } SA;
typedef struct { char pad[8]; void **f8; } Tbl;
typedef struct { char pad[0x74]; Tbl *f74; } SB;
typedef struct { unsigned char f0; unsigned char f1; char pad[0x1B2]; void *f1B4; } SC;
s32 func_8008D4B8(void *queue, void *item, s32 key);
s32 func_8008EFA8(SA *a, SB *b, SC *c) {
    s32 r;
    if (a->f18 != 0) {
        c->f1 |= 1;
    }
    if (a->f19 & 1) {
        c->f1 |= 4;
    }
    if (a->f19 & 2) {
        c->f1 |= 2;
    }
    r = func_8008D4B8(c->f1B4, b->f74->f8[a->f1C], 0);
    if (r == 0 && a->f24 != 0) {
        r = func_8008D4B8(c->f1B4, b->f74->f8[a->f28], 0);
    }
    return r;
}
