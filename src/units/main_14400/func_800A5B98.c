#include "common.h"
#include "map_rect.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { u8 unk0[0x1C]; u16 flags; } Obj;
extern MapRect D_801429C0;
void *func_800B221C(MapPos *p);
MapPos *func_800A33DC(MapPos *p, MapRect *r);
s32 func_800A4314(Obj *o, MapPos *p);
s32 func_800B5BDC(MapPos *p);
u32 func_800B1C6C(MapPos *p);
static inline s32 canPlace(s32 r) { return r == 1; }
static inline void rect_set(MapRect *r, s32 x0, s32 y0, s32 x1, s32 y1) {
    r->min.x = x0;
    r->min.y = y0;
    r->max.x = x1;
    r->max.y = y1;
}
s32 func_800A5B98(Obj *o, MapPos *out) {
    MapPos p;
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
        MapRect r;
        if (i >= 100) break;
        rect_set(&r, D_801429C0.min.x, D_801429C0.min.y, D_801429C0.max.x, D_801429C0.max.y);
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