#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct {
    u8 pad0[0x14];
    u8 x14[8];
    u8 pad1C[0x48 - 0x1C];
    s32 x48;
    u8 pad4C[4];
    s16 x50;
    u16 x52;
    s16 x54;
    u8 pad56[4];
    s16 x5A;
    s16 x5C;
    s16 x5E;
} Obj;
extern s32 D_80139B30;
extern u8 D_80139B3C[][8];
typedef struct { u16 id; u16 variant; s16 x; s16 y; s16 z; s16 wait; u16 f0C; s16 f0E; } Spawn;
extern Spawn D_80139B4C[112];
static inline void initialize_spawn(Obj *o, u16 value) {
    o->x5E = 1;
    o->x5C = 2;
    o->x50 = 1;
    o->x5A = 0;
    o->x52 = value;
}
void func_8005619C(Obj *o) {
    if (D_80139B30 != 1) {
        return;
    }
    o->x48 = ((const u8 *)&D_80139B4C[o->x54].variant)[1];
    o->x14[0] = D_80139B3C[((const u8 *)&D_80139B4C[o->x54].f0E)[0]][0];
    o->x14[1] = D_80139B3C[((const u8 *)&D_80139B4C[o->x54].f0E)[0]][1];
    o->x14[2] = D_80139B3C[((const u8 *)&D_80139B4C[o->x54].f0E)[0]][2];
    o->x14[3] = D_80139B3C[((const u8 *)&D_80139B4C[o->x54].f0E)[0]][3];
    o->x14[4] = D_80139B3C[((const u8 *)&D_80139B4C[o->x54].f0E)[0]][4];
    o->x14[5] = D_80139B3C[((const u8 *)&D_80139B4C[o->x54].f0E)[0]][5];
    o->x14[6] = D_80139B3C[((const u8 *)&D_80139B4C[o->x54].f0E)[0]][6];
    o->x14[7] = D_80139B3C[((const u8 *)&D_80139B4C[o->x54].f0E)[0]][7];
    initialize_spawn(o, D_80139B4C[o->x54].f0C);
}
