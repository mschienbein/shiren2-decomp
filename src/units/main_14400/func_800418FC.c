#include "common.h"
typedef struct { s32 f0; s32 f4; } Pair;
typedef struct { Pair pos; char pad[0xFC]; Pair *f104; } SB;
extern SB *D_801476B8;
void func_800418FC(s32 *outA, s32 *outB) {
    SB *b = D_801476B8;
    Pair t;
    if (b->f104 != 0) {
        t = *b->f104;
    } else {
        t = b->pos;
    }
    *outA = t.f4;
    *outB = t.f0;
}
