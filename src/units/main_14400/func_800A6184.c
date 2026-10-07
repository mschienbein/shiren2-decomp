#include "common.h"
typedef short s16;
typedef s32 bool32;
typedef struct { s32 a; s32 b; } Pair;
typedef struct { s16 delta; s16 index; void (*fn)(void *); } VEntry;
typedef struct { char pad0[0x24]; VEntry *vtbl; } Obj;
bool32 func_800A5E24(Obj *, Pair *, s32, s32);
bool32 func_800A5FCC(Obj *, Pair *);
void func_800A58FC(Obj *, Pair *);
s32 func_800A6184(Obj *self, Pair *pos) {
    Pair tmp;
    Pair *t = &tmp;
    s32 missing;
    t->a = pos->a;
    t->b = pos->b;
    missing = func_800A5E24(self, t, 1, 1) ^ 1;
    if (missing) {
        missing = func_800A5FCC(self, t) ^ 1;
        if (missing) {
            self->vtbl[3].fn((char *)self + self->vtbl[3].delta);
            return 0;
        }
    }
    func_800A58FC(self, &tmp);
    return 1;
}
