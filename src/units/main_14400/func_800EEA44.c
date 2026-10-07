#include "common.h"
typedef struct { s32 f0; s32 f4; } Base;
typedef struct {
    char pad[0x1E];
    unsigned char f1E;
    char pad1F[0x84 - 0x1F];
    Base base;
    char pad8C[0x9A - 0x8C];
    unsigned short f9A;
} Unit;
extern void *func_800C5F60(void);
static inline Base *toBase(Unit *u) { return u ? &u->base : 0; }
s32 func_800EEA44(Unit *a, Unit *b, unsigned char *out) {
    s32 flag;
    unsigned char v;
    *out = 0;
    if (b == 0) {
        return 0;
    }
    {
        s32 inactive = toBase(a)->f4 != 1;
        if (inactive) {
            return 0;
        }
    }
    flag = 0;
    if ((b->f1E >> 2) & 1) {
        flag = 1;
    } else if (b == func_800C5F60()) {
        flag = 1;
    }
    if (flag) {
        return toBase(a)->f4 != 0;
    }
    v = b->f1E;
    if ((v >> 3) & 1) {
        return toBase(a)->f4 != 0 && toBase(b)->f4 != 0;
    }
    if ((v >> 4) & 1) {
        return ((b->f9A & 0x40) == 0) << 1;
    }
    return 0;
}
