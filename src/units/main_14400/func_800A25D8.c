#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s32 x, y; } Vec2;
typedef struct { s8 d; } Dir;
extern Vec2 D_80142940[];
Vec2 *func_800A25D8(Vec2 *out, Vec2 *p, Dir dir, s32 scale){
    Dir *dp = &dir;
    s32 dx = scale * D_80142940[(u8)dp->d].x;
    s32 dy = scale * D_80142940[(u8)dp->d].y;
    Vec2 r;
    s32 px = p->x;
    s32 py = p->y;
    r.x = px + dx;
    r.y = py + dy;
    out->x = r.x;
    out->y = r.y;
    return out;
}
