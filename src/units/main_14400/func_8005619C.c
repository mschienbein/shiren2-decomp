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
extern u8 D_80139B4F[][16];
extern u16 D_80139B58[][8];
extern u8 D_80139B5A[][16];
void func_8005619C(Obj *o) {
    u16 value;
    if (D_80139B30 != 1) {
        return;
    }
    o->x48 = D_80139B4F[o->x54][0];
    o->x14[0] = D_80139B3C[D_80139B5A[o->x54][0]][0];
    o->x14[1] = D_80139B3C[D_80139B5A[o->x54][0]][1];
    o->x14[2] = D_80139B3C[D_80139B5A[o->x54][0]][2];
    o->x14[3] = D_80139B3C[D_80139B5A[o->x54][0]][3];
    o->x14[4] = D_80139B3C[D_80139B5A[o->x54][0]][4];
    o->x14[5] = D_80139B3C[D_80139B5A[o->x54][0]][5];
    o->x14[6] = D_80139B3C[D_80139B5A[o->x54][0]][6];
    o->x14[7] = D_80139B3C[D_80139B5A[o->x54][0]][7];
    value = D_80139B58[o->x54][0];
    o->x5E = 1;
    o->x5C = 2;
    o->x50 = 1;
    o->x5A = 0;
    o->x52 = value;
}
