#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { s16 x; s16 y; s16 state; } Cell;
typedef struct {
    s8 count;
    u8 pad1;
    Cell from[3];
    Cell to[3];
    u8 pad26[8];
} Group;
typedef struct {
    u8 pad0[2];
    s16 x2;
    u8 pad4[4];
    u8 x8;
    u8 pad9[0x3E];
    u8 x47;
    u8 pad48[0xB0 - 0x48];
} Unit;
extern Group D_801A79E8[];
extern Unit D_801DEAB4[];
s32 func_8007BE1C(s32 x, s32 y);
void func_80052260(s16 id);
void func_80077498(s32 mode) {
    s32 changed = 0;
    s32 i;
    for (i = 0; i < 30; i++) {
        Group *g = &D_801A79E8[i];
        Unit *u = &D_801DEAB4[i];
        s32 j;
        if (u->x2 == -1 || g->count == 0 || u->x47 == 0 || u->x8 == 1) {
            continue;
        }
        for (j = 0; j < g->count; j++) {
            if (g->to[j].state == g->from[j].state) {
                continue;
            }
            if (mode == 0 && g->from[j].state == 1) {
                changed = 1;
                func_8007BE1C(g->from[j].x, g->from[j].y);
            }
            if (mode == 1 && g->to[j].state == 1) {
                changed = 1;
                func_8007BE1C(g->to[j].x, g->to[j].y);
            }
        }
    }
    if (changed) {
        func_80052260(0x20);
    }
}
