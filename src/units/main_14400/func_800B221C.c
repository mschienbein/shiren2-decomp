#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s32 x, y; } Vec2;
typedef struct { u8 data[0x14]; } Entry;
/* Owner object held by D_80142B10. This TU only forwards its address: func_800BAB28
 * selects a position inside the 16-byte rectangle the owner embeds at +0x9BC. */
typedef struct Owner800B221C Owner800B221C;
extern u8 D_8014344C;
extern u16 D_8014344A;
extern Entry D_801431F0[];
extern u8 D_801429C0[16];
extern u8 D_80147620[];
extern u8 D_80142F20;
extern Owner800B221C *D_80142B10;
extern u16 func_800C58DC(void *, u16);
extern s32 func_800A3214(Entry *);
extern void *func_800A33DC(void *out, void *rect);
extern void func_800BAB28(Vec2 *out, Owner800B221C *owner);
void *func_800B221C(Vec2 *out){
    s32 i;
    u16 roll;
    Entry *e;
    if (D_8014344C == 0) {
        func_800A33DC(out, D_801429C0);
        return out;
    }
    roll = func_800C58DC(D_80147620, D_8014344A - 1);
    i = 0;
    e = D_801431F0;
    for (;;) {
        s32 w;
        if (i >= D_8014344C) break;
        w = func_800A3214(e);
        if (roll < w) break;
        roll -= w;
        e++;
        i++;
    }
    if (i >= D_8014344C) {
        s32 same;
        i = 0;
        same = (D_80142F20 & 0xE0) == 0x20;
        if (same) {
            Vec2 v;
            Vec2 *pv;
            func_800BAB28(&v, D_80142B10);
            pv = &v;
            if (pv->y | pv->x) {
                out->x = pv->x;
                out->y = pv->y;
                goto done;
            }
        }
    }
    func_800A33DC(out, &D_801431F0[i]);
done:
    return out;
}
