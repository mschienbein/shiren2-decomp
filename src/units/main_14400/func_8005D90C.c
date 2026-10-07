#include "common.h"

typedef struct { unsigned char pad[0x12]; unsigned char f12; unsigned char f13; unsigned char f14; } State;
extern State D_80165960;
void func_8005D90C(unsigned char value, unsigned char flag) {
    State *s = &D_80165960;
    s->f13 = value;
    if (flag) {
        s->f12 = 0;
        s->f14 |= 2;
    } else {
        s->f12 = value;
        s->f14 &= ~2;
    }
}
