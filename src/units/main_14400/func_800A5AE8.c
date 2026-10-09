#include "common.h"

typedef unsigned char u8;

typedef struct Unit Unit;

typedef struct {
    s32 x;
    s32 y;
} Pos;

extern void *func_800B221C(Pos *out);
extern s32 func_800B5BDC(Pos *p);
extern s32 func_800A4314(Unit *o, Pos *p);
extern u8 D_8014344C;

s32 func_800A5AE8(Unit *u, Pos *pos)
{
    s32 anywhere = D_8014344C == 1;
    s32 tries = 100;
    Pos candidate;

    for (;;) {
        if (--tries == -1) {
            return 0;
        }
        func_800B221C(&candidate);
        if (!anywhere && func_800B5BDC(&candidate)) {
            continue;
        }
        if (func_800A4314(u, &candidate)) {
            break;
        }
    }
    *pos = candidate;
    return 1;
}
