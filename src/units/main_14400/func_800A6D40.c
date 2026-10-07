#include "common.h"
typedef unsigned char u8;
typedef struct { u8 value; } Dir;
typedef struct { char pad[8]; Dir x8; } S;
typedef struct { s32 a, b; } Iter;
void *func_800A2594(void *out, void *from, Dir dir);
void *func_800B4928(Iter *it);
s32 func_800B1DF8(Iter *it);
void func_800A2758(void *pos, Dir dir);
void *func_800A6D40(S *p) {
    Iter it;
    s32 i;
    void *found;
    func_800A2594(&it, p, p->x8);
    i = 3;
    for (;;) {
        if (i-- <= 0) return 0;
        found = func_800B4928(&it);
        if (found) return found;
        if ((func_800B1DF8(&it) ^ 1) != 0) break;
        func_800A2758(&it, p->x8);
    }
    return 0;
}
