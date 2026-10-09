#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u16 base; u8 count; } Range;
extern u8 D_801480F4[];
void *func_80044ECC(u8 kind, u8 level);

s32 func_800D7ED0(s32 x) {
    Range *r;
    s32 base;
    s32 rest;

    r = func_80044ECC(0x29, 1);
    base = r->base;
    if (x < base) {
        return x;
    }
    if (x < base + 9) {
        x = base + D_801480F4[x - base] - 1;
    } else {
        rest = x - 9;
        x = rest + r->count;
        r = func_80044ECC(0x50, 1);
        base = r->base;
        if (x < base) {
            return x;
        }
        x += r->count;
        r = func_80044ECC(0x51, 1);
        base = r->base;
        if (x < base) {
            return x;
        }
        x += r->count;
        r = func_80044ECC(0x55, 1);
        base = r->base;
        if (x < base) {
            return x;
        }
        x += r->count;
        r = func_80044ECC(0x56, 1);
        base = r->base;
        if (x < base) {
            return x;
        }
        x += r->count;
    }
    return x;
}
