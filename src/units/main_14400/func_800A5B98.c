#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x; s32 y; } Pos;
typedef struct { s32 x0; s32 y0; s32 x1; s32 y1; } Rect;
typedef struct { u8 unk0[0x1C]; u16 flags; } Obj;
extern Rect D_801429C0;
void *func_800B221C(Pos *p);
void *func_800A33DC(Pos *p, Rect *r);
s32 func_800A4314(Obj *o, Pos *p);
s32 func_800B5BDC(Pos *p);
u32 func_800B1C6C(Pos *p);
static inline s32 canPlace(s32 r) { return r == 1; }
static inline void rect_set(Rect *r, s32 x0, s32 y0, s32 x1, s32 y1) {
    r->x0 = x0;
    r->y0 = y0;
    r->x1 = x1;
    r->y1 = y1;
}
s32 func_800A5B98(Obj *o, Pos *out) {
    Pos p;
    s32 i;
    for (i = 0; ; i++) {
        u32 attr;
        if (i >= 100) break;
        func_800B221C(&p);
        if (!canPlace(func_800A4314(o, &p))) continue;
        if (func_800B5BDC(&p)) continue;
        if (o->flags & 8) {
            attr = func_800B1C6C(&p);
            if (attr & 0x2000) { *out = p; return 1; }
        } else {
            attr = func_800B1C6C(&p);
            if ((attr & 0xEB70) == 0x200) { *out = p; return 1; }
        }
    }
    for (i = 0; ; i++) {
        Rect r;
        if (i >= 100) break;
        rect_set(&r, D_801429C0.x0, D_801429C0.y0, D_801429C0.x1, D_801429C0.y1);
        func_800A33DC(&p, &r);
        if (!canPlace(func_800A4314(o, &p))) continue;
        if (func_800B5BDC(&p)) continue;
        if (o->flags & 8) {
            if (func_800B1C6C(&p) & 0x2000) { *out = p; return 1; }
        } else {
            if ((func_800B1C6C(&p) & 0xEB70) == 0x200) { *out = p; return 1; }
        }
    }
    return 0;
}