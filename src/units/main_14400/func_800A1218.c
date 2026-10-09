#include "common.h"

typedef unsigned char u8;
typedef struct { char pad[4]; u8 *f4; char pad8[3]; u8 fB; } S;
s32 func_8010BE60(u8 *, u8);
u8 func_800A1218(S *s) {
    func_8010BE60(s->f4, s->fB);
    return *s->f4;
}
