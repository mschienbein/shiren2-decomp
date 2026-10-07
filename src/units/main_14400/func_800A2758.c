#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 v; } Dir;
/* Direction step table: one {dx, dy} pair per direction. */
extern Pos D_80142940[];

static inline void addDir(Pos *p, Dir *d)
{
    p->x += D_80142940[d->v].x;
    p->y += D_80142940[d->v].y;
}

void func_800A2758(Pos *p, Dir d) { addDir(p, &d); }
