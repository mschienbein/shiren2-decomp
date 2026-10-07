#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct {
    s32 x0;
    s32 x4;
    u8 pad8[8];
    s32 x10;
    u8 pad14[8];
    s32 x1C;
    u8 pad20[0x58];
    s32 x78;
    u8 pad7C[0x13C - 0x7C];
} Obj;
extern s32 D_801CA6D4;
extern Obj *D_801CA6DC;
void func_8012A2F8(s32 flags, s32 value) {
    s32 i;
    Obj *o;
    s32 v = value;
    if (v == 0) {
        v = 1;
    }
    o = D_801CA6DC;
    for (i = 0; i < D_801CA6D4; i++, o++) {
        if (o->x78 ? (flags & 1) : (flags & 2)) {
            if (o->x4 != 0 && o->x10 == -1) {
                if (o->x0 & 1) {
                    o->x1C = 1;
                    o->x10 = 0;
                    o->x0 &= ~1;
                } else {
                    o->x1C = v;
                    o->x10 = value;
                }
            }
        }
    }
}
