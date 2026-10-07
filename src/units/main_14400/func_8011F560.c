#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { s32 x; s32 y; } Pt;
typedef struct { u8 v; } Byte;
s32 func_80049CB4(s32 id, ...);
s32 func_800B1AB8(Pt *);
void *func_800B31E8(void *pos, s32 team);
s32 func_800B5300(Pt *, void *, u8);
u32 func_800B1C6C(void *pos);
void func_800A2758(Pt *, Byte);
extern u8 D_801569FF;

static __inline__ void copyPt(Pt *dst, Pt *src) {
    dst->x = src->x;
    dst->y = src->y;
}
void func_8011F560(void *arg0, void *arg1, Pt *start, Byte *dir) {
    Pt pos;
    s32 i;
    u8 hits = 0;

    copyPt(&pos, start);
    func_80049CB4(6);
    i = 0;
    while (1) {
        Pt *p;
        s32 open;
        if (i >= 5) {
            break;
        }
        open = func_800B1AB8(&pos) == 1;
        if (!open) {
            break;
        }
        p = &pos;
        if (func_800B31E8(p, 10) == 0) {
            if (func_800B5300(p, arg1, D_801569FF)) {
                hits++;
            } else if (func_800B1C6C(p) & 0x4000) {
                break;
            }
            func_80049CB4(0x129, 6);
        }
        func_800A2758(&pos, *dir);
        i++;
    }
    func_80049CB4(7);
    if (hits) {
        func_80049CB4(0x12E);
    }
    func_80049CB4(0x132);
}
