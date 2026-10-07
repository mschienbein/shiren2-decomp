#include "common.h"
typedef short s16;
typedef struct { s16 delta; s16 index; void *fn; } VtblEntry;
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Pos800A6348;
typedef struct { char pad[0x24]; VtblEntry *vtbl; } Obj800A6348;
extern u8 D_8014344C;
s32 func_800A5E24(Obj800A6348 *, Pos800A6348 *, s32, s32);
void func_800A58FC(Obj800A6348 *, Pos800A6348 *);
s32 func_800A6348(Obj800A6348 *self, Pos800A6348 *pos) {
    Pos800A6348 tmp;
    s32 ok = 0;
    Pos800A6348 *t = &tmp;
    t->x = pos->x;
    t->y = pos->y;
    if (D_8014344C >= 2) {
        ok = func_800A5E24(self, &tmp, 0, 0) != 0;
    } else if (func_800A5E24(self, &tmp, 1, 1)) {
        ok = 1;
    }
    if (ok) {
        func_800A58FC(self, &tmp);
        return 1;
    }
    if (self != 0) {
        VtblEntry *e = &self->vtbl[1];
        ((void (*)(void *, s32))e->fn)((char *)self + e->delta, 3);
    }
    return 0;
}
