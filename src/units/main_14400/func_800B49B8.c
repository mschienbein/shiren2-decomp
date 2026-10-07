#include "common.h"

typedef struct { s32 row; s32 col; } Pos;
/* D_80146468: 76-byte (0x4C) rows of byte cells */
extern unsigned char D_80146468[][0x4C];
extern void *func_800B4928(Pos *);
void *func_800B49B8(Pos *p) {
    void *r = func_800B4928(p);
    if (r != 0) D_80146468[p->row][p->col] = 0xFF;
    return r;
}
